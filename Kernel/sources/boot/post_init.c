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

void kernel_keyboard_callback(uint32 device, uint32 main, void* additional)
{
	char input = keyboard_chlookup_asci(main, *((uint32*)additional));
	process_t* cproc = get_current_process();

	cproc->m_inputBuffer[cproc->m_inputCurPosition] = input;

	if (cproc->m_inputCurPosition + 1 < cproc->m_inputBufferLength)
	{
		cproc->m_inputCurPosition = cproc->m_inputCurPosition + 1;
	}
}

fs_node_t* get_node(fs_node_t* node, const char* Name)
{
	return finddir_fs(node, Name);
}

extern page_directory_t* page_tables;
extern heap_t kernel_heap;

void post_init() 
{
    register_input_listener(DEVICE_KEYBOARD, &kernel_keyboard_callback);

    fs_node_t* system = get_node(init_vfs(), "system");
    fs_node_t* root = get_node(system, "root");

    fs_node_t* line = get_node(root, "Line.x");

    enable_interrupts();

    if (line != 0)
    {
	printf("Found Line.x\n");
	printf("Line.x returned %i\n", loadAndExecuteElf(line));
    }

    long long data = clock() + (CLOCKS_PER_SECOND * 3);

    printf("Reboot in 3 seconds...\n");

    while (data > clock())
    {
    }

    printf("Reboot.\n");

    kernel_reboot();
}
