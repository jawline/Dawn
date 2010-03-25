#include <stdio.h>

#include <int_types.h>
#include <initrd_dirent.h>
#include <initrd_fent.h>
#include <initrd_header.h>

/* NOTES ABOUT INITIAL RAM DISKS*/

/* They need to be compiled with the same compiler & for the same archetecture as the kernel itself. 
   They are not cross platform compatable (In their current state any way)
   Two compilers may pack the structs or order the integers in different ways (For instance Little endian - Big Endian conversion), this could lead to big problems.
   Recompile for each desired archetecture
*/

#define VER_MAJOR 0
#define VER_MINOR 2
#define VER_REV 6

typedef struct {
	struct initrd_fent st; //File header structure
	uint32 loc; //Header location
	uint32 mloc; //Memory location
	uint32 mlen; //Memory length
} file_helper;

file_helper * helper_list;

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
		strcpy(ent.name, argv[i + 2]);
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
		fin = fopen(helper_list[i].st.name, "rb");
		
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

	for (i = 0; i < nmfiles; i++) { //Third (And Final) file pass.
		fseek(fout, helper_list[i].loc, SEEK_SET); //Seek to the start of the header
		helper_list[i].st.start = helper_list[i].mloc;
		helper_list[i].st.size = helper_list[i].mlen;

		if (fwrite(&helper_list[i].st, sizeof(struct initrd_fent), 1, fout) != 1) {
			printf("Unable to write a file chunk\n");
			return 0;	
		}
	}

	printf("RAM disk written. %d bytes\n", head.ramdisk_size);

	return 0;
}