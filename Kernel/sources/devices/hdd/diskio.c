#include "diskio.h"

uint8 isDeviceUsable(disk_device dev)
{
	uint8 type = ideDetectDeviceType(dev);

	if (type == T_UNKNOWN)
	{
		return 0;
	} else
	{
		return 1;
	}
}

//Returns 1 if success 0 if fail
uint8 diskRead(uint16* dest, uint32 offset_sectors, uint32 num_sectors, disk_device device)
{
	if (num_sectors == 0) return;

	for (;;)
	{
		if (num_sectors  == 256)
		{
		  if (pio_28bit_read(device, dest, offset_sectors, 0) == 0) return 0; //Error fail to read
		  break;
		} else if (num_sectors > 256)
		{
		  num_sectors = num_sectors - 256;
		  if (pio_28bit_read(device, dest, offset_sectors, 0) == 0) return 0; //Error, fail to read
		  offset_sectors = offset_sectors + 256; //Increment the offset by 256
		  dest = ((uint16*)dest) + (sizeof(uint16) * 256); //Increment the destination pointer by that
		} else if (num_sectors < 1)
		{
		  break;
		} else {
		  uint8 ntr = num_sectors; //Will fit in a uint8
		  if (pio_28bit_read(device, dest, offset_sectors, ntr) == 0) return 0; //Error, fail to read
		  break;
		}
	}

	return 1;
}

uint8 diskWrite(uint16* source, uint32 offset_sectors, uint32 num_sectors, disk_device device)
{
	//TODO: Writing to disk
	return 0;
}
