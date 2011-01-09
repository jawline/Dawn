#include <loaders/executable_parser.h>
#include <stdlib.h>
#include <common.h>

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

	if (read_fs(File, 0, sizeof(e32_header), (uint8_t*) &Ret) != sizeof(e32_header))
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

	if (read_fs(File, start_loc, Header.e_phentsize, (uint8_t*) &headerStruct) != Header.e_phentsize)
	{
		//Error reading. Null the return and then exit the function
		memset(&headerStruct, 0, sizeof(e32_pheader));
	}

	//Return value should equal pointer to the program header structure
	return headerStruct;
}

//The purpose of this function is to find a string table entry from a ELF file
void parseStringTableEntry(char* CharBuffer, unsigned int max_length, unsigned int idx, fs_node_t* File, e32info* info)
{
	//Check that its not out of bounds
	if (info->m_numSectionHeaders < info->m_mainHeader.e_shtrndx)
	{
		return 0;
	}

	//Find the section header which contains the string table
	e32_sheader stringtable_header = info->m_sectionHeaders[info->m_mainHeader.e_shtrndx];

	//The strings location is at the sh_offset of the stringtable + the idx
	MEM_LOC read_loc = stringtable_header.sh_offset + idx;

	//Use this character to store each character as it is read
	char Buffer = ' ';

	//Used to tell where we are
	unsigned int iter = 0;

	//While Buffer isn't a null terminator
	while (Buffer != '\0')
	{
		if (iter > max_length) break;
		//Read a character from the file
		read_fs(File, read_loc, 1, &Buffer);
		//Move that character to the characterbuffer supplied
		CharBuffer[iter] = Buffer;
		//Increment both of the iterators
		iter++; read_loc++;
	}

	return;
}

e32_sheader parseElfSectionHeader(fs_node_t* File, e32_header Header, unsigned int number)
{
	//Create a new section header structure
	e32_sheader headerStruct;

	//Find the start location in the file of the section headers
	MEM_LOC start_loc = Header.e_shoff + (Header.e_shentsize * number);

	//If the start_loc is out of bounds return a blank header
	if (File->length < start_loc) {
		//Out of bounds. Null the return and then exit the function
		memset(&headerStruct, 0, sizeof(e32_sheader));
		return headerStruct;
	}

	//Try and read, if fail then return a blank header
	if (read_fs(File, start_loc, Header.e_shentsize, (uint8_t*) &headerStruct) != Header.e_shentsize)
	{
		//Error reading. Null the return and then exit the function
		memset(&headerStruct, 0, sizeof(e32_sheader));
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

	//Allocate the space for the section headers
	info->m_sectionHeaders = (e32_sheader*) malloc(sizeof(e32_sheader) * info->m_mainHeader.e_shnum);
	info->m_numSectionHeaders = info->m_mainHeader.e_shnum;

	//Iterate through and load each section header
	for (headerIterator = 0; headerIterator < info->m_numSectionHeaders; headerIterator++)
	{
		info->m_sectionHeaders[headerIterator] = parseElfSectionHeader(File, info->m_mainHeader, headerIterator);
	}


	char Buffer[512];

	printf("Printing section headers\n");
	for (headerIterator = 0; headerIterator < info->m_numSectionHeaders; headerIterator++)
	{
		parseStringTableEntry(Buffer, 512, info->m_sectionHeaders[headerIterator].sh_name, File, info);
		printf("Entry: %s\n", Buffer);
	}

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