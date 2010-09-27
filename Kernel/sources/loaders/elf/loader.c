#include "loader.h"
#include "parser.h"

#include <mm/virt_mm.h>
#include <types/int_types.h>
#include <types/memory.h>
#include <panic/panic.h>

typedef int (*entry_point) (int argc, void* argv);

unsigned char mapMemoryUsingHeader(e32_pheader program_header)
{
	printf("Loaded program header. p_type 0x%x p_offset 0x%x v_addr 0x%x p_addr 0x%x p_filesz 0x%x p_memsz 0x%x p_align 0x%x\n", program_header.p_type, program_header.p_offset, program_header.p_vaddr, program_header.p_paddr, program_header.p_filesz, program_header.p_memsz, program_header.p_align);

	//If the program header is a loadable object, map it into memory
	if (program_header.p_type == PT_LOAD)
	{
		MEM_LOC v_addr_start = program_header.p_vaddr;
		MEM_LOC v_addr_end = v_addr_start + program_header.p_memsz;

		printf("Mapping memory between 0x%x and 0x%x\n", v_addr_start, v_addr_end);

		MEM_LOC iterator = v_addr_start;
			
		while (iterator < v_addr_end)
		{
			map(iterator, allocateFrame(), PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
			printf("Mapped from 0x%x to 0x%x\n", iterator, iterator + PAGE_SIZE);
			iterator += PAGE_SIZE;
		}

		printf("Done\n");
	} else
	{
		printf("Not to be mapped\n");
	}

	return 1;
}

unsigned char loadToMemoryUsingHeader(e32_pheader program_header, fs_node_t* Node)
{
	//If the program header is a loadable object, map it into memory
	if (program_header.p_type == PT_LOAD)
	{
		if (read_fs(Node, program_header.p_offset, program_header.p_filesz, program_header.p_vaddr) != program_header.p_filesz)
		{
			printf("Error bad read\n");
			return 0;
		}
	}

	return 1;
}

//NOTE: This function relies on Node being readable (read_fs functioning properly)
int loadAndExecuteElf(fs_node_t* Node, unsigned char usermode)
{
	//Read the ELF info
	e32info* fileInfo = malloc(sizeof(e32info));
	memset(fileInfo, 0, sizeof(e32info));

	parseElfFile(fileInfo, Node);

	//Use the header from the parsed file
	e32_header head = fileInfo->m_mainHeader;

	//Is it a executable?
	if (head.e_type != ELF_EXE)
	{
		return LOAD_ERROR_NOT_EXE;
	}

	//Valid header?
	if (elfHeaderValid(head) != 1)
	{
		return LOAD_ERROR_BAD_HEAD;
	}

	//32bit executable?
	if (getElfHeaderClass(head) != ELF_CLASS_32)
	{
		return LOAD_ERROR_BAD_PLATFORM;
	}

	//Version correct?
	if (getElfHeaderVersion(head) != ELF_VERSION_CURRENT)
	{
		return LOAD_ERROR_BAD_HEAD;
	}

	//ELF_DATA_LSB + ELF_CLASS_32 = IA32 = 8086 compatable
	if (getElfHeaderData(head) != ELF_DATA_LSB)
	{
		return LOAD_ERROR_BAD_PLATFORM;
	}

	//This segment of code correlates to the loading of the data into virtual memory

	printf("Iterating through %i program headers\n", fileInfo->m_numProgramHeaders);

	//Iterate through every program header
	unsigned int header_iter = 0;
	for (header_iter = 0; header_iter < fileInfo->m_numProgramHeaders; header_iter++)
	{

		e32_pheader program_header = fileInfo->m_programHeaders[header_iter];
		
		if (mapMemoryUsingHeader(program_header) != 1)
		{
			printf("Error mapping program header %i\n", header_iter);
			return LOAD_ERROR_BAD_MAP;
		}

	}

	//Load the object blocks that where just mapped
	for (header_iter = 0; header_iter < fileInfo->m_numProgramHeaders; header_iter++)
	{

		e32_pheader program_header = fileInfo->m_programHeaders[header_iter];
		if (loadToMemoryUsingHeader(program_header, Node) != 1)
		{
			printf("Error unable to load header %i\n", header_iter);
			return LOAD_ERROR_BAD_LOAD;
		}

	}

	printf("Running program\n");

	//This segment of code correlates to its execution
	entry_point program_entry_ponter = head.e_entry;

	//This free's up the ELF info and then deletes the structure from the heap
	freeElfFileInfo(fileInfo);
	free(fileInfo);

	if (usermode == 1)
	{
		switchToUserMode();
	}

	program_entry_ponter(0, 0);

	return; //Should never really get here
}
