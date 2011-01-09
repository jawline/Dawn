#include <debug/debug.h>

unsigned char debugMode = 1;

void setDebugModeOn()
{
	settingsExecuteLine("kernel.debug_state = 1");
}

void setDebugModeOff()
{
	settingsExecuteLine("kernel.debug_state = 0");
}

unsigned char getDebugMode()
{
	if (strcmp(settingsReadValue("kernel.debug_state"), "1") == 0) return 1;
	return 0;
}
