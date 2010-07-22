#include <stdio.h>

int main(int argc, char **argv)
{
	FILE* f = fopen("./blank.kt", "w");
	if (!f) return -1;
	
	char *blank = malloc(256);
	memset(blank, 126, 256);
	
	fwrite(blank, 1, 256, f);
	fwrite(blank, 1, 256, f);
	fwrite(blank, 1, 256, f);

	fclose(f);

	return 0;
}
