#ifndef _HEAP_DEF_H_
#define _HEAP_DEF_H_
#include <int_types.h>
#include "virt_mm.h"
#include "phys_mm.h"

#define MASK_USED 0x1

typedef struct {
	uint8 used;
	uint32 size; //Size (in bytes) of this block
	uint32 * next; //Location of the next header
	uint32 * prev; //Location of the previous header.
} heap_entry_t;

typedef struct {
	uint32 heap_location; //Heaps location (In virtual memory)
} heap_t;

void init_heap(heap_t * heap, uint32 address);

#endif
