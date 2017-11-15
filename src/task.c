#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <stdbool.h>
#include <inttypes.h>
#include "task.h"

static task_s * _tasks[MAX_TASKS];
static volatile uint8_t _task_count;

static uint16_t _task_trigger_count;
static uint16_t _task_trigger_last_count;

uint8_t _timer_value;
uint8_t _timer_clock_source;

static double _timer_interval;

static void     _task_enable_trigger_isr(void);
static void     _task_disable_trigger_isr(void);
static bool     _task_is_trigger_isr_enabled(void);

uint8_t tasks_triggered(void)
{
    return _task_trigger_count;
}

bool tasks_ready(void)
{
	if (_task_trigger_count > _task_trigger_last_count)
	{
		return true;
	}
	return false;
}

void tasks_init(double time_interval_sec)
{
	// Timer 2 Setup
	/*
	 * WGM:		Waveform Gen Mode: CTC
	 * COM2: 	Set OC2 on match
	 * OCR: 	(INT rate desired OCR2 = f_clk/(prescale*f_desired) - 1)
	 */
	TCCR2 = _BV(WGM21)  | _BV(COM21) | _BV(COM20);
	DDRD |= _BV(PD7);

	if (tasks_calculate_counter(&_timer_value, &_timer_clock_source, time_interval_sec) != 255)
    {
    	OCR2 = _timer_value;
		_task_count = 0;
		for(uint8_t i = 0; i < MAX_TASKS; i++)
			_tasks[i] = NULL;
		_timer_interval = time_interval_sec;
    }
    else
    {
    	printf("Timer interval too large");
    }

    tasks_disable();
}

void tasks_enable(void)
{
    _task_trigger_count = 0;
    _task_trigger_last_count = 0;

    TCNT2 = 0;                      // reset counter
    _task_enable_trigger_isr();     // enable output compare interrupt
    /*
     * CS:	Desired Clock Source
     */
    TCCR2 |= _timer_clock_source;               // Start timer (connect clock source)
}

void tasks_disable(void)
{
    _task_disable_trigger_isr();    // disable output compare interrupt
    TCCR2 &= ~(_timer_clock_source);  // Stop timer (disconnect clock source)

    _task_trigger_count = 0;
}

void _task_enable_trigger_isr(void)
{
    TIMSK |= _BV(OCIE2);       // Enable output compare interrupt
}

void _task_disable_trigger_isr(void)
{
    TIMSK &= ~_BV(OCIE2);       // Disable output compare interrupt
}

bool _task_is_trigger_isr_enabled(void)
{
    return bit_is_set(TIMSK, OCIE2);
}

bool tasks_is_enabled(void)
{
    return _task_is_trigger_isr_enabled();
}

uint8_t tasks_add(task_s * task)
{
	if (task != NULL)
	{
		if (_task_count + 1 < MAX_TASKS)
		{
			_tasks[_task_count] = task;
			task->id = _task_count;
			return _task_count++;
		}
		else
		{
			task->id = 255;
			return 255;
		}
	}
	else
		return 255;
}

task_s* tasks_get_at(uint8_t index)
{
    return (index < MAX_TASKS && index < _task_count) ? _tasks[index] : NULL;
}

uint8_t tasks_remove_at(uint8_t index)
{
	if (index < _task_count)
	{
		_tasks[index]->id = 255;
		for(int i = index + 1; i < MAX_TASKS; i++)
		{
			_tasks[i-1] = _tasks[i];
		}
		_tasks[_task_count--] = NULL;
		return 0;
	}
	else
		return 255;
}

uint8_t tasks_remove(task_s * toRemove)
{
	uint8_t index = toRemove->id;
	return tasks_remove_at(index);
}

void tasks_run(void)
{
    if (_task_count > 0)
    {
    	uint16_t _task_trigger_count_tmp = _task_trigger_count;
    	if (_task_trigger_count_tmp >= _task_trigger_last_count + 1)
    	{
    		for(uint8_t i = 0; i < _task_count; i++)
    		{
    			if (_task_trigger_count_tmp % _tasks[i]->interval == 0 && _tasks[i]->enabled)
    				_tasks[i]->callback();
    		}

    		if (_task_trigger_count_tmp > 50000)
    			_task_trigger_count = _task_trigger_last_count = 0;
    		else
    			_task_trigger_last_count = _task_trigger_count_tmp;
    	}
    	else if (_task_trigger_count_tmp > _task_trigger_last_count + 5)
    	{
    		printf_P(PSTR("ERROR: TASKS RAN TOO LONG, DISABLING TASKS\n"));
    		tasks_disable();
    	}
    }
}

void tasks_trigger_isr(void)
{
    ++_task_trigger_count;
}

uint8_t tasks_calculate_counter(uint8_t * ocr2_val, uint8_t * timer2_clock_sel, double time_interval_sec)
{
	const uint16_t prescaler_values[] = {1, 8, 64, 256, 1024};
	const uint8_t clock_select_values[] = {(_BV(CS10)), (_BV(CS11)), (_BV(CS10) | _BV(CS11)), (_BV(CS12)), (_BV(CS10) | _BV(CS12))};

	for (uint8_t i = 0; i < 5; i++)
	{
		uint16_t tmp_val;
		tmp_val = round(F_CPU/(prescaler_values[i]/(4.0*time_interval_sec) - 1.0f));
		if (tmp_val < 255)
		{
			*ocr2_val = tmp_val & 0x00FF;
			*timer2_clock_sel = clock_select_values[i];
			return 0;
		}
	}
	return -1;
}

double tasks_get_task_interval(task_s *task)
{
	return task->interval*_timer_interval;
}

uint16_t tasks_time_interval_to_task_interval(double interval)
{
	return round(interval/_timer_interval);
}

double tasks_task_interval_to_time_interval(uint16_t interval)
{
	return _timer_interval*interval;
}
