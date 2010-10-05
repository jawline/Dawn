#ifndef _PROCESS_INFO_STRUCTURE_DEF_H_
#define _PROCESS_INFO_STRUCTURE_DEF_H_

/**
 * @ingroup Process Info
 * 
 * @brief This data structure is used to store information about a process recieved via the syscall API
 *
 */
typedef struct
{
	/**
	  * @ingroup Process Info
	  * @brief Process ID number
	  */

	int pID;

	/**
	  * @ingroup Process Info
	  * @brief 64 character ASCI string containing the process name
	  */

	char Name[64];

	/**
	  * @ingroup Process Info
	  * @brief unsigned long containing the time in ticks that the processor has spent on this process (Total)
	  */

	unsigned long processingTime;

} process_info_t;

#endif //_PROCESS_INFO_STRUCTURE_DEF_H_
