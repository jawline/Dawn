#ifndef _CPU_DEFINITION_DEF_H_
#define _CPU_DEFINITION_DEF_H_
#include <common.h>

//Export - Returns 1 if cpuid calls (And therefore the OS's ability to read cpu information) are supported
uint8 cpuid_supported();
const char* cpuid_getvendor();
uint32 cpuid_features(); //get the edx register from a cpuid with eax set to 1
uint32 cpuid_getfrequency(); //Get the frequency of the processor

#endif //_CPU_DEFINITION_DEF_H_
