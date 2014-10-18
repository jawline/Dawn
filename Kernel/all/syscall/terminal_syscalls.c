#include <syscall/syscall.h>
#include <stdio.h>
#include <scheduler/scheduler.h>
#include <heap/heap.h>
#include <panic/panic.h>
#include <mm/virt_mm.h>

void syscallPrint_t(const char* Line)
{
	if (getCurrentProcess()->processTerminal != 0)
	{
		while (*Line)
		{
			getCurrentProcess()->processTerminal->f_putchar(getCurrentProcess()->processTerminal, *Line);
			Line++;
		}
	}
}

void syscallClearscreen()
{
	if (getCurrentProcess()->processTerminal != 0)
	{
		getCurrentProcess()->processTerminal->f_clear(getCurrentProcess()->processTerminal);
	}
}

void syscallSetFgc(unsigned char fgc)
{
	if (getCurrentProcess()->processTerminal != 0)
	{
		getCurrentProcess()->processTerminal->f_setForeground(getCurrentProcess()->processTerminal, fgc);
	}	
}

void syscallSetBgc(unsigned char bgc)
{
	if (getCurrentProcess()->processTerminal != 0)
	{
		getCurrentProcess()->processTerminal->f_setBackground(getCurrentProcess()->processTerminal, bgc);
	}	
}
