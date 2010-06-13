#include "post_init.h"
#include <heap/heap.h>
#include <stdio.h>
#include <stdlib.h>
#include <system/reboot.h>
#include <common.h>
#include <timers/clock.h>
#include <input/input.h>
#include <input/mouse.h>
#include <terminal/terminal.h>
#include <process/process.h>
#include <panic/panic.h>
#include <fs/vfs.h>
#include "../loaders/elf/parser.h"

extern heap_t kernel_heap;
extern uint32 end; //The end of the kernel

typedef int (*cmdfunc) (void * arg);

typedef struct 
{
	char* str;
	char* description;
	cmdfunc function;
} cmd_list;

cmd_list cmds[256];

int ls_func (void * null) 
{
	print_directory(init_vfs(), 1);

	return 1;
}

int reboot_f(void * null) 
{
	kernel_reboot();
}

int help_f(void * null) 
{
	unsigned int iter = 0;
	
	for (iter = 0; iter < 256; iter++)
	{
		if (cmds[iter].str != 0)
		{
			printf("Function: %s Description: %s\n", cmds[iter].str, cmds[iter].description);
		}
	}

}

int mem_map (void * null) 
{
	//Call the heap list_chunks function
	list_chunks(&kernel_heap);

	//Calculate free pages in bytes
	extern uint32 phys_mm_slock;
	uint32 pmmt = phys_mm_slock;
	uint32 freeram = 0;

	while (pmmt > PHYS_MM_STACK_ADDR) 
	{
		pmmt -= sizeof(uint32);
		freeram += 4096; //1Kb of ram free
	}
	    
	uint32 freemb = freeram / 1024; //Bytes to Kb
	freemb = freemb / 1024; //Kb to  Mb
	printf("Unmapped RAM %u (MBs)\n", freemb);    

	return 1;
}

int cpuid_out (void * null) 
{

	if (cpuid_supported())
	{
		printf("CPUID Supported\n");
		printf("CPU Vendor: %s\n", cpuid_getvendor());
		printf("CPU Features: 0x%x\n", cpuid_features());
	}
	else
	{
		printf("CPUID not supported\n");
	}

	return 1;
}

int list_proc(void* null)
{

}

int crash_me(void* null)
{
	uint32* FX = 0xABCDEF12;
	*FX = 3;
}

char CBuffer[1024];
int cptr = 0;

void exec_cb() 
{
	if (cptr <= 0) 
	{
		cptr = 0;
		return;
	}

	uint8 done = 0;
	CBuffer[cptr] = '\0';

	int i = 0;
	for (i = 0; i < 256; i++) 
	{
		if (strcmp(CBuffer, cmds[i].str) == 0) 
		{
			printf("\n");
			cmds[i].function(0);
			done = 1;
		}	
	}

	if (done == 0)
	{
		printf("\nError, Function %s not found\n", CBuffer);
	}

	cptr = 0;
}

void kboard_callback(uint8 cb) 
{
	disable_interrupts();

	if (cb == '\r') //Carriage return
	{ 
		exec_cb(); 
	} 
	else 
	{

		if (cb == '\b') 
		{ 
			cptr--; 
			if (cptr >= 0) 
			{ 
				CBuffer[cptr] = ' '; 
			} 
			if (cptr >= 0)
			{ 
				putc('\b'); 
				putc(' ');
				putc('\b'); 
			} 
		}
		else 
		{  
			CBuffer[cptr] = cb;
			cptr++;
			putc(cb);
		}

	}

	return;
}

void input_callback(uint32 dev, uint32 dat, void* flags)
{
	uint32 kflags = *(uint32*)flags;
	kboard_callback(keyboard_chlookup_asci(dat, kflags));
}

void mouse_callback(uint32 dev, uint32 dat, void* flags)
{
	mouse_input_t* i = (mouse_input_t*) flags;
}

void Entry(void* Arg)
{
	printf("Entry\n");
	return;
}

void Exit()
{
	printf("Exit\n");
	return;
}

fs_node_t* get_node(fs_node_t* node, const char* Name)
{
	return finddir_fs(node, Name);
}

void post_init() 
{

    unsigned int iter = 0;
    for (iter = 0; iter < 256; iter++)
    {
	cmds[iter].str = 0;
	cmds[iter].function = 0;
	cmds[iter].description = 0;
    }

    cmds[0].str = "list_dirs";
    cmds[0].description = "List all directories from root";
    cmds[0].function = ls_func;

    cmds[1].str = "print_allocmap";
    cmds[1].description = "Print kernel memory map and unmapped RAM";
    cmds[1].function = mem_map;

    cmds[2].str = "reboot";
    cmds[2].description = "Reboot machine";
    cmds[2].function = (cmdfunc) reboot_f;

    cmds[3].str = "help";
    cmds[3].description = "List functions & descriptions";
    cmds[3].function = (cmdfunc) help_f;

    cmds[4].str = "cpuid";
    cmds[4].description = "List capabilities of the processor";
    cmds[4].function = cpuid_out;

    cmds[5].str = "crash_me";
    cmds[5].description = "Page fault to crash the kernel";
    cmds[5].function = crash_me;

    register_input_listener(DEVICE_KEYBOARD, input_callback);
    register_input_listener(DEVICE_MOUSE, mouse_callback);

    fs_node_t* system = get_node(init_vfs(), "system");
    fs_node_t* root = get_node(system, "root");

    fs_node_t* line = get_node(root, "Line.x");

    if (line != 0)
    {
	printf("Found Line.x\n");
	void* Data = malloc(line->length);
	read_fs(line, 0, line->length, Data);
	e32_header head = parseElfHeader(Data, line->length);

	if (head.e_type == ELF_EXE)
	{
		printf("Line.x is a executable\n");
	}

	if (elfHeaderValid(head) == 1)
	{
		printf("Valid elf header\n");
	} else
	{
		printf("Invalid elf header\n");
	}

	if (getElfHeaderClass(head) == ELF_CLASS_32)
	{
		printf("32 bits (Loadable)\n");
	} else 
	{
		printf("Prolly unloadable. Gohna try anyways\n");
	}

	if (getElfHeaderVersion(head) == ELF_VERSION_CURRENT)
	{
		printf("Versions are correct (looking good)\n");
	}

	if (getElfHeaderData(head) == ELF_DATA_LSB)
	{
		printf("ELF_DATA_LSB = true. If ELF_CLASS_32 should be IA32 compatable\n");
	}

	printf("Line %x\n", head.e_type);

	unsigned int i = 0;
	uint32 is = 0;

	for (i = 0; i < head.e_phnum; i++)
	{
		e32_pheader pHead = parseElfProgramHeader(Data, line->length, head, i);

		if (pHead.p_type == PT_LOAD)
		{
			printf("Loadable segment\n");
		} else
		{
			printf("Unloadable segment 0x%x\n", pHead.p_type);
		}

		printf("Offset %x\n", pHead.p_offset);
		printf("Virtual %x Physical %x\n", pHead.p_vaddr, pHead.p_paddr);

		printf("Mapping virtual\n");

		if (pHead.p_type == PT_LOAD)
		{
			//Check that every byte is mapped (CHEAP)
			for (is = pHead.p_vaddr; is < pHead.p_vaddr + pHead.p_memsz; is += PAGE_SIZE)
			{
				if (get_mapping(is, 0) == 0)
				{
					//Map it
					uint32 newframe = alloc_frame();
					map(is, newframe, PAGE_PRESENT | PAGE_WRITE);
				}
			}
		
			memcpy(pHead.p_vaddr, Data + pHead.p_offset, pHead.p_memsz);

			asm volatile("jmp %0" :: "r" (pHead.p_vaddr));
		}
	}
    }
    
}
