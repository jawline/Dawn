#include <types/memory.h>
#include <process/process.h>

extern process_t* get_current_process();
extern process_t* scheduler_return_process(unsigned int iter);

unsigned char syscallProcessValid(unsigned int iter)
{
	if (scheduler_return_process(iter) != 0) return 1;
	else return 0;
}

unsigned int syscallGetPid(unsigned int iter)
{
	if (scheduler_return_process(iter) == 0) return 0;
	return scheduler_return_process(iter)->m_ID;
}

unsigned long syscallGetProcessingTime(unsigned int iter)
{
	if (scheduler_return_process(iter) != 0) return 0;
	return scheduler_return_process(iter)->m_processingTime;
}

void syscallGetName(char* StrLocation, unsigned int iter)
{
	if (scheduler_return_process(iter) != 0) return;
	
	strcpy(StrLocation, scheduler_return_process(iter)->m_Name);
}

void syscallKillCurrentProcess()
{
	get_current_process()->m_shouldDestroy = 1;
}
