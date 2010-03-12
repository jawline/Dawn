#include <fs.h>

uint32 read_fs(fs_node_t *node, uint32 offset, uint32 size, uint8 * buffer)
{

  if (node->read != 0)
    return node->read(node, offset, size, buffer);
  else
    return 0;

}

uint32 write_fs(fs_node_t *node, uint32 offset, uint32 size, uint8 * buffer)
{

  if (node->write != 0)
    return node->write(node, offset, size, buffer);
  else
    return 0;

}

void open_fs(fs_node_t *node)
{

  if (node->open != 0)
    node->open(node);
  else
    return;

}

void close_fs(fs_node_t *node)
{

  if (node->close != 0)
    node->close(node);
  else
    return;

}

dirent_t * readdir_fs(fs_node_t *node, uint32 idx)
{
  if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0 )
    return node->readdir(node, idx);
  else
    return 0;

}

fs_node_t * finddir_fs(fs_node_t *node, char * name)
{
  if ((node->flags & 0x7) == FS_DIRECTORY && node->finddir != 0 )
    return node->finddir(node, name);
  else
    return 0;

}
