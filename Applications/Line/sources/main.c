#include <types/memory.h>
#include <common.h>
#include <printf.h>
#include <system/reboot.h>
#include <system/run.h>

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

#include <heap/heap.h>
#include <process/get_info.h>

#define BIT_0 1

char Pointer[1024];
int c_ptr = 0;

unsigned long cps = 0;

void list_chunks(heap_t * heap) //List all active chunks in a heap. 
{
	heap_entry_t * ptr = (heap_entry_t *) heap->heap_location;

	printf("Scanning 0x%x\n", ptr);

	while (ptr) 
	{
		printf("Chunk %x size %i used %i\n", ptr, ptr->size, ptr->used);
		ptr = (heap_entry_t *) ptr->next;
	}
}

char exec_cmd()
{
	printf("\n");

	if (strcmp("exit", Pointer) == 0)
	{
		printf("Exit called\n");
		exit(2);
	}
	else if (strcmp("help", Pointer) == 0)
	{
		printf("Line - Command line tool for SimpleOS\n");
		printf("Line - Executable compiled for Kernel version %i.%i codename \"%s\"\n", KVERSION_WHOLE, KVERSION_FRACTION, KVERSION_CODENAME);
		printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);
		printf("free - list memory information\n");
		printf("reboot - request the kernel to reboot the system\n");
		printf("uptime - Lists the seconds minutes and hours the system has been up for\n");
		printf("lproc - Lists information about all scheduled processes\n");
		printf("cls - Clear the screen\n");
	}
	else if (strcmp("cls", Pointer) == 0)
	{
		clearscreen();
	}
	else if (strcmp("gameoflife", Pointer) == 0)
	{
		systemRunNewProcess("./system/root/GameOfLife");
		exit(1);
	}
	else if (strcmp("uptime", Pointer) == 0)
	{
		unsigned long ticks = clock();
		unsigned long ticks_per_second = cps;

		unsigned int seconds = 0;
		unsigned int minutes = 0;
		unsigned int hours = 0;

		seconds = ticks / ticks_per_second; //Use this temporarely

		hours = (seconds / (60 * 60));
		
		unsigned int remainder = seconds % (60 * 60);

		minutes = remainder / 60;

		seconds = remainder % 60;

		printf("The system has been alive for %i hours %i minutes and %i seconds\n", hours, minutes, seconds);
	}
	else if (strcmp("armageddon", Pointer) == 0)
	{
		printf("Self-Destruct sequence activated\n");

		unsigned int tdeath = 16;

		while (1)
		{
			unsigned long time_for_splode = clock() + cps;
			while (time_for_splode > clock()) {}
			tdeath--;

			printf("%i Seconds remaining\n", tdeath);

			if (tdeath == 0)
				break;
		}

		printf("Hah, just kidding\n");
	}
	else if (strcmp("lproc", Pointer) == 0)
	{
		printf("Listing active processes\n");

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

	} else if (strcmp("free", Pointer) == 0) {
		printf("Free memory information\n");
		MEM_LOC free_frames = getNumberOfFreeFrames();
		MEM_LOC page_size = getPageSize();
		printf("%i free frames of memory\n", free_frames);
		printf("Each frame is %i bytes of memory\n", page_size);
		printf("Therefore there are %i MBs of memory left\n", (free_frames * page_size) / 1024 / 1024);

	} else if (strcmp("reboot", Pointer) == 0) {
		requestReboot();
	} else {
		printf("Command %s not found\n", Pointer);
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

	printf("Line - Executable compiled for Kernel version %i.%i codename \"%s\"\n", KVERSION_WHOLE, KVERSION_FRACTION, KVERSION_CODENAME);
	printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);

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

void _start(int argc, void* argv)
{
	int result = main(argc, argv);
	exit(result);
}
