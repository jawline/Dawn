#include <types/memory.h>
#include <process/process.h>
#include <scheduler/scheduler.h>
#include <debug/debug.h>

void syscallRequestRunNewProcess(const char* executablePath) {
	createNewProcess(executablePath, getCurrentProcess()->executionDirectory);
}
