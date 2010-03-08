#ifndef _COMMON_DEF_H_
#define _COMMON_DEF_H_
#include "headers/int_types.h"

#define disable_interrupts() asm volatile("cli");
#define enable_interrupts() asm volatile("sti");

//The itoa implementation found at http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
//Converts integer to a character array using the specified base
char* itoa(int value, char* result, int base);
char* itoa_64b(int64 value, char * result, int base);

//Memset implementation (Sets all bytes from dest untill dest + len to a specified value)
void memset(uint32 *dest, uint8 val, uint32 len);

#endif
