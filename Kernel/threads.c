#include "threads.h"

thread_t* current_thread;
thread_t* main_thread;

thread_entry_t* thread_list_start = 0;
thread_entry_t* thread_list_current = 0;

uint32 t_ids = 0;

static void thread_exit ();

void initialize_thread_scheduler(thread_t* start) 
{
	thread_list_start = malloc(sizeof(thread_entry_t));

	thread_list_start->thread = start;
	thread_list_start->next = thread_list_start; //Link back to itself as the next thread
	thread_list_start->quant = RR_QUANT;

	thread_list_current = thread_list_start;
}

void thread_schedule_tick() 
{
	if (thread_list_current == 0) return;

	if (thread_list_current->quant < 1) //Swap to the next thread 
	{
		thread_list_current = thread_list_current->next;
		thread_list_current->quant == RR_QUANT;

		asm volatile("sti"); //TODO: Pop everything off the stack before switching threads - REVISION, NVM code execution returns here.

		switch_thread(thread_list_current->thread);
	} else {
		thread_list_current->quant--;
	}
}

void add_thread(thread_t* thread) 
{
	//Where is the end of the list
	thread_entry_t* next = malloc(sizeof(thread_entry_t));
	thread_entry_t* iter = thread_list_start;

	while (1) //Find the entry that links back to the start
	{
		if (iter->next == thread_list_start) break;
		iter = iter->next;
	}

	next->next = thread_list_start;
	next->thread = thread;
	iter->next = next;
}

void remove_thread(thread_t* thread) {
	thread_entry_t* iter = thread_list_start;

	while (1) 
	{
		if (iter->next->thread == thread)
		{
			break;		
		}

		iter = iter->next;

	}

	//Iter = the thread entry before the thread that needs to be removed
	iter->next = iter->next->next; //Remove it from the list

	return;
}

thread_t* initialize_threading ()
{
  thread_t* thread = malloc (sizeof (thread_t));
  thread->t_id  = t_ids++;
  
  current_thread = thread;
  main_thread = thread;

  return thread;
}

thread_t* create_thread (int (*fn)(void*), void* arg, uint32* stack)
{
  thread_t* thread = malloc (sizeof (thread_t));
  memset (thread, 0, sizeof (thread_t));
  thread->t_id = t_ids++;
  
  *--stack = (uint32)arg;
  *--stack = (uint32)&thread_exit; // Fake return address.
  *--stack = (uint32)fn;
  *--stack = 0; // Fake EBP.
  thread->ebp = (uint32)stack;
  thread->esp = (uint32)stack;

  return thread;
}

void switch_thread (thread_t* next)
{
  asm volatile ("mov %%esp, %0" : "=r" (current_thread->esp));
  asm volatile ("mov %%ebp, %0" : "=r" (current_thread->ebp));
  asm volatile ("mov %%ebx, %0" : "=r" (current_thread->ebx));
  asm volatile ("mov %%esi, %0" : "=r" (current_thread->esi));
  asm volatile ("mov %%edi, %0" : "=r" (current_thread->edi));

  current_thread = next;

  asm volatile ("mov %0, %%edi" : : "r" (next->edi));
  asm volatile ("mov %0, %%esi" : : "r" (next->esi));
  asm volatile ("mov %0, %%ebx" : : "r" (next->ebx));
  asm volatile ("mov %0, %%esp" : : "r" (next->esp));
  asm volatile ("mov %0, %%ebp" : : "r" (next->ebp));


}

void thread_exit ()
{
  disable_interrupts();

  register uint32 val asm ("eax");

  printf("Thread exited with value %d\n", val);
  remove_thread(current_thread);
  kfree(current_thread);
  thread_list_current = thread_list_start;
  switch_thread(main_thread);
  enable_interrupts();
}

uint32 current_thread_id() {
	return current_thread->t_id;
}
