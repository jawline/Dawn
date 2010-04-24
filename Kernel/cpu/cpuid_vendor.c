#include "cpu.h"
#include <printf.h>

extern const char* cpuid_call_vendor();

const char* cpuid_getvendor()
{
	if (!cpuid_supported()) return "Unsupported";

	return cpuid_call_vendor();
}
