#ifndef _POSTBOX_API_DEF_H_
#define _POSTBOX_API_DEF_H_
#include <process/message.h>
#include <syscall/syscall.h>

extern unsigned char postboxHasNext();
extern process_message postboxGetNext();
extern void postboxSetFlags(uint32 flags);

#endif //_POSTBOX_API_DEF_H_
