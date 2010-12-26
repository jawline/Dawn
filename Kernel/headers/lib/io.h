#ifndef _INPUT_OUTPUT_DEF_H_
#define _INPUT_OUTPUT_DEF_H_
#include <types/int_types.h>

void outb(uint16 port, uint8 value);
uint8 inb(uint16 port);
uint16 inw(uint16 port);

#endif
