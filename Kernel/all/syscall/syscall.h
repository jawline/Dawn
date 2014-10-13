#ifndef _SYSTEM_CALL_DEF_H_
#define _SYSTEM_CALL_DEF_H_
#include <common.h>
#include <interrupts/idt.h>

//Architecture specific, each arch may initialze in a different way (Look in the arch folders)
void kernelInitializeSyscallSystem();

//Architecture general, initializes syscalls to the correct positions
void kernelInitializeSyscalls();

//General across all architecutres
void kernelRegisterSyscall(unsigned int num, void*);

#endif //_SYSTEM_CALL_DEF_H_
