#include <printf.h>
#include <process/end_process.h>

extern "C" {

	int _start(int argc, void* argv)
	{
		clearscreen();
		exit(0);
	}

}
