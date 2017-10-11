#ifndef MOTORS_H
#define MOTORS_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define MAX_VOLTAGE 12
#define MAX_PWM 65535

/*	Motor Control Pinout
 *  	PB2 = MR direction
 *  	PB3 = ML direction
 *  	(OC1B) PD4 = MR enable
 *  	(OC1A) PD5 = ML enable
 */

typedef enum
{
	MOTOR_LEFT,
	MOTOR_RIGHT
} MOTOR_SIDE;

void motors_init(void);

void motors_set_pwm(MOTOR_SIDE, int32_t);

int16_t motors_get_adc_reading(MOTOR_SIDE);

void motor_adc_isr(void);


#endif /* SRC_MOTORS_H_ */
