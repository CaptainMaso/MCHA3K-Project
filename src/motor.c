#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "controller.h"
#include "ctrl_param.def"
#include "motor.h"

#include "motor_ctrl_param.def"

static uint8_t adc_channel = 0;

static int16_t ML_ADC = 0;
static int16_t MR_ADC = 0;

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
			motors_set_pwm(MOTOR_LEFT, motor_ctrl_alloc(MOTOR_LEFT, ctrl_states));
			motors_set_pwm(MOTOR_RIGHT, motor_ctrl_alloc(MOTOR_RIGHT, ctrl_states));
		}
		else
		{
			//printf_P(PSTR("ERROR: Motor ctrl lagging, disabling control (LAG: %"PRIu32" counts)\n"), ctrl_count - motor_last_run - motor_interval);
			//ctrl_set_mode(CTRL_OFF);
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
			ML_req_current = 0.0f;
		}

		break;
	case MOTOR_RIGHT:
		value *= MR_T2C;
		if (value > ML_OFFS_POS)
		{
			MR_req_current = value + MR_OFFS_POS;
		}
		else if (value < -ML_OFFS_NEG)
		{
			MR_req_current = value - MR_OFFS_NEG;
		}
		else
		{
			MR_req_current = 0.0f;
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
		//ML_integrator = K_ML_int * ML_integrator + K_ML_V * ML_Voltage;

		// Update Controller Output
		//ML_Voltage = ML_HFG*(ML_req_current - K_ML_OUT*ML_integrator - ctrl_states->Current_ML);
		//ML_Voltage = (ML_Voltage > MAX_VOLTAGE) ? MAX_VOLTAGE : ((ML_Voltage < -MAX_VOLTAGE) ? -MAX_VOLTAGE : ML_Voltage);

		ML_Voltage = ML_R*ML_req_current + ML_KN*ctrl_states->dPhi_ML;

		return (int32_t)(ML_Voltage*MAX_PWM/MAX_VOLTAGE);
	} break;
	case MOTOR_RIGHT:
	{
		/*
		 * Motor Right Allocation Function
		 */

		// Update Controller State
		//MR_integrator = K_MR_int * MR_integrator + K_MR_V * MR_Voltage;

		// Update Controller Output
		//MR_Voltage = MR_HFG*(MR_req_current - K_MR_OUT*MR_integrator - ctrl_states->Current_MR);
		//MR_Voltage = (MR_Voltage > MAX_VOLTAGE) ? MAX_VOLTAGE : ((MR_Voltage < -MAX_VOLTAGE) ? -MAX_VOLTAGE : MR_Voltage);

		MR_Voltage = MR_R*MR_req_current + MR_KN*ctrl_states->dPhi_MR;

		return (int32_t)(MR_Voltage*MAX_PWM/MAX_VOLTAGE);
	}break;
	default:
		printf_P(PSTR("ERROR: Incorrect Side\n"));
		return 0;
	}
}
#endif

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

int16_t motors_get_adc_reading(MOTOR_SIDE side)
{
	int16_t temp = 0;
	if (side == MOTOR_LEFT)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			temp = ML_ADC;
		}
	}
	else if (side == MOTOR_RIGHT)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			temp = MR_ADC;
		}
	}
	return temp;
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
	if (adc_channel == 0)
	{
		ML_ADC = ((int16_t)ADCW) >> 6;
		if (ML_ADC > 512)
			ML_ADC = 0;
		/*
		 * This is done because the sense resistor is the output to the L298,
		 * so we only get the magnitude of the current, not the direction.
		 * As such, we assume that the current measured is flowing in the direction
		 * that the voltage is being applied (which will almost always be the case)
		 */
		if (!!(PORTB &_BV(PB3)))
			ML_ADC *= -1;
	}
	else if (adc_channel == 1)
	{
		MR_ADC = ((int16_t)ADCW) >> 6;
		if (MR_ADC > 512)
			MR_ADC = 0;
		if (!(PORTB &_BV(PB2)))
			MR_ADC *= -1;
	}
	_adc_toggle();
	ADCSRA |= (_BV(ADEN));
}
