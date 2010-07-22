#include <types/memory.h>
#include <process/process.h>

extern process_t* get_current_process();

MEM_LOC syscallReturnCurrentProcess()
{
	return get_current_process();
}

void syscallKillCurrentProcess()
{
	get_current_process()->m_shouldDestroy = 1;
}


MEM_LOC syscallGetProcess(unsigned int iter)
{
	return scheduler_return_process(iter);
}

