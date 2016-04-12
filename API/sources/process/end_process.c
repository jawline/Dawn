#include <process/end_process.h>

DEFN_SYSCALL0(kill_current_process, 15);
DEFN_SYSCALL1(request_exit, 20, int);

void killcurrentprocess() {
	syscall_kill_current_process();
	for (;;) {}
}

void exit(int value) {
	syscall_request_exit(value);
	for (;;) {}
}
