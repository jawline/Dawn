#include <input/keyboard.h>
#include <debug/debug.h>
#include <fs/vfs.h>

char lowercase[256] = 
{
			0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0, 
			'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\r', 0, 
			'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 0, 0, 0, 
			'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

char uppercase[256] = 
{
			0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0, 
			'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\r', 0, 
			'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0, 0, 0, 
			'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' '
};

char shifted[256] = 
{ 
			0, 0, '!', '"', '£', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 0, 
			'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\r', 0, 
			'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', 0, 0, 0, 
			'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '
};

char lookupAsciCharacterFromScancode(unsigned char scancode, uint32_t flags)
{
	char retch = 0;

	if ((flags & FLAG_CAPS_LOCK))
	{
		retch = uppercase[scancode];
	}
	else if ((flags & FLAG_SHIFT_STATE))
	{
		retch = shifted[scancode];
	}
	else
	{
		retch = lowercase[scancode];
	}

	return retch;
}
