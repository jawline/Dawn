#include <mm/virt_mm.h>
#include <process/process.h>
#include <process/message.h>
#include <messages/messages.h>
#include <debug/debug.h>

extern process_t* get_current_process();
extern process_t* scheduler_return_process();

process_t* systemIdlePtr = 0;
process_t* systemProcPtr = 0;

void systemIdleProcess()
{
	systemIdlePtr = get_current_process();

	for (;;)
	{
		//Halt the processor tell the next interrupt
		asm volatile("hlt");
	}
}

void systemMainProcess()
{
	createNewProcess(settingsReadValue("kernel.onstart"), init_vfs());
	enable_interrupts();

	systemProcPtr = get_current_process();

	int schedulerIter = 0;
	process_t* schedulerPtr = 0;
	int numNonSystem = 0;

	//Loop and continually halt the processor, this will cause the processor to idle between interrupts
	for (;;) {

		//Check all messages
		while (1)
		{
			//Grab the top message
			process_message pb_top = postbox_top(&get_current_process()->m_processPostbox);

			if (pb_top.ID != -1)
			{
				//Its a message to be handled

			
			}
			else
			{
				break;
			}

		}

		numNonSystem = 0;

		schedulerIter = 0;
		//Shut down all processes that need a-killin
		while (1)
		{
			schedulerPtr = scheduler_return_process(schedulerIter);

			if (schedulerPtr == 0)
			{
				break;
			}
			else if (schedulerPtr->m_shouldDestroy == 1)
			{
				//Must remove this process and kill it! so disable interrupts don't wanna be interrupted
				disable_interrupts();

				if ((schedulerPtr == systemIdlePtr) || (schedulerPtr == systemProcPtr))
				{
					DEBUG_PRINT("Cannot close SystemIdle or System\n");
					schedulerPtr->m_shouldDestroy = 0;
				}
				else
				{
					printf("Process %i (%s) terminated with return value %i\n", schedulerPtr->m_ID, schedulerPtr->m_Name, schedulerPtr->m_returnValue);

					scheduler_remove(schedulerPtr);

					DEBUG_PRINT("Freeing process %x (%i:%s) current process %i\n", schedulerPtr, schedulerPtr->m_ID, schedulerPtr->m_Name, get_current_process()->m_ID);
					freeProcess(schedulerPtr);

				}

				//Enable interrupts once the deed is done
				enable_interrupts();
			}
			else
			{
			}

			//Count up the number of non system processes (To check there is a interface active)
			if ((schedulerPtr != systemIdlePtr) && (schedulerPtr != systemProcPtr))
			{
				numNonSystem++;
			}

			schedulerIter++;
		}

		//If this is the last process alive?
		if (numNonSystem == 0)
		{
			if (strcmp(settingsReadValue("kernel.restart_onstart"), "yes") == 0)
			{

				disable_interrupts();

				printf("Creating new instance of %s\n", settingsReadValue("kernel.onstart"));

				createNewProcess( settingsReadValue("kernel.onstart"), init_vfs());

				enable_interrupts();

			}
		}

		//Sleep the current process
		scheduler_block_me();
	}
}

void systemProcess()
{
    //Disable interrupts while forking
    disable_interrupts();

    int forkedID = kfork();
    
    //Enable them once the processes are set up
    enable_interrupts();


    if (forkedID == 1)
    {
   	set_process_name(get_current_process(), "System");
	systemMainProcess();
    }
    else
    {
   	set_process_name(get_current_process(), "SystemIdle");
	systemIdleProcess();
    }

}
