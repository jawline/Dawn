#include "cpu.h"
#include <printf.h>

/**
 * Cached vendor string
 */
char* vendorString = 0;

void cpuidQueryVendor(char* buffer) {
	
	unsigned long input = 0;
	unsigned long ebx = 0;
	unsigned long edx = 0;
	unsigned long ecx = 0;

	__asm__ volatile("movl %3, %%eax; \
		      cpuid; \
		      movl %%ecx, %2; \
		      movl %%ebx, %0; \
		      movl %%edx, %1; " : "=r" (ebx), "=r" (edx), "=r" (ecx) : "r" (input));

	memcpy(buffer, &ebx, sizeof(unsigned long));
	memcpy(buffer + sizeof(unsigned long), &edx, sizeof(unsigned long));
	memcpy(buffer + sizeof(unsigned long) + sizeof(unsigned long), &ecx, sizeof(unsigned long));
	buffer[12] = '\0'; //Terminate the vendor string

	return;
}

char const* cpuidGetVendor() {
	//If cpuid isn't supported tell the user so
	if (!cpuidSupported()) return "Unsupported";

	if (!vendorString) {
		vendorString = malloc(13);
		cpuidQueryVendor(vendorString);
	}

	return vendorString;
}
