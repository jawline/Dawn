#include "hdd_fs.h"

fs_node_t* hd_fs_root = 0;

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
