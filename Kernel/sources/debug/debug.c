#include <debug/debug.h>

unsigned char debugMode = 0;

void setDebugModeOn()
{
	debugMode = 1;
}

void setDebugModeOff()
{
	debugMode = 0;
}

unsigned char getDebugMode()
{
	return debugMode;
}
