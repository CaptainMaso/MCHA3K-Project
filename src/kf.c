#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <kf.h>
#include <util/atomic.h>

#include "mpu6050.h"
#include "encoders.h"
#include "controller.h"

#include "ctrl_param.def"
#include "kalman_param.def"

static int16_t _ax = 0;
static int16_t _ay = 0;
static int16_t _az = 0;
static int16_t _gx = 0;
static int16_t _gy = 0;
static int16_t _gz = 0;

static covariance P;		//Covariance Variables

#ifdef FIXEDPOINT_KALMAN
void kf_init(void)
{
	//Initialise MPU6050
	mpu6050_init();

	mpu6050_getRawData(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);

	//Initialise Kalman Filter
	P.WW = (T_KF_FP*K_pw)/(K_FP_GAIN);
	P.WT = (T_KF*T_KF*K_pw/2)/(K_FP_GAIN);
	P.WB = 0;
	P.TT = (T_KF*T_KF*T_KF*K_pw/3)/(K_FP_GAIN);
	P.TB = 0;
	P.BB = (T_KF_FP*K_pb)/(K_FP_GAIN);

	P.PP_L = (T_KF*T_KF*T_KF*K_ps/3)/(K_FP_GAIN);
	P.SP_L = (T_KF*T_KF*K_ps/2)/(K_FP_GAIN);
	P.SS_L = (T_KF_FP*K_ps)/(K_FP_GAIN);

	P.PP_R = (T_KF*T_KF*T_KF*K_ps/3)/(K_FP_GAIN);
	P.SP_R = (T_KF*T_KF*K_ps/2)/(K_FP_GAIN);
	P.SS_R = (T_KF*K_ps)/(K_FP_GAIN);
}

void kf_correct(states *curState)
{
	mpu6050_getRawData(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);
	//_gy += 2621;
	kf_gyrocorrection(-_gy, curState);
	kf_acccorrection(imu_get_atanTheta(), curState);
	kf_enccorrection(MOTOR_LEFT, encoder_get_count(MOTOR_LEFT)*2*M_PI/CPR,curState);
	kf_enccorrection(MOTOR_RIGHT, encoder_get_count(MOTOR_RIGHT)*2*M_PI/CPR,curState);
}

void kf_timestep(states *curState)
{
	//State Update
	curState->Theta 	= T_TS*curState->dTheta + curState->Theta;
	//curState->dTheta 	= curState->dTheta;
	//curState->Bias 		= curState->Bias;

	//curState->dPhi_ML   = curState->dPhi_ML;
	//curState->dPhi_MR   = curState->dPhi_MR;
	curState->Phi_ML	= T_TS*curState->dPhi_ML + curState->Phi_ML;
	curState->Phi_MR	= T_TS*curState->dPhi_MR + curState->Phi_MR;

	//Covariance Update
	covariance P_n;		//Covariance Variables

	P_n.WW = P.WW + (T_TS*K_qw)/K_FP_GAIN;
	P_n.WT = P.WT + T_TS*P.WW/K_FP_GAIN + T_TS*T_TS*K_qw/2/K_FP_GAIN/K_FP_GAIN;
	P_n.WB = P.WB;
	P_n.TT = P.TT + 2*T_TS*P.WT/K_FP_GAIN + T_TS*T_TS*P.WW/K_FP_GAIN/K_FP_GAIN + T_TS*T_TS*T_TS*K_qw/3/K_FP_GAIN/K_FP_GAIN/K_FP_GAIN;
	P_n.TB = P.TB + T_TS*P.WB/K_FP_GAIN;
	P_n.BB = P.BB + T_TS*K_qb/K_FP_GAIN;

	P_n.PP_L = P.PP_L + 2*P.SP_L*T_TS/K_FP_GAIN 	+ T_TS*T_TS*T_TS*K_qs/3/K_FP_GAIN/K_FP_GAIN/K_FP_GAIN;
	P_n.SP_L = P.SP_L + P.SS_L*T_TS/K_FP_GAIN 	+ T_TS*T_TS*K_qs/2/K_FP_GAIN/K_FP_GAIN;
	P_n.SS_L = P.SS_L + T_TS * K_qs/K_FP_GAIN;

	P_n.PP_R = P.PP_R + 2*P.SP_R*T_TS/K_FP_GAIN 	+ T_TS*T_TS*T_TS*K_qs/3/K_FP_GAIN/K_FP_GAIN/K_FP_GAIN;
	P_n.SP_R = P.SP_R + P.SS_R*T_TS/K_FP_GAIN	+ T_TS*T_TS*K_qs/2/K_FP_GAIN/K_FP_GAIN;
	P_n.SS_R = P.SS_R + T_TS * K_qs/K_FP_GAIN;

	P = P_n;
}

void kf_gyrocorrection(int32_t yw, states *curState)
{
	// Kalman Gain
	uint32_t divider = (P.WW+ 2*P.WB + P.BB + K_RW_T);
	uint32_t kw = (P.WW + P.WB)*K_FP_GAIN/divider;
	uint32_t kt = (P.WT + P.TB)*K_FP_GAIN/divider;
	uint32_t kb = (P.WB + P.BB)*K_FP_GAIN/divider;

#ifdef FIXEDPOINT_STATES
	// State Correction
	int32_t error = (yw-curState->dTheta-curState->Bias);
	curState->dTheta 	=  curState->dTheta + kw*error;
	curState->Theta 	=  curState->Theta 	+ kt*error;
	curState->Bias 		=  curState->Bias 	+ kb*error;
#else
	// State Correction
	float error = ((yw/(float)K_FP_GAIN)-curState->dTheta-curState->Bias);
	curState->dTheta 	=  curState->dTheta + kw*error/K_FP_GAIN;
	curState->Theta 	=  curState->Theta 	+ kt*error/K_FP_GAIN;
	curState->Bias 		=  curState->Bias 	+ kb*error/K_FP_GAIN;
#endif
	// Covariance Correction
	covariance P_n = P;		//Covariance Variables

	P_n.WW = P.WW - P.WB*kw/K_FP_GAIN - P.WW*kw/K_FP_GAIN;
	P_n.WT = P.WT - P.TB*kw/K_FP_GAIN - P.WT*kw/K_FP_GAIN;
	P_n.WB = P.WB - P.BB*kw/K_FP_GAIN - P.WB*kw/K_FP_GAIN;
	P_n.TT = P.TT - P.TB*kt/K_FP_GAIN - P.WT*kt/K_FP_GAIN;
	P_n.TB = P.TB - P.BB*kt/K_FP_GAIN - P.WB*kt/K_FP_GAIN;
	P_n.BB = P.BB - P.BB*kb/K_FP_GAIN - P.WB*kb/K_FP_GAIN;

	P = P_n;
}

void kf_acccorrection(int32_t yt, states *curState)
{
	// Kalman Gain
	uint32_t divider = (P.TT + K_RT_T);
	uint32_t kw = P.WT*K_FP_GAIN/divider;
	uint32_t kt = P.TT*K_FP_GAIN/divider;
	uint32_t kb = P.TB*K_FP_GAIN/divider;

	// State Correction
#ifdef FIXEDPOINT_STATES
	// State Correction
	int32_t error = (yt-curState->Theta);
	curState->dTheta 	=  curState->dTheta + kw*error;
	curState->Theta 	=  curState->Theta 	+ kt*error;
	curState->Bias 		=  curState->Bias 	+ kb*error;
#else
	// State Correction
	float error = ((yt/(float)K_FP_GAIN)-curState->Theta);
	curState->dTheta 	=  curState->dTheta + kw*error/K_FP_GAIN;
	curState->Theta 	=  curState->Theta 	+ kt*error/K_FP_GAIN;
	curState->Bias 		=  curState->Bias 	+ kb*error/K_FP_GAIN;
#endif

	// Covariance Correction
	covariance P_n = P;		//Covariance Variables

	P_n.WW = P.WW - P.WT*kw/K_FP_GAIN;
	P_n.WT = P.WT - P.TT*kw/K_FP_GAIN;
	P_n.WB = P.WB - P.TB*kw/K_FP_GAIN;
	P_n.TT = P.TT - P.TT*kt/K_FP_GAIN;
	P_n.TB = P.TB - P.TB*kt/K_FP_GAIN;
	P_n.BB = P.BB - P.TB*kb/K_FP_GAIN;

	P = P_n;
}

void kf_enccorrection(MOTOR_SIDE side, int32_t phi, states *curState)
{
	switch (side)
	{
	case MOTOR_LEFT:
	{
		// Kalman Gain
		uint32_t divider = (P.PP_L + K_RP_T);
		uint32_t kp = P.PP_L*K_FP_GAIN/divider;
		uint32_t ks = P.SP_L*K_FP_GAIN/divider;

		// State Correction
#ifdef FIXEDPOINT_STATES
		int32_t error 		= (phi - curState->Phi_ML);
		curState->Phi_ML 	=  curState->Phi_ML 	+ kp*error;
		curState->dPhi_ML 	=  curState->dPhi_ML 	+ ks*error;
#else
		float error 		= ((float)phi/K_FP_GAIN - curState->Phi_ML);
		curState->Phi_ML 	=  curState->Phi_ML 	+ (float)kp*error/K_FP_GAIN;
		curState->dPhi_ML 	=  curState->dPhi_ML 	+ (float)ks*error/K_FP_GAIN;
#endif

		// Covariance Correction
		covariance P_n = P;		//Covariance Variables

		P_n.PP_L = P.PP_L - P.PP_L*kp/K_FP_GAIN;
		P_n.SP_L = P.SP_L - P.SP_L*kp/K_FP_GAIN;
		P_n.SS_L = P.SS_L - P.SP_L*ks/K_FP_GAIN;

		P = P_n;
	} break;
	case MOTOR_RIGHT:
	{
		// Kalman Gain
		uint32_t divider = (P.PP_R + K_RP_T);
		uint32_t kp = P.PP_R*K_FP_GAIN/divider;
		uint32_t ks = P.SP_R*K_FP_GAIN/divider;

		// State Correction
#ifdef FIXEDPOINT_STATES
		int32_t error 		= (phi - curState->Phi_MR);
		curState->Phi_MR 	=  curState->Phi_MR 	+ kp*error;
		curState->dPhi_MR 	=  curState->dPhi_MR 	+ ks*error;
#else
		float error 		= ((float)phi/K_FP_GAIN - curState->Phi_MR);
		curState->Phi_MR 	=  curState->Phi_MR 	+ kp*error/K_FP_GAIN;
		curState->dPhi_MR 	=  curState->dPhi_MR 	+ ks*error/K_FP_GAIN;
#endif

		// Covariance Correction
		covariance P_n = P;		//Covariance Variables

		P_n.PP_R = P.PP_R - P.PP_R*kp/K_FP_GAIN;
		P_n.SP_R = P.SP_R - P.SP_R*kp/K_FP_GAIN;
		P_n.SS_R = P.SS_R - P.SP_R*ks/K_FP_GAIN;

		P = P_n;
	} break;
	default:
		break;
	}
}

int32_t atan_lookup(int16_t y, int16_t x)
{
	uint16_t y_abs = abs(y);
	uint16_t x_abs = abs(x);
	int32_t res;

	if (y_abs > x_abs)
	{
		if (x_abs == 0)
			res = M_PI*K_FP_GAIN/2;
		else
		{
			uint8_t val = (uint8_t)((float)y_abs/x_abs)*atan_table_len;
			if (val < atan_table_len - 1)
				res = M_PI*K_FP_GAIN/2 - (atan_table[val] + atan_table[val+1])/2;
			else
				res = atan_table[val];
		}
	}
	else
	{
		if (y_abs == 0)
			res = 0;
		else
		{
			uint8_t val = (uint8_t)((float)y_abs/x_abs)*atan_table_len;
			if (val < atan_table_len - 1)
				res = (atan_table[val]+atan_table[val+1])/2;
			else
				res = atan_table[val];
		}
	}
	if (y < 0)
		res = - res;
	if (x < 0)
		res = M_PI - res;
	return res;
}

int16_t imu_get_atanTheta(void)
{

	return (int16_t) + ATAN_OFFSET;
}
#else
void kf_init(void)
{
	//Initialise MPU6050
	mpu6050_init();

	mpu6050_getRawData(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);

	//Initialise Kalman Filter
	P.WW = T_KF*K_pw;
	P.WT = T_KF*T_KF*K_pw/2;
	P.WB = 0;
	P.TT = T_KF*T_KF*T_KF*K_pw/3;
	P.TB = 0;
	P.BB = T_KF*K_pb;

	P.PP_L = T_KF*T_KF*T_KF*K_ps/3;
	P.SP_L = T_KF*T_KF*K_ps/2;
	P.SS_L = T_KF*K_ps;

	P.PP_R = T_KF*T_KF*T_KF*K_ps/3;
	P.SP_R = T_KF*T_KF*K_ps/2;
	P.SS_R = T_KF*K_ps;
}

void kf_correct(states *curState)
{
	mpu6050_getRawData(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);
	//mpu6050_readBytes(MPU6050_RA_GYRO_YOUT_H, 2, (uint8_t *)(&_gy));
	//mpu6050_readBytes(MPU6050_RA_ACCEL_XOUT_H, 2, (uint8_t *)(&_ax));
	//mpu6050_readBytes(MPU6050_RA_ACCEL_ZOUT_H, 2, (uint8_t *)(&_az));

	kf_gyrocorrection((float)-_gy*GY2RAD, curState);
	kf_acccorrection(imu_get_atanTheta(), curState);
	kf_enccorrection(MOTOR_LEFT, encoder_get_count(MOTOR_LEFT)*2*M_PI/CPR, curState);
	kf_enccorrection(MOTOR_RIGHT, encoder_get_count(MOTOR_RIGHT)*2*M_PI/CPR,curState);
}

void kf_timestep(states *curState)
{
	//State Update

	curState->Theta 	= T_TS*curState->dTheta + curState->Theta;
	//curState->dTheta 	= curState->dTheta;
	//curState->Bias 		= curState->Bias;

	//curState->dPhi_ML   = curState->dPhi_ML;
	//curState->dPhi_MR   = curState->dPhi_MR;
	curState->Phi_ML	= T_TS*curState->dPhi_ML + curState->Phi_ML;
	curState->Phi_MR	= T_TS*curState->dPhi_MR + curState->Phi_MR;

	//Covariance Update
	covariance P_n = P;		//Covariance Variables

	P_n.WW = P.WW + T_TS*K_qw;
	P_n.WT = P.WT + T_TS*P.WW + T_TS*T_TS*K_qw/2;
	P_n.WB = P.WB;
	P_n.TT = P.TT + 2*T_TS*P.WT + T_TS*T_TS*P.WW + T_TS*T_TS*T_TS*K_qw/3;
	P_n.TB = P.TB + T_TS*P.WB;
	P_n.BB = P.BB + T_TS*K_qb;

	P_n.PP_L = P.PP_L + 2*P.SP_L*T_TS 	+ T_TS*T_TS*T_TS*K_qs/3;
	P_n.SP_L = P.SP_L + P.SS_L*T_TS 	+ T_TS*T_TS*K_qs/2;
	P_n.SS_L = P.SS_L + T_TS * K_qs;

	P_n.PP_R = P.PP_R + 2*P.SP_R*T_TS 	+ T_TS*T_TS*T_TS*K_qs/3;
	P_n.SP_R = P.SP_R + P.SS_R*T_TS 	+ T_TS*T_TS*K_qs/2;
	P_n.SS_R = P.SS_R + T_TS * K_qs;

	P = P_n;
	//PORTA &= ~(_BV(PA6));
}

void kf_gyrocorrection(float yw, states *curState)
{
	// Kalman Gain
	float divider = 1.0/(P.WW+ 2*P.WB+ P.BB + K_RW_T);
	float kw = (P.WW + P.WB)*divider;
	float kt = (P.WT + P.TB)*divider;
	float kb = (P.WB + P.BB)*divider;

	// State Correction
	float error = (yw-curState->dTheta-curState->Bias);
	curState->dTheta 	=  curState->dTheta + kw*error;
	curState->Theta 	=  curState->Theta 	+ kt*error;
	curState->Bias 		=  curState->Bias 	+ kb*error;

	// Covariance Correction
	covariance P_n = P;		//Covariance Variables

	P_n.WW = P.WW - P.WB*kw - P.WW*kw;
	P_n.WT = P.WT - P.TB*kw - P.WT*kw;
	P_n.WB = P.WB - P.BB*kw - P.WB*kw;
	P_n.TT = P.TT - P.TB*kt - P.WT*kt;
	P_n.TB = P.TB - P.BB*kt - P.WB*kt;
	P_n.BB = P.BB - P.BB*kb - P.WB*kb;

	P = P_n;
}

void kf_acccorrection(float yt, states *curState)
{
	// Kalman Gain
	float divider = 1.0/(P.TT + K_RT_T);
	float kw = P.WT*divider;
	float kt = P.TT*divider;
	float kb = P.TB*divider;

	// State Correction
	float error 		= (yt-curState->Theta);
	curState->dTheta 	=  curState->dTheta + kw*error;
	curState->Theta 	=  curState->Theta 	+ kt*error;
	curState->Bias 		=  curState->Bias 	+ kb*error;

	// Covariance Correction
	covariance P_n = P;		//Covariance Variables

	P_n.WW = P.WW - P.WT*kw;
	P_n.WT = P.WT - P.TT*kw;
	P_n.WB = P.WB - P.TB*kw;
	P_n.TT = P.TT - P.TT*kt;
	P_n.TB = P.TB - P.TB*kt;
	P_n.BB = P.BB - P.TB*kb;

	P = P_n;
}

void kf_enccorrection(MOTOR_SIDE side, float phi, states *curState)
{
	switch (side)
	{
	case MOTOR_LEFT:
	{
		// Kalman Gain
		float divider = 1.0/(P.PP_L + K_RP_T);
		float kp = P.PP_L*divider;
		float ks = P.SP_L*divider;

		// State Correction
		float error 		= (phi - curState->Phi_ML);
		curState->Phi_ML 	=  curState->Phi_ML 	+ kp*error;
		curState->dPhi_ML 	=  curState->dPhi_ML 	+ ks*error;

		// Covariance Correction
		covariance P_n = P;		//Covariance Variables

		P_n.PP_L = P.PP_L - P.PP_L*kp;
		P_n.SP_L = P.SP_L - P.SP_L*kp;
		P_n.SS_L = P.SS_L - P.SP_L*ks;

		P = P_n;
	} break;
	case MOTOR_RIGHT:
	{
		// Kalman Gain
		float divider = 1.0/(P.PP_R + K_RP_T);
		float kp = P.PP_R*divider;
		float ks = P.SP_R*divider;

		// State Correction
		float error 		= (phi - curState->Phi_MR);
		curState->Phi_MR 	=  curState->Phi_MR 	+ kp*error;
		curState->dPhi_MR 	=  curState->dPhi_MR 	+ ks*error;

		// Covariance Correction
		covariance P_n = P;		//Covariance Variables

		P_n.PP_R = P.PP_R - P.PP_R*kp;
		P_n.SP_R = P.SP_R - P.SP_R*kp;
		P_n.SS_R = P.SS_R - P.SP_R*ks;

		P = P_n;
	} break;
	default:
		break;
	}
}

float imu_get_atanTheta(void)
{
	//return atan_lookup(-(_az + AX_OFFSET), _ax + AX_OFFSET);
	return atan2(-(_az + AX_OFFSET), _ax + AX_OFFSET)-0.148;
}

float atan_lookup(float y, float x)
{
	float y_abs = fabs(y);
	float x_abs = fabs(x);
	float res;

	if (y_abs > x_abs)
	{
		if (x_abs == 0)
			res = M_PI/2;
		else
		{
			uint8_t val = (uint8_t)(y_abs/x_abs)*atan_table_len;
			if (val < atan_table_len - 1)
				res = M_PI/2 - (atan_table[val] + atan_table[val+1])/2;
			else
				res = atan_table[val];
		}
	}
	else
	{
		if (y_abs == 0)
			res = 0;
		else
		{
			uint8_t val = (uint8_t)((float)y_abs/x_abs)*atan_table_len;
			if (val < atan_table_len - 1)
				res = (atan_table[val]+atan_table[val+1])/2;
			else
				res = atan_table[val];
		}
	}
	if (y < 0)
		res = - res;
	if (x < 0)
		res = M_PI - res;
	return res;
}
#endif

int16_t imu_get_ax(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _ax + AX_OFFSET;
	}

	return tmp;
}
int16_t imu_get_ay(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _ay;
	}

	return tmp;
}
int16_t imu_get_az(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _az+ AZ_OFFSET;
	}

	return tmp;
}
int16_t imu_get_gx(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _gx;
	}

	return tmp;
}
int16_t imu_get_gy(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _gy;
	}

	return tmp;
}
int16_t imu_get_gz(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _gz;
	}

	return tmp;
}

