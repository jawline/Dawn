#include <system/memory.h>

DEFN_SYSCALL0(num_free_frames, 7);
DEFN_SYSCALL0(get_page_size, 8);

MEM_LOC getNumberOfFreeFrames() {
	return syscall_num_free_frames();
}

unsigned long getPageSize() {
	return syscall_get_page_size();
}
