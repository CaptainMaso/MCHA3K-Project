#ifndef UTIL_DELAY_H
#define UTIL_DELAY_H

#include "avr/io.h"
#include <stdint.h>

void _delay_ms (uint16_t ms) { (void)(ms); return; }

void _delay_us (uint16_t us) { (void)(us); return; }

#endif
