#include <types/memory.h>
#include <common.h>
#include <printf.h>

#include <version/kernel_version.h>
#include <version/os_version.h>

#include <process/postbox.h>

#include <syscall/syscall.h>
#include <printf.h>

#include <process/message.h>
#include <messages/messages.h>
#include <input/devices.h>

DEFN_SYSCALL0(postbox_location, 1);
DEFN_SYSCALL0(postbox_pop_top, 2);

char Pointer[1024];

void exec_cmd()
{
	printf("\n");

	if (strcmp("exit", Pointer) == 0)
	{
		printf("Exit called\n");
	}
	else if (strcmp("help", Pointer) == 0)
	{
		printf("Line - Command line tool for SimpleOS\n");
		printf("Line - Executable compiled for Kernel version %i.%i.%i codename \"%s\"\n", KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION, KVERSION_CODENAME);
		printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);
		printf("No real functions yet.\n");
	}
	else
	{
		printf("Command %s not found\n", Pointer);
	}
}

int start(int argc, void* argv)
{

	printf("Line - Executable compiled for Kernel version %i.%i.%i codename \"%s\"\n", KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION, KVERSION_CODENAME);
	printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);

	printf("Line.x:> ");
	Pointer[0] = '\0';

	for (;;)
	{
		process_postbox* m_pb = (process_postbox*) syscall_postbox_location();
		if (m_pb->first != 0)
		{
			process_message message = m_pb->first->data;
			
			if (message.ID == INPUT_MESSAGE)
			{
				//Its a input message alright
				if (message.message_data[0] == DEVICE_KEYBOARD)
				{
					//its a keyboard message even!
				}
			}
		}
	}

	return 32;
}
