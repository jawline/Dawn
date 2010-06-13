#include <mm/virt_mm.h>
#include <mm/phys_mm.h>

#include <interrupts/interrupt_handler.h>
#include <panic/panic.h>
#include <interrupts/idt.h>
#include <debug/debug.h>
#include <types/memory.h>

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

POINTER free_kernel_virtual_address();

unsigned int PAGE_SIZE = 4096;

extern uint32 used_mem_end; //End of kernel used memory at initialization of paging

void page_fault (idt_call_registers_t regs)
{
  cls();
  printf("--------------------------------------------------------------------------------");
  printf("|  W        W       OO         OO        OO       PPPPPP   SSSSSSS  YY     YY  |");
  printf("|  W        W     OO  OO     OO  OO    OO  OO     P    P   S         YY   YY   |");
  printf("|  W        W    O      O   O      O  O      O    P    P   S          YY YY    |");
  printf("|  W   WW   W    O      O   O      O  O      O    PPPPPP   S           YYY     |");
  printf("|  W  W  W  W    O      O   O      O  O      O    P        SSSSSSS      Y      |");
  printf("|  W W    W W    O      O   O      O  O      O    P              S      Y      |");
  printf("|  WW      WW     OO  OO     OO  OO    OO  OO     P              S      Y      |");
  printf("|  W        W       OO         OO        OO       P        SSSSSSS      Y      |");
  printf("--------------------------------------------------------------------------------");

  uint32 faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  printf("| The kernel has crashed. The reason was a paging fault.                       |");
  printf("| below are the details                                                        |");
  printf("| current page directory location 0x%x", current_pagedir);
  kmovecx(79);
  printf("|");
 
  if (current_pagedir == kernel_pagedir)
  {
  	printf("| Current page directory = Kernel page directory                               |");
  }
  else
  {
  	printf("| Current page directory != Kernel page directory                              |");
  }

  printf("|                                                                              |");
  printf("|                                                                              |");
  printf("| Page fault at location 0x%x", faulting_address);
  kmovecx(79);
  printf("|");
  

  int present   = (regs.err_code & 0x1); // Page not present
  int rw = regs.err_code & 0x2;           // Write operation?
  int us = regs.err_code & 0x4;           // Processor was in user-mode?
  int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

  if (present) 
  {
  	printf("| PAGE PRESENT                                                                 |");
  } 
  else 
  {
  	printf("| PAGE NOT PRESENT                                                             |");
  }
  
  if (rw)
  {
	printf("| WRITE OPERATION                                                              |");
  }
  else
  {
  	printf("| READ OPERATION                                                               |");
  }

  if (us)
  {
  	printf("| USER MODE FAULT                                                              |");
  }
  else
  {
  	printf("| KERNEL MODE FAULT                                                            |");
  }

  if (reserved) 
  {
 	printf("| RESERVED MEMORY OVERWRITE                                                    |");
  }
  else
  {
  	printf("| NOT RESERVED MEMORY OVERWRITE                                                |");
  }

  if (id)
  {
 	printf("| INSTRUCTION FETCH                                                            |");
  }
  else
  {
	printf("| NOT INSTRUCTION FETCH                                                        |");
  }

  printf("--------------------------------------------------------------------------------");

  PANIC("Page Fault!");

  for (;;) ;
}

//Map the virtual address VA to the physical address PA with the appropriate flags.
//VA = Virtual Address, PA = Physical Address, Flags = The flags to be set with the page.
void map (POINTER va, POINTER pa, uint32 flags)
{
  POINTER virtual_page = (POINTER)(((MEM_LOC)va) / 0x1000);
  PAGE_INDEX pt_idx = PAGE_DIR_IDX(virtual_page); //Page table index

  // Find the appropriate page table for the physical address.
  if (page_directory[pt_idx] == 0)
  {
    //Null page table (Needs to be created) so allocate a frame and initialize (Null) it.
    page_directory[pt_idx] = (alloc_frame()) | PAGE_PRESENT | PAGE_WRITE;
    ReloadCR3();
  }

  // Now that the page table definately exists, we can update the PTE.
  page_tables[(MEM_LOC)virtual_page] = (((MEM_LOC)pa)) | flags;
  __flush_tlb_single(va);
}

//Unmap the virtual address VA from its physical address
void unmap (POINTER va)
{
  POINTER virtual_page = (POINTER)(((MEM_LOC)va) / 0x1000);
  
  page_tables[(MEM_LOC)virtual_page] = 0;

  //Signal to the CPU that a page mapping has been invalidated.
  __flush_tlb_single((MEM_LOC)va);
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

  return 0;
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

uint32 get_table(uint32 address)
{
	return address / PAGE_SIZE / 1024;
}

//Used in initialization only!
void identity_map_pages(page_directory_t* pagedir)
{

	MEM_LOC frame = alloc_frame();
	pagedir[0] = frame | PAGE_PRESENT | PAGE_WRITE;

	//ID map the first 4MB Of memory
	POINTER pt = (POINTER) frame; //Pointer to the page directory

	unsigned int i = 0;
	for (i = 0; i < 1024; i++) //Loop 1024 times so 1024 * 4096 bytes of data are mapped (4MB)
	{
		pt[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_WRITE;	
	}

	pagedir[get_table(KERNEL_START)] = pagedir[0];

}

void init_virt_mm(uint32 mem_end) 
{
	register_interrupt_handler (14, &page_fault); //Register the page fault handler.
	
	unsigned int i = 0;

	page_directory_t * pagedir = (page_directory_t *) alloc_frame(); //Paging isn't enabled so this should just give us 4kb at the end of used memory.
	
	for (i = 0; i < 1024; i++) {
		if (pagedir[i] != 0) {
			pagedir[i] = 0;
		}
	}

	//Null it all!!! (Initialize the table)
	memset(pagedir, 0, PAGE_SIZE);

	identity_map_pages(pagedir);

	// Assign the second-last table and zero it.
	MEM_LOC frame = alloc_frame();
	pagedir[1022] = (frame & PAGE_MASK) | PAGE_PRESENT | PAGE_WRITE;
	POINTER pt = (POINTER) frame;
	memset (pt, 0, PAGE_SIZE);

	pt[1023] = ((MEM_LOC)pagedir & PAGE_MASK) | PAGE_PRESENT | PAGE_WRITE; //The last entry of table 1022 is the page directory

	pagedir[1023] = ((MEM_LOC)pagedir & PAGE_MASK) | PAGE_PRESENT | PAGE_WRITE; //Loop back to the page directory

	switch_page_directory(pagedir); //Set the current page dir
	kernel_pagedir = current_pagedir;
	start_paging();

	//Map where the physical memory manager keeps its stack.
	uint32 pt_idx = PAGE_DIR_IDX((PHYS_MM_STACK_ADDR  >> 12));
	frame = alloc_frame();
	page_directory[pt_idx] = (frame & PAGE_MASK) | PAGE_PRESENT | PAGE_WRITE;

	memset((POINTER)frame, 0, PAGE_SIZE);

	mark_paging_enabled();

	for (i = get_table(KERNEL_START); i < 1022; i++)
	{
		if (page_directory[i] == 0) {
			MEM_LOC address = alloc_frame();
			page_directory[i] = (address & PAGE_MASK) | PAGE_PRESENT | PAGE_WRITE;
			POINTER pt = free_kernel_virtual_address();
			map(pt, address, PAGE_PRESENT | PAGE_WRITE);
			memset(pt, 0, PAGE_SIZE);
			unmap(pt);
		}
	}
}

MEM_LOC copy_page(MEM_LOC pt)
{
	MEM_LOC new_page_addr = alloc_frame();

	POINTER temp_read_addr = free_kernel_virtual_address();
	map(temp_read_addr, (POINTER) pt, PAGE_PRESENT | PAGE_WRITE);

	POINTER temp_write_addr = free_kernel_virtual_address();
	map(temp_write_addr, (POINTER) new_page_addr, PAGE_PRESENT | PAGE_WRITE);

	memcpy(temp_write_addr, temp_read_addr, PAGE_SIZE);

	unmap(temp_read_addr);
	unmap(temp_write_addr);

	return new_page_addr;
}

MEM_LOC copy_page_table(MEM_LOC pt, uint8 copy)
{
	MEM_LOC new_page_table = alloc_frame();

	POINTER temp_read_addr = free_kernel_virtual_address();
	map(temp_read_addr, pt, PAGE_PRESENT | PAGE_WRITE);

	POINTER temp_write_addr = free_kernel_virtual_address();
	map(temp_write_addr, new_page_table, PAGE_PRESENT | PAGE_WRITE);
	memset(temp_write_addr, 0, PAGE_SIZE);	
	
	unsigned int i = 0;
	for (i = 0; i < 1024; i++)
	{
		if (temp_read_addr[i] != 0)
		{
			MEM_LOC New_Frame = copy_page(temp_read_addr[i]);
			temp_write_addr[i] = New_Frame | PAGE_PRESENT | PAGE_WRITE;
		}
		else
		{
			//Nufink to copy
		}
	}

	unmap(temp_read_addr);
	unmap(temp_write_addr);

	return new_page_table;
}

page_directory_t* copy_page_dir(page_directory_t* pagedir) 
{
	disable_interrupts(); //Disable interrupts

	page_directory_t* return_location = (page_directory_t*) alloc_frame();

	DEBUG_PRINT("Copying page dir ");
	DEBUG_PRINTX(pagedir);
	DEBUG_PRINT("\n");

	POINTER being_copied = free_kernel_virtual_address();
	map(being_copied, pagedir, PAGE_PRESENT | PAGE_WRITE);

	POINTER copying_to = free_kernel_virtual_address();
	map(copying_to, return_location, PAGE_PRESENT | PAGE_WRITE);
	memset(copying_to, 0, PAGE_SIZE);

	DEBUG_PRINT("Mapped copying and to be copied\n");

	//First 4 megabytings are ID Mapped. Kernel pages are identical across all page directories. The rest gets copied

	copying_to[0] = being_copied[0];

	unsigned int i = 0;
	for (i = 1; i < get_table(KERNEL_START); i++)
	{
		if (being_copied[i] != 0)
		{

			MEM_LOC Location = copy_page_table(being_copied[i] & PAGE_MASK, 1);
			copying_to[i] = Location | PAGE_PRESENT | PAGE_WRITE;

		}
		else
		{
			//Do nuting iniyt

		}
	}

	for (i = get_table(KERNEL_START); i < 1022; i++)
	{
		copying_to[i] = being_copied[i];
	}

	// Assign the second-last table and zero it.
	MEM_LOC frame = alloc_frame();
	pagedir[1022] = frame | PAGE_PRESENT | PAGE_WRITE;

	POINTER pt = free_kernel_virtual_address();
	map(pt, frame, PAGE_PRESENT | PAGE_WRITE);
	memset (pt, 0, PAGE_SIZE);

	pt[1023] = (uint32)pagedir | PAGE_PRESENT | PAGE_WRITE; //The last entry of table 1022 is the page directory
	unmap(pt);

	copying_to[1023] = ((MEM_LOC)return_location) | PAGE_PRESENT | PAGE_WRITE; //Loop back address

	unmap(being_copied);
	unmap(copying_to);

	page_directory_t* backup = current_pagedir;

	switch_page_directory(pagedir);

		unmap(being_copied);
		unmap(copying_to);

	switch_page_directory(backup);

	return return_location;
}

POINTER free_kernel_virtual_address()
{
	unsigned int i = KERNEL_RESERVED_START; //Start after the IDMapped region

	for (i; i < KERNEL_MEMORY_END; i += PAGE_SIZE)
	{
		if (get_mapping(i, 0) == 0)
		{
			return i;
		}
	}

	return 0; //NullPtr = FAIL
}
