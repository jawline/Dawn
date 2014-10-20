#ifndef _KERNEL_HEAP_DEF_H_
#define _KERNEL_HEAP_DEF_H_

/**
 * Allocates x amount of memory from the kernel heap
 */

void* kmalloc(unsigned long mem);

/**
 * Free memory at location x on the kernel heap making it available again
 */

void kfree(void* addr);

/**
 * Initializes the kernel heap
 */

void initializeKernelHeap();

#endif //_KERNEL_HEAP_DEF_H_
