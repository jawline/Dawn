#include <stdio.h>
#include <dirent.h>

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
unsigned int i;

int main(int argc, char **argv) {
	file_list.name = 0;
	file_list.next = 0;

	num_files = 0;

	printf("Compiler Version %i.%i.%i\n", VER_MAJOR, VER_MINOR, VER_REV);

	if (argc != 3) {
		printf("Usage: RamdiskCompiler Directory OutputFile ...\n");
		return -1;
	}

	DIR           *d;
	struct dirent *dir;
	d = opendir(argv[1]);

	if (!d) {
		printf("Could not open %s for reading\n", argv[1]);
		return -1;
	}
	
	while ((dir = readdir(d)) != NULL) {
		if (dir->d_type != DT_DIR) {
			struct file_list_entry* iter = &file_list;
			while (1) {
				if (iter->name == 0) {
					iter->name = malloc(strlen(argv[1]) + strlen(dir->d_name) + 1);
					strcpy(iter->name, argv[1]);
					strcpy(iter->name + strlen(argv[1]), dir->d_name);
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
		}
	}

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

	struct initrd_fent ent;

	struct file_list_entry* fiter = &file_list;

	for (i = 0; i < num_files; i++) {

		//Strip the name from the entity
		char* lastString = strrchr(fiter->name, '/');
		if (lastString == 0)
		{
			printf("lastString null\n");
			lastString = strchr(fiter->name, '\\');
			if (lastString == 0)
			{
				printf("lastString still null\n");
				lastString = fiter->name;
			}
			else
			{
				lastString = lastString + 1;
			}
		} else
		{
			lastString = lastString + 1;
		}

		strcpy(ent.name, lastString);

		helper_list[i].st = ent;
		helper_list[i].loc = ftell(fout);

		if (fwrite(&ent, sizeof(struct initrd_fent), 1, fout) != 1) {

			printf("Unable to write a file chunk\n");
			return 0;

		}

		fiter = fiter->next;
	}

	FILE * fin = 0;
	size_t end;

	fiter = &file_list;

	for (i = 0; i < num_files; i++) {

		helper_list[i].mloc = ftell(fout) + 1;
		fin = fopen(fiter->name, "rb");

		if (fin == 0) {
			printf("Unable to open helper file %s\nAbort\n", fiter->name); return 0;
		}

		fseek(fin, 0, SEEK_END);
		end = ftell(fin);
		helper_list[i].mlen = end;
		fseek(fin, 0, SEEK_SET);
		printf("File %s File Size (Bytes): %i\n", helper_list[i].st.name, end);

		void * mem = malloc(end);

		if (fread(mem, end, 1, fin) != 1) {
			printf("Read Error\n");	return 0;
		}

		if (fwrite(mem, end, 1, fout) != 1) {
			printf("Write Error\n"); return 0;
		}

		free(mem);

		fclose(fin);

		fiter = fiter->next;
	}

	fseek(fout, 0, SEEK_END);
	end = ftell(fout);
	fseek(fout, 0, SEEK_SET);
	head.ramdisk_size = end;

	if (fwrite(&head, sizeof(struct initial_ramdisk_header), 1, fout) != 1) {
		printf("Unable to write the initial RAM disk header\n");
	}

	for (i = 0; i < nmfiles; i++) { //Third file pass.
		fseek(fout, helper_list[i].loc, SEEK_SET); //Seek to the start of the header
		helper_list[i].st.start = helper_list[i].mloc;
		helper_list[i].st.size = helper_list[i].mlen;

		if (fwrite(&helper_list[i].st, sizeof(struct initrd_fent), 1, fout) != 1) {
			printf("Unable to write a file chunk\n");
			return 0;
		}
	}

	fclose(fout);

	fout = fopen(argv[2], "rb");
	fseek(fout, sizeof(struct initial_ramdisk_header), SEEK_SET);

	void* Data = malloc(end - sizeof(struct initial_ramdisk_header));
	size_t Read = fread(Data, end - sizeof(struct initial_ramdisk_header), 1, fout);

	if (Read != 1)
	{
		printf("Error computing ramdisk hash\n");
		printf("RDBYTE %i\n", Read);
		return -1;
	}

	unsigned char digest[16];
	MDData(Data, end - sizeof(struct initial_ramdisk_header), digest);

	printf("CHECKSUM ");

	unsigned int iter = 0;
	for (iter = 0; iter < 16; iter++)
	{
		printf("%x", digest[iter]);
		head.ramdisk_checksum[iter] = digest[iter];
	}

	printf("\n");

	free(Data);

	fclose(fout);

	fout = fopen(argv[2], "r+b");

	if (fwrite(&head, sizeof(struct initial_ramdisk_header), 1, fout) != 1) {
		printf("Unable to write the initial RAM disk header\n");
	}

	fclose(fout);

	return 0;
}
