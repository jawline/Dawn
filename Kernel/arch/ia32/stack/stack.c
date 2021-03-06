#include <stack/stack.h>
#include <mm/virtual.h>
#include <debug/debug.h>
#include <types/memory.h>
#include <panic/panic.h>

stack_t moveStack(stack_t new_start, size_t size, MEM_LOC initial_esp) {
	MEM_LOC iter = 0; //for iterator
	MEM_LOC end = new_start - size; //The stack expands downwards.

	//This for loop maps new_start to new_start - size with physical memory
	for (iter = new_start; iter >= end; iter -= PAGE_SIZE) {
		MEM_LOC frame = allocateFrame(); //Allocate the frame
		map((MEM_LOC)iter,(MEM_LOC)frame, 0); //Map it to the virtual address
	}

	stack_t old_stack_pointer = 0;
	__asm__ volatile("mov %%esp, %0" : "=r" (old_stack_pointer)); //The old stack & base pointers = the esp & ebp registers respectivally

	stack_t old_base_pointer = 0;
	__asm__ volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

	//Use a big int because the stack may be moving backwards in memory (Needs to be possible positive or negative)
	sint64_t offset = ((size_t)new_start) - initial_esp;

	//Calculate the new stack & base pointers by adding the offset between the stack locations
	stack_t new_stack_pointer = (stack_t) (((size_t)old_stack_pointer) + offset);
	stack_t new_base_pointer  = (stack_t) (((size_t)old_base_pointer ) + offset);

	//Copy over the new stack - copy size initial_esp - esp to new_stack_pointer
	//This copies the size between the current stack pointer and the initial stack start to the new stack
	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp -  ((size_t)old_stack_pointer));

	MEM_LOC i = 0;

	for(i = (MEM_LOC) new_start; i > (MEM_LOC) (new_start - size); i -= sizeof(uint32_t)) {
           //Clarification on this ickle bit. tmp = *i (The actual value of memory at location i)
	   size_t tmp = *(size_t*)i;

	   // If the value of tmp is inside the range of the old stack, assume it is a base pointer
	   // and remap it. This will unfortunately remap ANY value in this range, whether they are
	   // base pointers or not.
	   if ((((size_t)old_stack_pointer) < tmp) && (tmp < initial_esp)) {
	     tmp = tmp + offset;
	     size_t *tmp2 = (size_t*)i;
	     *tmp2 = tmp;
	   }
	}

	//Set the ESP and EBP to the new stack.
	__asm__ volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
	__asm__ volatile("mov %0, %%ebp" : : "r" (new_base_pointer));

	return new_start;
}
