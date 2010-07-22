#ifndef _FORMATTED_PRINT_FUNCTION_DEF_H_
#define _FORMATTED_PRINT_FUNCTION_DEF_H_
#include <syscall/syscall.h>
#include <va_list.h>
#include <vsprintf.h>

#define printf api_printf
void api_printf(const char* fmt, ...);

#endif //_FORMATTED_PRINT_FUNCTION_DEF_H_
