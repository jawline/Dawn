#include <system/cmos.h>

uint8 CMOS_READ(uint8 Register) 
{
	disable_interrupts();
	outb(0x70, Register);
	uint8 ret = inb(0x71);
	enable_interrupts();
	return ret;
}

void CMOS_WRITE(uint8 Register, uint8 Value) 
{
	disable_interrupts();
	outb(0x70, Register);
	outb(0x71, Value);
	enable_interrupts();
	return;
}
