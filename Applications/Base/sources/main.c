#include <printf.h>
#include <process/end_process.h>

extern "C" {

	int _start(int argc, void* argv)
	{
		printf("BASE: Hello world\n");

		exit(3);
	}

}
