#ifndef _PROCESS_DEFINITION_H_
#define _PROCESS_DEFINITION_H_
#include <mm/virt_mm.h>

#define PROC_FLAG_KILLABLE 0x1

struct process_structure 
{

	char name[64];

	page_directory_t* page_dir;

	uint32 pid, pflags;

	uint32 esp, ebp, ebx, esi, edi;

};

typedef struct process_structure process_t;

struct process_scheduler_entry 
{
	process_t* process;
	struct process_scheduler_entry * next;
};

typedef struct process_scheduler_entry process_s_t;

process_t* initialize_process_manager(); //Returns the kernel process
void process_manager_tick();

#endif
