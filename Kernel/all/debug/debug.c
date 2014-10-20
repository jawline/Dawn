#include <debug/debug.h>

void setDebugModeOn() {
	settingsExecuteLine("kernel.debug_state = 1");
}

void setDebugModeOff() {
	settingsExecuteLine("kernel.debug_state = 0");
}

unsigned char getDebugMode() {

	//Read the value of the debug_state flag, if its 1 return true
	if (strcmp(settingsReadValue("kernel.debug_state", "0"), "1") == 0) {
		return 1;
	}

	//Otherwise return 0
	return 0;
}
