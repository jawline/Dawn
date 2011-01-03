#ifndef _PROCESS_DEF_H_
#define _PROCESS_DEF_H_

#include <mm/pagedir.h>
#include <common.h>
#include <process/postbox.h>
#include <terminal/terminal.h>
#include <fs/vfs.h>

#define INPUT_BIT 0x1 //First bit = Do I want to hear about input?!?

/**
 * @brief The structure of a process within Dawn
 */
struct process_structure
{
	/**
	 * @brief The process ID or pID unique to this process (At this run)
	 */
	unsigned int m_ID;

	/**
	 * @brief 64 byte character array to store the processes name
	 */
	char m_Name[64];

	uint32 esp /* Stack pointer */ , ebp /* Base Pointer */ , eip /* Instruction Pointer */; /* The rest is stored by the interrupt  that triggers the switch */

	/**
	 * @brief The number of ticks spent processing this process
	 */

	unsigned long m_processingTime; //How much time have we spent processing this process?

	/**
	 * @brief The flags set for what information this process wants to recieve in its postbox
	 */
	
	unsigned int m_postboxFlags; //What does this process want to recieve? Do I want to hear about input events etc?

	/**
	 * @brief The process postbox for this application
	 */
	process_postbox m_processPostbox;

	/**
	 * @brief The physical address of the page directory this process uses
	 */
	page_directory_t* m_pageDir;

	/**
	 * @brief The node directory in which this application is currently executing
	 */

	fs_node_t* m_executionDirectory;

	/**
	 * @brief A list of all the frames of memory mapped by this application
	 */
	MEM_LOC* m_usedListRoot; //Root location of the used list
	unsigned long m_usedListSize;
	unsigned long m_usedListLocation; //Location of the end of the current list irrespect to the root

	unsigned char m_shouldDestroy;

	/**
	 * @brief The terminal that gets written to when this application prints
	 */
	terminal_t* m_pTerminal;

	/**
	 * @brief The return value of this process is stored here when it is set
	 */
	int m_returnValue;
};

typedef struct process_structure process_t;

void switch_process(process_t* from, process_t* proc);
void set_process_input_buffer(process_t* process,char* m_Data, unsigned int len);
void setProcessName(process_t* proc, const char* Name);
void renameCurrentProcess(const char* Str);
void setProcessExecutionDirectory(process_t* proc, fs_node_t* node);

process_message postbox_peek(process_postbox* pb);
process_message postbox_top(process_postbox* pb);
void postbox_add(process_postbox* pb, process_message msg);
int createNewProcess(const char* Filename, fs_node_t* Where);
process_t* initializeKernelProcess();

extern MEM_LOC read_eip();

#endif //_PROCESS_DEF_H_
