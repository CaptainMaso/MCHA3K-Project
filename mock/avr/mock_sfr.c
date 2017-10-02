#include <stdint.h>
// #include "avr/io.h"
#include "avr/mock_sfr.h"

// Statically initialise to zero to emulate hardware startup behaviour
uint8_t mock_ddra   = 0;
uint8_t mock_ddrb   = 0;
uint8_t mock_ddrc   = 0;
uint8_t mock_ddrd   = 0;

uint8_t mock_porta  = 0;
uint8_t mock_portb  = 0;
uint8_t mock_portc  = 0;
uint8_t mock_portd  = 0;

MOCK_SFR16_T mock_adc = {.w = 0x0000};
uint8_t mock_admux  = 0;
uint8_t mock_adcsra = 0;

uint8_t mock_gicr   = 0;
uint8_t mock_gifr   = 0;
uint8_t mock_mcucr  = 0;
uint8_t mock_mcucsr = 0;

uint8_t mock_tccr0  = 0;
uint8_t mock_tccr1a = 0;
uint8_t mock_tccr1b = 0;
uint8_t mock_tccr2  = 0;

uint8_t mock_tcnt0  = 0;
uint8_t mock_tcnt0h = 0;
uint8_t mock_tcnt0l = 0;
uint8_t mock_tcnt2  = 0;

uint8_t mock_ocr0   = 0;
uint8_t mock_ocr1ah = 0;
uint8_t mock_ocr1al = 0;
uint8_t mock_ocr1bh = 0;
uint8_t mock_ocr1bl = 0;
uint8_t mock_ocr2   = 0;

uint8_t mock_timsk  = 0;