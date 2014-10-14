#ifndef _GDT_DEFINITIONS_DEF_H_
#define _GDT_DEFINITIONS_DEF_H_
#include <types/stdint.h>


//This structure is the structure of a single entry onto the GDT (Global descriptor table)
struct gdt_entry_struct
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

//The structure that should be used with lgdt to set up the GDT.
struct gdt_ptr_struct
{
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint32_t base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void initializeGdt();

#endif //_GDT_DEFINITIONS_DEF_H_
