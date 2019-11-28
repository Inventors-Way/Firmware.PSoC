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

enum MessageID Message_GetID(const uint64_t msg) { return (enum MessageID)    ((0xFFFF000000000000 & msg) >> 48); }

uint64_t Message_GetData(const uint64_t data) { return (uint64_t) (0x0000FFFFFFFFFFFF & data); }

/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

