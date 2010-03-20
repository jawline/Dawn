#ifndef _INITIAL_RAMDISK_FILE_ENTRY_
#define _INITIAL_RAMDISK_FILE_ENTRY_

struct initrd_fent {
	char name[128];
	uint32 start; //Offset of the data from the start of the RAM disk
	uint32 size; //Size (in bytes) of the file
} __attribute__((packed));

#endif
