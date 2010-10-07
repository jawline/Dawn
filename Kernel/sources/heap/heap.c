#include <heap/heap.h>
#include <printf.h>
#include <debug/debug.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>
#include <mm/phys_mm.h>

uint32 alloc_mem(uint32 size, heap_t * heap);
void free_mem(uint32 address, heap_t * heap);

#define HEAP_SHRINK_SIZE_MINIMUM 0x16000

/**
 * @brief Initializes a new heap at address specified and maps memory from free frames for it
 * @callgraph
 */
void initializeHeap(heap_t * heap, uint32 address)
{
	//Double check the heap ptr is valid
	if (heap == 0) return;

	DEBUG_PRINT("Debug Message: Creating Heap\n");
	memset(heap, 0, sizeof(heap_t));
	DEBUG_PRINT("Debug Message: 0'ed Heap\n");

	//Set the heap location to the specified address
	heap->heap_location = address & PAGE_MASK; //Page aligned boundry
	DEBUG_PRINT("Debug Message: Heap Location ");
	DEBUG_PRINTX(heap->heap_location);
	DEBUG_PRINT("\n");

	uint32 frame_addr = 0;

	frame_addr = 0;

	frame_addr = allocateFrame();

	DEBUG_PRINT("Debug Message: Allocated frame ");
	DEBUG_PRINTX(frame_addr);
	DEBUG_PRINT("\n");

	map(heap->heap_location, frame_addr, PAGE_PRESENT | PAGE_USER);

	DEBUG_PRINT("Debug Message: Mapped Heap\n");

	memset(heap->heap_location, 0, PAGE_SIZE);
	DEBUG_PRINT("Debug Message: 0'ed HEAP\n");

	heap_entry_t * ptr = (heap_entry_t *) heap->heap_location;
	memset(ptr, 0, sizeof(heap_entry_t)); //Initialize it.
	DEBUG_PRINT("Debug Message: Created first heap entry\n");

	ptr->used = 0; //Not used
	ptr->size = PAGE_SIZE - sizeof(heap_entry_t); //One frame has been allocated. 4096 bytes of heap space at the minute. The heap entrys are also in the space so we need to take into account for them

	DEBUG_PRINT("Debug Message: Set ptr size\n");

	ptr->next = 0; //ptr->next = NULL, this tells the heap that this is the last entry on the heap.
	ptr->prev = 0;
	DEBUG_PRINT("Debug Message: Ptr->next set\n");
}

/**
 * @brief Shink the heap
 * @bug Pretty sure this doesn't work write. Go ahead and rewrite the whole heap, its shit.
 */

void shrinkHeap(heap_t* heap, heap_entry_t* last_entry)
{

	//Calculate the end position and the start position of the memory block
	MEM_LOC startOfEntry = last_entry;
	startOfEntry += sizeof(heap_entry_t);

	MEM_LOC endOfEntry = startOfEntry;
	endOfEntry += last_entry->size;

	if (endOfEntry > heap + HEAP_SHRINK_SIZE_MINIMUM)
	{
		//XOR The (PAGE_SIZE - 1) will give the nearest page aligned boundry less than endOfEntry
		MEM_LOC nearestPageBoundry = endOfEntry;
		nearestPageBoundry = nearestPageBoundry & (~0xFFF);

		if (nearestPageBoundry < endOfEntry)
		{
			if (nearestPageBoundry > startOfEntry + 1)
			{

				//TODO: Investigate whether -1 is needed?
				//Set the blocks size to newSize
				size_t newSize = nearestPageBoundry - startOfEntry;
				last_entry->size = newSize;

				//Find out what location (Frame) this area is mapped to
				MEM_LOC mappingData = 0;
				getMapping(nearestPageBoundry, &mappingData);

				//Free that frame of memory
				freeFrame(mappingData);

				//Unmap it from the heap
				unmap(mappingData);

				shrinkHeap(heap, last_entry);
			}
		}

	}
}

/**
 * @brief Expand the heap
 */
void expandHeap(heap_t* heap, heap_entry_t* last_entry)
{
	//Map another 4kb onto the heap
	//This is the last entry on the heap SO if its not used we can just += the size otherwise we have to create a whole new one (More work for mee)

	MEM_LOC frame_addr = allocateFrame();
	MEM_LOC end = last_entry;
	end += sizeof(heap_entry_t);
	end += last_entry->size;

	map(end, frame_addr, PAGE_PRESENT | PAGE_USER); //Mapped now! =)

	memset(end, 0, PAGE_SIZE);
			
	if (last_entry->used == 0) 
	{
		last_entry->size += PAGE_SIZE;
	} 
	else 
	{
		//Create a new entry at end and link it to the heap
		heap_entry_t * nptr = (heap_entry_t *) end;
		nptr->used = 0;
		nptr->size = PAGE_SIZE - sizeof(heap_entry_t); //- the size of the new heap entry
		nptr->prev = last_entry;
		nptr->next = 0;
		last_entry->next = nptr;
	}

}

/**
 * @brief Allocates x bytes of memory from a heap, expanding it if necessary
 * @callgraph
 */
uint32 heapAllocateMemory(uint32 size, heap_t* heap) 
{
	heap_entry_t* ptr = (heap_entry_t*) heap->heap_location;
	uint8 result = 0;

	while (1) 
	{

		if (ptr->used == 0) //Is the chunk in use?
		{ 
			if (ptr->size > size) //Is it big enough!?!
			{ 
				ptr->used = 1; //Yay it fits!
				
				if (ptr->size < size + (sizeof(heap_entry_t))) 
				{ //If another chunk of memory wont fit then just allocate a few more bytes then asked for (Avoid leak)
					//Don't actually need to do anything. The next ptr is still the next pointer
				} 
				else 
				{
					heap_entry_t* newptr = (heap_entry_t*) (((uint32) ptr) + sizeof(heap_entry_t) + size);
					newptr->used = 0;
					newptr->size = ptr->size - size - sizeof(heap_entry_t);
					newptr->next = ptr->next;
					newptr->prev = ptr;
					ptr->next = newptr;
					ptr->size = size;
				}

				return ((uint32) ptr) + sizeof(heap_entry_t); //Return the location of the actual memory not the header
			}
		}

		if (ptr->next == 0) 
		{
			expandHeap(heap, ptr);
			return heapAllocateMemory(size, heap);
		}

		ptr = (heap_entry_t *) ptr->next; //Move onto the next chunk
	}

	return 0;
}

/**
 * @brief Free's the memory at location address on the heap specified. Shrinking if necessary
 * @callgraph
 * @bug Currently does not shrink
 */
void heapFreeMemory(uint32 address, heap_t* heap)
{
	heap_entry_t* ptr = (heap_entry_t*) (address - sizeof(heap_entry_t));
	heap_entry_t* optr = 0;

	if (ptr->used == 0) //If this happens then somethings gone wrong. Wtf moment much?
	{
		return;
	}

	ptr->used = 0;

	while (1)
	{
		//UNIFY Left
		optr = ptr->prev;
		if (optr != 0) 
		{
			if (optr->used == 0) //Can be unified!
			{ 
				optr->size = optr->size + sizeof(heap_entry_t) + ptr->size;
				optr->next = ptr->next;
				ptr = optr;
			}
			else
			{
				break;
			}
		} 
		else 
		{
			break;
		}
	}

	while (1) 
	{
		//UNIFY Right
		optr = ptr->next;
		if (optr != 0) 
		{
			if (optr->used == 0) //Can be unified!
			{ 
				ptr->size = ptr->size + sizeof(heap_entry_t) + optr->size;
				ptr->next = optr->next;
			} 
			else 
			{ 
				break; 
			}
		} 
		else 
		{ 
			break; 
		}
	}

	//Check if this is the last entry
	if (ptr->next == 0) 
	{
		shrinkHeap(heap, ptr);
	}
}
