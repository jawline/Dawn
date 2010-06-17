#ifndef _PROCESS_MESSAGE_DEF_H_
#define _PROCESS_MESSAGE_DEF_H_
#include <types/memory.h>

typedef struct {

	int ID;
	unsigned int from_PID; //Who is the message from (What PID)?

	unsigned int message_data[4]; //4 unsigned ints for data about the message
} process_message;

#endif //_PROCESS_MESSAGE_DEF_H_
