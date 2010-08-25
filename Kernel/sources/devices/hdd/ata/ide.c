#include "ide.h"
#include "pio.h"
#include <stdio.h>

void addToHddFs(disk_device newdev);

void initializeIdeHardDrive(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3)
{
	printf("Initializing hard drive 0x%x 0x%x 0x%x 0x%x\n", BAR0, BAR1, BAR2, BAR3);

	disk_device dev;
	memset(&dev, 0, sizeof(disk_device));

	dev.BAR0 = BAR0;
	dev.BAR1 = BAR1;

	dev.BAR2 = BAR2;
	dev.BAR3 = BAR3;

	dev.disk = 0;

	ideDriveOutputDiskType(dev);

	if (ideDetectDeviceType(dev) != T_UNKNOWN)
	{
		//Its it not unknown then use it
		addToHddFs(dev);
	}
}


void ideDriveOutputDiskType(disk_device dev)
{
	printf("Checking master connect\n");
	dev.disk = 0;

	uint8 device_type = ideDetectDeviceType(dev);

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

	dev.disk = 1;

	device_type = ideDetectDeviceType(dev);

	printf("Slave device type: ");

	switch (device_type)
	{
		case T_PATAPI:
		{
			printf("PATAPI\n");
			break;
		}
		case T_SATAPI:
		{
			printf("SATAPI\n");
			break;
		}
		case T_SATA:
		{
			printf("SATA\n");
			break;
		}
		case T_PATA:
		{
			printf("PATA\n");
			break;
		}
		case T_UNKNOWN: 
		{
			printf("UNKNOWN\n");			
			break;
		}
	}

	if (device_type == T_UNKNOWN)
	{
		printf("Slave device unknown. Either non-existant or incompatable hardware\n");
	}
}

unsigned char ideWaitBusy(disk_device dev, unsigned int timeout)
{
	uint8 rb;
	unsigned int timeout_timer = 0;

	for (;;) 
	{
		timeout_timer++;
		if (timeout_timer > RESET_TIMEOUT) { /* Timed out, return 0 *FAIL* */ return 0; }

		rb = inb(dev.BAR1 + CTRL_PORT);
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
unsigned char ideSoftReset(disk_device dev)
{
	uint8 rb = inb(dev.BAR1 + CTRL_PORT);
	
	outb(dev.BAR1 + CTRL_PORT, 4);
	outb(dev.BAR1 + CTRL_PORT, 0);

	/* Account for delay */
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);

	if (ideWaitBusy(dev, RESET_TIMEOUT) == 0)
	{
		printf("HDD SWRESET TIMEOUT PORT 0x%x 0x%x\n", dev.BAR0, dev.BAR1 + CTRL_PORT);
		return 1;
	}

	return 0;
}

void ideSelectDevice(disk_device dev)
{
	if (ideSoftReset(dev) == 1) return;
	outb(dev.BAR0 + REG_DEVSEL, 0xA0 | dev.disk << 4);
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);
	inb(dev.BAR1 + CTRL_PORT);
	ideWaitBusy(dev, RESET_TIMEOUT);
}

unsigned char ideDetectDeviceType(disk_device dev)
{

	if (ideSoftReset(dev) == 1)
	{
		return T_UNKNOWN;
	}

	ideSelectDevice(dev);

	uint8 cl = inb(dev.BAR0 + REG_CYCLE_LO);
	uint8 ch = inb(dev.BAR0 + REG_CYCLE_HI);

	if (cl==0x14 && ch==0xEB) return T_PATAPI;
	if (cl==0x69 && ch==0x96) return T_SATAPI;
	if (cl==0 && ch == 0) return T_PATA;
	if (cl==0x3c && ch==0xc3) return T_SATA;

	return T_UNKNOWN;
}
