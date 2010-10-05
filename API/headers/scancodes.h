#ifndef _SCANCODES_API_DEF_H_
#define _SCANCODES_API_DEF_H_

/**
 * @ingroup Character conversion
 * 
 * @brief This function converts a scancode from the keyboard into a ASCI character
 * @param unsigned char sc, the scancode. uint32 flags, the flags supplied with the scancode (Uppercase, Shifted, Etc)
 * @return ASCI Character from the scan code if found.
 */

extern char getAsciFromScancode(unsigned char sc, uint32 flags); 

#endif //_SCANCODES_API_DEF_H_
