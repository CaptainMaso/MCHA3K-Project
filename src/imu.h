#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>

#define DEGTORAD 3.141592/180
#define RADTODEG 180/3.141592

#define IMU_T 0.01
#define THETAGAIN 500/32767*3.141592/180
#define AX_OFFSET 0
#define AZ_OFFSET 0
#define ATAN_OFFSET -5*DEGTORAD

typedef enum {
	COV_WW = 0,		//Omega-Omega Covariance
	COV_WT = 1,		//Omega-Theta Covariance
	COV_WB = 2,		//Omega-Bias Covariance
	COV_TT = 3,		//Theta-Theta Covariance
	COV_TB = 4,		//Theta-Bias Covariance
	COV_BB = 5		//Bias-Bias Covariance
} COVARIANCE_T;

typedef struct _imu_kalman_state
{
	float Pcov[6];		//Covariance Variables
	float Theta;		//Theta Estimate
	float dTheta;	//dTheta Estimate
	float Bias;		//Bias Estimate
} imu_kalman_state;

void imu_init(void);
void imu_timestep(float TimeStep);

int16_t imu_get_ax(void);
int16_t imu_get_ay(void);
int16_t imu_get_az(void);
int16_t imu_get_gx(void);
int16_t imu_get_gy(void);
int16_t imu_get_gz(void);

float imu_get_atanTheta(void);
float imu_get_Theta(void);
float imu_get_dTheta(void);
float imu_get_Bias(void);
float imu_get_PWW(void);
float imu_get_PTT(void);
float imu_get_PBB(void);

#endif
