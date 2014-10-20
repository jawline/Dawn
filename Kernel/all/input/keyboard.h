#ifndef _KEYBOARD_DEF_H_
#define _KEYBOARD_DEF_H_
#include <common.h>
#include <fs/vfs.h>

/* Advanced keyboard related input functions and definitions */
#define FLAG_SHIFT_STATE 1
#define FLAG_ESCAPED 2
#define FLAG_CAPS_LOCK 4

/**
 * Lookup a asci character from the keyboard lookup table, if that key doesnt map to an ASCII character return 0
 */

char lookupAsciCharacterFromScancode(unsigned char scancode, uint32_t flags);

#endif //_KEYBOARD_DEF_H_
