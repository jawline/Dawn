
/* The purpose of this file is to initialize the kernel into a standard state with virtual memory, a heap, a virtual file system, a IDT and a GDT if necessary. 
   Also to initialize any other subsystems such as the timer which may be required for SimpleOS to function properly
*/

#include "kernel_init.h"
#include "../drivers/screen.h"
#include <multiboot.h>
#include <stdio.h>
#include <mm/gdt.h>
#include <time/cmos_time.h>
#include <panic/panic.h>
#include <system/cmos.h>
#include <printf.h>
#include <mm/phys_mm.h>
#include <mm/virt_mm.h>
#include <heap/heap.h>
#include "../stack/kstack.h"
#include "../fs/initrd.h"
#include <system/reboot.h>
#include <types/size_t.h>
#include <debug/debug.h>

#include <fs/vfs.h>
#include <fs/dir.h>

#include <stdlib.h>

#define iprintf(x) if (visual_output) { printf(x); }

void init_screen() //All we really need to do here is clear the screen
{
	text_mode_clearscreen();
}

void init_GDT(int visual_output) 
{
   initialize_gdt();
   iprintf("GDT [OK]\n");
}

void init_IDT(int visual_output) 
{
   Initialize_IDT();
   iprintf("IDT [OK]\n");
}

//Initialize the physical and virtual memory managers
//Use the last loaded module (The one after the RAM disk)
void init_MemoryManagers(struct multiboot * mboot_ptr, int visual_output)
{
    uint32* mods_addr = (uint32*) mboot_ptr->mods_addr;    
    mods_addr++;

    init_phys_mm(*mods_addr);
    init_virt_mm(*mods_addr);
    map_free_pages(mboot_ptr);

    iprintf("Memory Managers (PMM, VMM) [OK]\n");
}

void init_ramdisk(struct multiboot * mboot_ptr, fs_node_t * root) 
{

	if (mboot_ptr->mods_count != 1) {
		printf("Error, Initial RAM disk not loaded\n");
		PANIC("No RAM disk");
		return;	
	}

	uint32* addr = mboot_ptr->mods_addr;

	fs_node_t* initrd = initialize_initrd( *addr, "system", init_vfs() /* Returns root */);

	bindnode_fs(init_vfs() /* returns root */ , initrd);
}

//Run all the initial -one time- kernel initialization routines - once this is called the Kernel assumes a valid Heap, Page directory, Physical and virtual memory manager, etc
void init_kernel(struct multiboot * mboot_ptr, int visual_output, uint32 initial_esp) //visual_output signals whether or not to call printf
{
	initialize_system_clock();
	initialize_input();

	init_screen();
	iprintf("Initialization Started\n");
	init_GDT(visual_output);
	init_IDT(visual_output);
	init_MemoryManagers(mboot_ptr, visual_output);

	//Initialize the kernel heap
        init_kheap();

	fs_node_t * rootfs = init_vfs();
	init_ramdisk(mboot_ptr, rootfs);

	//Reason for moving the stack. fork() needs to copy a stack, however when its < the kernels end (As grub sets it up) it will be identity mapped, not copied
	//If its moved into higher memory, it gets copied, so fork() causes two processes with indavidual stacks to be created.
	move_stack(KERNEL_STACK_START, KERNEL_STACK_SIZE, initial_esp);

	extern page_directory_t kernel_pagedir;
	page_directory_t npg = copy_page_dir(kernel_pagedir);
	switch_page_directory(npg);
	
	iprintf("End of initialization\n");
}
