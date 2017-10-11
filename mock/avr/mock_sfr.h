#ifndef MOCK_SFR_H
#define MOCK_SFR_H

#include <stdint.h>

typedef struct {
    union {
        uint16_t w;
        struct {
            uint8_t l;
            uint8_t h;
        };
    };
} MOCK_SFR16_T;

extern uint8_t mock_ddra;
extern uint8_t mock_ddrb;
extern uint8_t mock_ddrc;
extern uint8_t mock_ddrd;

extern uint8_t mock_porta;
extern uint8_t mock_portb;
extern uint8_t mock_portc;
extern uint8_t mock_portd;

extern MOCK_SFR16_T mock_adc;
extern uint8_t mock_admux;
extern uint8_t mock_adcsra;

extern uint8_t mock_gicr;
extern uint8_t mock_gifr;
extern uint8_t mock_mcucr;
extern uint8_t mock_mcucsr;

extern uint8_t mock_sfior;
extern uint8_t mock_tifr;

extern uint8_t mock_tccr0;
extern uint8_t mock_tccr1a;
extern uint8_t mock_tccr1b;
extern uint8_t mock_tccr2;

extern uint8_t mock_tcnt0;
extern MOCK_SFR16_T mock_tcnt1;
extern uint8_t mock_tcnt2;

extern uint8_t mock_ocr0;
extern MOCK_SFR16_T mock_ocr1a;
extern MOCK_SFR16_T mock_ocr1b;
extern uint8_t mock_ocr2;

extern MOCK_SFR16_T mock_icr1;

extern uint8_t mock_timsk;

extern uint8_t mock_sreg;

extern uint8_t mock_TWSR;
extern uint8_t mock_TWCR;
extern uint8_t mock_TWDR;

#endif
