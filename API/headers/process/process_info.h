#ifndef _PROCESS_INFO_STRUCTURE_DEF_H_
#define _PROCESS_INFO_STRUCTURE_DEF_H_

typedef struct
{
	int pID; //Process ID number
	char Name[64]; //ASCI string containing process name
	unsigned long processingTime; //How much CPU has been blown on this thing
} process_info_t;

#endif //_PROCESS_INFO_STRUCTURE_DEF_H_
