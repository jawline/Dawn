#ifndef _CPU_DEFINITION_DEF_H_
#define _CPU_DEFINITION_DEF_H_
#include <common.h>

//Export - Returns 1 if cpuid calls (And therefore the OS's ability to read cpu information) are supported
unsigned char cpuidSupported();
const char* cpuidGetVendor();
unsigned long cpuidFeatures(); //get the edx register from a cpuid with eax set to 1

#endif //_CPU_DEFINITION_DEF_H_
