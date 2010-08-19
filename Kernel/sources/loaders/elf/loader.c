#include "loader.h"
#include "parser.h"

#include <mm/virt_mm.h>
#include <types/int_types.h>
#include <types/memory.h>
#include <panic/panic.h>

typedef int (*entry_point) (int argc, void* argv);

int loadAndExecuteElf(fs_node_t* Node)
{
	//Load the executable into RAM
	void* Data = malloc(Node->length);
	read_fs(Node, 0, Node->length, Data);

	//Parse the header
	e32_header head = parseElfHeader(Data, Node->length);

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

	printf("Iterating through %i program headers\n", head.e_phnum);

	//Iterate through every program header
	unsigned int header_iter = 0;
	for (header_iter = 0; header_iter < head.e_phnum; header_iter++)
	{

		e32_pheader program_header = parseElfProgramHeader(Data, Node->length, head, header_iter);

		printf("Loaded program head %i. p_type 0x%x p_offset 0x%x v_addr 0x%x p_addr 0x%x p_filesz 0x%x p_memsz 0x%x p_align 0x%x\n", header_iter, program_header.p_type, program_header.p_offset, program_header.p_vaddr, program_header.p_paddr, program_header.p_filesz, program_header.p_memsz, program_header.p_align);

		if (program_header.p_type == PT_LOAD)
		{
			MEM_LOC v_addr_iter = program_header.p_vaddr;

			for (v_addr_iter = program_header.p_vaddr; v_addr_iter <= program_header.p_vaddr + program_header.p_memsz; v_addr_iter += PAGE_SIZE)
			{
				if (getMapping(v_addr_iter, 0) == 0)
				{
					printf("Page not yet mapped\n");
					map(v_addr_iter, alloc_frame(), PAGE_PRESENT | PAGE_WRITE);
					printf("Page has been mapped\n");
				}
				else
				{
					printf("Page already mapped\n");
				}
			}

			memset(program_header.p_vaddr, 0, program_header.p_memsz);
			memcpy(program_header.p_vaddr, Data + program_header.p_offset, program_header.p_filesz);
			printf("Copied memory data\n");
		}

	}

	//This segment of code correlates to its execution
	int result_from_process = 0;

	entry_point program_entry_ponter = head.e_entry;

	result_from_process = program_entry_ponter(0, 0);

	return result_from_process;
}
