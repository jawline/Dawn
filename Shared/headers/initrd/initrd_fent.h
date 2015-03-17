#ifndef _INITIAL_RAMDISK_FILE_ENTRY_
#define _INITIAL_RAMDISK_FILE_ENTRY_

/**
 * Ramdisk file descriptor
 * Starts with the name of the name of the file, then its
 * start location in the ramdisk and its size in bytes
 */
struct initrd_fent {
	char name[128];
	uint32_t start;
	uint32_t size;
} __attribute__((packed));

#endif
