#include <terminal/terminal.h>
#include <stdlib.h>
#include <common.h>

terminal_t* make_terminal(unsigned int width, unsigned int height)
{
	//Allocate the memory for the terminal & null it
	terminal_t* ret = malloc(sizeof(terminal_t));
	memset(ret, 0, sizeof(terminal_t));

	//Set it to the width and height specified
	ret->m_width = width;
	ret->m_height = height;

	ret->m_backupData = malloc(sizeof(uint16) * ret->m_width * ret->m_height);

	return ret;
}

void free_terminal(terminal_t* term)
{
	free(term->m_backupData);
	free(term);
}
