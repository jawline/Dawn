#ifndef _KERNEL_STACK_DEF_H_
#define _KERNEL_STACK_DEF_H_
#include <mm/virt_mm.h>

#define KERNEL_STACK_START KERNEL_START - 0x00002000
#define KERNEL_STACK_SIZE 0x4000 //8kb stack

#endif
