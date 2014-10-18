#include "pit.h"
#include <clock/clock.h>
#include <scheduler/scheduler.h>
#include <interrupts/idt.h>
#include <lib/io.h>

//Callback is called every time the PIT fires
static idt_call_registers_t pitOnTickCallback(idt_call_registers_t regs) {

	clockHandleTick();
	return regs;
}

//Initialize the programmable interrupt time to fire at the frequency sent
void initializePit(unsigned int frequency) {
	registerInterruptHandler(GET_IRQ(0), &pitOnTickCallback);

	//The value sent to the PIT is the value 1193180 divided by the frequency wanted
	uint32_t divisor = 1193180 / frequency;

	//Command byte gets pit ready for programming
	outb(0x43, 0x36);

	//Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
}
