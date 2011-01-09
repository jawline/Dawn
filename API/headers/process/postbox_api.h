#ifndef _POSTBOX_API_DEF_H_
#define _POSTBOX_API_DEF_H_
#include <process/message.h>
#include <syscall/syscall.h>

/**
 * @ingroup Postbox API
 *
 * @brief This function returns true if the application postbox has data waiting to be read.
 * @param None
 * @return 1 if true, 0 if false
 */
extern unsigned char postboxHasNext();

/**
 * @ingroup Postbox API
 *
 * @brief Read the next message on the postbox
 * @param None
 * @return process_message with next message if it exists. otherwise blank process_message structure
 */
extern process_message postboxGetNext();

/**
 * @ingroup Postbox API
 *
 * @brief This allows the process to set the flags of what it wants to recieve in the kernel as postbox messages.
 * @param 32-bit integer used for bitmask.
 * @return None
 */
extern void postboxSetFlags(uint32_t flags);

#endif //_POSTBOX_API_DEF_H_
