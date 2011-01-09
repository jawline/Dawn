#include <system/cmos.h>

//Read a register from the CMOS
uint8_t CMOS_READ(uint8_t Register)
{
	//Don't want to be interrupted untill the register has been read
	disable_interrupts();

	//Write the specified CMOS register to be read to port 0x70
	outb(0x70, Register);

	//Read the value of the register from port 0x71
	uint8_t ret = inb(0x71);

	//Re-enable interrupts
	enable_interrupts();

	//Return the byte from the CMOS
	return ret;
}

//Write the value to the specified register of the CMOS
void CMOS_WRITE(uint8_t Register, uint8_t Value)
{
	disable_interrupts(); //Don't want to be interrupted untill the new value has been set

	//Send the register to write to port 0x70
	outb(0x70, Register);

	//Send the new value of the register to port 0x71
	outb(0x71, Value);

	enable_interrupts(); //Turn them back on after we are finished

	return;
}
