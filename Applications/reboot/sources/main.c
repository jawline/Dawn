#include <printf.h>
#include <process/end_process.h>
#include <system/reboot.h>

extern "C" {

	int _start(int argc, void* argv)
	{
		printf("Requesting the kernel to reboot\n");

		requestReboot();

		exit(0);
	}

}
