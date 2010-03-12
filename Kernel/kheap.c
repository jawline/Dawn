#include "heap.h"

heap_t kernel_heap;
#define HEAP_ADDR 0xC0000000

void init_kheap() {
	init_heap(&kernel_heap, HEAP_ADDR);
}

uint32 kmalloc(uint32 mem) {
	return alloc_mem(mem, &kernel_heap);
}

void kfree(uint32 addr) {
	free_mem(addr, &kernel_heap);
}
