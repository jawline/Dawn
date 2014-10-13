#include <types/stdint.h>
#include <debug/debug.h>

void syscallSetDebugMode(uint8_t Mode) {

	if (Mode != 0) {
		setDebugModeOn();
	} else {
		setDebugModeOff();
	}
}
