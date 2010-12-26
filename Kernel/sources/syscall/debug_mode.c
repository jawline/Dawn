#include <types/int_types.h>
#include <debug/debug.h>

void syscallSetDebugMode(uint8 Mode)
{
	if (Mode != 0)
	{
		setDebugModeOn();
	}
	else
	{
		setDebugModeOff();
	}
}
