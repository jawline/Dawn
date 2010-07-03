//Kernel Terminal
#include <terminal/terminal.h>
terminal_t* g_kernelTerminal = 0;

void kputc(char c);

void init_kterm()
{
	//Make a new terminal and set the screens terminal to this new terminal

	g_kernelTerminal = make_terminal(80, 25);
	text_mode_sett(g_kernelTerminal);
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
	if (!g_kernelTerminal) return;
	if (!g_kernelTerminal->m_used) return;

	g_kernelTerminal->f_putchar(g_kernelTerminal, c);
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
