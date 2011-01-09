#ifndef _INITIAL_RAMDISK_FILESYSTEM_H_
#define _INITIAL_RAMDISK_FILESYSTEM_H_
#include <fs/vfs.h>

fs_node_t* initialize_initrd(uint32_t start, char * name, fs_node_t * parent);

#endif //_INITIAL_RAMDISK_FILESYSTEM_H_
