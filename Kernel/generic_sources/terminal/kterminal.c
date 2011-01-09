//Kernel Terminal
#include <terminal/terminal.h>
#include <stdio.h>

terminal_t* g_kernelTerminal = 0;
terminal_t* g_terminalInContext = 0;

terminal_t* getTerminalInContext()
{
	return g_terminalInContext;
}

void kernel_terminal_putc(terminal_t* term, char c)
{
	if (term == g_terminalInContext)
		text_mode_tputc(term, c);
	else
	{
		//Write to the backup data which stores a copy of the terminal when its not in context
		text_mode_putc_prec(term->m_backupData, term->m_backgroundColour, term->m_foregroundColour, &term->m_cursorX, &term->m_cursorY, c);
	}
}

void kernel_terminal_setfg(terminal_t* term, uint8_t col)
{
	term->m_foregroundColour = col;

	if (term == g_terminalInContext)
		text_mode_tsetfg(term, col);
}

void kernel_terminal_setbg(terminal_t* term, uint8_t col)
{
	term->m_backgroundColour = col;

	if (term == g_terminalInContext)
		text_mode_tsetbg(term, col);
}

void kernel_terminal_clear(terminal_t* term)
{
	if (term == g_terminalInContext)
		text_mode_tclear(term);
	else
	{
		//Clear the screen using the direct video memory access on the backup data
		text_mode_clearscreen_prec(term->m_backupData, term->m_backgroundColour, &term->m_cursorX, &term->m_cursorY);
	}
}

void kernel_terminal_update_cursor(terminal_t* term)
{
	if (term == g_terminalInContext)
		text_mode_tup(term);
	else
	{
		//Don't need to do anything. Not in context
	}
}

void kputc(char c);

void initializeKernelTerminal()
{
	//Make a new terminal and set the screens terminal to this new terminal
	g_kernelTerminal = makeNewTerminal(80, 25);

	setDefaultTerminalCallbacks(g_kernelTerminal);
	setTerminalContext(g_kernelTerminal);
}

void setDefaultTerminalCallbacks(terminal_t* term)
{
	term->f_updateCursor = &kernel_terminal_update_cursor;
	term->f_putchar = &kernel_terminal_putc;

	term->f_setForeground = &kernel_terminal_setfg;
	term->f_setBackground = &kernel_terminal_setbg;

	term->f_clear = &kernel_terminal_clear;
}

void setTerminalContext(terminal_t* term)
{
	g_terminalInContext = term;
	text_mode_sett(term);
}

//Clear the kernel terminals screen
void kcls()
{
	if (!g_kernelTerminal) return;
	if (!g_kernelTerminal->m_used) return;

	g_kernelTerminal->f_clear(g_kernelTerminal);
}

//Put a character on the kernel terminals screen
void kputc(char c)
{
	if (g_kernelTerminal == 0)
	{

		text_mode_putc(c);

	}
	else
	{

		g_kernelTerminal->f_putchar(g_kernelTerminal, c);

	}
}

//Move the X cursor on the kernel terminal
void kmovecx(unsigned int x)
{
	g_kernelTerminal->m_cursorX = x;
	g_kernelTerminal->f_updateCursor(g_kernelTerminal);
}

//Move the Y cursor on the kernel terminal
void kmovecy(unsigned int y)
{
	g_kernelTerminal->m_cursorY = y;
	g_kernelTerminal->f_updateCursor(g_kernelTerminal);
}

//Get the x cursor on the kernel terminal
unsigned int kgetcx()
{
	return g_kernelTerminal->m_cursorX;
}

//Get the y cursor on the kernel terminal
unsigned int kgetcy()
{
	return g_kernelTerminal->m_cursorY;
}

//Put a string to the kernel terminal
void kputs(const char* s)
{
	for (; *s != 0; s++)
		kputc(*s);
}
