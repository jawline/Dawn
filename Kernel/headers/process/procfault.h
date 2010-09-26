#ifndef _PROCESS_FAULT_DEFINITIONS_
#define _PROCESS_FAULT_DEFINITIONS_

/* The purpose of this file is to provide a abstracted error handler for fatal process errors (And maybe non-fatal in the future) */

#include <process/process.h>

//Define FAULT_ID as a number and FAULT_MSG as a string
typedef unsigned int FAULT_ID;
typedef char* FAULT_MSG;

//Sent when the process page faults
#define FAULT_ID_PAGEFAULT 0

//Sent when a general protection fault occurs
#define FAULT_ID_GPF 1

void handleFatalProcessFault(FAULT_ID, FAULT_MSG);

#endif //_PROCESS_FAULT_DEFINITIONS_
