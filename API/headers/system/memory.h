#ifndef _MEMORY_DEF_H_
#define _MEMORY_DEF_H_
#include <types/memory.h>
#include <syscall/syscall.h>

MEM_LOC get_num_free_frames();
unsigned long get_page_size();

#endif //_MEMORY_DEF_H_
