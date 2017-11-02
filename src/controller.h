#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "cmd_parser.h"

//#define FIXEDPOINT_STATES


typedef struct _states
{
#ifdef FIXEDPOINT_STATES
	// Pendulum States
	int32_t Theta;
	int32_t dTheta;

	// Motor States
	int32_t Phi_ML;
	int32_t Phi_MR;

	int32_t dPhi_ML;
	int32_t dPhi_MR;

	int16_t Current_ML;
	int16_t Current_MR;

	// Internal States
	int32_t Bias;
	int16_t Vref;
#else
	// Pendulum States
	float Theta;
	float dTheta;

	// Motor States
	float Phi_ML;
	float Phi_MR;

	float dPhi_ML;
	float dPhi_MR;

	float Current_ML;
	float Current_MR;

	// Internal States
	float Bias;
	float Vref;
#endif
} states;

typedef enum
{
	CTRL_OFF,
	CTRL_NORMAL,
	CTRL_HIL,
	CTRL_IMUONLY
} CTRL_MODE;

void ctrl_init(void);
void ctrl_run(void);

CMD_STATUS ctrl_cmd(int argc, const char *argv[]);

void ctrl_set_mode(CTRL_MODE);

void ctrl_isr(void);


//Setter/Getter
states* ctrl_get_states(void);

void ctrl_set_states(states);

#endif
