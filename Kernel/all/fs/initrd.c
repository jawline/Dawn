#include "initrd.h"
#include <types/stdint.h>
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

static uint32_t* start_list = 0;
static fs_node_t* file_list = 0;
static uint32_t num_files = 0;
fs_node_t* initrd_root_node = 0;

struct dirent ird_root_readdir (fs_node_t* node, uint32_t idx) {
	if (num_files > idx) {
		struct dirent ret;
		memset(&ret, 0, sizeof(struct dirent));
		strcpy(ret.name, file_list[idx].name);
		return ret;
	}

	struct dirent ret;
	memset(&ret, 0, sizeof(struct dirent));
	strcpy(ret.name, "");
	return ret;
}

fs_node_t* ird_root_finddir (fs_node_t* node, char* name) {
	for (unsigned i = 0; i < num_files; i++) {
		if (strcmp(file_list[i].name, name) == 0) {
			return &file_list[i];
		}
	}
	return 0;
}

uint32_t read_ird(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t* buffer) {
	
	if (offset >= node->length) {
		return 0;
	}

	if (offset + size > node->length) {
		size = offset + size - node->length - 1;
	}

	uint8_t* loc = (uint8_t*) start_list[node->inode];
	memcpy(buffer, (void*) (((uint32_t)loc + (uint32_t)offset)), size);
	
	return size;
}

/**
 * Initialize the initial ramdisk and bind it to a node on the virtual filesystem.
 * This ramdisk based filesystem can be read from but not written to
 */
fs_node_t* initialiseRamdisk(uint8_t* ramdiskLocation, char const* name, fs_node_t* parent) {
	
	if (initrd_root_node) {
		return initrd_root_node;
	}

	//The front of the ramdisk contains a header with all the file descriptors
	struct initial_ramdisk_header* header = (struct initial_ramdisk_header*) ramdiskLocation;

	if (header->ramdisk_magic != RAMMAGIC) {
		printf("0x%x should be 0x%x\n", header->ramdisk_magic, RAMMAGIC);
		PANIC("Error, RAMDISK magic incorrect");
	}

	//Create the initrd node.
	initrd_root_node = malloc(sizeof(fs_node_t));

	//Find the file chunk header
	num_files = *((uint32_t*) ((uint32_t) ((uint32_t) ramdiskLocation) + sizeof(struct initial_ramdisk_header)));

	start_list = malloc(sizeof(uint32_t) * num_files);
	file_list = malloc(sizeof(fs_node_t) * num_files);

	struct initrd_fent* fe_ptr = (struct initrd_fent*) (ramdiskLocation + sizeof(struct initial_ramdisk_header) + sizeof(uint32_t));
	
	//Add all the files to a file list.
	for (unsigned int iter = 0; iter < num_files; iter++) {
		start_list[iter] = (uint8_t*)(ramdiskLocation + fe_ptr->start);
		memset(&file_list[iter], 0, sizeof(fs_node_t));
		strcpy(file_list[iter].name, fe_ptr->name);
		file_list[iter].inode = iter;
		file_list[iter].length = fe_ptr->size;
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
