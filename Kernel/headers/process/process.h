#ifndef _PROCESS_DEF_H_
#define _PROCESS_DEF_H_
#include <mm/virt_mm.h>
#include <common.h>
#include <process/postbox.h>

struct process_structure
{
	unsigned int m_ID;
	char m_Name[64];

	uint32 esp /* Stack pointer */ , ebp /* Base Pointer */ , eip /* Instruction Pointer */; /* The rest is stored by the interrupt  that triggers the switch */

	process_postbox m_processPostbox;
	page_directory_t* m_pageDir;
};

typedef struct process_structure process_t;

void switch_process(process_t* from, process_t* proc);
void set_process_input_buffer(process_t* process,char* m_Data, unsigned int len);

process_message postbox_peek(process_postbox* pb);
process_message postbox_top(process_postbox* pb);
void postbox_add(process_postbox* pb, process_message msg);

#endif //_PROCESS_DEF_H_
