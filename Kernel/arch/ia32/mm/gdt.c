#include <mm/gdt.h>

#define NUM_GDT_ENTRIES 6
gdt_entry_t gdt_entries[NUM_GDT_ENTRIES];
gdt_ptr_t   gdt_ptr;

void gdtSetGate(unsigned int, uint32_t, uint32_t, uint8_t, uint8_t);
extern void gdt_flush(uint32_t);

void initializeGdt() {
   memset(&gdt_entries, 0, sizeof(gdt_entry_t) * NUM_GDT_ENTRIES);

   gdt_ptr.limit = (sizeof(gdt_entry_t) * NUM_GDT_ENTRIES) - 1;
   gdt_ptr.base  = (uint32_t)&gdt_entries;

   gdtSetGate(0, 0, 0, 0, 0);                // Null segment
   gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment (Present, ring0, Executable)
   gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment (Present, ring0)
   gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment (Present, ring3, Executable)
   gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment (Present, ring3)

   flushGdt();
}

void flushGdt() {
   gdt_flush((uint32_t)&gdt_ptr);
}

void gdtSetGate(unsigned int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}
