#ifndef _DISK_DEVICE_DEF_H_
#define _DISK_DEVICE_DEF_H_

#define T_PATAPI 0x01
#define T_SATAPI 0x02
#define T_PATA 0x03
#define T_SATA 0x04
#define T_UNKNOWN 0x05

typedef struct {

	//Basic info about a disk drive
	unsigned int device_ctrl; //Control port
	unsigned int device_base; //Base port

	//Which disk to access if the controller handles more then one (For IDE Controllers 1 = slave 0 = primary)
	unsigned char disk;

} disk_device;

#endif //_DISK_DEVICE_DEF_H_
