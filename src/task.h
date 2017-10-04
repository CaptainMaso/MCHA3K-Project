#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS 10

typedef struct _task_s
{
	uint16_t frequency;
	void (*callback)(void);
} task_s;

void            tasks_init(double);

uint8_t         tasks_add(task_s*);
task_s*		 	tasks_get_at(uint8_t);
uint8_t		 	tasks_remove_at(uint8_t);
void            tasks_run(void);

uint8_t         tasks_triggered(void);
bool			tasks_ready(void);

void            tasks_enable(void);
void            tasks_disable(void);
bool            tasks_is_enabled(void);

void            tasks_trigger_isr(void);

uint8_t tasks_calculate_counter(uint8_t * ocr2_val, uint8_t * timer2_clock_sel, double time_interval_sec);

#endif
