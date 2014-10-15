#include <types/memory.h>
#include <process/process.h>
#include <process/postbox.h>
#include <process/message.h>
#include <scheduler/process_scheduler.h>

extern process_message postboxPeek(process_postbox* pb);

unsigned char postboxHasNext()
{
	process_message toTest;
	toTest = postboxPeek(&getCurrentProcess()->processPostbox);

	if (toTest.ID == -1) return 0;
	return 1;
}

void postboxReadTop(process_message* Message)
{
	process_message toCopy;
	toCopy = postboxPeek(&getCurrentProcess()->processPostbox);
	memcpy(Message, &toCopy, sizeof(process_message));
}

void postboxPopTop()
{
	postboxTop(&getCurrentProcess()->processPostbox);
}

void postboxSetFlags(uint32_t flags)
{
	getCurrentProcess()->postboxFlags = flags;
}
