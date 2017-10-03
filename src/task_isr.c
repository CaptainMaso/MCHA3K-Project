#include <avr/interrupt.h>
#include "task.h"

ISR(TIMER2_COMP_vect)
{
    tasks_trigger_isr();
}