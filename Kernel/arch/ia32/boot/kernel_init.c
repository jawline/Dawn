
/* The purpose of this file is to initialize the kernel into a standard state with virtual memory, a heap, a virtual file system, a IDT and a GDT if necessary.
   Also to initialize any other subsystems such as the timer which may be required for Dawn to function properly
*/

#include "kernel_init.h"
#include <screen/screen.h>
#include <multiboot.h>
#include <stdio.h>
#include <mm/gdt.h>
#include <panic/panic.h>
#include <printf.h>
#include <mm/phys_mm.h>
#include <mm/virtual.h>
#include <heap/heap.h>
#include <stack/kstack.h>
#include <system/reboot.h>
#include <types/size_t.h>
#include <debug/debug.h>
#include <devices/pci/pci.h>
#include <terminal/terminal.h>
#include <initrd/initrd_header.h>
#include <scheduler/scheduler.h>
#include <input/input.h>
#include <syscall/syscall.h>
#include <settings/settingsmanager.h>
#include <devices/devices.h>
#include <terminal/kterminal.h>
#include <clock/clock.h>
#include <screen/screen.h>
#include <heap/kheap.h>
#include <gpf/gpf.h>
#include <stack/stack.h>

#include <fs/vfs.h>
#include <fs/dir.h>

#include <stdlib.h>

extern terminal_t* getTerminalInContext();

/*
 @brief IDT = Interrupt descriptor table. Allows interrupt calls to trigger functions int he kernel pointed to by it
 @callgraph
 */
void initializeIdt()
{
   Initialize_IDT();
   printf("IDT [OK]\n");
}

/**
 @brief This segment of code makes a copy of the RAMDISK module loaded by the bootloader in a reserved memory area in the kheap
 @callgraph
 */
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
		map(start_mapping_location, start_mapping_location, MEMORY_RESTRICTED_ACCESS);
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

	struct initial_ramdisk_header* head = ramdisk_new_location;

	unsigned char digest[16];
	MEM_LOC DLOC = head;
	DLOC += sizeof(struct initial_ramdisk_header);

	MDData(DLOC, head->ramdisk_size - sizeof(struct initial_ramdisk_header), digest);

    printf("Digest on our side: ");

	unsigned int iter = 0;
	for (iter = 0; iter < 16; iter++)
	{
		printf("%x", digest[iter]);
	}

	printf("\n");

    printf("Digest on their side: ");

    iter = 0;
    for (iter = 0; iter < 16; iter++)
    {
        printf("%x", head->ramdisk_checksum[iter]);
    }

    printf("\n");

	if (MDCompare(head->ramdisk_checksum, digest) == 0)
	{
		PANIC("Ramdisk CHECKSUM bad");

	}
	else
	{

		//Load the ramdisk only of consistancy check is coo
		fs_node_t* initrd = initialize_initrd(ramdisk_new_location, "system", init_vfs());

		bindnode_fs(init_vfs() , initrd);

	}
}

extern heap_t kernel_heap;

/**
 @brief Run all the initial -one time- kernel initialization routines - once this is called the Kernel assumes a valid Heap, Page directory, Physical and virtual memory manager, etc
 @callgraph
 */
void initializeKernel(struct multiboot* mboot_ptr, uint32_t initial_esp) //visual_output signals whether or not to call printf
{
	extern void k_end;
	MEM_LOC kEndLocation = (POINTER)&k_end;

	initializeMemory(mboot_ptr, KERNEL_START, kEndLocation, initial_esp);

	//Initialize the kernel heap
        initializeKernelHeap();

	//Initialize interrupts
	initializeIdt();

	//Initialize the devices connected to the system (Abstracted - Uses w/e method to contact devices it pleases)
	initializeDevices();

	initializeScreen();


	initializeGeneralProtectionFaultHandler();





	//System timers
	initializeSystemClock();

	//Init the virtual file system
	fs_node_t* rootfs = init_vfs();

	if (mboot_ptr->mods_count == 1)
	{
		//Load and hook the ramdisk to the root file system
		initializeRamdisk(*((LPOINTER)(mboot_ptr->mods_addr)), *((LPOINTER)(mboot_ptr->mods_addr + 4)), rootfs);
	}

	//Init the kernel terminal //TODO: Improoove terminals, Abstractions cool and all but mine really isn't very good
	initializeKernelTerminal();
	getTerminalInContext()->f_clear(getTerminalInContext());

	//Map the kernel stack

	MEM_LOC iterator;
	for (iterator = KERNEL_STACK_START - PAGE_SIZE; iterator >= KERNEL_STACK_START - KERNEL_STACK_SIZE; iterator -= PAGE_SIZE)
	{
		MEM_LOC page = allocateFrame();
		map(iterator, page, MEMORY_RESTRICTED_ACCESS);
	}

	//Initialize the system call interface
	kernelInitializeSyscallSystem();

	kernelInitializeSyscalls();

	//Initialize the system scheduler
	schedulerInitialize(initializeKernelProcess());

	//Input interfaces
	inputInitialize();

	//Initialize the settings manager
	initializeSettingsManager();
}
