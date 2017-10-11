#include <avr/interrupt.h>
#include "motor.h"

ISR(ADC_vect) // ADC conversion complete
{
    motor_adc_isr();
}

ISR(TIMER1_OVF_vect) // Timer1 overflow
{
    // (Optional) TODO: diagnostic code for hardware testing
}
