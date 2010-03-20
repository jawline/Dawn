#ifndef _INITIAL_RAMDISK_DIRECTORY_ENTRY_
#define _INITIAL_RAMDISK_DIRECTORY_ENTRY_

//The header of the a directory entry
//Followed by a list of integer ID's of  the file's in this directory
struct initrd_dirent {
	char name[128];
	uint32 dentrys;
	uint32 fentrys;
}  __attribute__((packed));

#endif
