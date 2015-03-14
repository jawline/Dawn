#ifndef _PROCESS_ARCH_H_
#define _PROCESS_ARCH_H_
#include <mm/pagedir.h>
#include <common.h>
#include <process/postbox.h>
#include <terminal/terminal.h>
#include <heap/heap.h>
#include <fs/vfs.h>

/**
 * The process structure is an architecture specific structure which stores
 * information about a process.
 *
 * It must have a set of common properties across all architectures as well
 * as whatever other properties the architecture requires to store the process
 */
typedef struct processStructure {

	/**
	 * The process ID or pID unique to this process (At this run)
	 */
	unsigned int id;

	/**
	 * 64 byte character array to store the processes name
	 */
	char name[64];

	MEM_LOC esp /* Stack pointer */, ebp /* Base Pointer */,
			eip /* Instruction Pointer */; /* The rest is stored by the interrupt that triggers the switch */

	/**
	 * The number of ticks spent processing this process
	 */
	unsigned long processingTime;

	/**
	 * The flags set for what information this process wants to recieve in its postbox
	 */
	unsigned int postboxFlags;

	/**
	 * The process postbox for this application
	 */
	process_postbox processPostbox;

	/**
	 * The physical address of the page directory this process uses
	 */
	page_directory_t* pageDir;

	/**
	 * The node directory in which this application is currently executing
	 */
	fs_node_t* executionDirectory;

	/**
	 * A list of all the frames of memory mapped by this application
	 */
	MEM_LOC* usedListRoot; //Root location of the used list
	unsigned long usedListSize;
	unsigned long usedListLocation; //Location of the end of the current list irrespect to the root

	unsigned char shouldDestroy;

	/**
	 * The terminal that gets written to when this application prints
	 */
	terminal_t* processTerminal;

	/**
	 * The return value of this process is stored here when it is set
	 */
	int returnValue;
} process_t;

void switchProcess(process_t* from, process_t* proc);
void setProcessInputBuffer(process_t* process, char* data, unsigned int len);
int createNewProcess(const char* filename, fs_node_t* originFilesystemNode);
int kfork();
process_t* initializeKernelProcess();

#endif //_PROCESS_ARCH_H_
