#ifndef _PHYSICAL_MEMORY_MANAGER_DEF_H_
#define _PHYSICAL_MEMORY_MANAGER_DEF_H_
#include <types/int_types.h>
#include <process/process.h>

#define PHYS_MM_STACK_ADDR 0xE0000000

void init_phys_mm(uint32 start_addr);

MEM_LOC allocateFrame();
MEM_LOC allocateFrameForProcess(process_t* req_process);

void freeFrame(MEM_LOC frame);
unsigned long calculateFreeFrames();

#endif //PHYSICAL_MEMORY_MANAGER_DEF_H_
