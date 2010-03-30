#include <mm/virt_mm.h>
#include <mm/phys_mm.h>

#include <interrupts/interrupt_handler.h>
#include "panic.h"
#include <interrupts/idt.h>
#include <debug/debug.h>

#define ReloadCR3() \
      __asm__ __volatile__ ("push %eax;mov %cr3,%eax;mov %eax,%cr3;pop %eax");

#define __flush_tlb_single(addr) \
   __asm__ __volatile__("invlpg %0": :"m" (*(char *) addr))


uint8 paging_setup = 0;
uint8 paging_enabled = 0;

page_directory_t* current_pagedir = 0;
page_directory_t* kernel_pagedir = 0;

uint32* page_directory = (uint32*) PAGE_DIR_VIRTUAL_ADDR;
uint32* page_tables = (uint32*) PAGE_TABLE_VIRTUAL_ADDR;

void page_fault (idt_call_registers_t regs)
{
  printf("[KERNEL FAULT]\n");

  uint32 faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  printf("[PAGE FAULT]\n[LOCATION 0x%x]\n", faulting_address);
  int present   = (regs.err_code & 0x1); // Page not present
  int rw = regs.err_code & 0x2;           // Write operation?
  int us = regs.err_code & 0x4;           // Processor was in user-mode?
  int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = regs.err_code & 0x10;          // Caused by an instruction fetch?
  if (present) printf("[PAGE PRESENT]\n"); else printf("[PAGE NOT PRESENT]\n");
  if (rw) printf("[WRITE OPERATION]\n");
  if (us) printf("[USER MODE]\n");
  if (reserved) printf("[RESERVED MEMORY OVERWRITE]\n");
  if (id) printf("[INSTRUCTION FETCH]\n");
  PANIC("Page Fault!");
  for (;;) ;
}

//Map the virtual address VA to the physical address PA with the appropriate flags.
//VA = Virtual Address, PA = Physical Address, Flags = The flags to be set with the page.
void map (uint32 va, uint32 pa, uint32 flags)
{

  uint32 virtual_page = va / 0x1000;
  uint32 pt_idx = PAGE_DIR_IDX(virtual_page); //Page table index

  // Find the appropriate page table for the physical address.
  if (page_directory[pt_idx] == 0)
  {
    //Null page table (Needs to be created) so allocate a frame and initialize (Null) it.
    page_directory[pt_idx] = (alloc_frame() & PAGE_MASK) | PAGE_PRESENT | PAGE_WRITE;
    DEBUG_PRINT("Debug Message: virt_mm.c allocated frame\n");
    page_tables[pt_idx * 1024] = 0; //TODO: WHY does this fix!?!?! Assuming null memory somewhere (Have to be) fix asap.
    memset (page_tables[pt_idx*1024], 0, 0x1000);
    DEBUG_PRINT("Debug Message: virt_mm.c memset new page table\n");
  }

  // Now that the page table definately exists, we can update the PTE.
  page_tables[virtual_page] = (pa & PAGE_MASK) | flags;
  __flush_tlb_single(va);
}

//Unmap the virtual address VA from its physical address
void unmap (uint32 va)
{
  uint32 virtual_page = va / 0x1000;
  
  page_tables[virtual_page] = 0;

  //Signal to the CPU that a page mapping has been invalidated.
  __flush_tlb_single(va);
}

//Get whether the virtual address is mapped or not (Returns 1 or 0, true or false)
//If Pa is non null sets pa to the physical address of the mapping
char get_mapping (uint32 va, uint32 *pa)
{
  uint32 virtual_page = va / 0x1000;
  uint32 pt_idx = PAGE_DIR_IDX(virtual_page);

  // Find the appropriate page table for 'va'.
  if (page_directory[pt_idx] == 0)
    return 0;

  if (page_tables[virtual_page] != 0)
  {
    if (pa) *pa = page_tables[virtual_page] & PAGE_MASK;
    return 1;
  }
}


inline void switch_page_directory (page_directory_t * nd)
{
  current_pagedir = nd;
  asm volatile ("mov %0, %%cr3" : : "r" (nd));
}

inline void start_paging() 
{
  uint32 cr0;

  asm volatile ("mov %%cr0, %0" : "=r" (cr0)); //Get the current value of cr0

  cr0 |= 0x80000000; //Bitwise or

  asm volatile ("mov %0, %%cr0" : : "r" (cr0)); //Set the value of cr0 to the new desired value
}

void mark_paging_enabled() 
{
	paging_setup = 1;
	paging_enabled = 1;
}

//Declared in virt_mm_asm.s
extern void asm_disable_paging();
extern void asm_enable_paging();

//Function to switch paging back on
void enable_paging() 
{
	if (paging_setup == 1 && paging_enabled == 1) 
	{
		asm_enable_paging();
		paging_enabled = 0;
		return; 
	}
}

//Function to switch paging off
void disable_paging() 
{
	if (paging_setup == 1 && paging_enabled == 0)
	{
		asm_disable_paging();		
		paging_enabled = 1;
		return;
	}
}


void init_virt_mm(uint32 mem_end) 
{
	register_interrupt_handler (14, &page_fault); //Register the page fault handler.
	
	uint32 i = 0;

	page_directory_t * pagedir = (page_directory_t *) alloc_frame(); //Paging isn't enabled so this should just give us 4kb at the end of used memory.
	
	//Null it all!!! (Initialize the table)
	memset(pagedir, 0, 4096);

	//TODO: Remake this strip of code
	//Needs replacing - IDMap the first 4MB of memory
	pagedir[0] = alloc_frame() | PAGE_PRESENT | PAGE_WRITE; //Allocate a 4kb "Frame" of memory for this page.
	memset(pagedir[0], 0, 4096);

	uint32* pt = (uint32*) (pagedir[0] & PAGE_MASK); //Pointer to the page directory 	

	for (i = 0; i < 1024; i++) //Loop 1024 times so 1024 * 4096 bytes of data are mapped (4MB)
	{
		pt[i] = i * 4096 | PAGE_PRESENT | PAGE_WRITE;	
	}

	// Assign the second-last table and zero it. (TODO: Find out if this is necessary)
	pagedir[1022] = alloc_frame() | PAGE_PRESENT | PAGE_WRITE;
	memset(pagedir[1022], 0, 4096);
	pt = (uint32*) (pagedir[1022] & PAGE_MASK);
	memset (pt, 0, 0x1000);

	pt[1023] = (uint32)pagedir | PAGE_PRESENT | PAGE_WRITE; //The last entry of table 1022 is the page directory

	pagedir[1023] = (uint32)pagedir | PAGE_PRESENT | PAGE_WRITE; //Loop back to the page directory

	switch_page_directory(pagedir); //Set the current page dir
	kernel_pagedir = current_pagedir;
	start_paging();

	//Map where the physical memory manager keeps its stack.
	uint32 pt_idx = PAGE_DIR_IDX((PHYS_MM_STACK_ADDR  >> 12));
	page_directory[pt_idx] = alloc_frame() | PAGE_PRESENT | PAGE_WRITE;
	memset (page_tables[pt_idx*1024], 0, 0x1000);

	mark_paging_enabled();
}

uint32 first_free_virt_addr() //Very simple function to find the first free address (Unmapped virtual address)
{ 

	uint32 iter = 0;
		for (iter = 0x2000; iter < 0xFFFFFFFF; iter += 0x1000)
		{
			if (get_mapping(iter, 0) == 1) {
				return iter;			
			}
		}

	return 0;
}
