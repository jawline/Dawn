#include <interrupts/idt.h>
#include "common.h"
#include <interrupts/interrupt_handler.h>

extern isr_t interrupt_handlers[256];

void isr_handler(idt_call_registers_t regs)
{

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        regs = handler(regs);
    }
} 
