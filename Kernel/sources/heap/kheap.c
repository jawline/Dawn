#include <heap/heap.h>
#include <mm/virt_mm.h>
#define KERNEL_HEAP_ADDR KERNEL_START + 0x10000000

heap_t kernel_heap;

/**
 * @brief Allocates x amount of memory from the kernel heap
 */
uint32 kmalloc(uint32 mem) 
{
	return heapAllocateMemory(mem, &kernel_heap);
}

/**
 * @brief Free memory at location x on the kernel heap making it available again
 */
void kfree(uint32 addr)
{
	heapFreeMemory(addr, &kernel_heap);
}

/**
 * @brief Initializes the kernel heap
 * @callgraph
 */
void initializeKernelHeap() 
{
	initializeHeap(&kernel_heap, KERNEL_HEAP_ADDR);
}

