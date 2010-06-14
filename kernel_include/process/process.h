#ifndef _PROCESS_DEF_H_
#define _PROCESS_DEF_H_
#include <mm/virt_mm.h>
#include <common.h>

struct process_structure
{
	unsigned int m_ID;
	char m_Name[64];

	uint32 esp /* Stack pointer */ , ebp /* Base Pointer */ , eip /* Instruction Pointer */; /* The rest is stored by the interrupt */

	char* m_inputBuffer;
	unsigned int m_inputBufferLength;
	unsigned int m_inputCurPosition;

	page_directory_t* m_pageDir;
};

typedef struct process_structure process_t;

void switch_process(process_t* from, process_t* proc);
void set_process_input_buffer(process_t* process,char* m_Data, unsigned int len);

#endif //_PROCESS_DEF_H_
