#ifndef _PROCESS_DEF_H_
#define _PROCESS_DEF_H_
#include <process/process_arch.h>
#include <process/postbox.h>
#include <heap/heap.h>
#include <fs/vfs.h>
#define INPUT_BIT 0x1 //First bit = Do I want to hear about input?!?

void setProcessName(process_t* proc, const char* Name);
void renameCurrentProcess(const char* str);
void setProcessExecutionDirectory(process_t* proc, fs_node_t* node);

#endif //_PROCESS_DEF_H_
