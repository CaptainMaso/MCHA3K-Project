#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <util/delay.h>
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "task.h"

CLB_CREATE_STATIC(clb, 80);

void led_task_callback(void);
void led_task2_callback(void);

static uint8_t _led_task_id;
static task_s _led_task = {
		500, &led_task_callback
};

int main(void)
{
    // Initialise modules
    uart_init();
<<<<<<< HEAD
    tasks_init();
=======
    tasks_init(0.001);
>>>>>>> TaskScheduler
    
    // Enable global interrupts
	sei();

    // Wait a second at startup

    // Send initial string
    printf_P(PSTR("Program Started\n"));

    DDRA |= _BV(PA7);

    _led_task_id = tasks_add(&_led_task);

	if (_led_task_id != 255)
		tasks_enable();

    for(;/*ever*/;)
    {
<<<<<<< HEAD
        if (tasks_triggered())
        {
            tasks_run();
=======
        if (tasks_ready())
        {
        	tasks_run();
>>>>>>> TaskScheduler
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
