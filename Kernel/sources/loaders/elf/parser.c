#include "parser.h"
#include <stdlib.h>

//NOTE: Assumes the fs_node_t* is available to read
e32_header parseElfHeader(fs_node_t* File)
{
	e32_header Ret;
	
	if (File->length < sizeof(e32_header))
	{
		//Out of bounds. null the return and then memset to 0
		memset(&Ret, 0, sizeof(e32_header));
		return Ret;
	}

	if (read_fs(File, 0, sizeof(e32_header), &Ret) != sizeof(e32_header))
	{
		//Error reading, null and exit the function
		memset(&Ret, 0, sizeof(e32_header));
		return Ret;
	}
	
	return Ret;
}

//NOTE: Assumes the fs_node_t* is available to read
e32_pheader parseElfProgramHeader(fs_node_t* File, e32_header Header, unsigned int number)
{
	e32_pheader headerStruct;
	MEM_LOC start_loc = Header.e_phoff + (Header.e_phentsize * number);

	if (File->length < start_loc) {
		//Out of bounds. Null the return and then exit the function
		memset(&headerStruct, 0, sizeof(e32_pheader));
		return headerStruct;
	}

	if (read_fs(File, start_loc, Header.e_phentsize, &headerStruct) != Header.e_phentsize)
	{
		//Error reading. Null the return and then exit the function
		memset(&headerStruct, 0, sizeof(e32_pheader));
	}

	//Return value should equal pointer to the program header structure
	return headerStruct;
}

//NOTE: Assumes fs_node_t* is available to readora
void parseElfFile(e32info* info, fs_node_t* File)
{
	//Parse the main header
	info->m_mainHeader = parseElfHeader(File);

	//Allocate the space for the program headers
	info->m_programHeaders = (e32_pheader*) malloc(sizeof(e32_pheader) * info->m_mainHeader.e_phnum);
	info->m_numProgramHeaders = info->m_mainHeader.e_phnum;

	//Iterate through and load each header
	unsigned int headerIterator;
	for (headerIterator = 0; headerIterator < info->m_numProgramHeaders; headerIterator++)
	{
		info->m_programHeaders[headerIterator] = parseElfProgramHeader(File, info->m_mainHeader, headerIterator);
	}

	//TODO: Section headers and other bits of the ELF

}

void freeElfFileInfo(e32info* info)
{
	if (info->m_programHeaders)
	{
		free(info->m_programHeaders);
	}

	if (info->m_sectionHeaders)
	{
		free(info->m_sectionHeaders);
	}
}

unsigned char elfHeaderValid(e32_header h)
{
	if ((h.elf_ident[ID_MAG0] == ELF_MAG0) && (h.elf_ident[ID_MAG1] == ELF_MAG1) && (h.elf_ident[ID_MAG2] == ELF_MAG2) && (h.elf_ident[ID_MAG3] == ELF_MAG3)) return 1;
	return 0;
}

unsigned int getElfHeaderClass(e32_header h)
{
	return h.elf_ident[ID_CLASS];
}

unsigned int getElfHeaderVersion(e32_header header)
{
	return header.elf_ident[ID_VERSION];
}

unsigned int getElfHeaderData(e32_header header)
{
	return header.elf_ident[ID_DATA];
}
