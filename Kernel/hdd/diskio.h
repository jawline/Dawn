#ifndef _DISK_INPUT_OUTPUT_DEF_H_
#define _DISK_INPUT_OUTPUT_DEF_H_
#include <types/int_types.h>
#include <hdd/disk_device.h>
#include "ata/pio.h"

void device_init(disk_device dev);
void device_reset(disk_device dev);
uint8 device_available(disk_device dev);
uint8 device_read(uint16* dest, uint32 offset, uint32 num_sectors, disk_device device);
uint8 device_write(uint16* source, uint32 offset, uint32 num_sectors, disk_device device);

#endif //_DISK_INPUT_OUTPUT_DEF_H_
