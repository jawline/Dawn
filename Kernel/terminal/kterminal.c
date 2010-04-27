//Kernel Terminal
#include <terminal/terminal.h>
static terminal_t* g_kernelTerminal = 0;

void kputc(char c);

void init_kterm()
{
	g_kernelTerminal = make_terminal(80, 25);

	text_mode_sett(g_kernelTerminal);
}

void kcls()
{
	if (!g_kernelTerminal) return;
	if (!g_kernelTerminal->m_used) return;

	g_kernelTerminal->f_clear(g_kernelTerminal);	
}

void kputc(char c)
{
	if (!g_kernelTerminal) return;
	if (!g_kernelTerminal->m_used) return;

	g_kernelTerminal->f_putchar(g_kernelTerminal, c);
}

void kputs(const char* s)
{
	for (; *s != 0; s++)
		kputc(*s);
}
