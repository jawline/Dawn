#include "cpu.h"
#include <printf.h>

//Global string, used a buffer for the vendor string
char VendorString[13];

//Function: cpuidGetVendorReal
//Arguments: a character buffer
//Returns: None
//Description: Moves the vendor string of a cpuid call to the char*
void cpuidGetVendorReal(char* Buffer)
{
	unsigned long input = 0;

	unsigned long ebx = 0;
	unsigned long edx = 0;
	unsigned long ecx = 0;

	__asm__ volatile("movl %3, %%eax; \
		      cpuid; \
		      movl %%ecx, %2; \
		      movl %%ebx, %0; \
		      movl %%edx, %1; " : "=r" (ebx), "=r" (edx), "=r" (ecx) : "r" (input));

	memcpy(Buffer, &ebx, sizeof(unsigned long));
	memcpy(Buffer + sizeof(unsigned long), &edx, sizeof(unsigned long));
	memcpy(Buffer + sizeof(unsigned long) + sizeof(unsigned long), &ecx, sizeof(unsigned long));
	Buffer[12] = '\0'; //Terminate the vendor string

	return;
}

//Functions: cpuidGetVendor
//Arguments: None
//Returns: pointer to a 13 byte character array containing a null terminated vendor string
//Description: Get the vendor string from the cpuid function
const char* cpuidGetVendor()
{
	//If cpuid isn't supported tell the user so
	if (!cpuidSupported()) return "Unsupported";

	//Copy unkown over before copying the vendor string
	strcpy(VendorString, "Unknown");
	cpuidGetVendorReal(VendorString);

	return VendorString;
}
