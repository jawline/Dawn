#include "mouse_generic_ps2.h"
#include <common.h>
#include <interrupts/irq.h>
#include <interrupts/idt.h>
#include <interrupts/interrupt_handler.h>
#include <input/devices.h>
#include <panic/panic.h>

int mouse_cycle=0;     //unsigned char
char mouse_byte[3];    //signed char
mouse_input_t istruct;

idt_call_registers_t mouseIrqCallback(idt_call_registers_t regs) 
{

  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=inb(PORT_1);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=inb(PORT_1);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=inb(PORT_1);

      istruct.i_byte = mouse_byte[0];
      istruct.mouse_x = mouse_byte[1];
      istruct.mouse_y = mouse_byte[2];

      sendInputMessage(DEVICE_MOUSE, 0, &istruct);
      mouse_cycle=0;
      break;
  }
}

//Wait tell BIT_1 of PORT_1 is not set
inline void mouseWaitOutput()
{
	uint8 readbyte;

	int timeout = 0x100000;

	while (timeout--)
	{
		if ((inb(PORT_2) & BIT_0) == BIT_0)
		{
			break;
		}
	}


}


//Wait untill bit 0 of PORT_2 Is set.
inline void mouseWaitInput()
{
	uint8 readbyte;

	int timeout = 0x100000;

	while (timeout--)
	{
		if ((inb(PORT_2) & BIT_1) != BIT_1)
		{
			break;
		}
	}
}

inline void sendMouseCommand(uint8 cmd)
{
	mouseWaitOutput();
	outb(PORT_2, CMD);

	mouseWaitOutput();
	outb(PORT_1, cmd);
}

uint8 readMouseData()
{
	mouseWaitInput();
	uint8 ret = inb(PORT_2);

	return ret;
}

void initializeMouse()
{
	uint8 _status;

	mouseWaitOutput();
	outb(0x64, 0xA8); //Enable the auxillery mouse device

	//Enable interrupts

	//0x20 to 0x64 = Get compaq status byte
	mouseWaitOutput();
	outb(0x64, 0x20);

	//Read the value from 0x60 (The compaq status byte)
	mouseWaitInput();
	_status = inb(0x60);

	//Set bit 1 to true
	_status = _status | 0x2;
	
	//Send set compaq byte command (0x60) to mouse port (0x64)
	mouseWaitOutput();
	outb(0x64, 0x60);

	//Send the new status to 0x60
	mouseWaitOutput();
	outb(0x60, _status);

	//May send a ACK or may not
	printf("MD 0x%x\n", readMouseData()); //ACK

	//End of enabling interrupts
	
	sendMouseCommand(0xF6);
	printf("MD 0x%x\n", readMouseData()); //ACK

	sendMouseCommand(0xF4);
	printf("MD 0x%x\n", readMouseData()); //ACK

	register_interrupt_handler(GET_IRQ(12), mouseIrqCallback);

	mouse_cycle = 0;
}
