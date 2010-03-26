#include <fs/dir.h>
#include <stdlib.h>

void print_directory(fs_node_t* node, int recur) {

    printf("Listing directory %s\n", node->name);

    struct dirent* ent = 0;

    uint32 iter = 0; 
    while (1) {

	ent = readdir_fs(node, iter);

	if (ent == 0) {
		free(ent);
		break;	
	}


	fs_node_t* nent = finddir_fs(node, ent->name);

	free(ent);

	if (!is_directory(nent)) {

		printf("File %s\n", nent->name);

	} else {

		printf("Directory %s\n", nent->name);
		if (recur) {
			print_directory(nent, 1);
		}
	}

	iter++;
    }

    printf("End of directory %s\n", node->name);
}
