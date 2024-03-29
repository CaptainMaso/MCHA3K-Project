#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <kf.h>
#include <util/delay.h>
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "task.h"
#include "encoders.h"
#include "motor.h"
#include "controller.h"
#include "i2cmaster.h"
#include "mpu6050.h"

CLB_CREATE_STATIC(clb, 80);

void led_task_callback(void);
//void kalman_task_callback(void);

static task_s _led_task = {
		.interval = 500,
		.callback = &led_task_callback,
		.id = 255,
		.enabled = true
};

//static task_s _kalman_task = {
//		.interval = 200,
//		.callback = &kalman_task_callback,
//		.id = 255
//};


int main(void)
{
    // Initialise modules
    uart_init();
    i2c_init();
    tasks_init(0.001);

    ctrl_init();

    //motors_init();
    //OCR1A = 0xFFFF;

    // Enable global interrupts
	sei();

    // Send initial string
    //printf_P(PSTR("Program Started\n"));

    /*if (mpu6050_testConnection())
		printf_P(PSTR("I AM THE MPU6050\n"));
	else
		printf_P(PSTR("I AM NOT THE MPU6050\n"));*/
    //motors_set_pwm(MOTOR_RIGHT, 30000);

    DDRA |= _BV(PA7) | _BV(PA6) | _BV(PA5) | _BV(PA4);

    _led_task.interval = tasks_time_interval_to_task_interval(0.5);
    //_kalman_task.interval = tasks_time_interval_to_task_interval(0.1);
    tasks_add(&_led_task);
//    tasks_add(&_kalman_task);

	if (_led_task.id != 255)
		tasks_enable();
	else
		printf_P(PSTR("Could not start tasks\n"));

    for(;/*ever*/;)
    {
    	ctrl_run();
        if (tasks_ready())
        {
        	tasks_run();
        }
        else
        {
            clb_process(&clb);
        }
    }
    return 0;
}

void led_task_callback(void)
{
	if(bit_is_set(PORTA, PA7))
	{
		PORTA &= ~(_BV(PA7));
	}
	else
	{
		PORTA |= _BV(PA7);
	}
}

//void kalman_task_callback(void)
//{
//	imu_kalman_state* data = imu_get_kalman_state();
//	imu_timestep(0.1);
//	printf_P(PSTR("T:%.4g, mT:%.4g, dT:%.4g, mdT:%.4g, B:%g, WW: %.4g, TT: %.4g, BB: %.4g\n"), (float)(data->Theta)*THETAGAIN, imu_get_atanTheta()*THETAGAIN, (float)(data->dTheta)*THETAGAIN, (float)imu_get_gy()*THETAGAIN, data->Bias*RAD2DEG,
//																			data->Pcov[COV_WW], data->Pcov[COV_TT], data->Pcov[COV_BB]);
//}
