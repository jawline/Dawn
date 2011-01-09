#include <process/procfault.h>
#include <panic/panic.h>
#include <interrupts/interrupt_handler.h>

void gpfHandler()
{
	handleFatalProcessFault(FAULT_ID_GPF, "A GPF occured");
	PANIC("AHH (GPF Fault set to crash processor)");
}

void initializeGeneralProtectionFaultHandler()
{
	registerInterruptHandler (13 , &gpfHandler);
}
