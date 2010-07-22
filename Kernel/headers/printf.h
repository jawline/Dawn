#ifndef _PRINT_F_DEF_H_
#define _PRINT_F_DEF_H_
#include <types/int_types.h>

#define prints(string) kputs(string)
#define putc(char) kputc(char)
#define cls() kcls();

void printi(int32 i);
void printui(uint32 i);
void printuh(uint32 i);
void printf(const char* fmt, ...);

#endif
