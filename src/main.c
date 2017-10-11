#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <util/delay.h>
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "task.h"
#include "encoders.h"
#include "motor.h"
#include "i2cmaster.h"
//#include "imu.h"
#include "mpu6050.h"

CLB_CREATE_STATIC(clb, 80);

void led_task_callback(void);

static task_s _led_task = {
		.interval = 500,
		.callback = &led_task_callback,
		.id = 255
};

int main(void)
{
    // Initialise modules
    uart_init();
    i2c_init();
    motors_init();
    encoder_init();
    mpu6050_init();
    tasks_init(0.001);

    // Enable global interrupts
	sei();

    // Wait a second at startup

    // Send initial string
    printf_P(PSTR("Program Started\n"));

    if (mpu6050_testConnection())
		printf_P(PSTR("I AM THE MPU6050\n"));
	else
		printf_P(PSTR("I AM NOT THE MPU6050\n"));
    //motors_set_pwm(MOTOR_RIGHT, 30000);

    DDRA |= _BV(PA7);
    _led_task.interval = tasks_time_interval_to_task_interval(0.5);
    tasks_add(&_led_task);

	if (_led_task.id != 255)
		tasks_enable();

    for(;/*ever*/;)
    {
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
