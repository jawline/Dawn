#include "common.h"

uint64 __div_64_32(uint64* dividend, uint32 divisor);

char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

char* itoa_64(uint64 value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
	
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	
	do {
		tmp_value = value;
		__div_64_32(&value, base);
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	
	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}

void memset(uint32 *dest, uint8 val, uint32 len)
{
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

uint64 __div_64_32(uint64* dividend, uint32 divisor)
{
   uint32 dividendLow, dividendLow2, dividendHigh, remainder;
   dividendLow = (uint32)(*dividend & 0xFFFFFFFF);
   dividendHigh = (uint32)(*dividend >> 32);
   remainder = dividendHigh % divisor;
   dividendHigh = dividendHigh / divisor;
   dividendLow2 = dividendLow >> 16;
   dividendLow2+= remainder << 16;
   remainder = dividendLow2 % divisor;
   dividendLow2 = dividendLow2 / divisor;
   dividendLow = dividendLow & 0xFFFF;
   dividendLow+= remainder << 16;
   remainder = dividendLow % divisor;
   dividendLow = dividendLow / divisor;
   *dividend = dividendLow + ((uint64)dividendLow2 << 16) + ((uint64)dividendHigh << 32);

   return(remainder);
}
