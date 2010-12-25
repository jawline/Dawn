#ifndef _FORMATTED_PRINT_FUNCTION_DEF_H_
#define _FORMATTED_PRINT_FUNCTION_DEF_H_
#include <syscall/syscall.h>
#include <va_list.h>
#include <vsprintf.h>

#define printf api_printf

/**
 * @ingroup Output
 * 
 * @brief Formatted print function.
 * @param const char* fmt, the formatted string. ..., any additional arguments
 * @return None
 */

void api_printf(const char* fmt, ...);
void cls();
void clearscreen();

#endif //_FORMATTED_PRINT_FUNCTION_DEF_H_
