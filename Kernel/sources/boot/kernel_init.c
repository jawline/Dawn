
/* The purpose of this file is to initialize the kernel into a standard state with virtual memory, a heap, a virtual file system, a IDT and a GDT if necessary. 
   Also to initialize any other subsystems such as the timer which may be required for SimpleOS to function properly
*/

#include "kernel_init.h"
#include <screen/screen.h>
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
#include <devices/pci/pci.h>
#include <terminal/terminal.h>
#include <initrd/initrd_header.h>

#include <fs/vfs.h>
#include <fs/dir.h>

#include <stdlib.h>

extern terminal_t* getTerminalInContext();

#define iprintf(x) if (visual_output) { printf(x); }

//GDT allows for segmented memory model on x86 processors, outdated. Using paging instead but the system still needs a GDT for things like ring-3 jumps
void initializeGdt(int visual_output) 
{
   initialize_gdt();
   initializeTss();
   iprintf("GDT [OK]\n");
}

//IDT = Interrupt descriptor table. Allows interrupt calls to trigger functions int he kernel pointed to by it
void initializeIdt(int visual_output) 
{
   Initialize_IDT();
   iprintf("IDT [OK]\n");
}

//Initialize the physical and virtual memory managers
//Use the last loaded module (The one after the RAM disk)
void initializeMemoryManagers(struct multiboot* mboot_ptr, MEM_LOC kernel_start, MEM_LOC kernel_end, int visual_output)
{

    kernel_end = kernel_end - kernel_start;

    init_phys_mm(kernel_end);
    initializeVirtualMemoryManager(kernel_end);

    map_free_pages(mboot_ptr);

    iprintf("Memory Managers (PMM, VMM) [OK]\n");
}

//This segment of code makes a copy of the RAMDISK module loaded by the bootloader in a reserved memory area in the kheap
void initializeRamdisk(MEM_LOC ramdisk_phys_start, MEM_LOC ramdisk_phys_end, fs_node_t * root) 
{


	/*	
	//OLD
	MEM_LOC addr = ramdisk_phys_start;

	fs_node_t* initrd = initialize_initrd( addr, "system", init_vfs());

	bindnode_fs(init_vfs() , initrd); 
	*/

	size_t ramdisk_size = ramdisk_phys_end - ramdisk_phys_start;

	printf("Ramdisk Size: 0x%x bytes\n", ramdisk_size);

	void* ramdisk_new_location = malloc(ramdisk_size);

	MEM_LOC start_mapping_location = ramdisk_phys_start;
	MEM_LOC end_mapping_location = ramdisk_phys_end;

	while (start_mapping_location < end_mapping_location)
	{
		map(start_mapping_location, start_mapping_location, PAGE_PRESENT | PAGE_WRITE);
		start_mapping_location += PAGE_SIZE;
	}

	start_mapping_location = ramdisk_phys_start;
	end_mapping_location = ramdisk_phys_end;

	memcpy(ramdisk_new_location, start_mapping_location, ramdisk_size);

	while (end_mapping_location > start_mapping_location)
	{
		unmap(end_mapping_location);
		end_mapping_location -= PAGE_SIZE;
	}

	printf("Computing consistency check: ");

	struct initial_ramdisk_header* head = ramdisk_new_location;

	unsigned char digest[16];
	MEM_LOC DLOC = head;
	DLOC += sizeof(struct initial_ramdisk_header);

	MDData(DLOC, head->ramdisk_size - sizeof(struct initial_ramdisk_header), digest);

	unsigned int iter = 0;
	for (iter = 0; iter < 16; iter++)
	{
		printf("%x", digest[iter]);
	}

	printf("\n");
	
	if (MDCompare(head->ramdisk_checksum, digest) == 0)
	{
		printf("Error: CHECKSUM Incorrect ");

		iter = 0;
		for (iter = 0; iter < 16; iter++)
		{
			printf("%x", head->ramdisk_checksum[iter]);
		}

		printf("\n");

		MEM_LOC dloc = ramdisk_new_location;
		dloc += head->ramdisk_size - 10;

		for (iter = 0; iter < 10; iter++)
		{
			unsigned char* byte = dloc;
			printf("%x ", *byte);
			dloc++;
		}
		
		free(ramdisk_new_location);

	}
	else
	{

		//Load the ramdisk only of consistancy check is coo
		fs_node_t* initrd = initialize_initrd(ramdisk_new_location, "system", init_vfs());

		bindnode_fs(init_vfs() , initrd);

	}
}

extern heap_t kernel_heap;

//Run all the initial -one time- kernel initialization routines - once this is called the Kernel assumes a valid Heap, Page directory, Physical and virtual memory manager, etc
void initializeKernel(struct multiboot * mboot_ptr, int visual_output, uint32 initial_esp) //visual_output signals whether or not to call printf
{
	//Load the archaic GDT 
	initializeGdt(visual_output);

	extern void k_end;
	MEM_LOC kEndLocation = (POINTER)&k_end;

	//Initialize the PMM and VMM
	initializeMemoryManagers(mboot_ptr, KERNEL_START, kEndLocation, visual_output);

	//Move the stack to a nicer location in memory
	moveStack(KERNEL_STACK_START, KERNEL_STACK_SIZE, initial_esp);

	//Interrupts description table
	initializeIdt(visual_output);

	//Initialize the kernel heap
        initializeKernelHeap();

	//Self Explanatory
	initializeScreen();

	//System timers
	initializeSystemClock();

	//Input interfaces
	initializeInputCallbacks();

	//Init the virtual file system
	fs_node_t * rootfs = init_vfs();
	
	//Initialize the devices file system
	init_device_fs(init_vfs());

	//Init the kernel terminal //TODO: Improoove terminals, Abstractions cool and all but mine really isn't very good
	initializeKernelTerminal();
	getTerminalInContext()->f_clear(getTerminalInContext());

	DEBUG_PRINT("KTerm Started\n");

	//Initialize the system call interface
	kernelInitializeSyscalls();

	//Initialize the devices connected to the system (Abstracted - Uses w/e method to contact devices it pleases)
	initializeDevices();

	if (mboot_ptr->mods_count == 1)
	{ 
		//Load and hook the ramdisk to the root file system
		initializeRamdisk(*((LPOINTER)(mboot_ptr->mods_addr)), *((LPOINTER)(mboot_ptr->mods_addr + 4)), rootfs);
	}

	//Initialize the system scheduler
	initializeScheduler(initializeKernelProcess());

	print_directory(init_vfs(), 1);
}
