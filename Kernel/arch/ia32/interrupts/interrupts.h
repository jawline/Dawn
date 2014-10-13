/**

This file provides the IA32 implementions of enable and disable interrupts

**/

#ifndef _IA32_INTERRUPTS_DEF_H_
#define _IA32_INTERRUPTS_DEF_H_

#define disableInterrupts() __asm__ volatile ("cli");
#define enableInterrupts() __asm__ volatile ("sti");

#define haltTillNextInterrupt() __asm__ volatile ("hlt");

#endif //_IA32_INTERRUPTS_DEF_H_
