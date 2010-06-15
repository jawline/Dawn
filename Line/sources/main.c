#include <types/memory.h>
#include <common.h>
#include <printf.h>

#include <version/kernel_version.h>
#include <version/os_version.h>

#include <syscall/syscall.h>
#include <printf.h>

char Data[1024];

char Pointer[1024];

unsigned int c_pointer = 0;

DEFN_SYSCALL2(set_input_buffer, 1, const char*, unsigned int);
DEFN_SYSCALL0(get_process_cpos, 2);
DEFN_SYSCALL1(set_process_cpos, 3, unsigned int);

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
		printf("Line - Executable compiled for Kernel version %i.%i.%i codename %s\n", KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION, KVERSION_CODENAME);
		printf("Compiled as part of OS version %i.%i.%i codename %s\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);
		printf("No real functions yet.\n");
	}
	else
	{
		printf("Command %s not found\n", Pointer);
	}
}

int start(int argc, void* argv)
{
	syscall_set_input_buffer(Data, 1024);

	printf("Line - Executable compiled for Kernel version %i.%i.%i codename %s\n", KVERSION_MAJOR, KVERSION_MINOR, KVERSION_REVISION, KVERSION_CODENAME);
	printf("Compiled as part of OS version %i.%i.%i codename %s\n", OS_VERSION_MAJOR, OS_VERSION_MINOR, OS_VERSION_REVISION, OS_VERSION_CODENAME);

	printf("Line.x:> ");
	Pointer[0] = '\0';

	for (;;)
	{
		MEM_LOC cp = syscall_get_process_cpos();

		if (cp != 0)
		{
			unsigned int iter = 0;
			for (iter = 0; iter < cp; iter++)
			{
				if (Data[iter] == '\r')
				{
					if (c_pointer != 0) {
						exec_cmd();
						printf("Line.x:> ");
						c_pointer = 0;
						Pointer[c_pointer] = '\0';
					}
				} else if (Data[iter] == '\b')
				{
					if (c_pointer > 0) {
						printf("\b \b");
						c_pointer = c_pointer - 1;
						Pointer[c_pointer] = '\0';
					}
				}
				else
				{
					printf("%c", Data[iter]);
					Pointer[c_pointer] = Data[iter];
					Pointer[c_pointer + 1] = '\0';
					c_pointer = c_pointer + 1;
				}
			}

			syscall_set_process_cpos(0);

		}
	}

	return 32;
}
