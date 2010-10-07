#ifndef _HEAP_DEF_H_
#define _HEAP_DEF_H_
#include <types/memory.h>
#include <types/size_t.h>

#define MASK_USED 0x1

/**
 * @The structure of a single memory entry on the heap
 */

struct heap_entry {
	
	/**
	 * @brief Variable to store whether this block of memory is used (boolean)
	 */

	unsigned char used;

	/**
	 * @brief Variable to store the size of the chunk of memory
	 */

	size_t size; //Size (in bytes) of this block

	/**
	 * @brief Pointer to the next memory block NULL if last
	 */

	struct heap_entry* next; //Location of the next header

	/**
	 * @brief Pointer to the previous heap entry
	 */

	struct heap_entry* prev; //Location of the previous header.
};

typedef struct heap_entry heap_entry_t;

/**
 * @brief A structure to hold information about a heap in memory
 */

typedef struct {
	
	/**
	 * @brief The location of the heap in memory
	 */

	MEM_LOC heap_location; //Heaps location (In virtual memory)
} heap_t;

void init_heap(heap_t* heap, uint32 address);

#endif
