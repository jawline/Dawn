#include "pit.h"
#include <interrupts/idt.h>
#include <lib/io.h>

extern long systemClockTicks;

//Callback is called every time the PIT fires
static idt_call_registers_t pitOnTickCallback(idt_call_registers_t regs)
{
	systemClockTicks++;

	schedulerOnTick();

	return regs;
}

//Initialize the programmable interrupt time to fire at the frequency sent
void initializePit(unsigned int frequency)
{
   // Firstly, register our timer callback.
   registerInterruptHandler(GET_IRQ(0), &pitOnTickCallback);

   // Tge value sent to the PIT is the value 1193180 devided by the frequency wanted
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}
