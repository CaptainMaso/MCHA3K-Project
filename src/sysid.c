#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <math.h>

#include "sysid.h"
#include "task.h"
#include "encoders.h"
#include "motor.h"
#include "cmd_line_buffer.h"

void sysid_motor_free_callback(void);
void sysid_motor_load_callback(void);

static MOTOR_SIDE active_side;
static double _time_interval;
static float _time_total;
static float _time;

static task_s _sysid_task = {
		.interval = 10,
		.callback = sysid_motor_free_callback,
		.id = 255
};

static float _sin_freq = 1.0f;
static float _sin_gain = 1.0f;
static float _sin_bias = 0.0f;

// FMT: "motsysid_free <sample frequency> <time (s)> <side> <sin freq> <sin gain> <sin bias>"
CMD_STATUS sysid_motor_free_cmd(int argc, const char *argv[])
{
	if (_sysid_task.id == 255)
	{
		if (argc == 6)
		{
			// Select Side
			if (!strcmp_P(argv[0], PSTR("LEFT")))
			{
				active_side = MOTOR_LEFT;
			}
			else if (!strcmp_P(argv[0], PSTR("RIGHT")))
			{
				active_side = MOTOR_RIGHT;
			}
			else
			{
				printf_P(PSTR("sysid: expecting LEFT or RIGHT\n"));
				return CMD_INVALIDPARAM;
			}

			// Setup sin frequency
			if ((_sin_freq = atof(argv[1])) <= 0.0)
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			// Setup sin gain
			if ((_sin_gain = atof(argv[2])) <=  -MAX_VOLTAGE + 1 || _sin_gain > MAX_VOLTAGE - 1)
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			// Setup sin bias
			if ((_sin_bias = atof(argv[3])) < -MAX_VOLTAGE || _sin_bias > MAX_VOLTAGE)
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			// Setup interval and samples
			if ((_time_interval = 1.0/atof(argv[4])) <= 0.0 || !isfinite(_time_interval))
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			if (_sin_bias + _sin_gain > MAX_VOLTAGE || _sin_bias - _sin_gain < -MAX_VOLTAGE)
			{
				printf_P(PSTR("sysid: bias & gain must be within max voltage\n"));
				return CMD_REPORTEDERR;
			}

			_sysid_task.interval = tasks_time_interval_to_task_interval(_time_interval);

			if ((_time_total = atof(argv[5])) <= 0.0 || !isfinite(_time_total))
			{
				printf_P(PSTR("sysid: expected finite positive time\n"));
				return CMD_REPORTEDERR;
			}

			//Setup tasks
			tasks_add(&_sysid_task);

			if (_sysid_task.id != 255)
			{
				clb_disable();
				_time = 0;
				encoder_set_count(0, MOTOR_LEFT);
				encoder_set_count(0, MOTOR_RIGHT);
				printf_P(PSTR("Time(s), V(V),Encoder,ADC\n"));
				return CMD_OK;
			}
			else
			{
				_time_interval = 0.0;
				printf_P(PSTR("motsysid: too many tasks already running\n"));
				return CMD_REPORTEDERR;
			}
		}
		else
			return CMD_INVALIDPARAM;

	}
	else
	{
		printf_P(PSTR("Already doing sysID\n"));
		return CMD_REPORTEDERR;
	}
}

void sysid_motor_free_callback(void)
{
	float voltage = sin(2*3.1415*_sin_freq*_time) * _sin_gain + _sin_bias;
	int32_t duty_cycle = MAX_PWM*voltage/MAX_VOLTAGE;

	motors_set_pwm(active_side, duty_cycle);

	printf_P(PSTR("%.5g,%.5g,%"PRId32",%"PRId16"\n"), _time, voltage, encoder_get_count(active_side), motors_get_adc_reading(active_side));

	_time += _time_interval;
	if (_time >= _time_total)
	{
		tasks_remove(&_sysid_task);
		clb_enable();
		motors_set_pwm(active_side, 0);
	}
}

CMD_STATUS sysid_motor_free_cmd(int argc, const char *argv[])
{
	if (_sysid_task.id == 255)
	{
		if (argc == 6)
		{
			// Select Side
			if (!strcmp_P(argv[0], PSTR("LEFT")))
			{
				active_side = MOTOR_LEFT;
			}
			else if (!strcmp_P(argv[0], PSTR("RIGHT")))
			{
				active_side = MOTOR_RIGHT;
			}
			else
			{
				printf_P(PSTR("sysid: expecting LEFT or RIGHT\n"));
				return CMD_INVALIDPARAM;
			}

			// Setup sin frequency
			if ((_sin_freq = atof(argv[1])) <= 0.0)
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			// Setup sin gain
			if ((_sin_gain = atof(argv[2])) <=  -MAX_VOLTAGE + 1 || _sin_gain > MAX_VOLTAGE - 1)
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			// Setup sin bias
			if ((_sin_bias = atof(argv[3])) < -MAX_VOLTAGE || _sin_bias > MAX_VOLTAGE)
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			// Setup interval and samples
			if ((_time_interval = 1.0/atof(argv[4])) <= 0.0 || !isfinite(_time_interval))
			{
				printf_P(PSTR("sysid: expected frequency\n"));
				return CMD_REPORTEDERR;
			}

			if (_sin_bias + _sin_gain > MAX_VOLTAGE || _sin_bias - _sin_gain < -MAX_VOLTAGE)
			{
				printf_P(PSTR("sysid: bias & gain must be within max voltage\n"));
				return CMD_REPORTEDERR;
			}

			_sysid_task.interval = tasks_time_interval_to_task_interval(_time_interval);

			if ((_time_total = atof(argv[5])) <= 0.0 || !isfinite(_time_total))
			{
				printf_P(PSTR("sysid: expected finite positive time\n"));
				return CMD_REPORTEDERR;
			}

			//Setup tasks
			tasks_add(&_sysid_task);

			if (_sysid_task.id != 255)
			{
				clb_disable();
				_time = 0;
				encoder_set_count(0, MOTOR_LEFT);
				encoder_set_count(0, MOTOR_RIGHT);
				printf_P(PSTR("Time(s), V(V),Encoder,ADC\n"));
				return CMD_OK;
			}
			else
			{
				_time_interval = 0.0;
				printf_P(PSTR("motsysid: too many tasks already running\n"));
				return CMD_REPORTEDERR;
			}
		}
		else
			return CMD_INVALIDPARAM;

	}
	else
	{
		printf_P(PSTR("Already doing sysID\n"));
		return CMD_REPORTEDERR;
	}
}
