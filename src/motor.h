#ifndef MOTORS_H
#define MOTORS_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include "controller.h"

#define MAX_VOLTAGE 12
#define MAX_PWM 65535

#define ADC2CUR (512.0/5)

/*	Motor Control Pinout
 *  	PB2 = MR direction
 *  	PB3 = ML direction
 *  	(OC1B) PD4 = MR enable
 *  	(OC1A) PD5 = ML enable
 */

typedef enum {
	MOTOR_LEFT,
	MOTOR_RIGHT
} MOTOR_SIDE;

void motors_init(void);

void motor_adc_isr(void);

void motor_ctrl_run(uint32_t, states*);

int32_t motor_ctrl_alloc(MOTOR_SIDE, states*);

void motor_set_torque(MOTOR_SIDE, float);

void motors_set_pwm(MOTOR_SIDE, int32_t);
int32_t motors_get_pwm(MOTOR_SIDE);

int16_t motors_get_adc_reading(MOTOR_SIDE);

#endif /* SRC_MOTORS_H_ */
