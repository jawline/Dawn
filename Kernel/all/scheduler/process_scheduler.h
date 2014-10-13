#ifndef _PROCESS_SCHEDULER_DEF_H_
#define _PROCESS_SCHEDULER_DEF_H_
#include <process/process.h>
#define _STD_NANO_ 5

void schedulerInitialize(process_t* kproc);

void schedulerAdd(process_t* new_process);
void schedulerRemove(process_t* old_process);

void schedulerOnTick();
void schedulerBlockMe();

process_t* getCurrentProcess();
int schedulerNumProcesses();
void schedulerKillCurrentProcess();
process_t* schedulerReturnProcess(unsigned int iter);
process_t* schedulerGetProcessFromPid(unsigned int pid);

//Send a message to all processes
void schedulerGlobalMessage(process_message msg, unsigned int bit);

#endif //_PROCESS_SCHEDULER_DEF_H_
