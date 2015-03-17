#ifndef _INITIAL_RAMDISK_HEADER_
#define _INITIAL_RAMDISK_HEADER_

#define RAMMAGIC 0x453ABCDF

/**
 * The initial ramdisk header
 * Starts with magic, ramdisk checksum and ramdisk size in bytes
 */
struct initial_ramdisk_header {
	uint32_t ramdisk_magic;
	uint8_t ramdisk_checksum[16];
	uint32_t ramdisk_size;
} __attribute__((packed));

#endif
