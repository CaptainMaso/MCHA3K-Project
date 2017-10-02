#ifndef IO_H
#define IO_H

#include "avr/mock_sfr.h"

#define _BV(bit) (1 << (bit))
#define bit_is_set(sfr, bit) ((sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!((sfr) & _BV(bit)))
#define loop_until_bit_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))

#define DDRA mock_ddra
#define DDRB mock_ddrb
#define DDRC mock_ddrc
#define DDRD mock_ddrd

// Note: We define PINx and PORTx to the same virtual register to emulate the hardware behaviour
#define PINA mock_porta
#define PINB mock_portb
#define PINC mock_portc
#define PIND mock_portd

#define PORTA mock_porta
#define PORTB mock_portb
#define PORTC mock_portc
#define PORTD mock_portd

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB0 0
#define PB1 1
#define PB2 2
#define PB3 3
#define PB4 4
#define PB5 5
#define PB6 6
#define PB7 7

#define PC0 0
#define PC1 1
#define PC2 2
#define PC3 3
#define PC4 4
#define PC5 5
#define PC6 6
#define PC7 7

#define PD0 0
#define PD1 1
#define PD2 2
#define PD3 3
#define PD4 4
#define PD5 5
#define PD6 6
#define PD7 7

#define ADCL (mock_adc.l)
#define ADCH (mock_adc.h)
#define ADCW (mock_adc.w)

#define ADMUX mock_admux
#define MUX0 0
#define MUX1 1
#define MUX2 2
#define MUX3 3
#define MUX4 4
#define ADLAR 5
#define REFS0 6
#define REFS1 7

#define ADCSRA mock_adcsra
#define ADPS0 0
#define ADPS1 1
#define ADPS2 2
#define ADIE 3
#define ADIF 4
#define ADATE 5
#define ADSC 6
#define ADEN 7

#define GICR mock_gicr
#define INT2 5
#define INT0 6
#define INT1 7

#define GIFR mock_gifr
#define INTF2 5
#define INTF0 6
#define INTF1 7

#define MCUCR mock_mcucr
#define ISC00 0
#define ISC01 1
#define ISC10 2
#define ISC11 3

#define MCUCSR mock_mcucsr
#define ISC2 6

#define TCCR0 mock_tccr0
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM01 3
#define COM00 4
#define COM01 5
#define WGM00 6
#define FOC0 7

#define TCCR1A mock_tccr1a
#define WGM10 0
#define WGM11 1
#define FOC1B 2
#define FOC1A 3
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7

#define TCCR1B mock_tccr1b
#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4
// Bit 5 is reserved
#define ICES1 6
#define ICNC1 7

#define TCCR2 mock_tccr2
#define CS20 0
#define CS21 1
#define CS22 2
#define WGM21 3
#define COM20 4
#define COM21 5
#define WGM20 6
#define FOC2 7

#define TCNT0 mock_tcnt0
#define TCNT1H mock_tcnt0h
#define TCNT1L mock_tcnt0l
#define TCNT2 mock_tcnt2

#define OCR0 mock_ocr0
#define OCR1AH mock_ocr1ah
#define OCR1AL mock_ocr1al
#define OCR1BH mock_ocr1bh
#define OCR1BL mock_ocr1bl
#define OCR2 mock_ocr2

#define TIMSK mock_timsk
#define TOIE0 0
#define OCIE0 1
#define TOIE1 2
#define OCIE1B 3
#define OCIE1A 4
#define TICIE1 5
#define TOIE2 6
#define OCIE2 7


#endif