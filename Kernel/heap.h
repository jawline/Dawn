#ifndef _HEAP_DEF_H_
#define _HEAP_DEF_H_
#include "headers/int_types.h"
#include "virt_mm.h"
#include "phys_mm.h"

#define MASK_USED 0x1

typedef struct {
	uint8 flags;
	uint32 size;
	entry_header_t * next_header;
} entry_header_t;

typedef struct {
	uint32 heap_location; //Heaps location (In virtual memory)
} heap_t;

void init_heap(heap_t * heap, uint32 address);

#endif
