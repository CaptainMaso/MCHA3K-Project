#ifndef ENCODERS_H
#define ENCODERS_H

#include <stdint.h>
#include "motor.h"

/*	Encoder Pinouts
 * 		ML_A = PD3 (INT1)
 * 		ML_B = PB1
 * 		MR_A = PD2 (INT0)
 * 		MR_B = PB0
 */

#define ENC_ML_A !!(PIND & _BV(PD3))
#define ENC_ML_B !!(PINB & _BV(PB1))
#define ENC_MR_A !!(PIND & _BV(PD2))
#define ENC_MR_B !!(PINB & _BV(PB0))

#define CPR 660

void    encoder_init(void);
void    encoder_edge_ML_isr(void);
void    encoder_edge_MR_isr(void);

void    encoder_set_count(int32_t, MOTOR_SIDE);
int32_t encoder_get_count(MOTOR_SIDE);
int32_t encoder_pop_count(MOTOR_SIDE);

#endif
