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

//The kernel callback for a keyboard event
//Registered with the kernel input manager in void post_init(); with a register_input_listener(DEVICE_KEYBOARD) call
void kernel_keyboard_callback(uint32 device, uint32 main, void* additional)
{

	//Create a new message to send to the all processes
	process_message da;

	//Initialize the message and set the data
	memset(&da, 0, sizeof(process_message));
	da.ID = INPUT_MESSAGE;
	da.message_data[0] = device;
	da.message_data[1] = main;
	da.message_data[2] = *((uint32*)additional);

	//Tell the scheduler to send the message to all processes with the INPUT_BIT flag set
	scheduler_global_message(da, INPUT_BIT);
}

//External links to the kernel page directory and kernel heap
extern page_directory_t* kernel_pagedir;
extern heap_t kernel_heap;

void post_init() 
{
    register_input_listener(DEVICE_KEYBOARD, &kernel_keyboard_callback);

    enable_interrupts(); //Fork process disables interrupts
    
    int result = kfork();

    if (result == 0) //Parent
    {
	int result = kfork();

        if (result == 0)
	{
		//System idle halts the processor between interrupts when it is active. This is to slow the processor, lower memory usage & heat etcetera.
		//TODO: Give system idle a priority within the scheduler so that the processor spends less time idling when other processes want to be powered
		enable_interrupts();

		//Rename the idle process
		set_process_name(get_current_process(), "System Idle");

		//Loop and continually halt the processor, this will cause the processor to idle between interrupts
		for (;;) { 
			asm volatile("hlt"); 
		}

	}
	else
	{
		//Rename this to other process (Just here to prove system works)
		set_process_name(get_current_process(), "Other Process");

		//KPROC
		MEM_LOC num = 13;
		MEM_LOC a;
		asm volatile("int $127" : "=a" (a) : "0" (num));

		for (;;)
		{
			scheduler_block_me();
		}
	}

    } else
    {
	    //Setup a terminal
	    terminal_t* p_terminal = 0;
	    extern terminal_t* g_kernelTerminal;

	    p_terminal = make_terminal(80, 25);
	    get_current_process()->m_pTerminal = p_terminal;

	    set_default_tcallbacks(p_terminal);
	    set_terminal_context(p_terminal);
	    printf("Hello world\n");
	    

	    //Find Line.x
	    fs_node_t* system = finddir_fs(init_vfs(), "system");
	    fs_node_t* root = finddir_fs(system, "root");

	    fs_node_t* line = finddir_fs(root, "Line.x");

	    rename_current_process("Line.x");

	    //Execute Line.x
	    if (line != 0)
	    {
		printf("Found Line.x\n");
		printf("Line.x returned %i\n", loadAndExecuteElf(line));
	    }

	    //Set the root console as active
            printf("Reverting back to root console\n");
	    set_terminal_context(g_kernelTerminal);

            MEM_LOC num = 13;
	    MEM_LOC a;
	    asm volatile("int $127" : "=a" (a) : "0" (num));
    }
}
