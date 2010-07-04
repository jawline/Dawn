#include <types/memory.h>
#include <common.h>
#include <printf.h>
#include <system/reboot.h>

#include <version/kernel_version.h>
#include <version/os_version.h>

#include <process/postbox_api.h>

#include <syscall/syscall.h>
#include <printf.h>

#include <process/message.h>
#include <process/events.h>
#include <process/process.h>
#include <process/cprocess.h>

#include <messages/messages.h>
#include <input/devices.h>

#include <clock/clock.h>

#include <heap/heap.h>

DEFN_SYSCALL0(get_kheap, 14);
DEFN_SYSCALL0(cls, 15);
DEFN_SYSCALL2(scancode_to_asci, 3, unsigned char, unsigned long);
DEFN_SYSCALL1(set_flag, 5, unsigned int);

char Pointer[1024];
int c_ptr = 0;

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
		exit(3);
	}
	else if (strcmp("help", Pointer) == 0)
	{
		printf("Line - Command line tool for SimpleOS\n");
		printf("Line - Executable compiled for Kernel version %i.%i.%i codename \"%s\"\n", KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION, KVERSION_CODENAME);
		printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);
		printf("free - list memory information\n");
		printf("reboot - request the kernel to reboot the system\n");
		printf("uptime - Lists the seconds minutes and hours the system has been up for\n");
		printf("lproc - Lists information about all scheduled processes\n");
		printf("lheap - List the chunks in the kernel heap (Not this applications heap\n");
		printf("cls - Clear the screen");
	}
	else if (strcmp("lheap", Pointer) == 0)
	{
		heap_t* kheap = syscall_get_kheap();
		list_chunks(kheap);
	}
	else if (strcmp("cls", Pointer) == 0)
	{
		syscall_cls();
	}
	else if (strcmp("uptime", Pointer) == 0)
	{
		unsigned long ticks = clock();
		unsigned long ticks_per_second = get_clocks_per_second();

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
			unsigned long time_for_splode = clock() + get_clocks_per_second();
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
		process_t* proc = 0;

		while (1)
		{
			proc = get_process(iterator);

			if (proc == 0) break;
			printf("Process ID: %i - Name: \"%s\" - CPU attention: %ims\n", proc->m_ID, proc->m_Name, (proc->m_processingTime / (get_clocks_per_second() / 1000)));

			iterator++;
		}

		printf("Done\n");
	} else if (strcmp("free", Pointer) == 0) {
		printf("Free memory information\n");
		MEM_LOC free_frames = get_num_free_frames();
		MEM_LOC page_size = get_page_size();
		printf("%i free frames of memory\n", free_frames);
		printf("Each frame is %i bytes of memory\n", page_size);
		printf("Therefore there are %i MBs of memory left\n", (free_frames * page_size) / 1024 / 1024);

	} else if (strcmp("reboot", Pointer) == 0) {
		request_reboot();
	} else {
		printf("Command %s not found\n", Pointer);
	}

	return 0;
}

int main(int argc, void* argv)
{

	syscall_set_flag(INPUT_BIT);

	unsigned long cps = get_clocks_per_second();

	printf("Line - Executable compiled for Kernel version %i.%i.%i codename \"%s\"\n", KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION, KVERSION_CODENAME);
	printf("Compiled as part of OS version %i.%i.%i codename \"%s\"\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);

	printf("Line.x:> ");
	Pointer[0] = '\0';

	for (;;)
	{
		process_postbox* m_pb = get_process_postbox();

		if (m_pb->first != 0)
		{
			process_message message = m_pb->first->data;
			
			if (message.ID == INPUT_MESSAGE)
			{
				//Its a input message alright
				if (message.message_data[0] == DEVICE_KEYBOARD)
				{
					//its a keyboard message even!
					char C = syscall_scancode_to_asci(message.message_data[1], message.message_data[2]);

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
			} else
			{
				sleep_proc();
			}
			
			process_postbox_remove_top();
		}

		sleep_proc();
	}

	return 1;
}
