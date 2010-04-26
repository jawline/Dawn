#include <interrupts/interrupt_handler.h>

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8 n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}
