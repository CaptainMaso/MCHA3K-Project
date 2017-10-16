#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>


void imu_init(void);

int16_t imu_get_ax(void);
int16_t imu_get_ay(void);
int16_t imu_get_az(void);
int16_t imu_get_gx(void);
int16_t imu_get_gy(void);
int16_t imu_get_gz(void);

#endif
