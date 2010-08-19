#ifndef _DISK_DEVICE_DEF_H_
#define _DISK_DEVICE_DEF_H_

#define T_PATAPI 0x01
#define T_SATAPI 0x02
#define T_PATA 0x03
#define T_SATA 0x04
#define T_UNKNOWN 0x05

#define SECTOR_SIZE 512

typedef struct {
	uint32 device_ctrl; //Control port
	uint32 device_base; //Base port
	uint8 slave_bit; //Slave bit
} disk_device;

#endif //_DISK_DEVICE_DEF_H_
