#ifndef _DISK_INPUT_OUTPUT_DEF_H_
#define _DISK_INPUT_OUTPUT_DEF_H_
#include <types/stdint.h>
#include <devices/hdd/disk_device.h>
#include "ata/pio.h"

void device_init(disk_device dev);
void device_reset(disk_device dev);
uint8_t device_available(disk_device dev);
uint8_t device_read(uint16_t* dest, unsigned int offset, unsigned int num_sectors, disk_device device);
uint8_t device_write(uint16_t* source, uint32_t offset, uint32_t num_sectors, disk_device device);

#endif //_DISK_INPUT_OUTPUT_DEF_H_
