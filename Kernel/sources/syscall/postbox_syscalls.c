#include <types/memory.h>
#include <process/process.h>
#include <process/postbox.h>
#include <process/message.h>

extern process_t* get_current_process();
extern process_message postbox_peek(process_postbox* pb);

unsigned char postboxHasNext()
{
	process_message toTest;
	toTest = postbox_peek(&get_current_process()->m_processPostbox);

	if (toTest.ID == -1) return 0;
	return 1;
}

void postboxReadTop(process_message* Message)
{
	process_message toCopy;
	toCopy = postbox_peek(&get_current_process()->m_processPostbox);
	memcpy(Message, &toCopy, sizeof(process_message));
}

void postboxPopTop()
{
	postbox_top(&get_current_process()->m_processPostbox);
}

void postboxSetFlags(uint32 flags)
{
	get_current_process()->m_postboxFlags = flags;
}
