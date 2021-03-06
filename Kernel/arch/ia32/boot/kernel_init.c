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
void initializeIdt() {
	Initialize_IDT();
	printf("IDT [OK]\n");
}

/**
 @brief This segment of code makes a copy of the RAMDISK module loaded by the bootloader in a reserved memory area in the kheap
 @callgraph
 */
void initializeRamdisk(uint8_t* ramdisk_phys_start, uint8_t* ramdisk_phys_end, fs_node_t* root) {

	size_t ramdisk_size = ramdisk_phys_end - ramdisk_phys_start;

	printf("Ramdisk Size: 0x%x bytes\n", ramdisk_size);

	uint8_t* ramdiskNewLocation = malloc(ramdisk_size);

	//Identity map the ramdisk (1x1 mapping between virtual and physical)
	for (uint8_t* current = ramdisk_phys_start; current < ramdisk_phys_end; current += PAGE_SIZE) {
		map(current, current, MEMORY_RESTRICTED_ACCESS);
	}

	printf("MAP RD\n");

	memcpy(ramdiskNewLocation, ramdisk_phys_start, ramdisk_size);

	printf("CP RD\n");

	for (uint8_t* current = ramdisk_phys_start; current < ramdisk_phys_end; current += PAGE_SIZE) {
		unmap(current);
	}

	printf("UNMAP RD\n");

	unsigned char digest[16];
	
	struct initial_ramdisk_header* head = (struct initial_ramdisk_header*) ramdiskNewLocation;
	uint8_t* ramdiskDataLocation = ramdiskNewLocation + sizeof(struct initial_ramdisk_header);

	printf("PREPARING CHECKSUM\n");

	ASSERT(head->ramdisk_magic == RAMMAGIC, "Ramdisk MAGIC bad");

	MDData(ramdiskDataLocation, head->ramdisk_size - sizeof(struct initial_ramdisk_header), digest);

	//Print out RAMDisk MD5 hash
	printf("Digest on our side: ");
	for (unsigned int i = 0; i < 16; i++) {
		printf("%x", digest[i]);
	}
	printf("\n");

	//Print out the stored MD5 hash
	printf("Digest on their side: ");
	for (unsigned int i = 0; i < 16; i++) {
		printf("%x", head->ramdisk_checksum[i]);
	}
	printf("\n");

	ASSERT(MDCompare(head->ramdisk_checksum, digest), "Ramdisk CHECKSUM bad");

	//Well it looks like this RAMDisk is legit & loaded fine
	bindnode_fs(get_vfs(), initialiseRamdisk(ramdiskNewLocation, "system", get_vfs()));
}

/**
 * Run all the kernel initialisation routines - once this is called
 * the Kernel assumes a valid Heap, Page directory, Physical and virtual
 * memory manager, etc has been setup or a panic has occurred
 */
void initializeKernel(struct multiboot* mboot_ptr, uint32_t initial_esp) {

	extern void* k_end;

	initializeMemory(mboot_ptr, KERNEL_START, (MEM_LOC)&k_end, initial_esp);
	initializeKernelHeap();

	//Initialise interrupt handlers
	initializeIdt();
	initializeDevices();
	initializeScreen();
	initializeGeneralProtectionFaultHandler();
	initializeSystemClock();

	//Init the virtual file system
	fs_node_t* rootfs = get_vfs();

	ASSERT(mboot_ptr->mods_count == 1, "No RAMDisk or incorrect number of modules. Kernel should start with ramdisk only");

	//Load and hook the ramdisk to the root file system
	initializeRamdisk(*((LPOINTER)(mboot_ptr->mods_addr)), *((LPOINTER)(mboot_ptr->mods_addr + 4)), rootfs);

	//TODO: Improoove terminals, Abstractions cool and all but mine really isn't very good
	initializeKernelTerminal();
	getTerminalInContext()->f_clear(getTerminalInContext());

	//TODO: Move this into its own space
	//Map the kernel stack
	for (MEM_LOC iterator = KERNEL_STACK_START - PAGE_SIZE; iterator >= KERNEL_STACK_START - KERNEL_STACK_SIZE; iterator -= PAGE_SIZE) {
		MEM_LOC page = allocateFrame();
		map(iterator, page, MEMORY_RESTRICTED_ACCESS);
	}

	kernelInitializeSyscallSystem();
	kernelInitializeSyscalls();
	schedulerInitialize(initializeKernelProcess());
	inputInitialize();
	initializeSettingsManager();
}
