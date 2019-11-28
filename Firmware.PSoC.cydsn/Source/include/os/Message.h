#ifndef SERVICES_OS_MESSAGES_H
#define SERVICES_OS_MESSAGES_H
    
#include <stdint.h>

/** \addtogroup Message
 *  @{
 */

enum MessageID
{
    MSG_EOL
};

void Message_Create(const enum MessageID id, const uint64_t data);

enum MessageID Message_GetID(const uint64_t msg);

uint64_t Message_GetData(const uint64_t data);


/** @}*/

#endif