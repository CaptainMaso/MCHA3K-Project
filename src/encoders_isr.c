#include <avr/interrupt.h>
#include "encoders.h"

ISR(INT0_vect)
{
    encoder_edge_MR_isr();
}

ISR(INT1_vect)
{
    encoder_edge_ML_isr();
}
