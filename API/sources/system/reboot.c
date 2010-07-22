#include <system/reboot.h>

DEFN_SYSCALL0(reboot, 8);

void request_reboot()
{
	syscall_reboot();
}
