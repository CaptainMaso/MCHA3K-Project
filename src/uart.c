#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <ctype.h>
#include "uart.h"
#include "circ_buffer.h"

CB_CREATE_STATIC(rx_buf, 64);

CB_CREATE_STATIC(tx_buf, 256);

// Declare stream functions
static int _uart_putchar(char c, FILE *);
static int _uart_getchar(FILE *);
// Declare and initialise stream used for stdio functions
static FILE _uart_str = FDEV_SETUP_STREAM(_uart_putchar, _uart_getchar, _FDEV_SETUP_RW);

int _uart_receive(void);

void uart_init(void)
{
	// Initialise software FIFO buffers
	cb_init(&rx_buf);
	cb_init(&tx_buf);
	
    // Set baud rate
    UBRRH = (uint8_t)(UART_CALC_BAUDRATE(UART_BAUD_RATE) >> 8);
    UBRRL = (uint8_t)(UART_CALC_BAUDRATE(UART_BAUD_RATE));

#ifdef UART_DOUBLESPEED
    UCSRA = _BV(U2X);
#endif
	// Enable receiver (& INT) and transmitter
    UCSRB =  _BV(RXCIE) | _BV(UDRIE) | _BV(RXEN) | _BV(TXEN);  
	
    // Asynchronous 8N1
    UCSRC = _BV(URSEL) | (3 << UCSZ0);
    
    // Initialise standard IO streams
    stdout = stdin = &_uart_str;
}

int _uart_getchar(__attribute__((unused)) FILE *stream)
{
    if (cb_is_empty(&rx_buf))
		return EOF;		//No character waiting
	else
		return cb_dequeue(&rx_buf);
}

int _uart_putchar(char c, __attribute__((unused)) FILE *stream)
{
    while(cb_is_full(&tx_buf)); // Wait until we have room in the TX buffer
	cb_queue(&tx_buf, c); // Buffer char
	UCSRB |= _BV(UDRIE); // Enable UDRE interrupt
	return 0; // Success
}

int _uart_receive(void)
{
    uint8_t status = UCSRA;         // Need to read UCSRA before reading UDR
    uint8_t c = UDR;                // Read UDR to clear RXC flag and swap hardware FIFO buffers

    if (status & (_BV(FE) | _BV(DOR) | _BV(PE)))
    {
        if (bit_is_set(status, FE))  printf_P(PSTR("*** UART framing error! ***\n"));
        if (bit_is_set(status, DOR)) printf_P(PSTR("*** UART data overrun! ***\n"));
        if (bit_is_set(status, PE))  printf_P(PSTR("*** UART parity error! ***\n"));
        c = EOF;
    }

    return c;
}

void uart_rxc_isr(void)
{
    uint8_t c;
	if ((uint8_t)(c = _uart_receive()) != (uint8_t)EOF)
	{
		if (isdigit(c) || isalpha(c) || c == '\b' || c == ' ' || c == '-' || c == '.' || c == '\n' || c == '\r')
		{
			cb_queue(&rx_buf, c);
			printf_P(PSTR("%c"), c);
		}
		else if (c == '\177')
		{
			cb_queue(&rx_buf, '\b');
			//printf_P(PSTR("\177"));
		}
	}
}

void uart_udre_isr(void)
{
    if (cb_is_empty(&tx_buf))
		UCSRB &= ~(_BV(UDRIE)); // Disable UDRE interrupt to avoid retriggering
	else
		UDR = cb_dequeue(&tx_buf);
}
