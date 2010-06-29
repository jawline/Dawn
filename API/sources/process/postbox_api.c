#include <process/postbox_api.h>

DEFN_SYSCALL0(postbox_location, 1);
DEFN_SYSCALL0(postbox_pop_top, 2);

void process_postbox_remove_top()
{
	syscall_postbox_pop_top();
}

process_postbox* get_process_postbox()
{
	return (process_postbox*) syscall_postbox_location();
}
