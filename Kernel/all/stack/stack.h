#ifndef _STACK_DEF_H_
#define _STACK_DEF_H_
#include <common.h>
#include <types/size_t.h>

/* A set of utility functions concerning the stack */

typedef void* stack_t;

/**
 * Move the stack from the initial stack pointer to a new position in memory, (Stack expands downard so the start location is the highest address used
 * and -size is the earliest address used)
 */
stack_t moveStack(stack_t new_start, size_t size, size_t initial_esp);

#endif //_STACK_DEF_H_
