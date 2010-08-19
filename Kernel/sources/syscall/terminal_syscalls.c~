#include "syscall.h"
#include <stdio.h>
#include "../scheduler/default/process_scheduler.h"
#include <heap/heap.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>
extern process_t* get_current_process();

void syscallPrintf(const char* Line)
{
	if (get_current_process()->m_pTerminal != 0)
	{
		while (*Line)
		{
			get_current_process()->m_pTerminal->f_putchar(get_current_process()->m_pTerminal, *Line);
			Line++;
		}
	}
}

void syscallClearscreen()
{
	if (get_current_process()->m_pTerminal != 0)
	{
		get_current_process()->m_pTerminal->f_clear(get_current_process()->m_pTerminal);
	}
}

void syscallSetFgc(unsigned char fgc)
{
	if (get_current_process()->m_pTerminal != 0)
	{
		get_current_process()->m_pTerminal->f_setForeground(get_current_process()->m_pTerminal, fgc);
	}	
}

void syscallSetBgc(unsigned char bgc)
{
	if (get_current_process()->m_pTerminal != 0)
	{
		get_current_process()->m_pTerminal->f_setBackground(get_current_process()->m_pTerminal, bgc);
	}	
}
