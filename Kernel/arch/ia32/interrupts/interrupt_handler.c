#include <interrupts/interrupt_handler.h>

isr_t interrupt_handlers[256];

void registerInterruptHandler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}
