#ifndef MOTORS_H
#define MOTORS_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include "controller.h"

#define MAX_VOLTAGE 12.0f
#define MAX_PWM 65535

#define ADC2CUR (0.5/255)

//#define FIXEDPOINT_MOTORCTRL

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

float motor_ctrl_alloc(MOTOR_SIDE, states*);

#ifdef FIXEDPOINT_MOTORCTRL
void motor_set_torque(MOTOR_SIDE, int32_t);
#else
void motor_set_torque(MOTOR_SIDE, float);
#endif

void motors_set_voltage(MOTOR_SIDE, float);
void motors_set_pwm(MOTOR_SIDE, int32_t);
int32_t motors_get_pwm(MOTOR_SIDE);

float motors_get_adc_reading(MOTOR_SIDE);

#endif /* SRC_MOTORS_H_ */
