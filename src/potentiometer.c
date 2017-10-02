#include <stdint.h>
#include <avr/io.h>
//#include <avr/interrupt.h>
#include "potentiometer.h"

static uint8_t potInit = 0;

void pot_init(void)
{
	potInit = 1;
	//Set to AVcc reference
    ADMUX = _BV(REFS0);
	
	ADCSRA = _BV(ADEN);
}

uint16_t pot_get_value(void)
{
	if (potInit)
	{
		ADCSRA |= _BV(ADSC);
		while(!(ADCSRA & _BV(ADIF)));
		ADCSRA |= _BV(ADIF);

		//cli();
		uint16_t val = ADCW;
		//sei();
		return val;
	}
	else
		return 0;
}

