#include <os/Message.h>
#include <stdlib.h>
#include <os/System.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

/**
 * \addtogroup Message
 * @{
 */


/** @} */

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void Message_Create(const enum MessageID id, const uint64_t data)
{
    uint64_t msg = (((uint64_t) id) << 48) + data;
    System_DispatchMessage(msg);
}

/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

