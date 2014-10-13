#ifndef _PROCESS_USED_LIST_DEF_H_
#define _PROCESS_USED_LIST_DEF_H_

void initializeUsedList(process_t* process);
void expandUsedList(process_t* process);
MEM_LOC usedListTop(process_t* process);
void usedListAdd(process_t* process, MEM_LOC location);
void usedListRemove(process_t* process, MEM_LOC location);

#endif //_PROCESS_USED_LIST_DEF_H_
