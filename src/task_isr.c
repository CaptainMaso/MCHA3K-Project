#include <avr/interrupt.h>
#include "task.h"

ISR(TIMER2_COMP_vect)
{
    task_trigger_isr();
}