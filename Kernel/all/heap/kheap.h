#ifndef _KERNEL_HEAP_DEF_H_
#define _KERNEL_HEAP_DEF_H_

void* kmalloc(unsigned long mem);
void kfree(void* addr);
void initializeKernelHeap();

#endif //_KERNEL_HEAP_DEF_H_
