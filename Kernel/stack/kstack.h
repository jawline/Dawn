#ifndef _KERNEL_STACK_DEF_H_
#define _KERNEL_STACK_DEF_H_
#include <mm/virt_mm.h>

#define KERNEL_STACK_START KERNEL_START + 0x20002000
#define KERNEL_STACK_SIZE 0x2000 //8kb stack

#endif
