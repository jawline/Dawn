#ifndef _VGA_DRIVER_SETUP_
#define _VGA_DRIVER_SETUP_

#define PORT_1 0x3C0

#define PORT_2_READ 0x3C2
#define PORT_2_WRITE 0x3CC

#define IDX_PORT_1 0x3C4
#define IDX_PORT_2 0x3CE
#define IDX_PORT_3 0x3D4

#define DAC_MASK_REGISTER 0x3C6

#define DAC_CTRL_PORT_1 0x3C8
#define DAC_CTRL_PORT_2 0x3C9
#define DAC_CTRL_PORT_3 0x3C7

void init_vga_driver();

#endif //_VGA_DRIVER_SETUP_
