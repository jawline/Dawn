#include "keyboard_generic_ps2.h"
#include <interrupts/irq.h>
#include <interrupts/idt.h>
#include <printf.h>
#include <types/int_types.h>
#include <input/keyboard.h>
#include <input/input.h>

static uint32 flags = 0;

void keyboard_isr_callback(idt_call_registers_t regs) 
{
    uint8 sc = inb(0x60);
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
		send_input_message(DEVICE_KEYBOARD, sc, &flags);
           }
           break;
     }
}

void init_keyboard() 
{
	register_interrupt_handler(GET_IRQ(1), &keyboard_isr_callback);
	
	//Clear the existing keyboard buffer after the keyboard IRQ has been registered. Makes sure that the keyboard controller will trigger a IRQ
	uint8 temp, t2;
	while (1) 
	{
		temp = inb(0x60);
		if (temp == t2) break;
		t2 = temp; 
	}

}
