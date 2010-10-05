#ifndef _SYSTEM_REBOOT_API_
#define _SYSTEM_REBOOT_API_
#include <syscall/syscall.h>

/**
 * @ingroup System Managment
 * 
 * @brief This function requests that the kernel reboot. likely to be rejected unless the application is given special privilages
 * @param None
 * @return None
 */

extern void requestReboot();

#endif //_SYSTEM_REBOOT_API_
