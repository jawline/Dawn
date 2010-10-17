#include <process/procfault.h>
#include <panic/panic.h>

void gpfHandler()
{
	handleFatalProcessFault(FAULT_ID_GPF, "A GPF occured");
	PANIC("AHH (GPF Fault set to crash processor)");
}

void initializeGeneralProtectionFaultHandler()
{
	register_interrupt_handler (13 , &gpfHandler);
}
