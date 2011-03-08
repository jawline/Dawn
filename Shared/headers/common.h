#ifndef _COMMON_DEF_H_
#define _COMMON_DEF_H_
#include <types/stdint.h>

//#define push_eflags() asm volatile("pushf");
//#define pop_eflags() asm volatile("popf");

//The itoa implementation found at http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
//Converts integer to a character array using the specified base
char* itoa(int value, char* result, int base);

//Memset implementation (Sets all bytes from dest untill dest + len to a specified value)
void memset(void* dest, uint8_t val, unsigned long len);
void memcpy(void* dest, void* src, unsigned long len);

char *strcpy(char *dest, const char *src);
int strcmp (const char * src, const char * dst);

//A implementation of Strlen from Wikipedia (Lifted from a FreeBSD implementation)
unsigned int strlen(const char * str);
char* strchr (register const char* s, int c);

#endif
