#include "terminal.h"
#include <stdlib.h>
#include <common.h>

terminal_t* make_terminal(unsigned int width, unsigned int height)
{
	terminal_t* ret = malloc(sizeof(terminal_t));
	memset(ret, 0, sizeof(terminal_t));

	ret->m_width = width;
	ret->m_height = height;

	return ret;
}
