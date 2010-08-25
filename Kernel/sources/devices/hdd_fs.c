#include "hdd_fs.h"
#include <devices/hdd/disk_device.h>
#include <fs/vfs.h>
#include <stdlib.h>

fs_node_t* hd_fs_root = 0;

unsigned int loaded_hdds = 0;

//Create a blank RFS directory or just return a existing one
fs_node_t* init_hard_drive_fs(fs_node_t* dev_fs)
{
	if (hd_fs_root != 0)
	{
		return hd_fs_root;
	}

	hd_fs_root = create_rfs_directory("Drive", dev_fs);
	bindnode_fs(dev_fs, hd_fs_root);

	return hd_fs_root;
}

void hdd_fs_read(struct filesystem_node* node, uint32* offset, uint32 size, uint8* ptr)
{
}

void hdd_fs_write(struct filesystem_node* node, uint32* offset, uint32 size, uint8* ptr)
{
}

void addToHddFs(disk_device newdev)
{
	if (hd_fs_root == 0) return 0;

	disk_device* device = malloc(sizeof(disk_device));
	memset(device, 0, sizeof(disk_device));
	memcpy(device, &newdev, sizeof(disk_device));

	fs_node_t* new_node = malloc(sizeof(fs_node_t));

	new_node->write = hdd_fs_write;
	new_node->read = hdd_fs_read;

	new_node->device = device;

	char Buf[64];
	itoa(loaded_hdds, &Buf, 10);

	strcpy(new_node->name, "drive");
	strcpy(new_node->name + 5, Buf);

	loaded_hdds++;

	bindnode_fs(hd_fs_root, new_node);
}
