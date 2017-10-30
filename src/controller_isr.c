#include <avr/interrupt.h>
#include "controller.h"

ISR(TIMER0_COMP_vect)
{
    ctrl_isr();
}
