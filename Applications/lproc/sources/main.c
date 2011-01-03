#include <printf.h>

#include <process/end_process.h>
#include <process/message.h>
#include <process/events.h>
#include <process/process_info.h>
#include <process/get_info.h>
#include <process/postbox_api.h>

int _start(int argc, void* argv)
{

	unsigned int iterator = 0;

	while (1)
	{
		int pid = getProcessID(iterator);

		if (pid == -1) break;

		process_info_t info = getProcessInfo(pid);
		printf("Process %i Name %s Time %i\n", info.pID, info.Name, info.processingTime);

		iterator++;
	}

	printf("Done listing %i processes\n", iterator);

	exit(0);
}
