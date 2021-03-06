#include <mm/phys_mm.h>
#include <mm/virtual.h>
#include <debug/debug.h>
#include <panic/panic.h>
#include <types/memory.h>
#include <scheduler/scheduler.h>
#include <process/used_list.h>
#include <panic/panic.h>

MEM_LOC used_mem_end = 0;
MEM_LOC phys_mm_slock = PHYS_MM_STACK_ADDR;
MEM_LOC phys_mm_smax = PHYS_MM_STACK_ADDR;

extern uint32_t paging_enabled;

void initializePhysicalMemoryManager(MEM_LOC start) {
	DEBUG_PRINT("Debug Message: Used memory end 0x%x\n", start);
	//This ensures that the used_mem_end address is on a page-aligned boundry
	//(Which it has to be if I wish to identity map from 0 to used_mem_end)
	used_mem_end = (start + 0x1000) & ~(0xFFF);
}

MEM_LOC allocateFrame() {
	if (paging_enabled == 0) {
		//The reason why this works is that all memory up to used_mem_end
		//is identity mapped when paging is enabled
		//This means that when paging is enabled the
		//address will be mapped directly to the physical address
		//(0x1000 will still access 0x1000 in memory for example)

		used_mem_end += 4096; //Add 4096 bytes (4kb) to used_mem_end address
		return used_mem_end - 4096; //Return the old address
	} else {
		ASSERT(phys_mm_slock != PHYS_MM_STACK_ADDR, "out of memory frames");
		phys_mm_slock -= sizeof(uint32_t);
		uint32_t* stack = (uint32_t*) phys_mm_slock;
		return *stack;
	}
}

MEM_LOC allocateFrameForProcess(process_t* req_process) {

	ASSERT(paging_enabled, "cannot allocate for process without paging");
	MEM_LOC frame = allocateFrame();

	if (req_process) {
		usedListAdd(req_process, frame);
	}

	return frame;
}

void freeFrame(MEM_LOC frame) {

	//If paging isn't enabled we are not going to be able to free a frame of virtual memory
	//and the stacks location is virtual (Cannot be accessed without paging)
	if (paging_enabled == 0) {
		return;
	}

	//Anything under used_mem_end is identity mapped (Physical Address == Virtual Address)
	//never remap it.
	if (frame < used_mem_end + PAGE_SIZE) {
		return;
	}

	if (getCurrentProcess() != 0) {
		usedListRemove(getCurrentProcess(), frame);
	}

	//Run out of stack space *Shock Horror* Allocate this frame to the end
	//of the stack (Giving another 4kb (4096 bytes) of stack space)
	if (phys_mm_smax <= phys_mm_slock) {
		map((POINTER) phys_mm_smax, (POINTER) frame, MEMORY_RESTRICTED_ACCESS);
		phys_mm_smax += PAGE_SIZE;
	} else {
		//Add the frame to the end of the frame stack
		MEM_LOC* stack = (MEM_LOC*) phys_mm_slock;
		*stack = frame;
		phys_mm_slock += sizeof(uint32_t);
	}
}

/**
 * Handles initialisation of the free pages using the memory map provided by the mboot header.
 */
void mapFreePages(struct multiboot* mboot_ptr) {

	uint32_t i = mboot_ptr->mmap_addr;
	uint32_t freeCounter = 0;

	while (i < mboot_ptr->mmap_addr + mboot_ptr->mmap_length) {
		mmap_entry_t *me = (mmap_entry_t*) i;

		// Does this entry specify usable RAM?
		if (me->type == 1) {

			uint32_t j;

			// For every page in this entry, add to the free page stack.
			for (j = me->base_addr_low; j < me->base_addr_low + me->length_low; j += 0x1000) {
				freeFrame(j);
				freeCounter++;
			}

		}

		//The multiboot specification is strange in this respect - the size member
		//does not include "size" itself in its calculations, so we must add sizeof
		//a 32bit int
		i += me->size + sizeof(uint32_t);
	}

	DEBUG_PRINT("Debug Message: Map Free Pages finished with 0x%x pages of free memory (PAGE SIZE: 0x%x)\n", freeCounter, PAGE_SIZE);
}

unsigned long calculateFreeFrames() {
	return ((phys_mm_slock - PHYS_MM_STACK_ADDR) / sizeof(MEM_LOC));
}
