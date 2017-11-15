#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "encoders.h"

// Forward decl of private helper functions
static void _encoder_enable_interrupts(void);
static void _encoder_disable_interrupts(void);

static int32_t _ML_count = 0;
static int32_t _MR_count = 0;

void encoder_init(void)
{
    // Enables Pull-Up resistors on PD2 & PD3
	PORTD |= _BV(PD2) | _BV(PD3);
	//PORTB |= _BV(PB0) | _BV(PB1);
	
    // Any logical change on INT0 and INT1 triggers an interrupt
	MCUCR |= _BV(ISC10) | _BV(ISC00);// | _BV(ISC11) | _BV(ISC01);
	
    // Set default count
    _ML_count = 0;
    _MR_count = 0;

    // Enable interrupts INT0 and INT1
    _encoder_enable_interrupts();
}

void encoder_edge_ML_isr(void)
{
    // Implement A edge logic to increment or decrement _count
	if (ENC_ML_A == ENC_ML_B)
	{
		_ML_count--;
		//printf_P(PSTR("L^"));
	}
	else
	{
		_ML_count++;
		//printf_P(PSTR("Lv"));
	}
	//printf_P(PSTR("%"PRIu8"%"PRIu8"\n"), ENC_ML_A, ENC_ML_B);
}

void encoder_edge_MR_isr(void)
{
    // Implement B edge logic to increment or decrement _count--
	if (ENC_MR_A == ENC_MR_B)
	{
		_MR_count--;
		//printf_P(PSTR("R^"));
	}
	else
	{
		_MR_count++;
		//printf_P(PSTR("Rv"));
	}
	//printf_P(PSTR("%"PRIu8"%"PRIu8"\n"), ENC_MR_A, ENC_MR_B);
}

void encoder_set_count(int32_t count, MOTOR_SIDE side)
{
    // Atomically set _count
    _encoder_disable_interrupts();
    if (side == MOTOR_LEFT)
    	_ML_count = count;
    else if(side == MOTOR_RIGHT)
    	_MR_count = count;
    _encoder_enable_interrupts();
}

int32_t encoder_get_count(MOTOR_SIDE side)
{
    // Atomically get _count
	_encoder_disable_interrupts();
	int32_t count = 0;
	if (side == MOTOR_LEFT)
		count = _ML_count;
	else if(side == MOTOR_RIGHT)
		count = _MR_count;
	_encoder_enable_interrupts();
	return count;
}

int32_t encoder_pop_count(MOTOR_SIDE side)
{
    // Atomically read and reset _count
    _encoder_disable_interrupts();
    int32_t count = 0;
    if (side == MOTOR_LEFT) {
        count = _ML_count;
        _ML_count = 0;
    }
    else if(side == MOTOR_RIGHT)
    {
    	count = _MR_count;
    	_MR_count = 0;
    }
    _encoder_enable_interrupts();
    return count;
}

void _encoder_enable_interrupts(void)
{
    // Enable INT0 and INT1 interrupts
	GICR |= (_BV(INT0)) | (_BV(INT1));
}

void _encoder_disable_interrupts(void)
{
    // Disable INT0 and INT1 interrupts
	GICR &= ~((_BV(INT0)) | (_BV(INT1)));
}

