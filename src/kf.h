#ifndef KF_H
#define KF_H

#include <stdint.h>
#include <stdbool.h>
#include "controller.h"
#include "motor.h"

#define DEG2RAD (3.141592/180)
#define RAD2DEG (180/3.141592)

#define THETAGAIN (8.7266/32767.0)
#define AX_OFFSET 0
#define AZ_OFFSET 0
#define ATAN_OFFSET 0

//#define FIXEDPOINT_KALMAN

typedef struct {
#ifdef FIXEDPOINT_KALMAN
	// IMU Covariance
	uint32_t WW;		//Omega-Omega Covariance
	uint32_t WT;		//Omega-Theta Covariance
	uint32_t WB;		//Omega-Bias Covariance
	uint32_t TT;		//Theta-Theta Covariance
	uint32_t TB;		//Theta-Bias Covariance
	uint32_t BB;		//Bias-Bias Covariance

	// Encoder Left Covariance
	uint32_t SS_L;		//Psi-Psi Covariance (Psi = dPhi)
	uint32_t SP_L;		//Psi-Phi Covariance
	uint32_t PP_L;		//Phi-Phi Covariance

	// Encoder Right Covariance
	uint32_t SS_R;		//Psi-Psi Covariance (Psi = dPhi)
	uint32_t SP_R;		//Psi-Phi Covariance
	uint32_t PP_R;		//Phi-Phi Covariance
#else
	// IMU Covariance
	float WW;		//Omega-Omega Covariance
	float WT;		//Omega-Theta Covariance
	float WB;		//Omega-Bias Covariance
	float TT;		//Theta-Theta Covariance
	float TB;		//Theta-Bias Covariance
	float BB;		//Bias-Bias Covariance

	// Encoder Left Covariance
	float SS_L;		//Psi-Psi Covariance (Psi = dPhi)
	float SP_L;		//Psi-Phi Covariance
	float PP_L;		//Phi-Phi Covariance

	// Encoder Right Covariance
	float SS_R;		//Psi-Psi Covariance (Psi = dPhi)
	float SP_R;		//Psi-Phi Covariance
	float PP_R;		//Phi-Phi Covariance
#endif
} covariance;

void kf_init(void);

void kf_correct(states *);
void kf_timestep(states *);

#ifdef FIXEDPOINT_KALMAN
void kf_gyrocorrection(int32_t , states *);
void kf_acccorrection(int32_t , states *);
void kf_enccorrection(MOTOR_SIDE, int32_t, states *);
#else
void kf_gyrocorrection(float , states *);
void kf_acccorrection(float , states *);
void kf_enccorrection(MOTOR_SIDE, float, states *);
#endif

int16_t imu_get_ax(void);
int16_t imu_get_ay(void);
int16_t imu_get_az(void);
int16_t imu_get_gx(void);
int16_t imu_get_gy(void);
int16_t imu_get_gz(void);

#ifdef FIXEDPOINT_KALMAN
int16_t imu_get_atanTheta(void);
int32_t atan_lookup(int16_t, int16_t);
#else
float imu_get_atanTheta(void);
float atan_lookup(float, float);
#endif

#endif
