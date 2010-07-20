#include "rfs.h"
#include <fs/vfs.h>

fs_node_t* dev_root = 0;

fs_node_t* init_device_fs(fs_node_t* parent)
{

	if (dev_root != 0)
	{
		return dev_root;
	}

	dev_root = create_rfs_directory("Devices", parent);
	bindnode_fs(parent, dev_root);

	//Initialize deviceFS subsystems
	init_hard_drive_fs(dev_root);

	return dev_root;
}
