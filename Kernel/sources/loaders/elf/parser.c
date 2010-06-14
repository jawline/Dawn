#include "parser.h"
#include <stdlib.h>

e32_header parseElfHeader(void* Data, unsigned int Length)
{
	e32_header Ret;
	
	if (Length < sizeof(e32_header))
	{
		memset(&Ret, 0, sizeof(e32_header));
		return Ret;
	}

	memcpy(&Ret, Data, sizeof(e32_header));

	return Ret;
}

e32_pheader parseElfProgramHeader(void* Data, unsigned int Length, e32_header Header, unsigned int number)
{
	e32_pheader Ret;
	MEM_LOC start_loc = Header.e_phoff + (Header.e_phentsize * number);
	MEM_LOC read_loc = ((MEM_LOC)Data) + start_loc;

	if (Length < start_loc) {
		memset(&Ret, 0, sizeof(e32_pheader));
	}

	memcpy(&Ret, Data + start_loc, Header.e_phentsize);

	return Ret;
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
