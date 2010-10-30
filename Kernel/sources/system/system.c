#include <mm/virt_mm.h>
#include <process/process.h>
#include <process/message.h>
#include <messages/messages.h>

extern process_t* get_current_process();
extern process_t* scheduler_return_process();

void system_process()
{
    int schedulerIter = 0;
    process_t* schedulerPtr = 0;

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

			scheduler_remove(schedulerPtr);

			printf("Freeing process %x (%i:%s) current process %i\n", schedulerPtr, schedulerPtr->m_ID, schedulerPtr->m_Name, get_current_process()->m_ID);
			freeProcess(schedulerPtr);

			//Enable interrupts once the deed is done
			enable_interrupts();
		}

		schedulerIter++;
	}

	//If this is the last process alive?
	if (scheduler_num_processes() == 1)
	{
		printf("Creating new instance of Line\n");
		createNewProcess("./system/root/Line", init_vfs());
		enable_interrupts();
	}

	//Halt the processor tell the next interrupt
	asm volatile("hlt");

	//Sleep the current process
	scheduler_block_me();
    }

}
