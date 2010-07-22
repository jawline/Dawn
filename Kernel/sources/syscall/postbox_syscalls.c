#include <types/memory.h>
#include <process/process.h>

extern process_t* get_current_process();

MEM_LOC postboxLocation()
{
	return &get_current_process()->m_processPostbox;
}

void postboxPopTop()
{
	postbox_top(&get_current_process()->m_processPostbox);
}

void postboxSetBit(unsigned int bit)
{
	get_current_process()->m_postboxFlags = get_current_process()->m_postboxFlags | bit;
}
