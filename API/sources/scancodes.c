#include <syscall/syscall.h>

DEFN_SYSCALL2(scancode_to_asci, 4, unsigned char, uint32_t);

char getAsciFromScancode(unsigned char sc, uint32_t flags) {
	char ch = syscall_scancode_to_asci(sc, flags);
	return ch;
}
