#ifndef _INITIAL_RAMDISK_FILESYSTEM_H_
#define _INITIAL_RAMDISK_FILESYSTEM_H_
#include <vfs.h>

fs_node_t * initialize_initrd(uint32 start, char * name, fs_node_t * parent);

#endif