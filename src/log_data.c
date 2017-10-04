#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "task.h"
//#include "potentiometer.h"
//#include "encoder.h"
#include "log_data.h"

void log_task_function(void);

typedef enum {
    LOG_POT,
    LOG_ENC
} LOG_DEVICE_T;

static uint32_t _n_samples;
static LOG_DEVICE_T _device;
static float _time;

static uint8_t _log_task_id;
static task_s _log_task = {
		10, &log_task_function
};

CMD_STATUS log_cmd(int argc, const char *argv[])
{
	if (_log_task_id != 255)
	{
		if (argc != 2)
		{
			return CMD_INVALIDPARAM;
		}

		int n = atoi(argv[1]);
		if (n < 0)
		{
			printf_P(PSTR("pot: expecting positive number of samples\n"));
			return CMD_REPORTEDERR;
		}
		_n_samples = n;
		_time = 0.0f;

		/*if (!strcmp_P(argv[0], PSTR("pot")))
		{
			_device = LOG_POT;
			pot_init();
			printf_P(PSTR("Time [sec],Potentiometer [V]\n"));
		}
		else if (!strcmp_P(argv[0], PSTR("enc")))
		{
			_device = LOG_ENC;
			encoder_init();
			printf_P(PSTR("Time [sec],Encoder [-]\n"));
		}
		else*/
		{
			printf_P(PSTR("log: unknown device \"%s\", syntax is: %s [pot|enc] <samples>\n"), argv[1], argv[0]);
			return CMD_REPORTEDERR;
		}

<<<<<<< HEAD
    task_set(log_task);
    tasks_enable();
    return CMD_OK;
=======
		_log_task_id = tasks_add(&_log_task);
		if (_log_task_id != 255)
		{
			return CMD_OK;
		}
		else
		{
			printf("log: too many tasks already running\n");
			return CMD_REPORTEDERR;
		}
	}
	else
	{
		printf("log: already running\n");
		return CMD_REPORTEDERR;
	}
>>>>>>> TaskScheduler
}

void log_task_function(void)
{
    switch (_device)
    {
    /*case LOG_POT:
        printf_P(PSTR("%.2f,%g\n"), _time, (float)pot_get_value()*5.0f/POT_FULL_SCALE);
        break;
    case LOG_ENC:
        printf_P(PSTR("%.2f,%" PRId32 "\n"), _time, encoder_get_count());
        break;*/
    default:
    	break;
    }

    _time += 0.01f;
    --_n_samples;

<<<<<<< HEAD
    if (_n_samples == 0) tasks_disable();
=======
    if (_n_samples == 0){
    	tasks_remove_at(_log_task_id);
    	_log_task_id = 255;
    }
>>>>>>> TaskScheduler
}
