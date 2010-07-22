#ifndef _POSTBOX_API_DEF_H_
#define _POSTBOX_API_DEF_H_
#include <process/postbox.h>
#include <syscall/syscall.h>

extern void process_postbox_remove_top();
extern process_postbox* get_process_postbox();

#endif //_POSTBOX_API_DEF_H_
