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
#include <messages/messages.h>
#include <fs/vfs.h>
#include "../loaders/elf/parser.h"

extern heap_t kernel_heap;
extern uint32 end; //The end of the kernel
extern process_t* get_current_process();

void kernel_keyboard_callback(uint32 device, uint32 main, void* additional)
{
	process_message da;

	memset(&da, 0, sizeof(process_message));
	da.ID = INPUT_MESSAGE;
	da.message_data[0] = device;
	da.message_data[1] = main;
	da.message_data[2] = *((uint32*)additional);

	postbox_add(&get_current_process()->m_processPostbox, da);
}

fs_node_t* get_node(fs_node_t* node, const char* Name)
{
	return finddir_fs(node, Name);
}

extern page_directory_t* kernel_pagedir;
extern heap_t kernel_heap;

void post_init() 
{
    register_input_listener(DEVICE_KEYBOARD, &kernel_keyboard_callback);

    enable_interrupts(); //Fork process disables interrupts
    
    int result = kfork();

    if (result == 0) //Parent
    {
	enable_interrupts();

	printf("The kernel process is still running over here! And we has cookies to (Proof the multiple code paths/processes are working)\n");
	for (;;) {}
    } else
    {

	    process_message da;
	    memset(&da, 0, sizeof(process_message));

	    da.ID = 3;

	    postbox_add(&get_current_process()->m_processPostbox, da);

	    //Find Line.x
	    fs_node_t* system = get_node(init_vfs(), "system");
	    fs_node_t* root = get_node(system, "root");

	    fs_node_t* line = get_node(root, "Line.x");

	    enable_interrupts();

	    //Execute Line.x
	    if (line != 0)
	    {
		printf("Found Line.x\n");
		printf("Line.x returned %i\n", loadAndExecuteElf(line));
	    }

	    //Wait 3 seconds
	    long long data = clock() + (CLOCKS_PER_SECOND * 3);
	     
	    printf("Reboot in 3 seconds...\n");
	    
	    while (data > clock())
	    {
	    }

	    printf("Reboot.\n");

	    //Reboot system
	    kernel_reboot();
    }
}
