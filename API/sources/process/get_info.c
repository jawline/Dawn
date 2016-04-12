#include <process/get_info.h>
#include <common.h>

//This returns the PID of a process given by the scheduler at iter i or -1 if out of the schedulers range
DEFN_SYSCALL1(get_process_id, 17, unsigned int);

//These three require a PID
DEFN_SYSCALL1(valid_process, 16, unsigned int);
DEFN_SYSCALL1(get_processing_time, 18, unsigned int);
DEFN_SYSCALL2(get_process_name, 19, char*, unsigned int);

int getProcessID(unsigned int n) {
	int pid = syscall_get_process_id(n);
	return pid;
}

process_info_t getProcessInfo(int pid) {
	process_info_t info;
	memset((void*)&info, 0, sizeof(process_info_t));

	if (syscall_valid_process(pid) == 1) {
		info.pID = pid;
		info.processingTime = syscall_get_processing_time(pid);
		syscall_get_process_name(info.Name, pid);
	} else {
		info.pID = -1;
	}

	return info;
}
