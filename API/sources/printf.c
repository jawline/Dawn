#include <printf.h>

DEFN_SYSCALL1(kernel_print_string, 0, const char*);

void api_printf(const char* fmt, ...)
{
        static char buf [1024]; //Maximum size to be printed is 1024 chars.
        va_list args;

        int i;
 
        va_start(args, fmt);

        i = vsprintf(buf,fmt,args);

        va_end(args);

        buf[i] = '\0';

        syscall_kernel_print_string(buf); //Send the buffer to the kernel for printing
}