#ifndef _PHYSICAL_MEMORY_MANAGER_DEF_H_
#define _PHYSICAL_MEMORY_MANAGER_DEF_H_
#include <types/int_types.h>

#define PHYS_MM_STACK_ADDR 0xE0000000

void init_phys_mm(uint32 start_addr);

uint32 alloc_frame();

void free_frame(uint32 frame);

#endif //PHYSICAL_MEMORY_MANAGER_DEF_H_
