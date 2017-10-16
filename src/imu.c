#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

#include "task.h"

#include "imu.h"
#include "mpu6050.h"

void imu_update(void);

static task_s _imu_update_task = {
		.interval = 10,
		.callback = imu_update,
		.id = 255
};

static int16_t _ax = 0;
static int16_t _ay = 0;
static int16_t _az = 0;
static int16_t _gx = 0;
static int16_t _gy = 0;
static int16_t _gz = 0;

/*
 * ar = az
 * at = ax
 * dTheta = gy
 */

void imu_init(void)
{
	//Initialise MPU6050
	mpu6050_init();

	//Set up IMU task regularly update data
	_imu_update_task.interval = tasks_time_interval_to_task_interval(0.1);

	tasks_add(&_imu_update_task);
}

void imu_update(void)
{
	mpu6050_getRawData(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);
}

int16_t imu_get_ax(void)
{
	int16_t tmp;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		tmp = _ax;
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
		tmp = _az;
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

