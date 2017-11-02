#ifndef PEND_CTRL_H
#define PEND_CTRL_H

#include <stdint.h>
#include "controller.h"

//#define FIXEDPOINT_PENDCTRL

void pend_ctrl_run(uint32_t, states *);

#ifdef FIXEDPOINT_PENDCTRL
int32_t pend_ctrl_alloc(states *);
#else
float pend_ctrl_alloc(states *);
#endif

#endif
