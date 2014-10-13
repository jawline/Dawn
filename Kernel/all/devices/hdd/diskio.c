#include "diskio.h"

uint8_t isDeviceUsable(disk_device dev) {
	uint8_t type = ideDetectDeviceType(dev);

	if (type == T_UNKNOWN) {
		return 0;
	} else {
		return 1;
	}
}

//Returns 1 if success 0 if fail
uint8_t diskRead(uint16_t* dest, uint32_t offset_sectors, uint32_t num_sectors,
		disk_device device) {

	if (num_sectors == 0)
		return 0;

	for (;;) {
		if (num_sectors == 256) {
			if (pio_28bit_read(device, dest, offset_sectors, 0) == 0)
				return 0; //Error fail to read
			break;
		} else if (num_sectors > 256) {
			num_sectors = num_sectors - 256;
			if (pio_28bit_read(device, dest, offset_sectors, 0) == 0)
				return 0; //Error, fail to read
			offset_sectors = offset_sectors + 256; //Increment the offset by 256
			dest = ((uint16_t*) dest) + (sizeof(uint16_t) * 256); //Increment the destination pointer by that
		} else if (num_sectors < 1) {
			break;
		} else {
			uint8_t ntr = num_sectors; //Will fit in a uint8_t
			if (pio_28bit_read(device, dest, offset_sectors, ntr) == 0)
				return 0; //Error, fail to read
			break;
		}
	}

	return 1;
}

uint8_t diskWrite(uint16_t* source, uint32_t offset_sectors,
		uint32_t num_sectors, disk_device device) {
	//TODO: Writing to disk
	return 0;
}
