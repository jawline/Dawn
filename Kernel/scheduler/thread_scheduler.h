#ifndef _THREAD_SCHEDULER_DEF_H_
#define _THREAD_SCHEDULER_DEF_H_
#include <threads/threads.h>

struct thread_entry; //Predefine so thread_entry can self reference.

struct thread_entry 
{
	uint32 quant; //The number of times the clock is called before the thread is shifted out of context and moved to the baack of the queue
	thread_t* thread; //A pointer to the thread this references
	struct thread_entry* next; //Linked list, Next points to the next entry in the list
};

typedef struct thread_entry thread_entry_t;

void initialize_thread_scheduler(thread_t* start);
void thread_schedule_tick();
void add_thread(thread_t* thread);
uint32 current_thread_id();
void switch_thread (thread_t* next);

#endif //_THREAD_SCHEDULER_DEF_H_
