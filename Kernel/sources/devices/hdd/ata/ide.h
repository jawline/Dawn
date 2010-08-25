#ifndef _IDE_HARD_DRIVE_DRIVER_
#define _IDE_HARD_DRIVE_DRIVER_
#include <devices/hdd/disk_device.h>

#define CTRL_PORT 2

#define RESET_TIMEOUT 1000000
#define FLOATING_BUS 0xFF

#define REG_CYCLE_LO 4
#define REG_CYCLE_HI 5
#define REG_DEVSEL 6

void ideDriveOutputDiskType(disk_device dev);

unsigned char ideSoftReset(disk_device dev);
unsigned char ideDetectDeviceType(disk_device dev);

void initializeIdeHardDrive(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3);

#endif //_ATA_IDE_HARD_DRIVE_DRIVER_
