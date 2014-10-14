#ifndef _PROGRAMMABLE_INTERRUPT_TIMER_DEF_H_
#define _PROGRAMMABLE_INTERRUPT_TIMER_DEF_H_
#include <interrupts/interrupt_handler.h>

/**
 * Clock callback is a function called on every clock tick
 */
typedef void (*clock_callback)();

void initializePit(unsigned int frequency);
void setPitCallback(clock_callback cb);

#endif //_PROGRAMMABLE_INTERRUPT_TIMER_DEF_H_
