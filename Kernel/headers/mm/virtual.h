#ifndef _VIRTUAL_MEMORY_ABSTRACT_HEADER_
#define _VIRTUAL_MEMORY_ABSTRACT_HEADER_
#include <types/memory.h>

#define MEMORY_RESTRICTED_ACCESS 1

extern unsigned int PAGE_SIZE;
void map(MEM_LOC virtual_location, MEM_LOC physical_location, unsigned char flags);
void unmap(MEM_LOC virtual_location);

#endif //_VIRTUAL_MEMORY_ABSTRACT_HEADER_
