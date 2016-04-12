#ifndef _FORMATTED_PRINT_FUNCTION_DEF_H_
#define _FORMATTED_PRINT_FUNCTION_DEF_H_
#include <syscall/syscall.h>
#include <va_list.h>
#include <vsprintf.h>

/**
 * API definition of printf, sends a formatted string to syscall 0 (Should print to active terminal)
 */
void printf(const char* fmt, ...);
void cls();
void clearscreen();

#endif //_FORMATTED_PRINT_FUNCTION_DEF_H_
