#ifndef _INTERRUPT_DESCRIPTOR_TABLE_DEF_H_
#define _INTERRUPT_DESCRIPTOR_TABLE_DEF_H_
#include "headers/stdint.h"
#include "isr.h"
#include "irq.h"

/* A entry onto the interrupt descriptor table */

struct idt_entry
{
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
} __attribute__((packed));


/* A pointer passed to the processor to show the  size and position of the IDT */

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;

} __attribute__((packed));

typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} idt_call_registers_t;

void Initialize_IDT(); //Start the IDT!

#endif //_INTERRUPT_DESCRIPTOR_TABLE_DEF_H_
