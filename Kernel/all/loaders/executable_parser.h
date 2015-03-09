#ifndef _ELF_EXECUTABLE_PARSER_DEF_H_
#define _ELF_EXECUTABLE_PARSER_DEF_H_
#include <loaders/elf_header.h>
#include <types/memory.h>
#include <fs/vfs.h>

/**
 * Parse information about a ELF file
 */
void parseElfFile(e32info* info, fs_node_t* file);

/**
 * Free the info about a loaded elf file
 */
void freeElfFileInfo(e32info* info);

/**
 * Parse the base elf header
 */
e32_header parseElfHeader(fs_node_t* file);

/**
 * Parse an elf program header
 */
e32_pheader parseElfProgramHeader(fs_node_t* file, e32_header header, unsigned int number);

/**
 * Parse an ELF string table entry
 */
void parseStringTableEntry(char* output, unsigned int max_length, unsigned int idx, fs_node_t* file, e32info* info);

/**
 * Returns 1 if the e32_header is valid
 */
unsigned char elfHeaderValid(e32_header const& header);

unsigned int getElfHeaderClass(e32_header const& header);
unsigned int getElfHeaderVersion(e32_header const& header);
unsigned int getElfHeaderData(e32_header const& header);

#endif //_ELF_EXECUTABLE_PARSER_DEF_H_
