#include <debug/debugmode.h>
#include <syscall/syscall.h>

DEFN_SYSCALL1(request_debug_level, 22, uint8);

void requestSystemDebugMode()
{
	syscall_request_debug_level(1);
}

void requestDisableSystemDebugMode()
{
	syscall_request_debug_level(0);
}

