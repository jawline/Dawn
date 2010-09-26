#include <process/procfault.h>

void gpfHandler()
{
	handleFatalProcessFault(FAULT_ID_GPF, "A GPF occured");
}

void initializeGeneralProtectionFaultHandler()
{
	register_interrupt_handler (13 , &gpfHandler);
}
