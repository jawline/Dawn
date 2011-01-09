#ifndef _ELF_HEADER_DEF_H_
#define _ELF_HEADER_DEF_H_
#include <types/stdint.h>

#define ELF_NUMBER_IDENT 16

typedef uint32_t	Elf32_Addr;
typedef uint32_t	Elf32_Off;
typedef uint16_t	Elf32_Half;
typedef uint32_t	Elf32_Word;
typedef uint8_t	Elf32_Char;

#define ELF_NONE 0
#define ELF_RELOC 1
#define ELF_EXE 2
#define ELF_SHARE 3
#define ELF_CORE 4
#define ELF_LOOS 0xfe00
#define ELF_HIOS 0xfeff
#define ELF_LOPROC 0xff00
#define ELF_HIPROC 0xffff

#define ID_MAG0 0
#define ID_MAG1 1
#define ID_MAG2 2
#define ID_MAG3 3
#define ID_CLASS 4
#define ID_DATA 5
#define ID_VERSION 6

#define ELF_MAG0 0x7F
#define ELF_MAG1 0x45
#define ELF_MAG2 0x4C
#define ELF_MAG3 0x46

#define ELF_CLASS_64 2
#define ELF_CLASS_32 1
#define ELF_CLASS_NONE 0

#define ELF_VERSION_CURRENT 1

#define ELF_DATA_NONE 0
#define ELF_DATA_LSB 1
#define ELF_DATA_MSB 2

#define PT_LOAD 1

/**
 @brief the structure for a 32Bit elf header
 */

struct elf_32_header {
	uint8_t elf_ident[ELF_NUMBER_IDENT];

	/**
	 * The file type (Relocatable, Executable, Etcetera)
	 */

        Elf32_Half      e_type;

	/**
	 * Target architecture
	 */

        Elf32_Half      e_machine;

	/**
	 * Object file version (Set to 1)
	 */

        Elf32_Word      e_version;

	/**
	 * The entry point of the application (Address)
	 */

        Elf32_Addr      e_entry;

	/**
	 * Program headers offset from start of file
	 */

        Elf32_Off       e_phoff;

	/**
	 * Section headers offset from start of file
	 */

        Elf32_Off       e_shoff;

	/**
	 * Not used
	 */

        Elf32_Word      e_flags;

	/**
	 * The size of the elf header
	 */

        Elf32_Half      e_ehsize;

	/**
	 * The size in bytes of each program header entry
	 */

        Elf32_Half      e_phentsize;

	/**
	 * The number of program headers
	 */

        Elf32_Half      e_phnum;

	/**
	 * The size in bytes of each section headerer entry
	 */
        Elf32_Half      e_shentsize;

	/**
	 * The number of section headers
	 */

        Elf32_Half      e_shnum;

	/**
	 * The section header index which contains the string table
	 */

        Elf32_Half      e_shtrndx;

} __attribute__((packed));

/**
 * @brief The structure for a 32 bit elf section header
 */

struct elf_32_section_header {

	/**
	 * @brief String tables index for the section header name
	 */

	Elf32_Word sh_name;

	/**
	 * @brief Numeric identified for the section header type
	 */

	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} __attribute__((packed));

/**
 * @brief The structure for a 32 bit elf program header
 */

struct elf_32_program_header {
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} __attribute__((packed));

/**
 * @brief Dawn data structure for the parsing of 32 bit elf files
 */

struct elf_32_info {

	/**
	 * @brief The main elf file header
	 */

	struct elf_32_header m_mainHeader;

	/**
	 * @brief A pointer to a array of section headers
	 */

	struct elf_32_section_header* m_sectionHeaders;

	/**
	 * @brief The number of section headers in the array pointed to by m_sectionHeaders
	 */

	int m_numSectionHeaders;

	/**
	 * @brief A pointer to a array of program headers
	 */

	struct elf_32_program_header* m_programHeaders;

	/**
	 * @brief The number of program headers in the array pointed to by m_programHeaders
	 */

	int m_numProgramHeaders;
};


typedef struct elf_32_header e32_header;
typedef struct elf_32_section_header e32_sheader;
typedef struct elf_32_program_header e32_pheader;
typedef struct elf_32_info e32info;

#endif //_ELF_HEADER_DEF_H_
