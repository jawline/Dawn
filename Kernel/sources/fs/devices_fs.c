#include "rfs.h"
#include <fs/vfs.h>

fs_node_t* dev_root = 0;

void init_dev_fs()
{
	dev_root = create_rfs_directory("Devices", init_vfs());
	bindnode_fs(init_vfs(), dev_root);
}
