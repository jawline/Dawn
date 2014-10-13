#ifndef _LOADER_DEF_H_
#define _LOADER_DEF_H_
#include <loaders/elf_header.h>
#include <fs/vfs.h>
#include <stdlib.h>

#define LOAD_ERROR_NOT_EXE -5
#define LOAD_ERROR_BAD_PLATFORM -6
#define LOAD_ERROR_BAD_HEAD -7
#define LOAD_ERROR_BAD_LOAD -8
#define LOAD_ERROR_BAD_MAP -9

int loadAndExecuteProgram(fs_node_t* Node, unsigned char);

#endif //_LOADER_DEF_H_
