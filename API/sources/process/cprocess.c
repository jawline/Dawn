#include "process/cprocess.h"

DEFN_SYSCALL1(get_process, 11, unsigned int);
DEFN_SYSCALL0(get_process_cproc, 1);

process_t* cprocess()
{
	return (process_t*) syscall_get_process_cproc();
}

process_t* get_process(unsigned int number)
{
	return (process_t*) syscall_get_process(number);
}
