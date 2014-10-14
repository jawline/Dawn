#include "tss.h"
#include <types/memory.h>
#include <stack/kstack.h>

tss_entry_t tss_entry;

void writeTss(int num, uint16_t ss0, uint32_t esp0)
{
   //Generate the base and limit values
   MEM_LOC base = (MEM_LOC) &tss_entry;
   MEM_LOC limit = base + sizeof(tss_entry) + 1;

   gdt_set_gate(num, base, limit, 0xE9, 0x00);

   //Null the TSS
   memset(&tss_entry, 0, sizeof(tss_entry));

   tss_entry.ss0  = ss0;  // Set the kernel stack segment.
   tss_entry.esp0 = esp0; // Set the kernel stack pointer.

   tss_entry.cs   = 0x0b;
   tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void setKernelStack(uint32_t reg)
{
	tss_entry.esp0 = reg;
}

void flushTss()
{
	__asm__ volatile("mov $0x2B, %ax; \
		      ltr %ax; \
		     ");
}

void initializeTss()
{
   writeTss(5, 0x10, KERNEL_STACK_START);
   flushGdt();
   flushTss();
}
