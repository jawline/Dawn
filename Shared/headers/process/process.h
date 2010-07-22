#ifndef _PROCESS_DEF_H_
#define _PROCESS_DEF_H_

#include <mm/pagedir.h>
#include <common.h>
#include <process/postbox.h>
#include <terminal/terminal.h>

#define INPUT_BIT 0x1 //First bit = Do I want to hear about input?!?

struct process_structure
{
	unsigned int m_ID;
	char m_Name[64];

	uint32 esp /* Stack pointer */ , ebp /* Base Pointer */ , eip /* Instruction Pointer */; /* The rest is stored by the interrupt  that triggers the switch */

	unsigned long m_processingTime; //How much time have we spent processing this process?

	unsigned int m_postboxFlags; //What does this process want to recieve? Do I want to hear about input events etc?
	process_postbox m_processPostbox;

	page_directory_t* m_pageDir;

	MEM_LOC* m_usedListRoot; //Root location of the used list
	unsigned long m_usedListSize;
	unsigned long m_usedListLocation; //Location of the end of the current list irrespect to the root

	unsigned char m_shouldDestroy;

	terminal_t* m_pTerminal;
};

typedef struct process_structure process_t;

void switch_process(process_t* from, process_t* proc);
void set_process_input_buffer(process_t* process,char* m_Data, unsigned int len);

process_message postbox_peek(process_postbox* pb);
process_message postbox_top(process_postbox* pb);
void postbox_add(process_postbox* pb, process_message msg);

#endif //_PROCESS_DEF_H_
