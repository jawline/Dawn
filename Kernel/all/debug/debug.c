#include <debug/debug.h>

unsigned char debugMode = 1;

void setDebugModeOn() {

	//Change the kernel.debug_state to on flag in the system settings
	settingsExecuteLine("kernel.debug_state = 1");
}

void setDebugModeOff() {

	//Change the kernel.debug_state to off in the settings
	settingsExecuteLine("kernel.debug_state = 0");
}

unsigned char getDebugMode() {

	//If there is no kernel.debug_state flag, assume debug mode
	if (settingsGetEntry("kernel.debug_state") == 0) {
		return 1;
	}

	//Read the value of the debug_state flag, if its 1 return true
	if (strcmp(settingsReadValue("kernel.debug_state", "0"), "1") == 0) {
		return 1;
	}

	//Otherwise return 0
	return 0;
}
