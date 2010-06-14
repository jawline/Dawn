#include "cpu.h"

extern uint8 check_cpuid();

uint8 cpuid_supported()
{
	return check_cpuid();
}
