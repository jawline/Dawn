#ifndef _VIRTUAL_FILE_SYSTEM_DEF_H_
#define _VIRTUAL_FILE_SYSTEM_DEF_H_
#include <types/int_types.h>

#define FS_FILE 0x1
#define FS_DIR 0x2

struct dirent // One of these is returned by the readdir call, according to POSIX.
{
  char name[128]; // Filename.
  uint32 ino;     // Inode number. Required by POSIX.
};

struct filesystem_node;

typedef uint32 (*io_operation) (struct filesystem_node * node, uint32 * offset ,uint32 size, uint8 * ptr);
typedef void (*cmd_operation) (struct filesystem_node * node);
typedef struct dirent * (*read_directory_t) (struct filesystem_node *, uint32);
typedef struct fs_node * (*find_directory_t) (struct filesystem_node *, char * name);

typedef struct fs_node_t * (*bind_node_t) (struct filesystem_node * boundto, struct filesystem_node * tobind);

struct filesystem_node {
	char name[128]; //Character array, name

	uint32 flags; //32 bit bitmask for flags
	uint32 length; //32 bit int, size of the file.
	uint32 inode; //Internal node, Unique for each file

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

fs_node_t * init_vfs();

int is_directory(fs_node_t * node);

#endif