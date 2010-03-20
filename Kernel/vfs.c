#include <vfs.h>
#include <stdlib.h>
#include "heap.h"

fs_node_t * root_fs = 0;

fs_node_t ** dir_entrys = 0;
uint32 nroot_entrys = 0;

struct dirent * root_readdir (fs_node_t * node, uint32 idx) {

	if (nroot_entrys > idx) {
		struct dirent * ret = malloc(sizeof(struct dirent));
		strcpy(ret->name, dir_entrys[idx]->name);
		return ret;
	}

	return 0;
}

fs_node_t * root_finddir (fs_node_t * node, char * name) {
	int iter = 0;

	for (iter = 0; iter < nroot_entrys; iter++) {
		if (strcmp(dir_entrys[iter]->name, name) == 0) return dir_entrys[iter];
	}

	return 0;
}

void root_bind_node(fs_node_t * node) {
	nroot_entrys++;
	
	if (dir_entrys == 0) //First bind
	{
		dir_entrys = malloc(sizeof(fs_node_t *) * nroot_entrys);
		dir_entrys[nroot_entrys - 1] = node;
	} else {
		fs_node_t ** dir_entrys_new = malloc(sizeof(fs_node_t *) * nroot_entrys);
		memcpy(dir_entrys_new, dir_entrys, sizeof(fs_node_t *) * (nroot_entrys - 1));
		dir_entrys_new[nroot_entrys - 1] = node;

		free(dir_entrys);
		dir_entrys = dir_entrys_new;
	}
}

void root_unbind_node(fs_node_t * node) {
	uint32 nid = 0;
	fs_node_t * nptr = 0;

	for (nid = 0; nid < nroot_entrys; nid++) {
		nptr = dir_entrys[nid];
		if (nptr == node) break;
		nptr = 0;	
	}

	if (nptr == 0) return; //No nptr, node isn't bound to the root.

	fs_node_t ** new_node_dir = malloc(sizeof(fs_node_t *) * nroot_entrys - 1);
	memcpy(new_node_dir, dir_entrys, sizeof(fs_node_t *) * nid); //Copy up to the nid

	if ((nid + 1) != nroot_entrys) {
		memcpy(new_node_dir + nid, dir_entrys + (nid + 1), sizeof(fs_node_t *) * nroot_entrys - (nid + 1));
		free(dir_entrys);
		dir_entrys = (fs_node_t **) new_node_dir;
		nroot_entrys--;
	} else {
		free(dir_entrys);
		dir_entrys = new_node_dir;
		nroot_entrys--;	
	}
	
}

void init_root_fs() {
	root_fs = malloc(sizeof(fs_node_t)); //Initialize the root node.
	memset(root_fs, 0, sizeof(fs_node_t));
	root_fs->flags = FS_DIR;
	root_fs->readdir = root_readdir;
	root_fs->finddir = root_finddir;	
}

fs_node_t * init_vfs() {
	if (root_fs != 0) return root_fs;

	init_root_fs();

	return root_fs;
}

int is_directory(fs_node_t * node) {

	if ((node->flags & FS_DIR) == FS_DIR) {
		return 1;	
	}

	return 0;
}

uint32 read_fs(fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer)
{
  // Has the node got a read callback?
  if (node->read != 0)
    return node->read(node, offset, size, buffer);
  else {
    return 0;
  }
}

uint32 write_fs(fs_node_t *node, uint32 offset, uint32 size, uint8 *buffer)
{
  // Has the node got a read callback?
  if (node->write != 0)
    return node->write(node, offset, size, buffer);
  else
    return 0;
}

void open_fs(fs_node_t * node) {
	if (node->open != 0)
		node->open(node);

	return;
}

void close_fs(fs_node_t * node) {
	if (node->close != 0)
		node->close(node);

	return;
}

struct dirent * readdir_fs (fs_node_t * node, uint32 idx) {
	if (node->readdir != 0)
		return node->readdir(node, idx);
	return 0;
}

fs_node_t * finddir_fs (fs_node_t * node, char * name) {
	if (node->finddir != 0) 
		return node->finddir(node, name);
	return 0;
}
