#ifndef _PROCESS_MESSAGE_DEF_H_
#define _PROCESS_MESSAGE_DEF_H_
#include <types/memory.h>

typedef struct {

	int ID;
	int from_PID; //Who is the message from (What PID)?

	long message_data[4]; //4 unsigned ints for data about the message

	MEM_LOC messageAdditionalData; //Additional data sent with the process (Can be used as a pointer)
} process_message;

#endif //_PROCESS_MESSAGE_DEF_H_
