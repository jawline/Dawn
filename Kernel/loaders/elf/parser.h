#ifndef _ELF_EXECUTABLE_PARSER_DEF_H_
#define _ELF_EXECUTABLE_PARSER_DEF_H_
#include "header.h"
#include <types/memory.h>

e32_header parseElfHeader(void* Data, unsigned int Length);
unsigned char elfHeaderValid(e32_header header);
unsigned int getElfHeaderClass(e32_header header);
unsigned int getElfHeaderVersion(e32_header header);
unsigned int getElfHeaderData(e32_header header);

e32_pheader parseElfProgramHeader(void* Data, unsigned int Length, e32_header Header, unsigned int number);

#endif //_ELF_EXECUTABLE_PARSER_DEF_H_
