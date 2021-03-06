#include <heap/heap.h>
#include <printf.h>
#include <debug/debug.h>
#include <panic/panic.h>
#include <mm/virtual.h>
#include <mm/phys_mm.h>
#include <scheduler/scheduler.h>

MEM_LOC heapAllocateMemory(size_t size, heap_t* heap);

#define HEAP_BASE_PAGES 4
#define MINIMUM_BLOCK_SIZE 100

size_t mapInitialHeap(MEM_LOC start) {

	MEM_LOC iter;

	for (iter = start; iter <= start + PAGE_SIZE * HEAP_BASE_PAGES; iter +=
			PAGE_SIZE) {

		map(iter, allocateFrameForProcess(getCurrentProcess()),
				MEMORY_RESTRICTED_ACCESS);
	}

	return HEAP_BASE_PAGES * PAGE_SIZE;
}

void initializeBaseEntry(heap_entry_t* base_entry, size_t heapSizeInBytes) {

	memset(base_entry, 0, sizeof(heap_entry_t));
	base_entry->magic = HEAP_MAGIC;
	base_entry->size = heapSizeInBytes - sizeof(heap_entry_t);
	base_entry->next = 0;
	base_entry->prev = 0;
}

/**
 * @brief Initializes a new heap at address specified and maps memory from free frames for it
 * @callgraph
 */
void initializeHeap(heap_t* heap, MEM_LOC address) {

	//Double check the heap ptr is valid
	if (heap == 0) {
		return;
	}

	DEBUG_PRINT("Initializing new heap at 0x%x\n", address);

	//Returns the number of bytes the heap allocated on initialization
	size_t heapSizeBytes = mapInitialHeap(address);

	heap_entry_t* base_entry = address;
	initializeBaseEntry(base_entry, heapSizeBytes);

	heap->heap_location = address;

	return;
}

/**
 * @brief Expands the heap by expansionSize pages
 * @callgraph
 */
void expandHeap(heap_entry_t* last_heap_entry, MEM_LOC heap_end,
		size_t expansionSize) {

	for (MEM_LOC iter = heap_end; iter <= heap_end + (PAGE_SIZE * expansionSize); iter +=
			PAGE_SIZE) {
		map(iter, allocateFrameForProcess(getCurrentProcess()),
				MEMORY_RESTRICTED_ACCESS);
	}

	if (last_heap_entry->used == 0) {

		//If the last entry is still free
		last_heap_entry->size += (expansionSize * PAGE_SIZE);
	} else {

		//If not. create a new entry and link it too the list
		heap_entry_t* new_entry = heap_end;
		memset(new_entry, 0, sizeof(heap_entry_t));

		new_entry->magic = HEAP_MAGIC;
		new_entry->used = 0;
		new_entry->prev = last_heap_entry;
		new_entry->next = 0;
		new_entry->size = (expansionSize * PAGE_SIZE) - sizeof(heap_entry_t);

		last_heap_entry->next = new_entry;
	}

	return;
}

/**
 * @brief Allocates x bytes of memory from a heap, expanding it if necessary
 * @callgraph
 * @bug Heap never decreases in size
 */
MEM_LOC heapAllocateMemory(size_t size, heap_t* heap) {

	heap_entry_t* iterator = (heap_entry_t*) heap->heap_location;

	if (!iterator) {
		PANIC("Cannot allocate memory, invalid heap");
	}

	//Loop through all heap entrys
	while (1) {

		//If it isn't used and its bigger then the size requested
		if (!iterator->used && iterator->size >= size) {

			heap_entry_t* used_block = iterator;
			used_block->used = 1;
			size_t remainder = used_block->size - size; //How much is left over

			if (remainder > sizeof(heap_entry_t) + MINIMUM_BLOCK_SIZE) {

				//Create a new entry and reduce the size of the original

				//The new entry
				heap_entry_t* new_block = ((MEM_LOC) used_block)
						+ sizeof(heap_entry_t) + size;

				memset(new_block, 0, sizeof(heap_entry_t));
				new_block->magic = HEAP_MAGIC;
				new_block->size = remainder - sizeof(heap_entry_t);
				new_block->next = used_block->next;
				new_block->prev = used_block;

				//The used entry
				used_block->size = size;
				used_block->next = new_block;
			}

			MEM_LOC returnLocation = used_block;
			returnLocation += sizeof(heap_entry_t);

			return returnLocation;
		}

		if (!iterator->next) {
			break;
		}

		iterator = iterator->next;
	}

	//This bit finds the end of the heap
	MEM_LOC heap_end = iterator;
	heap_end += sizeof(heap_entry_t) + iterator->size;

	expandHeap(iterator, heap_end, (size / PAGE_SIZE) + 1);

	//Recall this function
	return heapAllocateMemory(size, heap);
}

/**
 * Merge this entry with the entry to the right
 */
void mergeRight(heap_entry_t* entry) {
	entry->size = entry->size + entry->next->size + sizeof(heap_entry_t);
	entry->next = entry->next->next;
}

/**
 * Merge adjacent unused entries together
 */
void unifyHeapEntry(heap_entry_t* entry) {

	while (entry->prev && !entry->prev->used) {
		entry = entry->prev;
		mergeRight(entry);
	}

	while (entry->next && !entry->next->used) {
		mergeRight(entry);
	}

}

void printHeap(heap_t* heap) {

	heap_entry_t* iter = heap->heap_location;

	while (iter) {
		printf("heap_entry 0x%x, used: %i size: 0x%x\n", iter, iter->used,
				iter->size);
		iter = iter->next;
	}

	DEBUG_PRINT("Done printing heap\n");
}

/**
 * @brief Free's the memory at location address on the heap specified. Shrinking if necessary
 * @callgraph
 * @bug Doesn't shrink yet
 */
void heapFreeMemory(MEM_LOC address, heap_t* heap) {

	MEM_LOC entry_address = address - sizeof(heap_entry_t);

	heap_entry_t* specificEntry = (heap_entry_t*) entry_address;

	if (specificEntry->magic != HEAP_MAGIC) {
		DEBUG_PRINT("Entry magic != Heap magic\n");
		PANIC("AHHHH HEAP_MAGIC error\n");
	} else {
		specificEntry->used = 0;
		unifyHeapEntry(specificEntry);
	}
}
