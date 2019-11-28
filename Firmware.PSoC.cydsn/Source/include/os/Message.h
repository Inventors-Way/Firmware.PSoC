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

inline enum MessageID Message_GetID(const uint64_t msg) { return (enum MessageID)    ((0xFFFF000000000000 & msg) >> 48); }

inline uint64_t Message_GetData(const uint64_t data) { return (uint64_t) (0x0000FFFFFFFFFFFF & data); }


/** @}*/

#endif