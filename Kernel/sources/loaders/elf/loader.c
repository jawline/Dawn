#include "loader.h"
#include "parser.h"

#include <mm/virt_mm.h>
#include <types/int_types.h>
#include <types/memory.h>

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

	unsigned int i = 0;
	uint32 is = 0;

	for (i = 0; i < head.e_phnum; i++)
	{
		e32_pheader pHead = parseElfProgramHeader(Data, Node->length, head, i);

		if (pHead.p_type == PT_LOAD)
		{
			//Check that every byte is mapped (CHEAP)
			for (is = pHead.p_vaddr; is < pHead.p_vaddr + pHead.p_memsz; is += PAGE_SIZE)
			{
				if (get_mapping(is, 0) == 0)
				{
					//Map it
					uint32 newframe = alloc_frame();
					map(is, newframe, PAGE_PRESENT | PAGE_WRITE);
				}
			}

			memcpy(pHead.p_vaddr, Data + pHead.p_offset, pHead.p_filesz);
		}
	}

	entry_point nao = head.e_entry;
	int ret = nao(0, 0);

	return ret; //Return the return value of the executable
}
