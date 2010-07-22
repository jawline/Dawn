#include <system/memory.h>

DEFN_SYSCALL0(num_free_frames, 6);
DEFN_SYSCALL0(get_page_size, 7);

MEM_LOC get_num_free_frames()
{
	return syscall_num_free_frames();
}

unsigned long get_page_size()
{
	return syscall_get_page_size();
}
