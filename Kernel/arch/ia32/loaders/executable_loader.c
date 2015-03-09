#include <loaders/executable_loader.h>
#include <loaders/executable_parser.h>

#include <mm/virtual.h>
#include <mm/phys_mm.h>
#include <types/stdint.h>
#include <types/memory.h>
#include <panic/panic.h>
#include <debug/debug.h>
#include <usermode/usermode.h>
#include <scheduler/scheduler.h>
#include <fs/vfs.h>

typedef int (*entry_point)(int argc, void* argv);

unsigned char mapMemoryUsingHeader(e32_pheader program_header) {

	//If the program header is a loadable object, map it into memory
	if (program_header.p_type == PT_LOAD) {

		MEM_LOC v_addr_start = program_header.p_vaddr;
		MEM_LOC v_addr_end = v_addr_start + program_header.p_memsz;

		MEM_LOC iterator = v_addr_start;

		while (iterator < v_addr_end) {
			map(iterator, allocateFrameForProcess(getCurrentProcess()), 0);
			iterator += PAGE_SIZE;
		}
	} else {
	}

	return 1;
}

unsigned char loadToMemoryUsingHeader(e32_pheader program_header,
		fs_node_t* Node) {
	//If the program header is a loadable object, map it into memory
	if (program_header.p_type == PT_LOAD) {
		if (read_fs(Node, program_header.p_offset, program_header.p_filesz,
				(uint8_t*) program_header.p_vaddr) != program_header.p_filesz) {
			DEBUG_PRINT("Error bad read\n");
			return 0;
		}
	}

	return 1;
}

//NOTE: This function relies on Node being readable (read_fs functioning properly)
int loadAndExecuteProgram(fs_node_t* Node, unsigned char usermode) {
	DEBUG_PRINT("Loading program\n");

	//Read the ELF info
	e32info* fileInfo = malloc(sizeof(e32info));
	memset(fileInfo, 0, sizeof(e32info));

	parseElfFile(fileInfo, Node);

	//Use the header from the parsed file
	e32_header head = fileInfo->m_mainHeader;

	//Is it a executable?
	if (head.e_type != ELF_EXE) {
		return LOAD_ERROR_NOT_EXE;
	}

	//Valid header?
	if (elfHeaderValid(&head) != 1) {
		return LOAD_ERROR_BAD_HEAD;
	}

	//32bit executable?
	if (getElfHeaderClass(&head) != ELF_CLASS_32) {
		return LOAD_ERROR_BAD_PLATFORM;
	}

	//Version correct?
	if (getElfHeaderVersion(&head) != ELF_VERSION_CURRENT) {
		return LOAD_ERROR_BAD_HEAD;
	}

	//ELF_DATA_LSB + ELF_CLASS_32 = IA32 = 8086 compatable
	if (getElfHeaderData(&head) != ELF_DATA_LSB) {
		return LOAD_ERROR_BAD_PLATFORM;
	}

	//This segment of code correlates to the loading of the data into virtual memory

	//Iterate through every program header
	unsigned int header_iter = 0;
	for (header_iter = 0; header_iter < fileInfo->m_numProgramHeaders;
			++header_iter) {

		e32_pheader program_header = fileInfo->m_programHeaders[header_iter];

		if (mapMemoryUsingHeader(program_header) != 1) {
			DEBUG_PRINT("Error mapping program header %i\n", header_iter);
			return LOAD_ERROR_BAD_MAP;
		}

	}

	//Load the object blocks that where just mapped
	for (header_iter = 0; header_iter < fileInfo->m_numProgramHeaders;
			++header_iter) {

		e32_pheader program_header = fileInfo->m_programHeaders[header_iter];
		if (loadToMemoryUsingHeader(program_header, Node) != 1) {
			DEBUG_PRINT("Error unable to load header %i\n", header_iter);
			return LOAD_ERROR_BAD_LOAD;
		}

	}

	//This segment of code correlates to its execution
	entry_point program_entry_ponter = (entry_point) head.e_entry;

	//This free's up the ELF info and then deletes the structure from the heap
	freeElfFileInfo(fileInfo);
	free(fileInfo);

	if (usermode == 1) {
		switchToUserMode();
	}

	program_entry_ponter(0, 0);
	schedulerKillCurrentProcess();
	PANIC("I should never get here");
	return 0;
}
