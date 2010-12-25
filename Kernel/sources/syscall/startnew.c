#include <types/memory.h>
#include <process/process.h>
#include <scheduler/process_scheduler.h>

void syscallRequestRunNewProcess(const char* NewProcess)
{

	createNewProcess(NewProcess, getCurrentProcess()->m_executionDirectory);

}
