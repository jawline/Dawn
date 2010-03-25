#include "kernel_init.h"
#include "drivers/screen.h"
#include "multiboot.h"
#include <stdio.h>
#include "gdt.h"
#include "cmos_time.h"
#include "panic.h"
#include "cmos.h"
#include <printf.h>
#include "phys_mm.h"
#include "virt_mm.h"
#include "heap.h"
#include "threads.h"
#include "fs/initrd.h"
#include "reboot.h"

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
    uint32 * mods_addr = (uint32 *)mboot_ptr->mods_addr;    
    mods_addr++;

    init_phys_mm(mods_addr);
    init_virt_mm();
    map_free_pages(mboot_ptr);
    iprintf("Memory Managers (PMM, VMM) [OK]\n");
}

//Init the system clock
void init_Timer(int visual_output) 
{
   init_timer(50);
   iprintf("System Timer [OK]\n");
}

void init_ramdisk(struct multiboot * mboot_ptr, fs_node_t * root) 
{

	if (mboot_ptr->mods_count != 1) {
		printf("Error, Initial RAM disk not loaded\n");
		PANIC("No RAM disk");
		return;	
	}

	fs_node_t * initrd = initialize_initrd(*((uint32*)mboot_ptr->mods_addr), "system", init_vfs() /* Returns root */);
	bindnode_fs(init_vfs() /* returns root */ , initrd);
}

//Run all the initial -one time- kernel initialization routines - once this is called the Kernel assumes a valid Heap, Page directory, Physical and virtual memory manager, etc
void init_kernel(struct multiboot * mboot_ptr, int visual_output) //visual_output signals whether or not to call printf
{
	init_screen();
	iprintf("Initialization Started\n");
	init_GDT(visual_output);
	init_IDT(visual_output);
	init_MemoryManagers(mboot_ptr, visual_output);
	init_Timer(visual_output);


	//Initialize the kernel heap
        init_kheap();
   
	//Initialize the main thread (Or, in other words, setup the kernel as a single thread) and initialize the thread scheduler
	thread_t * maint = initialize_threading();
	initialize_thread_scheduler(maint);

	fs_node_t * rootfs = init_vfs();
	init_ramdisk(mboot_ptr, rootfs);
	
	iprintf("End of initialization\n");
}
