#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS 10

typedef struct _task_s
{
	uint16_t interval;
	void (*callback)(void);
	uint8_t id;
} task_s;

void            tasks_init(double);

uint8_t         tasks_add(task_s*);
task_s*		 	tasks_get_at(uint8_t);
uint8_t		 	tasks_remove_at(uint8_t);
uint8_t		 	tasks_remove(task_s*);
void            tasks_run(void);

uint8_t         tasks_triggered(void);
bool			tasks_ready(void);

void            tasks_enable(void);
void            tasks_disable(void);
bool            tasks_is_enabled(void);

void            tasks_trigger_isr(void);

uint8_t tasks_calculate_counter(uint8_t * ocr2_val, uint8_t * timer2_clock_sel, double time_interval_sec);

double 			tasks_get_task_interval(task_s *);
uint16_t		tasks_time_interval_to_task_interval(double);
double 			tasks_task_interval_to_time_interval(uint16_t);

#endif
