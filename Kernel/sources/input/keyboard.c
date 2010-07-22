#include <input/keyboard.h>
#include <debug/debug.h>

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

char keyboard_chlookup_asci(uint8 scancode,uint32 flags)
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

uint8 load_keytable_asci(fs_node_t* node)
{
	DEBUG_PRINT("Loading new keytable ");
	DEBUG_PRINT(node->name);
	DEBUG_PRINT("\n");

	if (!node) return 0;
	if (node->length != (256 * 3)) //Has to be the exact right size for a keytable
	{
		if (read_fs(node, 0, 256, lowercase) != 256) return 0;
		if (read_fs(node, 256, 256, uppercase) != 256) return 0;
		if (read_fs(node, 256 * 2, 256, shifted) != 256) return 0;	
	}
	else
	{
		return 0;
	}
}
