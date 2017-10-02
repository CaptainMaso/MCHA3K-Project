#include <stdint.h>
#include <avr/io.h>
#include "led.h"

//Timer Initialisation and ISR
void timer0_init(void)
{
	// Set WGM to Fast PWM | Set Compare output 11b | Set No prescaler
	TCCR0 = /*(_BV(WGM01)) | */(_BV(WGM00)) | (_BV(COM01)) | (_BV(COM00)) | (_BV(CS00));
	
	OCR0 = 0;
	
	//No timer0 interrupts
	//TIMSK |= _BV(TOIE0);
}

void led_init(void)
{
    DDRB |= _BV(PB3);
	timer0_init();
}

void led_on(void)
{
    led_set_brightness(255);
}

void led_off(void)
{
    led_set_brightness(0);
}

void led_set_brightness(uint8_t value)
{
	//if (value != 0)
		OCR0 = value;
	//else
		
}

uint8_t led_get_brightness(void)
{
    return OCR0;
}