#ifndef _PIO_TRANSFER_DEF_H_
#define _PIO_TRANSFER_DEF_H_
#include <types/int_types.h>
#include <stdio.h>

#define T_PATAPI 0x01
#define T_SATAPI 0x02
#define T_PATA 0x03
#define T_SATA 0x04
#define T_UNKNOWN 0x05

typedef struct {
	uint32 device_ctrl; //Control port
	uint32 device_base; //Base port
	uint8 slave_bit; //Slave bit
} pio_device;

uint8 pio_soft_reset(pio_device dev);

void pio_init();

#endif //_PIO_TRANSFER_DEF_H_
