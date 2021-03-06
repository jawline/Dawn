#ifndef _PHYSICAL_MEMORY_MANAGER_DEF_H_
#define _PHYSICAL_MEMORY_MANAGER_DEF_H_
#include <types/stdint.h>
#include <process/process.h>
#include <mm/virtual.h>
#include <mm/physical.h>
#include <multiboot.h>

#define PHYS_MM_STACK_ADDR 0xE0000000

void initializePhysicalMemoryManager(MEM_LOC start);
void mapFreePages(struct multiboot* mboot_ptr);

#endif //PHYSICAL_MEMORY_MANAGER_DEF_H_
