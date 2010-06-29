#ifndef _VIRTUAL_MEMORY_MANAGER_DEF_H_
#define _VIRTUAL_MEMORY_MANAGER_DEF_H_

#define PAGE_MASK 0xFFFFF000
#define PAGE_PRESENT   0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4

#define PAGE_DIR_VIRTUAL_ADDR   0xFFBFF000
#define PAGE_TABLE_VIRTUAL_ADDR 0xFFC00000
#define PAGE_DIR_IDX(x) ((uint32) x/1024)
#define PAGE_TABLE_IDX(x) ((uint32) x%1024)

#define KERNEL_START 0xC0000000
#define KERNEL_RESERVED_START KERNEL_START + 0x20001000
#define KERNEL_MEMORY_END 0xFFFFFFFF

#include <mm/pagedir.h>
#include <types/memory.h>

extern unsigned int PAGE_SIZE;

void init_virt_mm(uint32 val);

void mark_paging_enabled();
void map (POINTER va, POINTER pa, uint32 flags);
void unmap (POINTER va);
char get_mapping (uint32 va, uint32* pa);

void enable_paging();
void disable_paging();

#endif //_VIRTUAL_MEMORY_MANAGER_DEF_H_
