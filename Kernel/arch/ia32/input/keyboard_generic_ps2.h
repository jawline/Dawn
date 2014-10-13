#ifndef _KEYBOARD_GENERIC_PS2_
#define _KEYBOARD_GENERIC_PS2_

#include <interrupts/interrupt_handler.h>
#include <types/stdint.h>

typedef void (*keyboard_callback) (unsigned char ascik);

void initializeKeyboard();

#endif //_KEYBOARD_GENERIC_PS2_
