/*
 * process.c
 *
 *  Created on: 15 Oct 2014
 *      Author: blake
 */

#include <process/process.h>

void renameCurrentProcess(const char* str) {
	setProcessName(getCurrentProcess(), str);
}

void setProcessName(process_t* proc, const char* Name) {
	strcpy(proc->name, Name);
}

void setProcessExecutionDirectory(process_t* proc, fs_node_t* node) {
	proc->executionDirectory = node;
}
