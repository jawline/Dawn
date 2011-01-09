#include "common.h"

char* itoa(int value, char* result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0)
	{

		*ptr++ = '-';

	}

	*ptr-- = '\0';

	while(ptr1 < ptr)
	{

		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;

	}

	return result;
}

void memset(void* dest, uint8_t val, unsigned long len)
{
    uint8_t* temp = (uint8_t*) dest;

    for ( ; len != 0; len--)
    {
	*temp++ = val;
    }
}

void memcpy(void* dest, void* src, unsigned long len)
{
	int iter = 0;

	uint8_t* dbuffer = (uint8_t*) dest;
	uint8_t* sbuffer = (uint8_t*) src;

	for (iter = 0; iter < len; iter++)
	{
		*dbuffer = *sbuffer;
		dbuffer++;
		sbuffer++;
	}
}

//FreeBSD's implementation of strlen.
unsigned int strlen(const char* str)
{
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}

int strcmp (const char * src, const char * dst)
{
        int ret = 0 ;

        while( ! (ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
                ++src, ++dst;

        if ( ret < 0 )
                ret = -1 ;
        else if ( ret > 0 )
                ret = 1 ;

        return( ret );
}

char *strcpy(char *dest, const char *src)
{
   char *save = dest;

   while((*dest++ = *src++))
   {
   }

   return save;
}

char* strchr (register const char* s, int c)
{
  do {
    if (*s == c)
      {
	return (char*)s;
      }
  } while (*s++);
  return (0);
}

