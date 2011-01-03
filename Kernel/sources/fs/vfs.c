#include <stdlib.h>
#include <heap/heap.h>
#include <fs/vfs.h>
#include "rfs.h"
#include <debug/debug.h>
#include <common.h>
#include "rfs.h"

/*
   The intent of this file is to create a set of functions providing the kernel with easy manipulation of the virtual file system.
   the file also contains a pointer to the root node of root_fs for easy access

   STATUS: Working, no immediate revision needed
*/

fs_node_t* root_fs = 0;

void init_root_fs() 
{
	root_fs = createRfsDirectory("root", 0);
}

fs_node_t* init_vfs() 
{
	if (root_fs != 0) return root_fs;

	init_root_fs();

	return root_fs;
}

int is_directory(fs_node_t* node) 
{

	if ((node->flags & FS_DIR) == FS_DIR) 
	{
		return 1;	
	}

	return 0;
}

unsigned long read_fs(fs_node_t* node, unsigned long offset, unsigned long size, uint8* buffer)
{

  // Has the node got a read callback?
  if (node->read != 0)
  {
    return node->read(node, offset, size, buffer);
  }
  else 
  {
    return 0;
  }

}

unsigned long write_fs(fs_node_t* node, unsigned long offset, unsigned long size, uint8* buffer)
{

  // Has the node got a read callback?
  if (node->write != 0)
  {
    return node->write(node, offset, size, buffer);
  }
  
  else
  {
    return 0;
  }

}

void open_fs(fs_node_t* node) {
	
	if (node->open != 0)
        {
		DEBUG_PRINT("Opening file %s\n", node->name);
		node->open(node);
	}

	return;
}

void close_fs(fs_node_t* node) {

	if (node->close != 0)
 	{
		DEBUG_PRINT("Closing file %s\n", node->name);
		node->close(node);
	}

	return;
}

struct dirent* readdir_fs (fs_node_t* node, uint32 idx) {

	if (node->parent == 0)
	{
		if (idx == 0)
		{
			struct dirent* ret = (struct dirent*) malloc(sizeof(struct dirent));
			memset(ret, 0, sizeof(struct dirent));
			strcpy(ret->name, ".");
			return ret;
		}
		else
		{
			idx = idx - 1;
		}
	}
	else
	{
		if (idx == 0)
		{
			struct dirent* ret = (struct dirent*) malloc(sizeof(struct dirent));
			memset(ret, 0, sizeof(struct dirent));
			strcpy(ret->name, ".");
			return ret;
		}
		else if (idx == 1)
		{
			struct dirent* ret = (struct dirent*) malloc(sizeof(struct dirent));
			memset(ret, 0, sizeof(struct dirent));
			strcpy(ret->name, "..");
			return ret;
		}
		else
		{
			idx = idx - 2;
		}
	}

	if (node->readdir != 0)
	{
		return node->readdir(node, idx);
	}

	return 0;
}

fs_node_t* finddir_fs (fs_node_t* node, const char* name) {

	if (strcmp(name, "..") == 0)
	{
		return node->parent;
	} else if (strcmp(name, ".") == 0)
	{
		return node;
	}

	if (node->finddir != 0) 
	{
		return (fs_node_t*) node->finddir(node, name);
	}

	return 0;
}

/**
 * @brief Binds the specified target node to the node supplied
 */
void bindnode_fs(fs_node_t* node, fs_node_t* target) {

	if (node->bindnode != 0)
	{
		node->bindnode(node, target);
	}

}

/**
 * @brief Unbinds the specified target node from the node supplied
 */

void unbindnode_fs(fs_node_t* node, fs_node_t* target) 
{

	if (node->unbindnode != 0)
	{
		node->unbindnode(node, target);
	}

}

/**
 @brief Recursive find node given string
 @bug Not finished!
 */
fs_node_t* evaluatePath(const char* path, fs_node_t* current_node)
{
	if (path == 0) return 0;

	if (path[0] == '/')
	{
		return evaluatePath(path + 1, init_vfs());
	}

	char* Buffer = strchr(path, '/');

	if (Buffer == 0)
	{
		return finddir_fs(current_node, path);
	}
	else
	{
		char* split_left = malloc(Buffer - path + 1);
		memcpy((void*) split_left, (void*) path, Buffer - path);
		split_left[Buffer - path] = '\0';

		fs_node_t* temp = finddir_fs(current_node, split_left);
		free(split_left);

		if (temp != 0)
		{
			return evaluatePath(Buffer + 1, temp);
		}

		return 0;
	}
}
