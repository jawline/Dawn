#include <syscall/syscall.h>
#include <stdio.h>
#include <scheduler/process_scheduler.h>
#include <heap/heap.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>

void syscallPrint_t(const char* Line)
{
	if (getCurrentProcess()->m_pTerminal != 0)
	{
		while (*Line)
		{
			getCurrentProcess()->m_pTerminal->f_putchar(getCurrentProcess()->m_pTerminal, *Line);
			Line++;
		}
	}
}

void syscallClearscreen()
{
	if (getCurrentProcess()->m_pTerminal != 0)
	{
		getCurrentProcess()->m_pTerminal->f_clear(getCurrentProcess()->m_pTerminal);
	}
}

void syscallSetFgc(unsigned char fgc)
{
	if (getCurrentProcess()->m_pTerminal != 0)
	{
		getCurrentProcess()->m_pTerminal->f_setForeground(getCurrentProcess()->m_pTerminal, fgc);
	}	
}

void syscallSetBgc(unsigned char bgc)
{
	if (getCurrentProcess()->m_pTerminal != 0)
	{
		getCurrentProcess()->m_pTerminal->f_setBackground(getCurrentProcess()->m_pTerminal, bgc);
	}	
}
