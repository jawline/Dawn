#include <stdlib.h>
#include <heap/heap.h>
#include <fs/vfs.h>
#include "rfs.h"
#include <debug/debug.h>
#include <common.h>

fs_node_t* root_fs = 0;

void initialiseRootFilesytem() {
	root_fs = createRfsDirectory("root", 0);
}

fs_node_t* init_vfs() {
	if (!root_fs) {
		initialiseRootFilesytem();
	}
	return root_fs;
}

char is_directory(fs_node_t* node) {
	return node->flags & FS_DIR;
}

unsigned long read_fs(fs_node_t* node, unsigned long offset, unsigned long size, uint8_t* buffer) {
	return node->read ? node->read(node, offset, size, buffer) : 0;
}

unsigned long write_fs(fs_node_t* node, unsigned long offset, unsigned long size, uint8_t* buffer) {
	return node->write ? node->write(node, offset, size, buffer) : 0;
}

void open_fs(fs_node_t* node) {
	if (node->open) {
		DEBUG_PRINT("Opening file %s\n", node->name);
		node->open(node);
	}
}

void close_fs(fs_node_t* node) {
	if (node->close) {
		DEBUG_PRINT("Closing file %s\n", node->name);
		node->close(node);
	}
}

struct dirent readdir_fs(fs_node_t* node, uint32_t idx) {

	struct dirent ret;
	memset(&ret, 0, sizeof(struct dirent));

	if (!node->parent) {
		if (idx == 0) {
			strcpy(ret.name, ".");
			return ret;
		}
		idx = idx - 1;
	} else {
		if (idx == 0) {
			strcpy(ret.name, ".");
			return ret;
		} else if (idx == 1) {
			strcpy(ret.name, "..");
			return ret;
		}
		idx = idx - 2;
	}

	if (node->readdir) {
		return node->readdir(node, idx);
	}

	strcpy(ret.name, "");
	return ret;
}

fs_node_t* finddir_fs(fs_node_t* node, const char* name) {

	if (strcmp(name, "..") == 0) {
		return node->parent;
	} else if (strcmp(name, ".") == 0) {
		return node;
	}

	if (node->finddir) {
		return (fs_node_t*) node->finddir(node, name);
	}

	return 0;
}

/**
 * @brief Binds the specified target node to the node supplied
 */
void bindnode_fs(fs_node_t* node, fs_node_t* target) {
	if (node->bindnode) {
		node->bindnode(node, target);
	}
}

/**
 * @brief Unbinds the specified target node from the node supplied
 */

void unbindnode_fs(fs_node_t* node, fs_node_t* target) {
	if (node->unbindnode) {
		node->unbindnode(node, target);
	}
}

/**
 * Recursive search for node with string
 */
fs_node_t* evaluatePath(const char* path, fs_node_t* current_node) {

	if (!path) {
		return 0;
	}

	if (path[0] == '/') {
		return evaluatePath(path + 1, init_vfs());
	}

	char* buffer = strchr(path, '/');

	if (buffer == 0) {
		if (strcmp(path, "") == 0) {
			return current_node;
		} else {
			return finddir_fs(current_node, path);
		}
	} else {
		char splitLeftBuffer[buffer - path + 1];
		memcpy((void*) splitLeftBuffer, (void*) path, buffer - path);
		splitLeftBuffer[buffer - path] = '\0';

		fs_node_t* temp = finddir_fs(current_node, splitLeftBuffer);

		if (temp != 0) {
			return evaluatePath(buffer + 1, temp);
		}

		return 0;
	}
}
