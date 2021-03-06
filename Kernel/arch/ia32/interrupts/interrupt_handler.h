#ifndef _INTERRUPT_HANDLER_DEF_H_
#define _INTERRUPT_HANDLER_DEF_H_
#include <types/stdint.h>
#include "idt.h"

#define GET_IRQ(x) x + 32

typedef struct registers (*isr_t) (struct registers);
void registerInterruptHandler(uint8_t n, isr_t handler);

#endif
