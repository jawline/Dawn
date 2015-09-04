#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include <types/stdint.h>
#include <initrd/initrd_fent.h>
#include <initrd/initrd_header.h>

#include "md5use.h"

#define VER_MAJOR 0
#define VER_MINOR 2
#define VER_REV 8

typedef struct {
	struct initrd_fent st; //File header structure
	uint32_t loc; //Header location
	uint32_t mloc; //Memory location
	uint32_t mlen; //Memory length
} file_helper;

struct file_list_entry {
	char* name;
	struct file_list_entry* next;
};

file_helper* helper_list;

struct file_list_entry file_list;

unsigned int num_files;

char is_directory(char const* path) {
	struct stat r;
	if (stat(path, &r)) {
		return S_ISDIR(r.st_mode);
	} else {
		return 0;
	}
}

char is_not_local(char const* name) {
	if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
		return 0;
	}
	return 1;
}

void process_fiter(struct file_list_entry* fiter, unsigned int i, FILE* fout) {
	struct initrd_fent ent;

	//Strip the name from the entity
	char* lastString = strrchr(fiter->name, '/');
	if (!lastString) {
		printf("lastString null\n");
		lastString = strchr(fiter->name, '\\');
		if (!lastString) {
			printf("lastString still null\n");
			lastString = fiter->name;
		} else {
			lastString = lastString + 1;
		}
	} else {
		lastString = lastString + 1;
	}

	strcpy(ent.name, lastString);

	helper_list[i].st = ent;
	helper_list[i].loc = ftell(fout);

	if (fwrite(&ent, sizeof(struct initrd_fent), 1, fout) != 1) {
		printf("Unable to write a file chunk\n");
		exit(-1);
	}
}

size_t file_length(FILE* fin, size_t returnTo) {
	fseek(fin, 0, SEEK_END);
	size_t len = ftell(fin);
	fseek(fin, returnTo, SEEK_SET);
	return len;
}

void write_fiter(struct file_list_entry* fiter, unsigned int i, FILE* fout) {
	
	helper_list[i].mloc = ftell(fout);
	FILE* fin = fopen(fiter->name, "rb");
	
	if (!fin) {
		printf("Unable to open helper file %s\nAbort\n", fiter->name);
		exit(-1);
	}

	helper_list[i].mlen = file_length(fin, 0);
	
	printf("File %s File Size (Bytes): %i\n", helper_list[i].st.name, helper_list[i].mlen);

	char mem[2048];
	size_t read;
		
	while ((read = fread(mem, 1, 2048, fin)) > 0) {
		if (!fwrite(mem, read, 1, fout)) {
			printf("Could not write block\n");
			exit(-3);
		}
		printf("Wrote %i bytes\n", read);
	}
	
	fclose(fin);
}

void construct_file_list(char const* target_dir) {
	DIR           *d;
	struct dirent *dir;
	d = opendir(target_dir);

	if (!d) {
		printf("Could not open %s for reading\n", target_dir);
		return -1;
	}
	
	while ((dir = readdir(d)) != NULL) {
		
		char* tempName = malloc(strlen(target_dir) + strlen(dir->d_name) + 1);
		strcpy(tempName, target_dir);
		strcpy(tempName + strlen(target_dir), dir->d_name);

		if (is_not_local(dir->d_name) && !is_directory(tempName)) {
			struct file_list_entry* iter = &file_list;
			while (1) {
				if (iter->name == 0) {
					iter->name = tempName;
					num_files++;
					break;
				}
				else {
					if (iter->next == 0) {
						iter->next = (struct file_list_entry*) malloc(sizeof(struct file_list_entry));
						iter->next->name = 0;
						iter->next->next = 0;
					}
					iter = iter->next;
				}
			}
		} else {
			free(tempName);
		}
	}
}

void calculate_checksum(struct initial_ramdisk_header* head, char const* path) {

	FILE* fout = fopen(path, "rb");

	fseek(fout, sizeof(struct initial_ramdisk_header), SEEK_SET);

	void* data = malloc(head->ramdisk_size - sizeof(struct initial_ramdisk_header));
	
	size_t read;
	if (!(read = fread(data, head->ramdisk_size - sizeof(struct initial_ramdisk_header), 1, fout))) {
		printf("Error computing ramdisk hash\n");
		printf("RDBYTE %i\n", read);
		return -1;
	}

	unsigned char digest[16];
	MDData(data, head->ramdisk_size - sizeof(struct initial_ramdisk_header), digest);

	printf("CHECKSUM ");
	for (unsigned int iter = 0; iter < 16; iter++) {
		printf("%x", digest[iter]);
		head->ramdisk_checksum[iter] = digest[iter];
	}
	printf("\n");
	
	fclose(fout);
}

void rewrite_header(struct initial_ramdisk_header* head, char const* path) {
	FILE* fout = fopen(path, "r+b");
	fseek(fout, 0, SEEK_SET);
	
	if (!fout || fwrite(head, sizeof(struct initial_ramdisk_header), 1, fout) != 1) {
		printf("Unable to rewrite the RAMDisk header\n");
		exit(-5);
	}

	fclose(fout);
}

int main(int argc, char **argv) {
	file_list.name = 0;
	file_list.next = 0;

	num_files = 0;

	printf("Compiler Version %i.%i.%i\n", VER_MAJOR, VER_MINOR, VER_REV);

	if (argc != 3) {
		printf("Usage: RamdiskCompiler Directory OutputFile ...\n");
		return -1;
	}

	construct_file_list(argv[1]);

	helper_list = malloc(sizeof(file_helper) * num_files);

	FILE * fout = 0;
	fout = fopen(argv[2], "wb");

	if (!fout) {
		printf("Unable to open %s (Cannot write ramdisk)\n");
		return -1;
	}

	struct initial_ramdisk_header head;
	memset(&head, 0, sizeof(struct initial_ramdisk_header));

	head.ramdisk_checksum[0] = 0;
	head.ramdisk_checksum[1] = 0;
	head.ramdisk_checksum[2] = 0;
	head.ramdisk_checksum[3] = 0;

	head.ramdisk_magic = RAMMAGIC;
	head.ramdisk_size = sizeof(struct initial_ramdisk_header);

	if (fwrite(&head, sizeof(struct initial_ramdisk_header), 1, fout) != 1) {
		printf("Unable to write the initial RAM disk header\n");
		return -1;
	}

	uint32_t nmfiles = num_files;

	if (fwrite(&nmfiles, sizeof(uint32_t), 1, fout) != 1) {
		printf("Unable to write File chunk header\n");
		return -1;
	}

	struct file_list_entry* fiter = &file_list;

	for (unsigned int i = 0; i < num_files; i++) {
		process_fiter(fiter, i, fout);
		fiter = fiter->next;
	}

	fiter = &file_list;

	for (unsigned int i = 0; i < num_files; i++) {
		write_fiter(fiter, i, fout);
		fiter = fiter->next;
	}

	head.ramdisk_size = file_length(fout, 0);

	for (unsigned i = 0; i < nmfiles; i++) { //Update the start and size locations
		fseek(fout, helper_list[i].loc, SEEK_SET); //Seek to the start of the header

		helper_list[i].st.start = helper_list[i].mloc;
		helper_list[i].st.size = helper_list[i].mlen;

		printf("File %s starts %i length %i\n", helper_list[i].st.name, helper_list[i].st.start, helper_list[i].st.size);

		if (fwrite(&helper_list[i].st, sizeof(struct initrd_fent), 1, fout) != 1) {
			printf("Unable to write a file chunk\n");
			return 0;
		}
	}

	fclose(fout);

	calculate_checksum(&head, argv[2]);
	rewrite_header(&head, argv[2]);

	return 0;
}
