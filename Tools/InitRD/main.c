#include <stdio.h>

#include <types/int_types.h>
#include <initrd/initrd_dirent.h>
#include <initrd/initrd_fent.h>
#include <initrd/initrd_header.h>

#include "md5use.h"

#define VER_MAJOR 0
#define VER_MINOR 2
#define VER_REV 7

typedef struct {
	struct initrd_fent st; //File header structure
	uint32 loc; //Header location
	uint32 mloc; //Memory location
	uint32 mlen; //Memory length
} file_helper;

file_helper* helper_list;

int main(int argc, char **argv)
{
	printf("Compiler Version %i.%i.%i\n", VER_MAJOR, VER_MINOR, VER_REV);

	if (argc < 2) {
		printf("Error, Expected Argc greater then 2\n");
		printf("Usage: Outputfile Input Input Input ...\n");	
		return 0;
	}

	helper_list = malloc(sizeof(file_helper) * argc - 2);
	FILE * fout = 0;
	fout = fopen(argv[1], "wb");

	if (fout == 0) {
		printf("Unable to open output file\n");
		return 0;	
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
	}

	uint32 nmdirs = 1;
	if (fwrite(&nmdirs, sizeof(uint32), 1, fout) != 1) {
		printf("Unable to write Directory chunk header\n");	
		return 0;
	}

	struct initrd_dirent direntry;
	strcpy(direntry.name, "root");
        direntry.dentrys = 0;
	direntry.fentrys = argc - 2;

	if (fwrite(&direntry, sizeof(struct initrd_dirent), 1, fout) != 1) {
		printf("Unable to write a directory chunk\n");
		return 0;	
	}
	
	uint32 i = 0;
	for (i = 0; i < direntry.fentrys; i++) {
		if (fwrite(&i, sizeof(uint32), 1, fout) != 1) {
			printf("Unable to write dentry fentry\n");		
		}
	}

	uint32 nmfiles = argc - 2;

	if (fwrite(&nmfiles, sizeof(uint32), 1, fout) != 1) {
		printf("Unable to write File chunk header\n");
		return 0;
	}

	struct initrd_fent ent;

	for (i = 0; i < nmfiles; i++) {

		//Strip the name from the entity
		char* lastString = strrchr(argv[i + 2], '/');
		if (lastString == 0)
		{
			printf("lastString null\n");
			lastString = strchr(argv[i + 2], '\\');
			if (lastString == 0)
			{
				printf("lastString still null\n");
				lastString = argv[i + 2];
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

		printf("Given name %s\n", ent.name);

		helper_list[i].st = ent;
		helper_list[i].loc = ftell(fout);
		if (fwrite(&ent, sizeof(struct initrd_fent), 1, fout) != 1) {
			printf("Unable to write a file chunk\n");
			return 0;	
		}
	}

	FILE * fin = 0;
	size_t end;

	for (i = 0; i < nmfiles; i++) {
		helper_list[i].mloc = ftell(fout) + 1;
		fin = fopen(argv[i + 2], "rb");
		
		if (fin == 0) {
			printf("Unable to open helper file\nAbort\n"); return 0;		
		}

		fseek(fin, 0, SEEK_END);
		end = ftell(fin);
		helper_list[i].mlen = end;
		fseek(fin, 0, SEEK_SET);
		printf("File Size (Bytes): %i\n", end);

		void * mem = malloc(end);
		
		if (fread(mem, end, 1, fin) != 1) {
			printf("Read Error\n");	return 0;		
		}

		if (fwrite(mem, end, 1, fout) != 1) {
			printf("Write Error\n"); return 0;		
		}

		free(mem);
		
		fclose(fin);
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

	fout = fopen(argv[1], "rb");
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

	printf("Computing checksum\n");

	unsigned int iter = 0;
	for (iter = 0; iter < 16; iter++)
	{
		printf("%x", digest[iter]);
		head.ramdisk_checksum[iter] = digest[iter];
	}

	printf("\nEnd of digest\n");

	free(Data);

	fclose(fout);

	fout = fopen(argv[1], "r+b");

	if (fwrite(&head, sizeof(struct initial_ramdisk_header), 1, fout) != 1) {
		printf("Unable to write the initial RAM disk header\n");	
	}	

	printf("RAM disk written. %d bytes\n", head.ramdisk_size);

	fclose(fout);

	printf("Compiled\n");

	return 0;
}
