#ifndef _PROCESS_SCHEDULER_DEF_H_
#define _PROCESS_SCHEDULER_DEF_H_
#include <process/process.h>

void scheduler_init(process_t* kproc);
void scheduler_on_tick();

#endif //_PROCESS_SCHEDULER_DEF_H_
