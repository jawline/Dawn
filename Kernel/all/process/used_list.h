#ifndef _PROCESS_USED_LIST_DEF_H_
#define _PROCESS_USED_LIST_DEF_H_

void initializeUsedList(process_t* process);
void expandUsedList(process_t* process);
void* usedListTop(process_t* process);
void usedListAdd(process_t* process, void* location);
void usedListRemove(process_t* process, void* location);
char usedListHasItems(process_t* process);
void usedListFree(process_t* process);

#endif //_PROCESS_USED_LIST_DEF_H_
