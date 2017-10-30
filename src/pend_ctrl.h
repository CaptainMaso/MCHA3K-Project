#ifndef PEND_CTRL_H
#define PEND_CTRL_H

#include <stdint.h>
#include "controller.h"

void pend_ctrl_run(uint32_t, states *);
#ifdef FIXEDPOINT
int32_t pend_ctrl_alloc(states *);
#else
float pend_ctrl_alloc(states *);
#endif

#endif
