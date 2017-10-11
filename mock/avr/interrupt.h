#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "avr/io.h"

#define sei() SREG |= _BV(SREG_I)
#define cli() SREG &= ~_BV(SREG_I)

#define ISR_BLOCK
#define ISR_NOBLOCK

#define EMPTY_INTERRUPT(vector)

#define EMPTY_INTERRUPT(vector) \
    void vector (void); \
    void vector (void) { return; }

#define SIGNAL(vector) \
    void vector (void); \
    void vector (void)

#define ISR(vector, ...) \
    void vector (void) __VA_ARGS__; \
    void vector (void)

#endif