#include <types/memory.h>
#include <process/process.h>
#include <process/postbox.h>
#include <process/message.h>
#include <scheduler/scheduler.h>

extern process_message postboxPeek(process_postbox* pb);

unsigned char postboxHasNext() {
	process_message toTest;
	return postboxPeek(&getCurrentProcess()->processPostbox, &toTest) != 0;
}

void postboxReadTop(process_message* message) {
	postboxPeek(&getCurrentProcess()->processPostbox, message);
}

void postboxPopTop() {
	process_message toTest;
	postboxTop(&getCurrentProcess()->processPostbox, &toTest);
}

void postboxSetFlags(uint32_t flags) {
	getCurrentProcess()->postboxFlags = flags;
}
