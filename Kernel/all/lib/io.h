#ifndef _INPUT_OUTPUT_DEF_H_
#define _INPUT_OUTPUT_DEF_H_
#include <types/stdint.h>

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#endif
