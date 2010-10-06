#ifndef _END_CURRENT_PROCESS_API_DEF_H_
#define _END_CURRENT_PROCESS_API_DEF_H_
#include <syscall/syscall.h>

/**
 * @brief Call the exit syscall without supplying a return argument
 * @params None
 * @return Nothing
 */

void killcurrentprocess();

/**
 * @brief Call the exit syscall supplying a return argument
 * @params int Return. the return value of the process
 * @return Nothing
 */

void exit(int value);

#endif //_END_CURRENT_PROCESS_API_DEF_H_
