#ifndef _CURRENT_PROCESS_API_
#define _CURRENT_PROCESS_API_
#include <process/process.h>
#include <syscall/syscall.h>

extern process_t* cprocess();
extern process_t* get_process(unsigned int number); //Asks the scheduler for the nTH process on its list of processes and returns it (Not the PID)

#endif //_CURRENT_PROCESS_API_
