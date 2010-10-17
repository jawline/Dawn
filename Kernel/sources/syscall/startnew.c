#include <types/memory.h>
#include <process/process.h>

extern process_t* get_current_process();
extern process_t* scheduler_return_process(unsigned int iter);

void syscallRequestRunNewProcess(const char* NewProcess)
{
	createNewProcess(NewProcess, get_current_process()->m_executionDirectory);
	enable_interrupts();
}
