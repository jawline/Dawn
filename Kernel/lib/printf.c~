#include <int_types.h>
#include <printf.h>
#include <va_list.h>

void printi(int32 s) {
	char Buffer[11];
	itoa(s, Buffer, 10);
	prints(Buffer);
}

void printui(uint32 s) {
	char Buffer[11];
	itoa(s, Buffer, 10);
	prints(Buffer);
}

void printuh(uint32 n) {
    int32 tmp;

    prints("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            putc(tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            putc( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        putc(tmp-0xA+'a');
    }
    else
    {
        putc(tmp+'0');
    }
}

void printf(const char * fmt, ...) {
        static char buf [1024]; //Maximum size to be printed is 1024 chars.
        va_list args;

        int i;
 
        va_start(args, fmt);

        i = vsprintf(buf,fmt,args);

        va_end(args);

        buf[i] = '\0';

        prints(buf);
}
