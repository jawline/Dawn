#include <printf.h>
#include <process/end_process.h>
#include <system/memory.h>

extern "C" {

	int _start(int argc, void* argv)
	{
		printf("Free memory information\n");
		MEM_LOC free_frames = getNumberOfFreeFrames();
		MEM_LOC page_size = getPageSize();
		printf("%i free frames of memory\n", free_frames);
		printf("Each frame is %i bytes of memory\n", page_size);
		printf("Therefore there are %i MBs of memory left\n", (free_frames * page_size) / 1024 / 1024);
		exit(0);
	}

}
