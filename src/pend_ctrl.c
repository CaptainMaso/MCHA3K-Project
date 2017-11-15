#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <kf.h>
#include <stdbool.h>
#include <util/atomic.h>

#include "pend_ctrl.h"
#include "controller.h"
#include "encoders.h"

#include "ctrl_param.def"
#include "pend_ctrl_param.def"


static uint32_t ctrl_last_run = 0;

#ifdef FIXEDPOINT_PENDCTRL
static int32_t err_int;
#else
static float err_int;
#endif

void pend_ctrl_init(void)
{
	err_int = 0;
}

#ifdef FIXEDPOINT_PENDCTRL
int32_t pend_ctrl_alloc(states *ctrl_states)
{
	int32_t dPhi = (ctrl_states->dPhi_ML + ctrl_states->dPhi_MR)/2;

	err_int = ((Kerr_int * err_int) + (Kerr_dTheta * ctrl_states->dTheta) + (Kerr_dPhi * dPhi) + (Kerr_Vref * ctrl_states->Vref))/K_FP_GAIN;
	int32_t Torque = ((K_int * err_int) + (K_Theta * ctrl_states->Theta) + (K_dTheta * ctrl_states->dTheta) + (K_dPhi * dPhi))/K_FP_GAIN;

	return Torque;
}
#else
float pend_ctrl_alloc(states *ctrl_states)
{
	float dPhi = (ctrl_states->dPhi_ML + ctrl_states->dPhi_MR)/2;

	err_int = ((Kerr_int * err_int) + (Kerr_dTheta * ctrl_states->dTheta) + (Kerr_dPhi * dPhi) + (Kerr_Vref * ctrl_states->Vref));
	float Torque = ((K_int * err_int) + (K_Theta * ctrl_states->Theta) + (K_dTheta * ctrl_states->dTheta) + (K_dPhi * dPhi));

	return Torque;
}
#endif

void pend_ctrl_run(uint32_t ctrl_count, states *ctrl_states)
{
	if (ctrl_count == 0)
		ctrl_last_run = 0;
	if ((ctrl_count % pend_interval) == 0 && ctrl_count != ctrl_last_run)
	{
		PORTA |= _BV(PA5);
		if (ctrl_count - ctrl_last_run <= pend_interval)
		{
			float Torque = pend_ctrl_alloc(ctrl_states);

			motor_set_torque(MOTOR_LEFT, Torque/2.0);
			motor_set_torque(MOTOR_RIGHT, Torque/2.0);
			ctrl_last_run = ctrl_count;
		}
		else
		{
			printf_P(PSTR("ERROR: Pend ctrl lagging, disabling control (LAG: %"PRIu32" counts)\n"), ctrl_count - ctrl_last_run - pend_interval);
			ctrl_set_mode(CTRL_OFF);
		}
		PORTA &= ~_BV(PA5);
	}
}
