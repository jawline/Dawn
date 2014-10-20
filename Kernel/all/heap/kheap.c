#include <heap/heap.h>
#include <mm/virt_mm.h>
#define KERNEL_HEAP_ADDR KERNEL_START + 0x10000000

heap_t kernel_heap;

void* kmalloc(unsigned long mem) {
	return heapAllocateMemory(mem, &kernel_heap);
}

void kfree(void* addr) {
	heapFreeMemory(addr, &kernel_heap);
}

void initializeKernelHeap() {
	initializeHeap(&kernel_heap, KERNEL_HEAP_ADDR);
}

