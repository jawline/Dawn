#ifndef _PROCESS_POSTBOX_DEF_H_
#define _PROCESS_POSTBOX_DEF_H_
#include <process/message.h>

struct process_postbox_message {
	process_message data;
	struct process_postbox_message* next; //The next entry
};

//Another approach would be just to skip the process_postbox structure and use first as a pointer in process. Don't know why I picked this route, makes almost no difference
typedef struct {
	struct process_postbox_message* first;
} process_postbox;

typedef struct process_postbox_message postbox_message_entry;

process_message postboxPeek(process_postbox* pb);
process_message postboxTop(process_postbox* pb);
void postboxPush(process_postbox* pb, process_message msg);

#endif //_PROCESS_POSTBOX_DEF_H
