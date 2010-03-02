// SimpleOS Entry Point
// Main function is the entry point of the kernel.

#include "multiboot.h"
#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "interrupt_handler.h"
#include "cmos_time.h"
#include "panic.h"
#include "cmos.h"

void Print_Memory_Information(struct multiboot *mboot_ptr) {
    char TEXT_BUFFER[256];

    text_mode_write("Memory Limits:\n");

    //Write out the lower and upper memory limits in the multiboot header
    itoa(mboot_ptr->mem_lower, TEXT_BUFFER, 10);
	
    text_mode_write("Lower Memory: ");
    text_mode_write(TEXT_BUFFER);
    text_mode_write("\n");

    itoa(mboot_ptr->mem_upper, TEXT_BUFFER, 10);
	
    text_mode_write("Upper Memory: ");
    text_mode_write(TEXT_BUFFER);
    text_mode_write("\n");
}

void Print_Loaded_Module_Information(struct multiboot *mboot_ptr) {
    char TEXT_BUFFER[256];

    text_mode_write("Modules Information:\n");
    itoa(mboot_ptr->mods_count, TEXT_BUFFER, 10);
    text_mode_write("Modules Count: ");
    text_mode_write(TEXT_BUFFER);    
    text_mode_write("\n");
}

void Init_GDT() {
   text_mode_write("Initializing GDT");
   initialize_gdt();
   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

    text_mode_write("\n");
}

void Init_IDT() {
   text_mode_write("Initializing IDT");
   Initialize_IDT();
   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

    text_mode_write("\n");
}

void Init_Paging(uint32 umem) {
   text_mode_write("Initializing Paging");
   Initialize_Paging(umem * 0x400);
   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

   text_mode_write("\n");
}

void Init_Timer() {
   text_mode_write("Initializing the system timer");

   init_timer(50);

   text_mode_set_x(70);
   text_mode_set_fg_color(GREEN);
   text_mode_write("[ OK ]");
   text_mode_set_fg_color(WHITE);

   text_mode_write("\n");
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

//Main entry point of the Kernel. It is passed the multiboot header by GRUB when the bootloader begins the Kernel execution. (Multiboot header defined in multiboot.h)
int main(struct multiboot *mboot_ptr)
{
    //Clear all the screen as there may be (Probably will be) stuff written on the screen by the bootloader.
    text_mode_clearscreen();

    //Begin the boot procedure.
    text_mode_write("SimpleOS Booting...\n\n");

    Init_GDT();
    Init_IDT();
    Init_Paging((mboot_ptr->mem_upper + mboot_ptr->mem_lower) - (0x1000 * 3));
    Init_Timer();
    text_mode_write("\n");

    //Print out the upper and lower limits of memory
    Print_Memory_Information(mboot_ptr);
    text_mode_write("\n");

    Print_Loaded_Module_Information(mboot_ptr);
    text_mode_write("\n");

    enable_interrupts();
    Init_SysTime();

    Print_Time_Info();

    char TBuffer[128];
    uint64 Val = 18446744073709551615;
    itoa_64(Val, TBuffer, 10);
    text_mode_write(TBuffer);

    return 0xDEADBABA;
}
