#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

#include <util/atomic.h>

#include "motor.h"

uint8_t currentADC = 0;
uint8_t adc_channel = 0;

int16_t adc_left = 0;
int16_t adc_right = 0;

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
	ADCSRA 	 = (_BV(ADEN)) 	| (_BV(ADATE)) 	| (_BV(ADIE)) 	| (_BV(ADPS1));
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

	//ADCSRA =
}

void motors_set_pwm(MOTOR_SIDE side, int32_t value)
{
	if (value <= MAX_PWM && value >= -MAX_PWM)
	{
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

				PORTB &= ~(_BV(PB3));
			}
			else
			{
				PORTB |= _BV(PB3);
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
}

int16_t motors_get_adc_reading(MOTOR_SIDE side)
{
	int16_t temp = 0;
	if (side == MOTOR_LEFT)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			temp = adc_left;
		}
	}
	else if (side == MOTOR_RIGHT)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			temp = adc_right;
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
		adc_left = ((int16_t)ADCW) >> 6;
		if (adc_left > 512)
			adc_left = 0;
		/*
		 * This is done because the sense resistor is the output to the L298,
		 * so we only get the magnitude of the current, not the direction.
		 * As such, we assume that the current measured is flowing in the direction
		 * that the voltage is being applied (which will almost always be the case)
		 */
		if (!!(PORTB &_BV(PB3)))
			adc_left *= -1;
	}
	else if (adc_channel == 1)
	{
		adc_right = ((int16_t)ADCW) >> 6;
		if (adc_right > 512)
			adc_right = 0;
		if (!!(PORTB &_BV(PB2)))
			adc_right *= -1;
	}
	_adc_toggle();
	ADCSRA |= (_BV(ADEN));
}
