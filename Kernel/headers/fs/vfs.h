#ifndef _VIRTUAL_FILE_SYSTEM_DEF_H_
#define _VIRTUAL_FILE_SYSTEM_DEF_H_
#include <types/int_types.h>
#include <devices/hdd/disk_device.h>

#define FS_FILE 0x1
#define FS_DIR 0x2

struct dirent // One of these is returned by the readdir call, according to POSIX.
{
  char name[512]; // Filename.
  uint32 ino;     // Inode number. Required by POSIX.
};

struct filesystem_node;

typedef unsigned long (*io_operation) (struct filesystem_node* node, unsigned long offset, unsigned long size, uint8* ptr);

typedef void (*cmd_operation) (struct filesystem_node* node);

typedef struct dirent (*read_directory_t) (struct filesystem_node* node, uint32 idx);

typedef struct fs_node* (*find_directory_t) (struct filesystem_node* node, const char* name);

typedef struct fs_node_t* (*bind_node_t) (struct filesystem_node* boundto, struct filesystem_node* tobind);

struct filesystem_node {
	char name[512]; //Character array, name
	struct filesystem_node* parent; //Pointer to the parent

	uint32 flags; //32 bit bitmask for flags
	unsigned long length; //long integer, size of the file.
	unsigned long inode;
	disk_device* device; //Pointer to the disk device this file is located on

	io_operation write;
	io_operation read;

	cmd_operation open;
	cmd_operation close;

	read_directory_t readdir;
	find_directory_t finddir;
	
	bind_node_t bindnode;
	bind_node_t unbindnode;
};

typedef struct filesystem_node fs_node_t;

fs_node_t* init_vfs();

int is_directory(fs_node_t* node);

fs_node_t* evaluatePath(const char* path, fs_node_t* current_node);


unsigned long read_fs(fs_node_t* node, unsigned long offset, unsigned long size, uint8* buffer);
unsigned long write_fs(fs_node_t* node, unsigned long offset, unsigned long size, uint8* buffer);
void open_fs(fs_node_t* node);
void close_fs(fs_node_t* node);
struct dirent readdir_fs (fs_node_t* node, uint32 idx);
fs_node_t* finddir_fs (fs_node_t* node, const char* name);
void bindnode_fs(fs_node_t* node, fs_node_t* target);
void unbindnode_fs(fs_node_t* node, fs_node_t* target);

#endif
