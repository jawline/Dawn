#include "pit.h"
#include <interrupts/idt.h>

static clock_callback on_pit_tick = 0;

static idt_call_registers_t pit_tick_callback(idt_call_registers_t regs)
{
	if (on_pit_tick)
	{
		on_pit_tick();
	}
}

void init_pit(uint32 frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(GET_IRQ(0), &pit_tick_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32 divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8 l = (uint8)(divisor & 0xFF);
   uint8 h = (uint8)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}

void set_pit_callback(clock_callback cb)
{
	on_pit_tick = cb;
}