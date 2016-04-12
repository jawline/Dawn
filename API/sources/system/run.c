#include <system/run.h>
#include <syscall/syscall.h>
DEFN_SYSCALL1(request_run_nproc, 21, const char*);

void systemRunNewProcess(const char* filename) {
	syscall_request_run_nproc(filename);
}
