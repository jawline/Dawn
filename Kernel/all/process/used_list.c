#include <process/process.h>
#include <stdlib.h>
#include <debug/debug.h>

const unsigned long usedListExpansionSize = 1024;

void initializeUsedList(process_t* process) {
	process->usedListRoot = malloc(1024 * sizeof(void*));
	process->usedListMaxItems = 1024;
	process->usedListNumItems = 0;
}

void expandUsedList(process_t* process) {
	void** newAllocation = malloc((process->usedListMaxItems + usedListExpansionSize) * sizeof(void*));
	memcpy(newAllocation, process->usedListRoot, process->usedListNumItems * sizeof(void*));
	free(process->usedListRoot);
	process->usedListRoot = newAllocation;
	process->usedListMaxItems += usedListExpansionSize;
}

void* usedListTop(process_t* process) {
	return process->usedListRoot[0];
}

char usedListHasItems(process_t* process) {
	return process->usedListRoot && process->usedListNumItems > 0;
}

void usedListAdd(process_t* process, void* location) {

	if (process->usedListNumItems == process->usedListMaxItems) {
		expandUsedList(process);
	}

	void** ulocation = &process->usedListRoot[process->usedListNumItems];

	*ulocation = location;
	ulocation--;

	process->usedListNumItems++;
}

void usedListRemove(process_t* process, void* location) {

	void** ulocation = process->usedListRoot;
	unsigned long iter = 0;

	for (unsigned int i = 0; i < process->usedListNumItems; i++) {
		if (process->usedListRoot[i] == location) {
			memcpy(&process->usedListRoot[i], &process->usedListRoot[i + 1], (process->usedListNumItems - i) * sizeof(void*));
			process->usedListNumItems--;
			return;
		}
	}
}

void usedListFree(process_t* process) {

	while (usedListHasItems(process)) {
		void* top = usedListTop(process);
		freeFrame(top);
		usedListRemove(process, top);
	}

	if (process->usedListRoot != 0) {
		free(process->usedListRoot);
	}
}