#ifndef _PROCESS_DEFINITION_H_
#define _PROCESS_DEFINITION_H_
#include <mm/virt_mm.h>

#define KERNEL_PID 0

#define PROC_FLAG_KILLABLE 0x1

struct process_structure 
{

	char name[64];

	page_directory_t* page_dir;

	uint32 pid, pflags;

	uint32 esp, ebp, ebx, esi, edi;

};

typedef struct process_structure process_t;

process_t* initialize_kernel_process(); //Returns the kernel process

#endif
