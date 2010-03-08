#include "phys_mm.h"
#include "virt_mm.h"

#include "panic.h"

#include "headers/int_types.h"
#include "virt_mm.h"

uint32 used_mem_end = 0;

uint32 phys_mm_slock = PHYS_MM_STACK_ADDR;
uint32 phys_mm_smax = PHYS_MM_STACK_ADDR;

extern uint32 paging_enabled;

void init_phys_mm(uint32 start) {
	used_mem_end = (start + 0x1000) & PAGE_MASK; //This ensures that the used_mem_end address is on a page-aligned boundry (Which it has to be if I wish to identity map from 0 to used_mem_end)
}

uint32 alloc_frame() {
	if (paging_enabled == 0) {
		used_mem_end += 4096; //Add 4096 bytes (4kb) to used_mem_end address
		return used_mem_end - 4096; //Return the old address
		//The reason why this works is that all memory up to used_mem_end is identity mapped when paging is enabled
		//This meens that when paging is enabled the address will be mapped directly to the physical address (0x1000 will still access 0x1000 in memory for example) 
	} else {
		//Paging is enabled	

		if (phys_mm_slock == PHYS_MM_STACK_ADDR)
		    PANIC("Error:out of memory."); //This will crash the OS when we run out of frames (Bad idea much?)

		// Pop off the stack.
		phys_mm_slock -= sizeof (uint32);
		uint32 * stack = (uint32 *)phys_mm_slock;
		  
		return *stack;
	}
}

void free_frame(uint32 frame) {
	if (paging_enabled == 0) {
		return; //If paging isn't enabled we are not going to be able to free a frame of virtual memory add the stacks location is virtual (Cannot be accessed without paging)	
	}	

	if (frame < used_mem_end) {
		return; //Anything under used_mem_end is identity mapped (Physical Address == Virtual Address) never remap it.
	}

	if (phys_mm_smax <= phys_mm_slock) { //Run out of stack space *Shock Horror* Allocate this frame to the end of the stack (Giving another 4kb of stack space)
		//TODO: MAP!
	} else {
		uint32 * stack = phys_mm_slock;
		*stack = frame;
		phys_mm_slock += sizeof(uint32);	
	}
}
