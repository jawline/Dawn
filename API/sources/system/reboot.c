#include <system/reboot.h>

DEFN_SYSCALL0(reboot, 9);

void requestReboot()
{
	syscall_reboot();
}
