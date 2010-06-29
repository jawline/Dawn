#ifndef _PROCESS_SCHEDULER_DEF_H_
#define _PROCESS_SCHEDULER_DEF_H_
#include <process/process.h>
#define _STD_NANO_ 5

void scheduler_init(process_t* kproc);

void scheduler_add(process_t* new_process);
void scheduler_remove(process_t* old_process);

void scheduler_on_tick();

//Send a message to all processes
void scheduler_global_message(process_message msg, unsigned int bit);

#endif //_PROCESS_SCHEDULER_DEF_H_
