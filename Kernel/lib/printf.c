#include "../headers/int_types.h"
#include "../headers/printf.h"

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
