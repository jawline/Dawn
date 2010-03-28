#include <threads/threads.h>

uint32 t_ids = 0;

extern void thread_exit ();

thread_t* initialize_threading ()
{
  thread_t* thread = malloc (sizeof (thread_t));
  thread->t_id  = t_ids++;

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
