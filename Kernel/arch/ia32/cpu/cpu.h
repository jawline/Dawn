#ifndef _CPU_DEFINITION_DEF_H_
#define _CPU_DEFINITION_DEF_H_
#include <common.h>

/**
 * Returns 1 if cpuid can be used to query CPU features, 0 otherwise
 */
unsigned char cpuidSupported();

/**
 * Returns the CPU vendor from cpuid
 */
const char* cpuidGetVendor();

/**
 * Returns the CPUID features list
 */
unsigned long cpuidFeatures();

#endif //_CPU_DEFINITION_DEF_H_
