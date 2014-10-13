#ifndef _ELF_EXECUTABLE_PARSER_DEF_H_
#define _ELF_EXECUTABLE_PARSER_DEF_H_
#include <loaders/elf_header.h>
#include <types/memory.h>
#include <fs/vfs.h>


unsigned char elfHeaderValid(e32_header header);
unsigned int getElfHeaderClass(e32_header header);
unsigned int getElfHeaderVersion(e32_header header);
unsigned int getElfHeaderData(e32_header header);

e32_pheader parseElfProgramHeader(fs_node_t* File, e32_header Header, unsigned int number);
e32_header parseElfHeader(fs_node_t* File);
void parseStringTableEntry(char* CharBuffer, unsigned int max_length, unsigned int idx, fs_node_t* File, e32info* info);

void parseElfFile(e32info* info, fs_node_t* File);
void freeElfFileInfo(e32info* info);

#endif //_ELF_EXECUTABLE_PARSER_DEF_H_
