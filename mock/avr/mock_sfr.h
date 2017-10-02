#ifndef MOCK_SFR_H
#define MOCK_SFR_H

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

extern uint8_t mock_tccr0;
extern uint8_t mock_tccr1a;
extern uint8_t mock_tccr1b;
extern uint8_t mock_tccr2;

extern uint8_t mock_tcnt0;
extern uint8_t mock_tcnt0h;
extern uint8_t mock_tcnt0l;
extern uint8_t mock_tcnt2;

extern uint8_t mock_ocr0;
extern uint8_t mock_ocr1ah;
extern uint8_t mock_ocr1al;
extern uint8_t mock_ocr1bh;
extern uint8_t mock_ocr1bl;
extern uint8_t mock_ocr2;

extern uint8_t mock_timsk;

#endif