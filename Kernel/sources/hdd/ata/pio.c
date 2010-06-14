#include "pio.h"
#include <common.h>

#define RESET_TIMEOUT 10000
#define FLOATING_BUS 0xFF

#define REG_CYCLE_LO 4
#define REG_CYCLE_HI 5
#define REG_DEVSEL 6

uint8 wait_busy(pio_device dev, unsigned int timeout)
{
	uint8 rb;
	unsigned int timeout_timer = 0;

	for (;;) 
	{
		timeout_timer++;
		if (timeout_timer > RESET_TIMEOUT) { /* Timed out, return 0 *FAIL* */ return 0; }

		rb = inb(dev.device_ctrl);
		uint8 db = rb & 0x80;

		if (db == 0) //BSY Clear
		{
			break;
		}
	}

	return 1;
}

//Returns 0 if fine
//Returns 1 if time out
uint8 pio_soft_reset(pio_device dev)
{
	uint8 rb = inb(dev.device_ctrl);
	
	outb(dev.device_ctrl, 4);
	outb(dev.device_ctrl, 0);

	/* Account for delay */
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);

	if (wait_busy(dev, RESET_TIMEOUT) == 0)
	{
		printf("HDD SWRESET TIMEOUT PORT 0x%x\n", dev.device_base);
		return 1;
	}

	return 0;
}

void send_cmd_select_device(pio_device dev)
{
	if (pio_soft_reset(dev) == 1) return;
	outb(dev.device_base + REG_DEVSEL, 0xA0 | dev.slave_bit << 4);
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);
	inb(dev.device_ctrl);
	wait_busy(dev, RESET_TIMEOUT);
}

//Returns 1 if fail
//Returns 0 if success
uint8 pio_28bit_wait_busy_drq(pio_device device, uint32 timeout)
{
	uint8 timeout_timer = timeout;

	for (;;)
	{
		if (timeout_timer < 1)
		{
			return 1; //Fail
		}

		uint8 rb = inb(device.device_ctrl);
		if ((rb & 0x80) == 0)
		{
			//BSY Not set
			if ((rb & 0x8) == 0x8)
			{
				//DRQ Set
				break;
			}
		}
	}

	return 0;
}

//PIO 28bit read
//Returns 1 if success
//Returns 0 if fail
uint8 pio_28bit_read(pio_device device, uint16* destination, uint32 lba, uint8 numberOfSectors)
{

	outb(device.device_base + REG_DEVSEL, 0xE0 | (device.slave_bit << 4) | ((lba >> 24) & 0x0F));  //Select the device (Slave or master) and send the last 4 bits of the LBA
	outb(device.device_base + 1, 0); //Null the register
	outb(device.device_base + 2, numberOfSectors); //Set the numberOfSectors
	outb(device.device_base + 3, (uint8) lba); //First 8 bits of the lba
	outb(device.device_base + 4, (uint8) (lba >> 8)); //Bits 8-16 of the LBA
	outb(device.device_base + 5, (uint8) (lba >> 16)); //Bits 16-24 of the LBA
	outb(device.device_base + 7, 0x20); //Send the read sectors command

	unsigned int toRead = numberOfSectors;
	if (numberOfSectors == 0)
	{
		toRead = 256;
	}

	for (;;)
	{
		if (toRead < 1)
		{
			break;
		}

		inb(device.device_base); //Wait 400ns
		inb(device.device_base);
		inb(device.device_base);
		inb(device.device_base);

		if (pio_28bit_wait_busy_drq(device, RESET_TIMEOUT) == 1)
		{
			printf("ERROR: DISK READ ERROR WAIT TIMEOUT\n");
			return 0;
		}

		unsigned int i = 0;
		for (i = 0; i < 256; i++) {		
			*destination = inw(device.device_base);
			destination++;
		}

		toRead--;
	}

	return 1;
}

uint8 detect_device_type(pio_device dev)
{

	if (pio_soft_reset(dev) == 1)
	{
		return T_UNKNOWN;
	}

	send_cmd_select_device(dev);

	uint8 cl = inb(dev.device_base + REG_CYCLE_LO);
	uint8 ch = inb(dev.device_base + REG_CYCLE_HI);

	if (cl==0x14 && ch==0xEB) return T_PATAPI;
	if (cl==0x69 && ch==0x96) return T_SATAPI;
	if (cl==0 && ch == 0) return T_PATA;
	if (cl==0x3c && ch==0xc3) return T_SATA;
	return T_UNKNOWN;
}

void print_pio_debug(pio_device dev)
{
	printf("Checking master connect\n");
	dev.slave_bit = 0;

	uint8 device_type = detect_device_type(dev);

	printf("Master device type: ");

	switch (device_type)
	{
		case T_PATAPI:
			printf("PATAPI\n");
			break;
		case T_SATAPI:
			printf("SATAPI\n");
			break;
		case T_SATA:
			printf("SATA\n");
			break;
		case T_PATA:
			printf("PATA\n");
			break;
		case T_UNKNOWN: {
			printf("UNKNOWN\n");		
			break;
		}
	}


	if (device_type == T_UNKNOWN)
	{
		printf("Master device unknown. Either non-existant or incompatable hardware\n");
	}

	printf("Checking slave connect\n");

	dev.slave_bit = 1;

	device_type = detect_device_type(dev);

	printf("Slave device type: ");

	switch (device_type)
	{
		case T_PATAPI:
			printf("PATAPI\n");
			break;
		case T_SATAPI:
			printf("SATAPI\n");
			break;
		case T_SATA:
			printf("SATA\n");
			break;
		case T_PATA:
			printf("PATA\n");
			break;
		case T_UNKNOWN: {
			printf("UNKNOWN\n");			
			break;
		}
	}

	if (device_type == T_UNKNOWN)
	{
		printf("Slave device unknown. Either non-existant or incompatable hardware\n");
	}
}
