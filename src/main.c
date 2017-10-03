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

int main(void)
{
    // Initialise modules
    uart_init();
    tasks_init();
    
    // Enable global interrupts
    sei();      

    // Wait a second at startup

    // Send initial string
    printf_P(PSTR("%c"), (char)0x05);



    for(;/*ever*/;)
    {
        if (tasks_triggered())
        {
            tasks_run();
        }
        else
        {
            clb_process(&clb);
        }
    }
    return 0;
	/*DDRA |= _BV(PA7);

	for(;/*ever*//*;)
	{
		PORTA |= _BV(PA7);
		_delay_ms(250);
		PORTA &= ~(_BV(PA7));
		_delay_ms(250);
	}*/
}
