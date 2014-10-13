#ifndef _PRINT_F_DEF_H_
#define _PRINT_F_DEF_H_
#include <types/stdint.h>

#define prints(string) kputs(string)
#define putc(char) kputc(char)
#define cls() kcls();

void printf(const char* fmt, ...);

#endif
