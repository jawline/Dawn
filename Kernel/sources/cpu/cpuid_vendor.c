#include "cpu.h"
#include <printf.h>

extern const char* cpuid_call_vendor();

char VendorString[13];

void cpuid_get_vendor_real(char* Buffer)
{
	unsigned long input = 0;

	unsigned long ebx = 0;
	unsigned long edx = 0;
	unsigned long ecx = 0;

	asm volatile("movl %3, %%eax; \
		      cpuid; \
		      movl %%ecx, %2; \
		      movl %%ebx, %0; \
		      movl %%edx, %1; " : "=r" (ebx), "=r" (edx), "=r" (ecx) : "r" (input));

	memcpy(Buffer, &ebx, sizeof(unsigned long));
	memcpy(Buffer + sizeof(unsigned long), &edx, sizeof(unsigned long));
	memcpy(Buffer + sizeof(unsigned long) + sizeof(unsigned long), &ecx, sizeof(unsigned long));

	return;
}

const char* cpuid_getvendor()
{
	if (!cpuid_supported()) return "Unsupported";

	strcpy(VendorString, "Unknown");
	cpuid_get_vendor_real(VendorString);

	return VendorString;
}
