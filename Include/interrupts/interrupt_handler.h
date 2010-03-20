#ifndef _INTERRUPT_HANDLER_DEF_H_
#define _INTERRUPT_HANDLER_DEF_H_
#include <types/int_types.h>

#define GET_IRQ(x) x + 32

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(uint8 n, isr_t handler);

#endif
