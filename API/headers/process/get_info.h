#ifndef _GET_PROCESS_INFO_API_DEF_H_
#define _GET_PROCESS_INFO_API_DEF_H_

#include <process/process_info.h>
#include <syscall/syscall.h>

int getProcessID(unsigned int iter);
process_info_t getProcessInfo(int pid);

#endif //_GET_PROCESS_INFO_API_DEF_H_
