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
#include "../stack/kstack.h"

extern heap_t kernel_heap;
extern process_t* get_current_process();

//The kernel callback for a keyboard event
//Registered with the kernel input manager in void post_init(); with a register_input_listener(DEVICE_KEYBOARD) call
void kernelInputCallback(uint32 device, uint32 main, void* additional)
{

	//Create a new message to send to the all processes
	process_message da;

	//Initialize the message and set the data
	memset(&da, 0, sizeof(process_message));
	da.ID = INPUT_MESSAGE;
	da.message_data[0] = device;
	da.message_data[1] = main;
	da.message_data[2] = *((MEM_LOC*)additional);

	//Tell the scheduler to send the message to all processes with the INPUT_BIT flag set
	scheduler_global_message(da, INPUT_BIT);
}

void kernelMouseCallback(uint32 device, uint32 main, void* additional)
{

	//Create a new message to send to the all processes
	process_message da;

	//Initialize the message and set the data
	memset(&da, 0, sizeof(process_message));
	da.ID = INPUT_MESSAGE;
	da.message_data[0] = device;

	//Convert the message data to a fixed size message for the mouse callback
	mouse_input_t* message_data = additional;
	da.message_data[1] = message_data->i_byte;
	da.message_data[2] = message_data->mouse_x;
	da.message_data[3] = message_data->mouse_y;

	printf("%i %i %i\n", message_data->i_byte, message_data->mouse_x, message_data->mouse_y);

	//Tell the scheduler to send the message to all processes with the INPUT_BIT flag set
	scheduler_global_message(da, INPUT_BIT);
}

//External links to the kernel page directory and kernel heap
extern page_directory_t* kernel_pagedir;
extern heap_t kernel_heap;

int alive = 0;

void postInitialization() 
{
    //Disable interrupts
    disable_interrupts();

    //Register input listeners for the keyboard and mouse
    registerInputListener(DEVICE_KEYBOARD, &kernelInputCallback);
    registerInputListener(DEVICE_MOUSE, &kernelMouseCallback);

    //Enable interrupts now
    enable_interrupts();

    systemProcess();
}
