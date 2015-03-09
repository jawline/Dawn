#include "keyboard_generic_ps2.h"
#include <interrupts/irq.h>
#include <interrupts/idt.h>
#include <printf.h>
#include <types/stdint.h>
#include <input/keyboard.h>
#include <input/input.h>

static uint32_t flags = 0;

idt_call_registers_t keyboardIrqCallback(idt_call_registers_t regs) {
    uint8_t sc = inb(0x60);
    char new_char;

    switch(sc) {
        case 0x2a: 
            flags = flags | FLAG_SHIFT_STATE;
            break;
 
        case 0xaa: 
            flags = flags ^ FLAG_SHIFT_STATE;
            break;

	case 0xe0:
	    flags = flags | FLAG_ESCAPED; 
	    break;

	case 0x3a:

	    if ((flags & FLAG_CAPS_LOCK))
	    {
		//Caps lock is on
		flags = flags ^ FLAG_CAPS_LOCK; //Swap bit to 0 using XOR
            }
 	    else 
	    {
		flags = flags | FLAG_CAPS_LOCK; //Swap bit to 1 using OR
	    }

	    break;
 
        default:
	   if ((flags & FLAG_ESCAPED)) 
	   { 
		flags = flags ^ FLAG_ESCAPED; /* Set the bit to 0 */
		break; 
	   }
           if (sc & 0x80) 
	   {
              /* Ignore the break code */
           } 
	   else 
	   {
		sendInputMessage(DEVICE_KEYBOARD, sc, &flags);
           }
           break;
     }
}

void clearKeyboardBuffer() {
	int curr = -1;
	int prev = -1;

	do {
		prev = curr;
		curr = inb(0x60);
	} while (prev != curr);
}

void initializeKeyboard()  {
	registerInterruptHandler(GET_IRQ(1), &keyboardIrqCallback);
	clearKeyboardBuffer();
}
