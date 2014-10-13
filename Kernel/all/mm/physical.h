#ifndef _PHYSICAL_MEMORY_ABSTRACT_HEADER_
#define _PHYSICAL_MEMORY_ABSTRACT_HEADER_
#include <types/memory.h>
#include <process/process.h>

MEM_LOC allocateFrame();
MEM_LOC allocateFrameForProcess(process_t* proc);
void freeFrame(MEM_LOC);
unsigned long calculateFreeFrames();

#endif //_PHYSICAL_MEMORY_ABSTRACT_HEADER_
