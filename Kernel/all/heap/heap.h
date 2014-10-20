#ifndef _HEAP_DEF_H_
#define _HEAP_DEF_H_
#include <types/memory.h>
#include <types/size_t.h>

#define MASK_USED 0x1
#define HEAP_MAGIC 0xAB

/**
 * The structure of a single memory entry on the heap
 */

struct heap_entry {

	/**
	 * A magic number char used to check that it is a block and that free
	 * hasn't been called on an unmapped area
	 */

	unsigned char magic;

	/**
	 * Variable to store whether this block of memory is used (boolean)
	 */

	unsigned char used;

	/**
	 * Variable to store the size of the chunk of memory
	 */

	size_t size; //Size (in bytes) of this block

	/**
	 * Pointer to the next memory block NULL if last
	 */

	struct heap_entry* next; //Location of the next header

	/**
	 * Pointer to the previous heap entry
	 */

	struct heap_entry* prev; //Location of the previous header.
};

typedef struct heap_entry heap_entry_t;

/**
 * A structure to hold information about a heap in memory
 */

typedef struct {

	/**
	 * The location of the heap in mapped memory
	 */

	MEM_LOC heap_location;

} heap_t;

void init_heap(heap_t* heap, uint32_t address);

#endif
