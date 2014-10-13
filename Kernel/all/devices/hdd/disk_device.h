#ifndef _DISK_DEVICE_DEF_H_
#define _DISK_DEVICE_DEF_H_

#define T_PATAPI 0x01
#define T_SATAPI 0x02
#define T_PATA 0x03
#define T_SATA 0x04
#define T_UNKNOWN 0x05

typedef struct {

	//Basic info about a disk drive controller
	unsigned int BAR0;
	unsigned int BAR1;
	unsigned int BAR2;
	unsigned int BAR3;

	//Which disk to access if the controller handles more then one (For IDE Controllers 1 = slave 0 = primary)
	unsigned char disk;

	//Information about the disk device
	char name[64]; //64 byte character array to handle the name

} disk_device;

#endif //_DISK_DEVICE_DEF_H_
