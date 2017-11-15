#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <math.h>
#include <util/atomic.h>

#include "controller.h"
#include "ctrl_param.def"
#include "motor.h"

#include "motor_ctrl_param.def"

static uint8_t adc_channel = 0;

static int32_t 	ML_SUM = 0;
static uint8_t 	ML_COUNT = 0;
static int32_t 	MR_SUM = 0;
static uint8_t 	MR_COUNT = 0;
static float	ML_CURRENT = 0;
static float	MR_CURRENT = 0;

#define LPF 0.5

#ifdef FIXEDPOINT_MOTORCTRL
static int32_t ML_integrator;
static int32_t MR_integrator;

static int32_t ML_req_current;
static int32_t MR_req_current;

static int32_t ML_Voltage;
static int32_t MR_Voltage;
#else
static float ML_integrator;
static float MR_integrator;

static float ML_req_current;
static float MR_req_current;

static float ML_Voltage;
static float MR_Voltage;
#endif

static uint32_t motor_last_run = 0;

void motors_init(void)
{
	// ADC Initialisation TODO: Setup ADC
	/*
	REFS: 			Using external voltage reference (Vcc = 5V)
	ADLAR: 			Right adjusted
	MUX: 			10x gain ADC1-ADC0
	ADEN: 			Enabled
	ADATE: 			Auto trigger enabled
	ADIE: 			Finish conversion interupt enabled
	ADPS1:			4x prescaler
	ADTS: 			Timer 1 overflow trigger ADC
	*/
	ADMUX 	 = (_BV(ADLAR)) | (_BV(REFS0)) | (_BV(MUX3))   | (_BV(MUX2)) 	| (_BV(MUX0));
	ADCSRA 	 = (_BV(ADEN)) 	| (_BV(ADATE)) | (_BV(ADIE))   | (_BV(ADPS1));
	SFIOR 	|= (_BV(ADTS2)) | (_BV(ADTS1));
	adc_channel = 0;

	// Timer 1 Initialisation
	/*
	COM1A, COM1B: 	Set up, clear down
	WGM1: 			Phase correct PWM mode with ICR1 TOP
	IC: 			Not used
	CS: 			1x prescaling
	*/
	TCCR1A = (_BV(COM1A1)) 	/*| (_BV(COM1A0))*/ | (_BV(COM1B1)) /*| (_BV(COM1B0))*/ | (_BV(WGM11));
	TCCR1B = (_BV(WGM13)) 	| (_BV(CS10));
	TIMSK |= (_BV(TOIE1));
	ICR1 = MAX_PWM;	// Set ICR1 for minimum frequency

	DDRB |= _BV(PB2) | _BV(PB3);
	DDRD |= _BV(PD4) | _BV(PD5);
}

void motor_ctrl_run(uint32_t ctrl_count, states *ctrl_states)
{
	if (ctrl_count == 0)
		motor_last_run = 0;
	if ((ctrl_count % motor_interval) == 0 && ctrl_count != motor_last_run)
	{
		PORTA |= _BV(PA4);
		if (ctrl_count - motor_last_run <= motor_interval)
		{
			ctrl_states->Current_ML = motors_get_adc_reading(MOTOR_LEFT);// + LPF*ML_ADC_LAST);
			//ML_ADC_LAST = ML_ADC;
			ctrl_states->Current_MR = motors_get_adc_reading(MOTOR_LEFT); //- LPF*ML_ADC_LAST);
			//MR_ADC_LAST = MR_ADC;
			motors_set_pwm(MOTOR_LEFT, motor_ctrl_alloc(MOTOR_LEFT, ctrl_states));
			motors_set_pwm(MOTOR_RIGHT, motor_ctrl_alloc(MOTOR_RIGHT, ctrl_states));
		}
		else
		{
			printf_P(PSTR("ERROR: Motor ctrl lagging, disabling control (LAG: %"PRIu32" counts)\n"), ctrl_count - motor_last_run - motor_interval);
			ctrl_set_mode(CTRL_OFF);
		}
		PORTA &= ~_BV(PA4);
	}
}

#ifdef FIXEDPOINT_MOTORCTRL
void motor_set_torque(MOTOR_SIDE side, int32_t value)
{
	switch (side)
	{
	case MOTOR_LEFT:
		value *= ML_T2C;
		if (value > ML_OFFS_POS)
		{
			ML_req_current = value + ML_OFFS_POS;
		}
		else if (value < -ML_OFFS_NEG)
		{
			ML_req_current = value - ML_OFFS_NEG;
		}
		else
		{
			ML_req_current = 0;
		}

		break;
	case MOTOR_RIGHT:
		value *= MR_T2C;
		if (value > MR_OFFS_POS)
		{
			MR_req_current = value + MR_OFFS_POS;
		}
		else if (value < -MR_OFFS_NEG)
		{
			MR_req_current = value - MR_OFFS_NEG;
		}
		else
		{
			MR_req_current = 0;
		}
		break;
	default:
		printf_P("ERROR: Unknown Side\n");
	}
}

int32_t motor_ctrl_alloc(MOTOR_SIDE side, states *ctrl_states)
{
	switch (side)
	{
	case MOTOR_LEFT:
	{
		// Update Controller State
		ML_integrator = K_ML_int * ML_integrator + K_ML_V * ML_Voltage;

		// Update Controller Output
		ML_Voltage = ML_HFG*(ML_req_current - K_ML_OUT*ML_integrator/K_FP_GAIN - ctrl_states->Current_ML)/K_FP_GAIN;
		ML_Voltage = (ML_Voltage > MAX_VOLTAGE*K_FP_GAIN) ? MAX_VOLTAGE*K_FP_GAIN : ((ML_Voltage < -MAX_VOLTAGE*K_FP_GAIN) ? -MAX_VOLTAGE*K_FP_GAIN : ML_Voltage);

		return (int32_t)(ML_Voltage*MAX_PWM/K_FP_GAIN/MAX_VOLTAGE);
	} break;
	case MOTOR_RIGHT:
	{
		/*
		 * Motor Right Allocation Function
		 */

		// Update Controller State
		MR_integrator = K_MR_int * MR_integrator + K_MR_V * MR_Voltage;

		// Update Controller Output
		MR_Voltage = MR_HFG*(MR_req_current - K_MR_OUT*MR_integrator/K_FP_GAIN - ctrl_states->Current_MR)/K_FP_GAIN;
		MR_Voltage = (MR_Voltage > MAX_VOLTAGE*K_FP_GAIN) ? MAX_VOLTAGE*K_FP_GAIN : ((MR_Voltage < -MAX_VOLTAGE*K_FP_GAIN) ? -MAX_VOLTAGE*K_FP_GAIN : MR_Voltage);

		return (int32_t)(MR_Voltage/MAX_VOLTAGE);
	}break;
	default:
		printf_P(PSTR("ERROR: Incorrect Side\n"));
		return 0;
	}
}
#else
void motor_set_torque(MOTOR_SIDE side, float value)
{
	switch (side)
	{
	case MOTOR_LEFT:
		value *= ML_T2C;
		if (value > 0)
		{
			ML_req_current = value + ML_OFFS_POS;
		}
		else if (value < 0)
		{
			ML_req_current = value - ML_OFFS_NEG;
		}
		else
		{
			ML_req_current = 0;
		}
		//ML_req_current = value;
		break;
	case MOTOR_RIGHT:
		value *= MR_T2C;
		if (value > 0)
		{
			MR_req_current = value + MR_OFFS_POS;
		}
		else if (value < 0)
		{
			MR_req_current = value - MR_OFFS_NEG;
		}
		else
		{
			MR_req_current = 0;
		}
		//MR_req_current = value;
		break;
	default:
		printf_P("ERROR: Unknown Side\n");
	}
}

float motor_ctrl_alloc(MOTOR_SIDE side, states *ctrl_states)
{
	switch (side)
	{
	case MOTOR_LEFT:
	{
		// Update Controller State
		ML_integrator = K_ML_int * ML_integrator + K_ML_V * ML_Voltage;

		// Update Controller Output
		ML_Voltage = ML_HFG*(ML_req_current - K_ML_OUT*ML_integrator - motors_get_adc_reading(side));

		//ML_Voltage = ML_R*ML_req_current + ML_KN*ctrl_states->dPhi_ML;

		ML_Voltage = (ML_Voltage > MAX_VOLTAGE) ? MAX_VOLTAGE : ((ML_Voltage < -MAX_VOLTAGE) ? -MAX_VOLTAGE : ML_Voltage);
		return ML_Voltage;
	} break;
	case MOTOR_RIGHT:
	{
		/*
		 * Motor Right Allocation Function
		 */

		// Update Controller State
		 //MR_integrator = K_MR_int * MR_integrator + K_MR_V * MR_Voltage;

		// Update Controller Output
		//MR_Voltage = MR_HFG*(MR_req_current - K_MR_OUT*MR_integrator - motors_get_adc_reading(side));

		MR_Voltage = MR_R*MR_req_current + MR_KN*ctrl_states->dPhi_MR;
		MR_Voltage = (MR_Voltage > MAX_VOLTAGE) ? MAX_VOLTAGE : ((MR_Voltage < -MAX_VOLTAGE) ? -MAX_VOLTAGE : MR_Voltage);

		return MR_Voltage;
	}break;
	default:
		printf_P(PSTR("ERROR: Incorrect Side\n"));
		return 0;
	}
}
#endif

void motors_set_voltage(MOTOR_SIDE side, float value)
{
	value = (value > MAX_VOLTAGE) ? MAX_VOLTAGE : ((value < -MAX_VOLTAGE) ? -MAX_VOLTAGE : value);
	motors_set_pwm(side, MAX_PWM*(value*fabs(value)/(MAX_VOLTAGE*MAX_VOLTAGE)));
	//motors_set_pwm(side, MAX_PWM*value/MAX_VOLTAGE);
}

void motors_set_pwm(MOTOR_SIDE side, int32_t value)
{
	value = (value > MAX_PWM) ? MAX_PWM : ((value < -MAX_PWM) ?  -MAX_PWM : value);
	uint16_t tmp = 0;
	bool direction = false;
	if (value > 0)
	{
		tmp = (uint16_t)value;
		direction = true;
	}
	else
	{
		tmp = (uint16_t)(-1*value);

	}
	if (side == MOTOR_LEFT)
	{

		if (direction)
		{
			PORTB |= _BV(PB3);
		}
		else
		{
			PORTB &= ~(_BV(PB3));
		}

		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			OCR1A = tmp;
		}
		//printf_P(PSTR("SET ML to %"PRIu16"\n"), OCR1A);
	}
	else if (side == MOTOR_RIGHT)
	{
		if (direction)
		{
			PORTB |= _BV(PB2);
		}
		else
		{
			PORTB &= ~(_BV(PB2));
		}

		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			OCR1B = tmp;
		}
		//printf_P(PSTR("SET MR to %"PRIu16"\n"), OCR1B);
	}
}

float motors_get_adc_reading(MOTOR_SIDE side)
{
	if (side == MOTOR_LEFT)
	{
		if (ML_COUNT > 5)
		{
			ATOMIC_BLOCK(ATOMIC_FORCEON)
			{
				ML_CURRENT = (float)ML_SUM/ML_COUNT *  ADC2CUR;
				ML_SUM = 0;
				ML_COUNT = 0;
			}
		}
		return ML_CURRENT;
	}
	else if (side == MOTOR_RIGHT)
	{
		if (MR_COUNT > 5)
		{
			ATOMIC_BLOCK(ATOMIC_FORCEON)
			{
				MR_CURRENT = (float)MR_SUM/MR_COUNT *  ADC2CUR;
				MR_SUM = 0;
				MR_COUNT = 0;
			}
		}
		return MR_CURRENT;
	}
	return 0;
}

void _adc_toggle(void)
{
	if (adc_channel == 0) 	// Channel 2/3  (MUX = 01101)
	{
		ADMUX &= ~(_BV(MUX2));
		adc_channel = 1;
	}
	else if(adc_channel == 1) // Channel 1/2 	(MUX = 01001)
	{
		ADMUX |= (_BV(MUX2));
		adc_channel = 0;
	}
}

void motor_adc_isr(void)
{
	ADCSRA &= ~(_BV(ADEN));
	int32_t tmp = ((int16_t)ADCW) >> 6;
	if (tmp > 512)
		tmp = 0;
	if (adc_channel == 0 && ML_COUNT < 255)
	{
		if (!(PORTB &_BV(PB3)))
			tmp *= -1;
		ML_SUM += tmp;
		ML_COUNT++;
		/*
		 * This is done because the sense resistor is the output to the L298,
		 * so we only get the magnitude of the current, not the direction.
		 * As such, we assume that the current measured is flowing in the direction
		 * that the voltage is being applied (which will almost always be the case)
		 */

	}
	else if (adc_channel == 1 && MR_COUNT < 255)
	{
		if (!(PORTB &_BV(PB2)))
					tmp *= -1;
		MR_SUM += tmp;
		MR_COUNT++;
	}
	_adc_toggle();
	ADCSRA |= (_BV(ADEN));
}
