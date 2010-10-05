#ifndef _MEMORY_DEF_H_
#define _MEMORY_DEF_H_
#include <types/memory.h>
#include <syscall/syscall.h>

/**
 * @ingroup System Info
 * 
 * @brief Returns the number of free frames of memory the system has available
 * @param None
 * @return Integer with the value of the number of free frames available
 */

MEM_LOC getNumberOfFreeFrames();

/**
 * @ingroup System Info
 * @brief Returns the size of each page/frame
 * @param None
 * @return A integer with the value of each page/frame (In bytes)
 */

unsigned long getPageSize();

#endif //_MEMORY_DEF_H_
