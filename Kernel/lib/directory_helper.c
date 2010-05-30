#include <fs/dir.h>
#include <stdlib.h>

void print_directory(fs_node_t* node, int recur) {

    printf("Listing directory %s", node->name);
    printf(" Length %i\n", node->length);

    struct dirent* ent = 0;

    uint32 iter = 0; 
    while (1) {

	ent = readdir_fs(node, iter);

	if (ent == 0) {
		break;	
	}


	fs_node_t* nent = finddir_fs(node, ent->name);

	free(ent);

	if (!is_directory(nent)) {

		printf("File %s Length %i\n", nent->name, nent->length);

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
