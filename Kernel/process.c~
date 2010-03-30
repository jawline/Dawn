#include <process/process.h>
#include <stdlib.h>
#include <stdio.h>

//Returns whether the process is killable or not. (1 = true, 0 = false)
uint8 p_killable(process_t* proc) 
{
	if ((proc->pflags & PROC_FLAG_KILLABLE) == PROC_FLAG_KILLABLE) {
		return 1;	
	}

return 0;
}

process_t* initialize_kernel_process()
{
	extern page_directory_t* kernel_pagedir;

	process_t * ret = malloc(sizeof(process_t));
	memset(ret, 0, sizeof(process_t)); //Inialize it to 0
	ret->pid = KERNEL_PID;
	ret->page_dir = kernel_pagedir;
	strcpy(ret->name, "kernel");

	return ret;
}
