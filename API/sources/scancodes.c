#include <syscall/syscall.h>

DEFN_SYSCALL2(scancode_to_asci, 4, unsigned char, uint32);

char getAsciFromScancode(unsigned char sc, uint32 flags)
{
	char ch = syscall_scancode_to_asci(sc, flags);
	return ch;
}
