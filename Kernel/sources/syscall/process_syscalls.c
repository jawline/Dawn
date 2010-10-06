#include <types/memory.h>
#include <process/process.h>

extern process_t* get_current_process();
extern process_t* scheduler_return_process(unsigned int iter);

unsigned char syscallProcessValid(unsigned int pid)
{
	//Iterate untill found the correct PID
	process_t* iterator = 0;
	unsigned int number = 0;

	for (;;)
	{
		iterator = scheduler_return_process(number);

		if (iterator == 0) return 0;
		if (iterator->m_ID == pid) break;

		number = number + 1;
	}

	return 1;
}

int syscallGetPid(unsigned int iter)
{
	if (scheduler_return_process(iter) == 0) return -1;
	return scheduler_return_process(iter)->m_ID;
}

unsigned long syscallGetProcessingTime(unsigned int pid)
{

	//Iterate untill found the correct PID
	process_t* iterator = 0;
	unsigned int number = 0;

	for (;;)
	{
		iterator = scheduler_return_process(number);

		if (iterator == 0) return 0;
		if (iterator->m_ID == pid) break;

		number = number + 1;
	}

	//Return the processing time
	return iterator->m_processingTime;
}

void syscallGetName(char* StrLocation, unsigned int pid)
{	
	//Iterate untill found the correct PID
	process_t* iterator = 0;
	unsigned int number = 0;

	for (;;)
	{
		iterator = scheduler_return_process(number);

		if (iterator == 0)
		{
			strcpy(StrLocation, "Unknown");
			return;
		}
		if (iterator->m_ID == pid) break;

		number = number + 1;
	}

	//Copy the name into the StrLocation
	strcpy(StrLocation, iterator->m_Name);
}

void syscallKillCurrentProcess()
{
	scheduler_kill_current_process();
}

void syscallRequestExit(int returnValue)
{
	get_current_process()->m_returnValue = returnValue;
	scheduler_kill_current_process();
}
