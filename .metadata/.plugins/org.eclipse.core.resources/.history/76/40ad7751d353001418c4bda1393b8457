#include <process/process.h>
#include <stdlib.h>
#include <debug/debug.h>

const unsigned long usedListExpansionSize = 1024;

void initializeUsedList(process_t* process)
{
	process->m_usedListRoot = (MEM_LOC*) malloc(1024);
	process->m_usedListSize = 1024;
	process->m_usedListLocation = 0;
}

void expandUsedList(process_t* process)
{
	MEM_LOC* new_loc = (MEM_LOC*) malloc(process->m_usedListSize + usedListExpansionSize);

	memcpy(new_loc, process->m_usedListRoot, process->m_usedListSize);

	free(process->m_usedListRoot);

	process->m_usedListSize += usedListExpansionSize;
	process->m_usedListRoot = new_loc;
}

//TODO: Used list shrinking

MEM_LOC usedListTop(process_t* process)
{
	MEM_LOC* ulocation = process->m_usedListRoot;
	return *ulocation;
}

void usedListAdd(process_t* process, MEM_LOC location)
{
	if (process->m_usedListSize == process->m_usedListLocation)
	{
		//Gotta expand the used list
		expandUsedList(process);
	}

	MEM_LOC* ulocation = process->m_usedListRoot;
	ulocation = (MEM_LOC*) (((MEM_LOC) ulocation) + ((MEM_LOC)process->m_usedListLocation));

	*ulocation = location;
	ulocation--;

	process->m_usedListLocation += sizeof(MEM_LOC);
}

void usedListRemove(process_t* process, MEM_LOC location)
{
	MEM_LOC* ulocation = process->m_usedListRoot;
	
	unsigned long iter = 0;

	for (iter = 0; iter < process->m_usedListLocation; iter += sizeof(MEM_LOC))
	{
		if (*ulocation == location)
		{
			//Fouuund it
			memcpy(ulocation, ulocation + 1, process->m_usedListLocation - iter);

			process->m_usedListLocation -= sizeof(MEM_LOC);

			return;
		}

		ulocation++;
	}
}
