#include <process/process.h>
#include <stdlib.h>
#include <debug/debug.h>

const unsigned long usedListExpansionSize = 1024;

void initializeUsedList(process_t* process) {
	process->usedListRoot = (MEM_LOC*) malloc(1024);
	process->usedListSize = 1024;
	process->usedListLocation = 0;
}

void expandUsedList(process_t* process) {
	MEM_LOC* new_loc =
			(MEM_LOC*) malloc(process->usedListSize + usedListExpansionSize);
	memcpy(new_loc, process->usedListRoot, process->usedListSize);
	free(process->usedListRoot);
	process->usedListSize += usedListExpansionSize;
	process->usedListRoot = new_loc;
}

//TODO: Used list shrinking

MEM_LOC usedListTop(process_t* process) {
	return *((MEM_LOC*) process->usedListRoot);
}

void usedListAdd(process_t* process, MEM_LOC location) {

	if (process->usedListSize == process->usedListLocation) {
		//Gotta expand the used list
		expandUsedList(process);
	}

	MEM_LOC* ulocation = process->usedListRoot;
	ulocation = (MEM_LOC*) (((MEM_LOC) ulocation)
			+ ((MEM_LOC) process->usedListLocation));

	*ulocation = location;
	ulocation--;

	process->usedListLocation += sizeof(MEM_LOC);
}

void usedListRemove(process_t* process, MEM_LOC location) {

	MEM_LOC* ulocation = process->usedListRoot;
	unsigned long iter = 0;

	for (iter = 0; iter < process->usedListLocation; iter +=
			sizeof(MEM_LOC)) {

		if (*ulocation == location) {
			memcpy(ulocation, ulocation + 1,
					process->usedListLocation - iter);
			process->usedListLocation -= sizeof(MEM_LOC);
			return;
		}

		ulocation++;
	}
}
