#ifndef _PROGRAMMABLE_INTERRUPT_TIMER_DEF_H_
#define _PROGRAMMABLE_INTERRUPT_TIMER_DEF_H_

#include "common.h"
#include <interrupts/interrupt_handler.h>

typedef void (*clock_callback)(); //Call one of these on each tick if setup.

void initializePit(unsigned int frequency);
void setPitCallback(clock_callback cb);

#endif //_PROGRAMMABLE_INTERRUPT_TIMER_DEF_H_
