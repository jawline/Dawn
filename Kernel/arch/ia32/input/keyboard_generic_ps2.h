#ifndef _KEYBOARD_GENERIC_PS2_
#define _KEYBOARD_GENERIC_PS2_
#include <interrupts/interrupt_handler.h>
#include <types/stdint.h>

typedef void (*keyboard_callback) (unsigned char ascik);

/**
 * Initialise keyboard interrupt handlers and clear anything that's in the key buffer.
 * Any keypresses after this is called (when IRQs are enabled) should be processed.
 */
void initializeKeyboard();

#endif //_KEYBOARD_GENERIC_PS2_
