#ifndef _HARD_DRIVE_FILESYSTEM_H_
#define _HARD_DRIVE_FILESYSTEM_H_
#include "rfs.h"

/* The purpose of this file is to provide a abstracted (file based) method for accessing the hard drives */

fs_node_t* init_hard_drive_fs(fs_node_t* dev_fs);

#endif //_HARD_DRIVE_FILESYSTEM_H_
