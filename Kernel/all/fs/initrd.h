#ifndef _INITIAL_RAMDISK_FILESYSTEM_H_
#define _INITIAL_RAMDISK_FILESYSTEM_H_
#include <fs/vfs.h>

/**
 * Initialise the initial ramdisk, adding it to the filesystem
 */
fs_node_t* initialiseRamdisk(uint8_t* ramdiskLocation, char const* name, fs_node_t* parent);

#endif //_INITIAL_RAMDISK_FILESYSTEM_H_
