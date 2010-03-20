#ifndef _GDT_DEFINITIONS_DEF_H_
#define _GDT_DEFINITIONS_DEF_H_
#include <int_types.h>


//This structure is the structure of a single entry onto the GDT (Global descriptor table)
struct gdt_entry_struct
{
   uint16 limit_low;           // The lower 16 bits of the limit.
   uint16 base_low;            // The lower 16 bits of the base.
   uint8  base_middle;         // The next 8 bits of the base.
   uint8  access;              // Access flags, determine what ring this segment can be used in.
   uint8  granularity;
   uint8  base_high;           // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t; 

//The structure that should be used with lgdt to set up the GDT.
struct gdt_ptr_struct
{
   uint16 limit;               // The upper 16 bits of all selector limits.
   uint32 base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void initialize_gdt();

#endif //_GDT_DEFINITIONS_DEF_H_
