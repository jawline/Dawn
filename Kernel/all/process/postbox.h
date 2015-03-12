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

/**
 * Peeks into the postbox without removing anything from it, will copy the first message from the postbox to dest
 * Returns dest if there is a message waiting or 0 otherwise
 */
process_message* postboxPeek(process_postbox* pb, process_message* dest);

/**
 * Pops into the postbox, will copy the first message from the postbox to dest and then remove it from the postbox
 * Returns dest if there is a message waiting or 0 otherwise
 */
process_message* postboxTop(process_postbox* pb, process_message* dest);

/**
 * Pushes the given message to the postbox
 */
void postboxPush(process_postbox* pb, process_message* msg);

#endif //_PROCESS_POSTBOX_DEF_H
