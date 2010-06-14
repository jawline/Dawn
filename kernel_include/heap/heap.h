#ifndef _HEAP_DEF_H_
#define _HEAP_DEF_H_
#include <types/int_types.h>
#include <mm/virt_mm.h>
#include <mm/phys_mm.h>

#define MASK_USED 0x1

struct heap_entry {
	uint8 used;
	uint32 size; //Size (in bytes) of this block
	struct heap_entry* next; //Location of the next header
	struct heap_entry* prev; //Location of the previous header.
};

typedef struct heap_entry heap_entry_t;

typedef struct {
	uint32 heap_location; //Heaps location (In virtual memory)
} heap_t;

void init_heap(heap_t* heap, uint32 address);

#endif
