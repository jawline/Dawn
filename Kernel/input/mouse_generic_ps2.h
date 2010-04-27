#ifndef _PS2_MOUSE_DEF_H_
#define _PS2_MOUSE_DEF_H_
#include <common.h>
#include <input/mouse.h>

#define PORT_1 0x60
#define PORT_2 0x64

#define CMD 0xD4
#define ACK 0xFA

#define BIT_0 0x1
#define BIT_1 0x2

//Export, Setup the mouse + Its IRQ etc
void init_mouse();

#endif //_PS2_MOUSE_DEF_H_
