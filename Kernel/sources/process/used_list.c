#include <process/process.h>
#include <stdlib.h>

const unsigned long usedListExpansionSize = 1024;

void init_used_list(process_t* process)
{
	process->m_usedListRoot = (MEM_LOC*) malloc(1024);
	process->m_usedListSize = 1024;
	process->m_usedListLocation = 0;
}

void expand_used_list(process_t* process)
{
	MEM_LOC* new_loc = (MEM_LOC*) malloc(process->m_usedListSize + usedListExpansionSize);

	memcpy(new_loc, process->m_usedListRoot, process->m_usedListSize);
	free(process->m_usedListRoot);

	process->m_usedListSize += usedListExpansionSize;
	process->m_usedListRoot = new_loc;
}

void used_list_add(process_t* process, MEM_LOC location)
{
	if (process->m_usedListSize == process->m_usedListLocation)
	{
		//Gotta expand the used list
		expand_used_list(process);
	}

	MEM_LOC* ulocation = process->m_usedListRoot;
	ulocation = (MEM_LOC*) (((MEM_LOC) ulocation) + ((MEM_LOC)process->m_usedListLocation));

	*ulocation = location;
	ulocation--;

	process->m_usedListLocation += sizeof(MEM_LOC);
}

void used_list_remove(process_t* process, MEM_LOC location)
{
	MEM_LOC* ulocation = process->m_usedListRoot;
	
	unsigned long iter = 0;

	for (iter = 0; iter < process->m_usedListSize; iter += sizeof(MEM_LOC))
	{
		if (*ulocation == location)
		{
			//Fouuund it
			memcpy(ulocation, ulocation + 1, process->m_usedListSize - iter);

			process->m_usedListSize -= sizeof(MEM_LOC);

			return;
		}

		ulocation++;
	}
}
