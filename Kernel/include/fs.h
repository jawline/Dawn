#ifndef _FILE_SYSTEM_DEF_H_
#define _FILE_SYSTEM_DEF_H_
#include "int_types.h"

typedef struct dirent // One of these is returned by the readdir call, according to POSIX.
{
  char * name; // Filename.
  uint32 ino;     // Inode number. Required by POSIX.
} dirent_t;

struct fs_node;
#define fs_node_t struct fs_node

typedef uint32 (*read_function_t) (fs_node_t * ptr, void * dest, uint32 loc, uint32 num);
typedef uint32 (*write_function_t) (fs_node_t * ptr, void * dest, uint32 loc, uint32 num);

typedef void (*open_function_t) (fs_node_t *);
typedef void (*close_function_t) (fs_node_t *);

typedef dirent_t * (*readdir_function_t)(fs_node_t *, uint32 idx);
typedef fs_node_t * (*finddir_function_t)(fs_node_t *, char * name);

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 //Signifys a active mountpoint

struct fs_node {

	char * entry_name;

	uint32 permissions;
	uint32 flags;
	uint32 size;
	uint32 uid;

	read_function_t read;
	write_function_t write;

	open_function_t open;
	close_function_t close;

	readdir_function_t readdir;
	finddir_function_t finddir;
};



#endif
