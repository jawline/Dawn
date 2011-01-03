#include <types/memory.h>
#include <common.h>
#include <printf.h>
#include <system/reboot.h>
#include <system/run.h>

#include <debug/debugmode.h>

#include <version/kernel_version.h>
#include <version/os_version.h>

#include <process/postbox_api.h>

#include <syscall/syscall.h>
#include <printf.h>

#include <process/message.h>
#include <process/events.h>
#include <process/process_info.h>

#include <messages/messages.h>
#include <input/devices.h>

#include <clock/clock.h>
#include <process/get_info.h>
#include <scancodes.h>
#include <process/end_process.h>
#include <system/memory.h>
#include <process/sleep.h>

#define BIT_0 1

char Pointer[1024];
int c_ptr = 0;

unsigned long cps = 0;

char exec_cmd()
{
	printf("\n");

	if (strcmp("help", Pointer) == 0)
	{
		printf("Line - Command line tool for SimpleOS\n");
		printf("Line - Executable compiled for Kernel version %i.%i codename \"%s\"\n", KVERSION_WHOLE, KVERSION_FRACTION, KVERSION_CODENAME);
		printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);
		printf("Commands, debug_on, debug_off, cls. any other entry will try to start a program of that name\n");
	}
	else if (strcmp("cls", Pointer) == 0)
	{
		clearscreen();
	}
	else if (strcmp("debug_on", Pointer) == 0)
	{
		requestSystemDebugMode();
	}
	else if (strcmp("debug_off", Pointer) == 0)
	{
		requestDisableSystemDebugMode();
	}
	else {
		systemRunNewProcess(Pointer);
		exit(1);
	}

	return 0;
}

/**
 * @brief The entry point of the basic shell-style application for Dawn known as Line
 * @params int Argc and void* Argv are given by the OS and corrospond to the equivilents on other operating systems
 * @return The result of the application
 * @callgraph
 */
int main(int argc, void* argv)
{
	c_ptr = 0;
	postboxSetFlags(INPUT_BIT);

	cps = getClocksPerSecond();

	printf("Line.x:> ");
	Pointer[0] = '\0';

	for (;;)
	{

		if (postboxHasNext() == 1)
		{
			process_message message;
			message = postboxGetNext();
			
			if (message.ID == INPUT_MESSAGE)
			{
				//Its a input message alright
				if (message.message_data[0] == DEVICE_KEYBOARD)
				{
					//its a keyboard message even!
					char C = getAsciFromScancode(message.message_data[1], message.message_data[2]);

					if (C == '\r')
					{	
						if (c_ptr != 0)
						{
							if (exec_cmd() == 1)
								break;

							c_ptr = 0;
							Pointer[c_ptr] = '\0';
							printf("Line.x:> ");
						}
					}
					else if (C == '\b')
					{
						if (c_ptr != 0)
						{
							printf("\b \b");
							c_ptr--;
							Pointer[c_ptr] = '\0';
						}
					}
					else
					{
						Pointer[c_ptr] = C;
						Pointer[c_ptr + 1] = '\0';
						c_ptr++;
						printf("%c", C);
					}
				}
				else if (message.message_data[0] == DEVICE_MOUSE)
				{

					cls();

					//Message from a the mouse
					printf("Message Data: %i %i ", message.message_data[2], message.message_data[3]);

					if (message.message_data[1] & BIT_0 == 1)
					{
						printf("LBTN");
					}
					printf("\n");
				}
			}
		}

		sleepProcess();
	}

	return 1;
}

extern "C" {

	void _start(int argc, void* argv)
	{
		int result = main(argc, argv);
		exit(result);
	}

}
