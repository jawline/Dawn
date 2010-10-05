#ifndef _GET_PROCESS_INFO_API_DEF_H_
#define _GET_PROCESS_INFO_API_DEF_H_

#include <process/process_info.h>
#include <syscall/syscall.h>

/**
 * @ingroup Process Info
 * 
 * @brief This function allows a program to request the ID of the program at position n on the scheduler.
 * @param The id of a process on the scheduler
 * @return the process ID of that process if it exists. otherwise -1
 */
int getProcessID(unsigned int iter);

/**
 * @ingroup Process Info
 * 
 * @brief This function allows a program to request information on a another process. The information is returned in the process_info_t structure through the syscall API
 * @param The process ID of the function to get info from
 * @return process_info_t with information on the process if it exists. otherwise a blank process_info_t structure
 */
process_info_t getProcessInfo(int pid);

#endif //_GET_PROCESS_INFO_API_DEF_H_
