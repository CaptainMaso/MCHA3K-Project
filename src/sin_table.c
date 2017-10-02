#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "task.h"
#include "sin_table.h"


#ifndef M_PI
#define M_PI 3.141592653589
#endif

static uint32_t _n_samples;
static float _time, _amplitude, _frequency;

CMD_STATUS sin_table_cmd(int argc, const char *argv[])
{
    if (argc != 3)
    {
        return CMD_INVALIDPARAM;
    }

    _n_samples = atoi(argv[0]);
    _amplitude = atof(argv[1]);
    _frequency = atof(argv[2]);
    _time      = 0.0f;

    printf_P(PSTR("Time [sec],Value\n"));

    task_set(sin_table_task);
    task_enable();
    return CMD_OK;
}

void sin_table_task(void)
{
    float y = _amplitude*sin(2*M_PI*_frequency*_time);
    printf_P(PSTR("%.2f,%g\n"), _time, y);
    _time += 0.01f;
    --_n_samples;

    if (_n_samples == 0) task_disable();
}
