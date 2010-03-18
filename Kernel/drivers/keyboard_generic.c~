#include "keyboard_generic.h"
#include <interrupts/irq.h>
#include <interrupts/idt.h>
#include <printf.h>
#include <int_types.h>

char lowercase[256] = { 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0, 
			'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\r', 0, 
			'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 0, 0, 0, 
			'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '};

char uppercase[256] = { 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0, 
			'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\r', 0, 
			'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0, 0, 0, 
			'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' '};

char shifted[256] = { 0, 0, '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 0, 
			'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\r', 0, 
			'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', 0, 0, 0, 
			'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '};

uint8 shift_state = 0;
uint8 escaped = 0;
uint8 caps_lock = 0;

static keyboard_callback cback = 0;

void keyboard_isr_callback(idt_call_registers_t regs) {
    uint8 sc = inb(0x60);
    char new_char;

    switch(sc) {
        case 0x2a: 
            shift_state = 1; 
            break;
 
        case 0xaa: 
            shift_state = 0;
            break;

	case 0xe0:
	    escaped = 1;
	    break;

        case 0x36:
	    shift_state = !shift_state;
	    break;

	case 0x3a:
	    caps_lock = !caps_lock;
	    break;
 
        default:
	   if (escaped) { escaped = 0; break; }
           if (sc & 0x80) {
              /* Ignore the break code */
           } else {
	      if (shift_state != 0) 
              	cback(shifted[sc]);
	      else if (caps_lock)
		cback(uppercase[sc]);
	      else
              	cback(lowercase[sc]);
           }
           break;
     }
}

void init_keyboard() {
        printf("Registered interrupt handler\n");

	register_interrupt_handler(GET_IRQ(1), &keyboard_isr_callback);
	
	//Clear the existing keyboard buffer after the keyboard IRQ has been registered. Makes sure that the keyboard controller will trigger a IRQ
	uint8 temp, t2;
	while (1) {
		temp = inb(0x60);
		if (temp == t2) break;
		t2 = temp; 
	}

}

void set_keyboard_callback(keyboard_callback s) {
	cback = s;
}
