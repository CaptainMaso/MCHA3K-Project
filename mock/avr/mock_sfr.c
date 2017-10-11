#include <stdint.h>
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

uint8_t mock_sfior  = 0;
uint8_t mock_tifr   = 0;

uint8_t mock_tccr0  = 0;
uint8_t mock_tccr1a = 0;
uint8_t mock_tccr1b = 0;
uint8_t mock_tccr2  = 0;

uint8_t mock_tcnt0  = 0;
MOCK_SFR16_T mock_tcnt1 = {.w = 0x0000};
uint8_t mock_tcnt2  = 0;

uint8_t mock_ocr0   = 0;
MOCK_SFR16_T mock_ocr1a = {.w = 0x0000};
MOCK_SFR16_T mock_ocr1b = {.w = 0x0000};
uint8_t mock_ocr2   = 0;

MOCK_SFR16_T mock_icr1 = {.w = 0x0000};

uint8_t mock_timsk  = 0;

uint8_t mock_sreg = 0;

//TWI
uint8_t mock_TWSR = 0;
uint8_t mock_TWCR = 0;
uint8_t mock_TWDR = 0;
//uint8_t TWR = 0;
