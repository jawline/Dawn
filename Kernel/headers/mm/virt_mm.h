#ifndef _VIRTUAL_MEMORY_MANAGER_DEF_H_
#define _VIRTUAL_MEMORY_MANAGER_DEF_H_

#define PAGE_MASK 0xFFFFF000
#define PAGE_PRESENT   0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4
#define PAGE_WRITETHROUGH 0x8

#define PAGE_DIR_VIRTUAL_ADDR   0xFFBFF000
#define PAGE_TABLE_VIRTUAL_ADDR 0xFFC00000

//Macros to find the page directory index and the page table index of a entry
#define PAGE_DIR_IDX(x) ((MEM_LOC) x / (PAGE_SIZE / sizeof(MEM_LOC)) )
#define PAGE_TABLE_IDX(x) ((MEM_LOC) x % (PAGE_SIZE / sizeof(MEM_LOC)) )

#define KERNEL_START 0xC0000000
#define KERNEL_RESERVED_START KERNEL_START + 0x20000000
#define KERNEL_MEMORY_END 0xFFFFFFFF

#include <mm/pagedir.h>
#include <types/memory.h>
#include <process/process.h>

extern unsigned int PAGE_SIZE;

void init_virt_mm(uint32 val);

void mark_paging_enabled();
void map (MEM_LOC va,MEM_LOC pa, uint32 flags);
void unmap (POINTER va);
char get_mapping (MEM_LOC va, MEM_LOC* pa);
page_directory_t* copyPageDir(page_directory_t* pagedir, process_t* process);

void enable_paging();
void disable_paging();

#endif //_VIRTUAL_MEMORY_MANAGER_DEF_H_
