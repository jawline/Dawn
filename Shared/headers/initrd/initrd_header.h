#ifndef _INITIAL_RAMDISK_HEADER_
#define _INITIAL_RAMDISK_HEADER_

#define RAMMAGIC 0x453ABCDF

//The header at the start of the RAM disk
struct initial_ramdisk_header {
	uint32_t ramdisk_magic;
	uint8_t ramdisk_checksum[16];
	uint32_t ramdisk_size; //Size in bytes of the RD
} __attribute__((packed));

#endif
