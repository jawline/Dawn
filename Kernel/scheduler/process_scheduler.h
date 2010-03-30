#ifndef _PROCESS_SCHEDULER_DEF_H_
#define _PROCESS_SCHEDULER_DEF_H_

#include <process/process.h>

#define DEFAULT_PROCESS_QUANT 10

struct process_scheduler_entry 
{
	uint32 quant;
	process_t* process;
	struct process_scheduler_entry * next;
};

typedef struct process_scheduler_entry process_s_t;

void initialize_process_scheduler(process_t * kproc);
void process_scheduler_tick();
void list_processes();
uint32 current_pid();

#endif //_PROCESS_SCHEDULER_DEF_H_