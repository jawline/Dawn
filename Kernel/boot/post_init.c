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

fs_node_t* get_node(fs_node_t* node, const char* Name)
{
	return finddir_fs(node, Name);
}

void post_init() 
{

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
