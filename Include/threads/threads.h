#ifndef _THREAD_DEF_H_
#define _THREAD_DEF_H_
#include <types/int_types.h>
#include <stdlib.h>
#include <printf.h>
#include "common.h"

#define RR_QUANT 30

typedef struct 
{
	uint32 t_id;
	uint32 esp, ebp, ebx, esi, edi;
} thread_t;

thread_t* initialize_threading();
thread_t* create_thread (int (*fn)(void*), void* arg, uint32* stack);

#endif //_THREAD_DEF_H_
