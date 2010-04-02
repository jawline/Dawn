#ifndef _INITIAL_RAMDISK_HEADER_
#define _INITIAL_RAMDISK_HEADER_

#define RAMMAGIC 0x453ABCDF

//The header at the start of the RAM disk
struct initial_ramdisk_header {
	uint32 ramdisk_magic;
	uint32 ramdisk_size; //Size in bytes of the RD
} __attribute__((packed));

#endif
