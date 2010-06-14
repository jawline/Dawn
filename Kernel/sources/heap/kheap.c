#include <heap/heap.h>
#include <mm/virt_mm.h>
#define KERNEL_HEAP_ADDR KERNEL_START + 0x10000000

heap_t kernel_heap;
void init_kheap() 
{
	init_heap(&kernel_heap, KERNEL_HEAP_ADDR);
}

uint32 kmalloc(uint32 mem) 
{
	return alloc_mem(mem, &kernel_heap);
}

void kfree(uint32 addr)
{
	free_mem(addr, &kernel_heap);
}
