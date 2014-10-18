#include <types/memory.h>
#include <process/process.h>
#include <scheduler/scheduler.h>
#include <debug/debug.h>

void syscallRequestRunNewProcess(const char* NewProcess) {

	createNewProcess(NewProcess, getCurrentProcess()->executionDirectory);
}
