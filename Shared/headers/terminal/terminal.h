#ifndef _TERMINAL_DEFINITION_H_
#define _TERMINAL_DEFINITION_H_
#include <common.h>

struct abstract_terminal;

typedef void (*term_updatecursor)(struct abstract_terminal* term);
typedef void (*term_putc)(struct abstract_terminal* term, char c);

typedef void (*term_setfg)(struct abstract_terminal* term, uint8 col);
typedef void (*term_setbg)(struct abstract_terminal* term, uint8 col);

typedef void (*term_clear)(struct abstract_terminal* term);

struct abstract_terminal {
	uint8 m_used;

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_cursorX;
	unsigned int m_cursorY;
	unsigned int m_backgroundColour;
	unsigned int m_foregroundColour;

	void* m_backupData; //Can be used to store the data when a abstract terminal loses context.

	term_updatecursor f_updateCursor;
	term_putc f_putchar;

	term_setfg f_setForeground;
	term_setbg f_setBackground;

	term_clear f_clear;
};

typedef struct abstract_terminal terminal_t;

terminal_t* makeNewTerminal(unsigned int width, unsigned int height);
terminal_t* getTerminalInContext();
void setDefaultTerminalCallbacks(terminal_t* term);

#endif //_TERMINAL_DEFINITION_H_
