#ifndef __SERVICES_COMMUNICATION_COMMUNICATION_HANDLER_H
#define __SERVICES_COMMUNICATION_COMMUNICATION_HANDLER_H    

#include <stdint.h>    
#include <os/System.h>
    
/** \addtogroup SerialHandler 
 *  @{
 */
#define NACK                     0x00
#define DEVICE_IDENTIFICATION    0x01
#define SYSTEM_PING              0x02
#define GET_ENDIANNESS           0x03

#define PRINTF_MSG               0xFF

struct Task;
    
void SerialHandler_Create(struct Task* task);

void SerialHandler_Printf(void* vself, char* str);

/** @}*/

#endif    