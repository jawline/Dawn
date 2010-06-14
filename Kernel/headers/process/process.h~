#ifndef _PROCESS_DEF_H_
#define _PROCESS_DEF_H_
#include <mm/virt_mm.h>
#include <common.h>

struct process_structure
{
	unsigned int m_ID;
	char m_Name[64];

	uint32 esp /* Stack pointer */ , ebp /* Base Pointer */ , eip /* Instruction Pointer */;
	page_directory_t* m_pageDir;
};

typedef struct process_structure process_t;

void switch_process(process_t* from, process_t* proc);

#endif //_PROCESS_DEF_H_
