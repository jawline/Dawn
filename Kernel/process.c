#include "process.h"
#include <stdlib.h>
#include <stdio.h>

process_t* kernel_process = 0;

//Returns whether the process is killable or not. (1 = true, 0 = false)
static uint8 killable(process_t* proc) 
{
	if ((proc->pflags & PROC_FLAG_KILLABLE) == PROC_FLAG_KILLABLE) {
		return 1;	
	}

return 0;
}

process_s_t* current_entry;

process_t* initialize_process_manager()
{
	extern page_directory_t* kernel_pagedir;
	process_t * ret = malloc(sizeof(process_t));
	memset(ret, 0, sizeof(process_t)); //Inialize it to 0
	ret->pid = 0;
	ret->page_dir = kernel_pagedir;
	strcpy(ret->name, "kernel");

	current_entry = malloc(sizeof(process_s_t));
	memset(current_entry, 0, sizeof(process_s_t));
	current_entry->process = ret;
	current_entry->next = current_entry; //Loop back onto itself (Think of the list as a circle =) )

	kernel_process = ret;
	
	return ret;
}

void list_processes_output(process_t* proc) {
	printf("Process %i name %s pagedir 0x%x ", proc->pid, proc->name, proc->page_dir);
	if (killable(proc)) {
		printf("the process is killable ");	
	} else {
		printf("the process cannot be killed ");
	}

	printf("\n");
}

void list_processes() 
{
	printf("Listing active processes\n");
	process_s_t* entry = current_entry;
	list_processes_output(entry->process);
	entry = entry->next;
	
	while(entry != current_entry) {
		list_processes_output(entry->process);
		entry = entry->next;
	} 

	printf("Done listing processes\n");
}

void process_manager_tick() {
}
