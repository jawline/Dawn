#include <mm/virt_mm.h>
#include <process/process.h>
#include <process/message.h>
#include <messages/messages.h>

extern process_t* get_current_process();

void system_process()
{
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
