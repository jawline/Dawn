#ifndef _PHYSICAL_MEMORY_MANAGER_DEF_H_
#define _PHYSICAL_MEMORY_MANAGER_DEF_H_
#include <int_types.h>

#define PHYS_MM_STACK_ADDR 0xFF000000

void init_phys_mm(uint32 start_addr);
uint32 alloc_frame();
void free_frame(uint32 frame);

#endif
