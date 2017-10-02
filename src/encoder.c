#include <stdint.h>
#include <avr/io.h>
#include "encoder.h"

static int32_t _count = 0;

// Forward decl of private helper functions
static void _encoder_enable_interrupts(void);
static void _encoder_disable_interrupts(void);

void encoder_init(void)
{
    // Enables Pull-Up resistors on PD2 & PD3
	PORTD |= _BV(PD2) | _BV(PD3);
	
    // Any logical change on INT0 and INT1 triggers an interrupt
	MCUCR |= _BV(ISC10) | _BV(ISC00);
	
    // Set default count
    _count = 0;

    // Enable interrupts INT0 and INT1
    _encoder_enable_interrupts();
}

void encoder_edge_A_isr(void)
{
    // Implement A edge logic to increment or decrement _count
	if (!!(PIND & _BV(PD2)) == !!(PIND & _BV(PD3))) _count++;
	else _count--;
}

void encoder_edge_B_isr(void)
{
    // Implement B edge logic to increment or decrement _count--
	if (!!(PIND & _BV(PD2)) == !!(PIND & _BV(PD3))) _count--;
	else _count++;
}

void encoder_set_count(int32_t count)
{
    // Atomically set _count
    _encoder_disable_interrupts();
    _count = count;
    _encoder_enable_interrupts();
}

int32_t encoder_get_count(void)
{
    // Atomically read _count
    _encoder_disable_interrupts();
    uint32_t count = _count;
    _encoder_enable_interrupts();
    return count;
}

int32_t encoder_pop_count(void)
{
    // Atomically read and reset _count
    _encoder_disable_interrupts();
    int32_t count = _count;
    _count = 0;
    _encoder_enable_interrupts();
    return count;
}

void _encoder_enable_interrupts(void)
{
    // Enable INT0 and INT1 interrupts
	GICR |= (_BV(INT0)) | (_BV(INT1));
}

void _encoder_disable_interrupts(void)
{
    // Disable INT0 and INT1 interrupts
	GICR &= ~((_BV(INT0)) | (_BV(INT1)));
}

