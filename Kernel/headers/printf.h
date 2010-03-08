#ifndef _PRINT_F_DEF_H_
#define _PRINT_F_DEF_H_
#include "int_types.h"

#define prints(string) text_mode_write(string)
#define putc(char) text_mode_putc(char)

void printi(int32 i);
void printui(uint32 i);
void printuh(uint32 i);

#endif
