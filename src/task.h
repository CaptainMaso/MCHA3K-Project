#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*TASK_CALLBACK_T)(void);

void            task_init(void);

void            task_set(TASK_CALLBACK_T);
TASK_CALLBACK_T task_get(void);
void            task_run(void);

uint8_t         task_triggered(void);

void            task_enable(void);
void            task_disable(void);
bool            task_is_enabled(void);

void            task_trigger_isr(void);

#endif
