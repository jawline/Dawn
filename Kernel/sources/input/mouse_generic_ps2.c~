#include "mouse_generic_ps2.h"
#include <common.h>
#include <interrupts/irq.h>
#include <interrupts/idt.h>
#include <interrupts/interrupt_handler.h>
#include <input/devices.h>

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
void mouseWaitOutput()
{
uint8 readbyte;

	while (1)
	{
		readbyte = inb(PORT_1);
		if (readbyte & BIT_1)
		{
			//BIT_1 still set
		}
		else
		{
			break;
		}
	}


}


//Wait untill bit 0 of PORT_2 Is set.
void mouseWaitInput()
{
uint8 readbyte;

	while (1)
	{
		readbyte = inb(PORT_2);
		if (readbyte & BIT_0)
		{
			//BIT_0 still set
			break;
		}
		else
		{
			//0 Not set
		}
	}


}

void sendMouseCommand(uint8 cmd)
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
	register_interrupt_handler(GET_IRQ(12), mouseIrqCallback);

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

	//Check if bit 5 is set
	if (_status & 0x20 == 0x20)
	{
		//XOR 0x5 should leave bit 5 free 
		_status = _status ^ 0x20;
	}

	//Set bit 1 to true
	_status = _status | 0x2;
	
	//Send set compaq byte command (0x60) to mouse port (0x64)
	mouseWaitOutput();
	outb(0x64, 0x60);

	//Send the new status to 0x60
	mouseWaitOutput();
	outb(0x60, _status);

	//May send a ACK or may not
	readMouseData(); //ACK

	//End of enabling interrupts
	
	sendMouseCommand(0xF6);
	readMouseData(); //ACK
	sendMouseCommand(0xF4);
	readMouseData(); //ACK
}
