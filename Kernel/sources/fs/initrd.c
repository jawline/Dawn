#include "initrd.h"
#include <types/int_types.h>
#include <initrd/initrd_header.h>
#include <initrd/initrd_fent.h>
#include <stdlib.h>
#include <debug/debug.h>
#include <panic/panic.h>

/* The intent of this file it to provide a interface to bind the initial ramdisk in its structure to the virtual file system of the operating system
   to do this it creates several lists of directories, files and several other arrays of memory to link the two.
   It also contains some functions the OS uses the query the initial ram disk such as ird_finddir, ird_readdir and read_ird
   The initial ramdisk is not writable
*/

static uint32 mem_start = 0;

static uint32* start_list = 0;
static fs_node_t* file_list = 0;
static uint32 num_files = 0;

fs_node_t* initrd_root_node = 0;

struct dirent* ird_root_readdir (fs_node_t* node, uint32 idx) 
{
	if (num_files > idx)
	{
		struct dirent* ret = malloc(sizeof(struct dirent));
		strcpy(ret->name, file_list[idx].name);
		return ret;
	}

	return 0;
}

fs_node_t* ird_root_finddir (fs_node_t* node, char* name) 
{
	unsigned int i = 0;

	for (i = 0; i < num_files; i++)
	{
		if (strcmp(file_list[i].name, name) == 0)
		{
			return &file_list[i];
		}
	}

	return 0;
}

uint32 read_ird(fs_node_t *node, uint32 offset, uint32 size, uint8* buffer)
{
	if (offset + size > node->length) return 0;
	uint8* loc = (uint8*) start_list[node->inode];
	memcpy(buffer, (void*) (((uint32)loc + (uint32)offset)), size); //Copy the mem mems
	return size; //Not much error checking we can do. If a page fault occurs somethings gone wrong =)
}

uint32 write_ird(fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer)
{
	return 0; //Can't write to the RAM disk.
}

/**
 * @brief Initialize the initial ramdisk so that it is visible to Dawn as a file system which can be read from (Not written to)
 * @callgraph
 */
fs_node_t* initialize_initrd(uint32 start, char* name, fs_node_t* parent) 
{

	mem_start = start;
	struct initial_ramdisk_header* header;

	struct initrd_fent* fe_ptr = 0;
	uint32 iter = 0;

	header = (struct initial_ramdisk_header*) start; //Header = the start location

	if (header->ramdisk_magic != RAMMAGIC) {
		printf("0x%x should be 0x%x\n", header->ramdisk_magic, RAMMAGIC);
		PANIC("Error, RAMDISK magic incorrect");	
	}

	//Create the initrd node.
	initrd_root_node = malloc(sizeof(fs_node_t));

	//Find the file chunk header
	uint32* nm_files = (uint32*) ((uint32) ((uint32) start) + sizeof(struct initial_ramdisk_header));

	num_files = *nm_files;

	start_list = malloc(sizeof(uint32) * (*nm_files));
	file_list = malloc(sizeof(fs_node_t) * (*nm_files));

	fe_ptr = (struct initrd_fent*) (((uint32) nm_files) + sizeof(uint32));
	
	//Add all the files to a file list.
	for (iter = 0; iter < (*nm_files); iter++) 
	{

		start_list[iter] = (start + fe_ptr->start) - 1;

		memset(&file_list[iter], 0, sizeof(fs_node_t));
		strcpy(file_list[iter].name, fe_ptr->name);
		file_list[iter].inode = iter;
		file_list[iter].length = fe_ptr->size;
		file_list[iter].write = (io_operation) write_ird;
		file_list[iter].read = (io_operation) read_ird;
		file_list[iter].parent = initrd_root_node;

		fe_ptr++;
	}


	//Initialize the initrd node
	memset(initrd_root_node, 0, sizeof(fs_node_t));
	strcpy(initrd_root_node->name, name);

	initrd_root_node->flags = FS_DIR;
	initrd_root_node->readdir = (read_directory_t) ird_root_readdir;
	initrd_root_node->finddir = (find_directory_t) ird_root_finddir;
	initrd_root_node->length = num_files;

	initrd_root_node->parent = parent;

	return initrd_root_node;
}
