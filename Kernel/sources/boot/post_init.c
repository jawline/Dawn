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
extern process_t* get_current_process();

//The kernel callback for a keyboard event
//Registered with the kernel input manager in void post_init(); with a register_input_listener(DEVICE_KEYBOARD) call
void kernelKeyboardCallback(uint32 device, uint32 main, void* additional)
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

void postInitialization() 
{
    register_input_listener(DEVICE_KEYBOARD, &kernelKeyboardCallback);

    enable_interrupts(); //Fork process disables interrupts

    int result = kfork();

    if (result == 0) //Parent
    	{

		//System idle halts the processor between interrupts when it is active. This is to slow the processor, lower memory usage & heat etcetera.
		//TODO: Give system idle a priority within the scheduler so that the processor spends less time idling when other processes want to be powered
		enable_interrupts();

		//Rename the idle process
		set_process_name(get_current_process(), "System Idle");
		scheduler_block_me();

		enable_interrupts();

		//Loop and continually halt the processor, this will cause the processor to idle between interrupts
		for (;;) { 
			asm volatile("hlt"); 
		}

    } else
    {
            asm volatile("cli");
	    switchToUserMode();
	    for (;;) { }

	    extern terminal_t* g_kernelTerminal;

	    //Give this process its own terminal
	    terminal_t* m_processTerminal = make_terminal(80, 25);

	    //Set up the new terminals callbacks then bring it into context
	    set_default_tcallbacks(m_processTerminal);
	    set_terminal_context(m_processTerminal);

	    //Set the current processes terminal to the new terminal
	    get_current_process()->m_pTerminal = m_processTerminal;

	    //Rename it to the file name of the program its gohna run
	    rename_current_process("Line.x");

	    //Find Line.x
	    fs_node_t* system = finddir_fs(init_vfs(), "system");
	    fs_node_t* root = finddir_fs(system, "root");

	    fs_node_t* line = finddir_fs(root, "Line.x");

	    //Execute Line.x
	    if (line != 0)
	    {
		printf("Found Line.x\n");
		printf("Line.x returned %i\n", loadAndExecuteElf(line));
	    } else
	    {
		printf("Error Line.x not found\n");
	    }

	    //Set the root console as active
            printf("Reverting back to root console\n");

	    //(Bring it into context)
	    set_terminal_context(g_kernelTerminal);

	    //Call syscall 13 triggers kill this process
            MEM_LOC num = 13;
	    MEM_LOC a;
	    asm volatile("int $127" : "=a" (a) : "0" (num));

	    //Just a simple protection in case for some reason the
	    //processor continues executing down this code path
            for (;;) {}
    }

    for (;;) { }
}
