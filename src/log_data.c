#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "task.h"
#include "motor.h"
#include "encoders.h"
#include "mpu6050.h"
#include "log_data.h"
#include "cmd_line_buffer.h"

void log_task_function(void);

typedef enum {
    LOG_CUR_ML	= 0,
	LOG_CUR_MR	= 1,
    LOG_ENC_ML	= 2,
	LOG_ENC_MR	= 3,
	LOG_IMU		= 4
} LOG_DEVICE_T;

static uint32_t _n_samples;
static double _time_interval;
static bool _devices_active[MAX_DEVICES] = {false};
static float _time;

static task_s _log_task = {
		.interval = 10,
		.callback = &log_task_function,
		.id = 255
};

CMD_STATUS log_cmd(int argc, const char *argv[])
{
	if (_log_task.id == 255)
	{
		int n = atoi(argv[0]);
		if (n <= 0)
		{
			printf_P(PSTR("log: expecting positive number of samples\n"));
			return CMD_REPORTEDERR;
		}
		_n_samples = n;

		double interval = 1.0/atof(argv[1]);
		if (interval <= 0.0)
		{
			printf_P(PSTR("log: expecting frequency\n"));
			return CMD_REPORTEDERR;
		}
		_time_interval = interval;

		_log_task.interval = tasks_time_interval_to_task_interval(interval);

		int curArg = 2;

		for (uint8_t i = 0; i < MAX_DEVICES; i++)
		{
			_devices_active[i] = false;
		}

		while(curArg < argc)
		{
			if (!strcmp_P(argv[curArg], PSTR("IMU")))
			{
				//IMU device
				_devices_active[LOG_IMU] = true;
				//printf_P(PSTR("Logging IMU\n"));
			}
			else if(!strcmp_P(argv[curArg], PSTR("ENC_ML")))
			{
				//Encoder devices
				_devices_active[LOG_ENC_ML] = true;
			}
			else if(!strcmp_P(argv[curArg], PSTR("ENC_MR")))
			{
				//Encoder devices
				_devices_active[LOG_ENC_MR] = true;
			}
			else if (!strcmp_P(argv[curArg], PSTR("CUR_ML")))
			{
				//Current Sensing devices
				_devices_active[LOG_CUR_ML] = true;
			}
			else if (!strcmp_P(argv[curArg], PSTR("CUR_MR")))
			{
				//Current Sensing devices
				_devices_active[LOG_CUR_MR] = true;
			}
			else
			{
				//Fail out 'cause of invalid argument
				for (uint8_t i = 0; i < MAX_DEVICES; i++)
				{
					_devices_active[i] = false;
				}
				_time_interval = 0.0;
				_n_samples = 0;
				return CMD_INVALIDPARAM;
			}
			curArg++;
		}

		_time = 0.0f;

		tasks_add(&_log_task);

		if (_log_task.id != 255)
		{
			clb_disable();
			printf_P(PSTR("Time (s)"));
			for(uint8_t i = 0; i < MAX_DEVICES; i++)
			{
				if (_devices_active[i])
				{
					printf(",");
					switch (i)
					{
					case LOG_IMU:
						printf_P(PSTR("ax (g), ay (g), az (g), gx (d/s), gy(d/s), gz(d/s)"));
						break;
					case LOG_ENC_ML:
						printf_P(PSTR("ENCODER_LEFT"));
						break;
					case LOG_ENC_MR:
						printf_P(PSTR("ENCODER_RIGHT"));
						break;
					case LOG_CUR_ML:
						printf_P(PSTR("CURRENT_LEFT"));
						break;
					case LOG_CUR_MR:
						printf_P(PSTR("CURRENT_RIGHT"));
						break;
					default:
						break;
					}
				}
			}
			printf_P(PSTR("\n"));
			return CMD_OK;
		}
		else
		{
			//Fail out 'cause of invalid argument
			for (uint8_t i = 0; i < MAX_DEVICES; i++)
			{
				_devices_active[i] = false;
			}
			_time_interval = 0.0;
			_n_samples = 0;
			printf_P(PSTR("log: too many tasks already running\n"));
			return CMD_REPORTEDERR;
		}
	}
	else
	{
		printf_P(PSTR("log: already running\n"));
		return CMD_REPORTEDERR;
	}
}

void log_task_function(void)
{
	printf_P(PSTR("%.3g"), _time);
	for (LOG_DEVICE_T i = 0; i < MAX_DEVICES; i++)
	{
		if (_devices_active[i])
		{
			printf_P(PSTR(","));
			switch (i)
			{
			case LOG_IMU:
			{
				double ax, ay, az, gx, gy, gz;

					ATOMIC_BLOCK(ATOMIC_FORCEON)
					{
						//mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
						mpu6050_getConvData(&ax, &ay, &az, &gx, &gy, &gz);
					}

					//printf_P(PSTR("\nMPU: ax: %"PRIu16", ay: %"PRIu16", az: %"PRIu16", gx: %"PRIu16", gy: %"PRIu16", gz: %"PRIu16"\n"), ax, ay, az, gx, gy, gz);
					printf_P(PSTR("%g,%g,%g,%g, %g,%g"), ax, ay, az, gx, gy, gz);
			}	break;
			case LOG_ENC_ML:
				printf_P(PSTR("%"PRId32""), encoder_get_count(MOTOR_LEFT));
				break;
			case LOG_ENC_MR:
				printf_P(PSTR("%"PRId32""), encoder_get_count(MOTOR_RIGHT));
				break;
			case LOG_CUR_ML:
				printf_P(PSTR("%"PRId16""), motors_get_adc_reading(MOTOR_LEFT));
				break;
			case LOG_CUR_MR:
				printf_P(PSTR("%"PRId16""), motors_get_adc_reading(MOTOR_RIGHT));
				break;
			default:
				break;
			}
		}
	}
	printf_P(PSTR("\n"));

    _time += _time_interval;
    --_n_samples;

    if (_n_samples == 0){
    	tasks_remove(&_log_task);
    	//printf_P(PSTR("\x0D \0A\n"));
    	clb_enable();
    }
}
