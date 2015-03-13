#include <mm/virt_mm.h>
#include <process/process.h>
#include <process/message.h>
#include <messages/messages.h>
#include <debug/debug.h>
#include <scheduler/scheduler.h>
#include <settings/settingsmanager.h>
#include <interrupts/interrupts.h>

process_t* systemIdlePtr = 0;
process_t* systemProcPtr = 0;

void systemIdleProcess() {
	systemIdlePtr = getCurrentProcess();

	for (;;) {
		//Halt the processor tell the next interrupt
		__asm__ volatile("hlt");
	}
}

void systemMainProcess() {

	//Create the process set as onstart in the global settings
	createNewProcess(settingsReadValue("system.on_boot", "/system/Line"),
			init_vfs());

	//Enable interrupts
	enableInterrupts();

	//Store a pointer to the current process to be used when checking close requests
	systemProcPtr = getCurrentProcess();

	int schedulerIter = 0;
	process_t* schedulerPtr = 0;
	int numNonSystem = 0;

	//Loop and continually halt the processor, this will cause the processor to idle between interrupts
	for (;;) {

		process_message msg;

		//Check all messages
		while (postboxTop(&getCurrentProcess()->processPostbox, &msg)) {
			//TODO: Do something
			DEBUG_PRINT("System has recieved a message");
		}

		numNonSystem = 0;

		schedulerIter = 0;
		//Shut down all processes that need a-killin
		while (1) {
			schedulerPtr = schedulerReturnProcess(schedulerIter);

			if (schedulerPtr == 0) {
				break;
			} else if (schedulerPtr->shouldDestroy == 1) {
				//Must remove this process and kill it! so disable interrupts don't wanna be interrupted
				disableInterrupts();

				if ((schedulerPtr == systemIdlePtr)
						|| (schedulerPtr == systemProcPtr)) {
					DEBUG_PRINT("Cannot close SystemIdle or System\n");
					schedulerPtr->shouldDestroy = 0;
				} else {
					DEBUG_PRINT(
							"Process %i (%s) terminated with return value %i\n",
							schedulerPtr->id, schedulerPtr->name,
							schedulerPtr->returnValue);

					schedulerRemove(schedulerPtr);

					DEBUG_PRINT(
							"Freeing process %x (%i:%s) current process %i\n",
							schedulerPtr, schedulerPtr->id,
							schedulerPtr->name, getCurrentProcess()->id);
					freeProcess(schedulerPtr);

				}

				//Enable interrupts once the deed is done
				enableInterrupts();
			} else {
			}

			//Count up the number of non system processes (To check there is a interface active)
			if ((schedulerPtr != systemIdlePtr)
					&& (schedulerPtr != systemProcPtr)) {
				numNonSystem++;
			}

			schedulerIter++;
		}

		//If this is the last process alive?
		if (numNonSystem == 0) {

			//Is the system set to restart the boot program when there are no other active programs
			if (strcmp(settingsReadValue("system.boot_program_keep_alive", "yes"),
					"yes") == 0) {

				//Disable interrupts while the new process is being created
				disableInterrupts();

				DEBUG_PRINT("Creating new instance of %s\n",
						settingsReadValue("system.on_boot", "/system/Line"));

				//Create the new process with the program set as system.on_boot
				createNewProcess(settingsReadValue("system.on_boot", "/system/Line"),
						init_vfs());

				//Re enable them once your done
				enableInterrupts();
			}

		}

		schedulerYield();
	}
}

void systemProcess() {
	//Disable interrupts while forking
	disableInterrupts();

	//Fork the current process
	int forkedID = kfork();

	//Enable them once the processes are set up
	enableInterrupts();

	if (forkedID == 1) {

		//Create the main system process
		setProcessName(getCurrentProcess(), "System");

		//And run it
		systemMainProcess();

	} else {

		//Create the idle process
		setProcessName(getCurrentProcess(), "SystemIdle");

		//And jump into the idle loop
		systemIdleProcess();

	}

}
