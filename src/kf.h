#ifndef KF_H
#define KF_H

#include <stdint.h>
#include <stdbool.h>
#include "controller.h"
#include "motor.h"

#define DEG2RAD (3.141592/180)
#define RAD2DEG (180/3.141592)

#define THETAGAIN (500.0/32767.0)
#define AX_OFFSET 0
#define AZ_OFFSET 0
#define ATAN_OFFSET 0

typedef struct {
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

} covariance;

void kf_init(void);
void kf_timestep(float, states *);

void kf_correct(states *);

void kf_gyrocorrection(float , states *);
void kf_acccorrection(float , states *);
void kf_enccorrection(MOTOR_SIDE, float, states *);

int16_t imu_get_ax(void);
int16_t imu_get_ay(void);
int16_t imu_get_az(void);
int16_t imu_get_gx(void);
int16_t imu_get_gy(void);
int16_t imu_get_gz(void);

int16_t imu_get_atanTheta(void);

#endif
