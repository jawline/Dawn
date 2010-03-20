// SimpleOS Entry Point
// Main function is the entry point of the kernel.

#include "multiboot.h"
#include "drivers/screen.h"
#include "gdt.h"
#include "cmos_time.h"
#include "panic.h"
#include "cmos.h"
#include <printf.h>
#include "phys_mm.h"
#include "virt_mm.h"
#include "heap.h"
#include "threads.h"
#include "fs_binds/initrd.h"
#include "reboot.h"

#include "vfs.h"

#include <stdlib.h>


void recur_list_dir(fs_node_t * node) {
    printf("Listing directory %s\n", node->name);
    struct dirent * ent = 0;

    uint32 iter = 0; 
    while (1) {

	ent = readdir_fs(node, iter);

	if (ent == 0) {
		break;	
	}


	fs_node_t * nent = finddir_fs(node, ent->name);

	free(ent);

	if (!is_directory(nent)) {
		printf("File %s\n", nent->name);
	} else {
		printf("Directory %s\n", nent->name);
		recur_list_dir(nent);
	}

	iter++;
    }
    printf("End of directory %s\n", node->name);
}

char CBuffer[1024];
int cptr = 0;


void exec_cb() {
	CBuffer[cptr] = '\0';
	printf("\nExecute %s\n", CBuffer);

	if (strcmp(CBuffer, "ls") == 0) {
		recur_list_dir(init_vfs()); //Init VFS returns a fs_node_t * to root if its already been initialized	
	} else if (strcmp(CBuffer, "reboot") == 0) {
		reboot(); //Reboot the system	
	} else if (strcmp(CBuffer, "shutdown") == 0) {
		printf("System safe to power off\n");
		for (;;) { }
	}
	
	CBuffer[0] = '\0';
	cptr = 0;
}

void kboard_callback(uint8 cb) {
	if (cb == '\r') //Carriage return
	{ exec_cb(); printf("\n:> ");  } else {

	
	if (cb == '\b') { cptr--; if (cptr >= 0) { CBuffer[cptr] = ' '; } if (cptr >= 0) { putc('\b'); putc(' ');putc('\b'); } }
	else {  CBuffer[cptr] = cb;
		cptr++;
		putc(cb);
	}

	}
}

extern uint32 end; //The end of the kernel

void Init_GDT() {
   prints("Initializing GDT");

   initialize_gdt();

   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

   prints("\n");
}

void Init_IDT() {
   prints("Initializing IDT");
   
   Initialize_IDT();
   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

   prints("\n");
}

void Init_Timer() {
   prints("Initializing the system timer");

   init_timer(50);

   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

   prints("\n");
}

void Print_Time_Info() {
	cmos_time_struct LoadedTime;
	fill_cmos_tstruct(&LoadedTime);

	char TBuf[128];
	itoa(LoadedTime.seconds, TBuf, 10);
	text_mode_write("Seconds: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.minutes, TBuf, 10);
	text_mode_write("Minutes: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.hours, TBuf, 10);
	text_mode_write("Hours: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.dayofweek, TBuf, 10);
	text_mode_write("Day of the week: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.day, TBuf, 10);
	text_mode_write("Day of the month: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.month, TBuf, 10);
	text_mode_write("Month: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.year, TBuf, 10);
	text_mode_write("Year: ");
	text_mode_write(TBuf);
	text_mode_write("\n");

	itoa(LoadedTime.century, TBuf, 10);
	text_mode_write("Century: ");
	text_mode_write(TBuf);
	text_mode_write("\n");
}

void Init_VM(struct multiboot * mboot_ptr, uint32 nd) { //Initialize virtual memory

   prints("Initializing virtual memory");
   
   init_phys_mm(nd);
   init_virt_mm();
   map_free_pages(mboot_ptr);
   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

   prints("\n");
}

void Init_INITRD(struct multiboot * mboot_ptr, fs_node_t * root) {

	if (mboot_ptr->mods_count != 1) {
		printf("Error, Initial RAM disk not loaded\n");
		return;	
	}

	fs_node_t * initrd = initialize_initrd(*((uint32*)mboot_ptr->mods_addr), "system", root);
	root_bind_node(initrd);
}

//Main entry point of the Kernel. It is passed the multiboot header by GRUB when the bootloader begins the Kernel execution. (Multiboot header defined in multiboot.h)
int main(struct multiboot *mboot_ptr)
{
    //Clear all the screen as there may be (Probably will be) stuff written on the screen by the bootloader.
    text_mode_clearscreen();

    //Begin the boot procedure.
    prints("SimpleOS Booting...\n\n");

    Init_GDT();
    Init_IDT();

    uint32 * mods_addr = (uint32 *)mboot_ptr->mods_addr;    
    mods_addr++;
    Init_VM(mboot_ptr, *mods_addr);
    
    Init_Timer();
    prints("\n");

    enable_interrupts();

    init_kheap();
    
    extern uint32 phys_mm_slock;
    uint32 pmmt = phys_mm_slock;
    uint32 freeram = 0;

    while (pmmt > PHYS_MM_STACK_ADDR) {
	pmmt -= sizeof(uint32);
        freeram += 4096; //1Kb of ram free
    }
    
    uint32 freemb = freeram / 1024;
    freemb = freemb / 1024;
    printf("Unmapped RAM %u (MBs) after heap initialization\n", freemb);    
    
    thread_t * maint = initialize_threading();
    initialize_thread_scheduler(maint);

    fs_node_t * rootfs = init_vfs();
    Init_INITRD(mboot_ptr, rootfs);

    init_keyboard();
    set_keyboard_callback(&kboard_callback);

    fs_node_t * initrd = finddir_fs(rootfs, "system");

    printf(":> ");
    for (;;) {  }

    return 0xDEADBABA;
}
