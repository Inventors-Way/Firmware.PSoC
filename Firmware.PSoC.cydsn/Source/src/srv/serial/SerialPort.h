#ifndef __SERVICES_COMMUNICATION_SERIALPORT
#define __SERVICES_COMMUNICATION_SERIALPORT    

/** 
 * \addtogroup comm
 * @{
 * \addtogroup pcomm Private Classes
 * @{
 * \addtogroup SerialPort
 *  @{
 */

#include <stdint.h>
#include "Packet.h"    
#include <os/System.h>    
       
void SerialPort_Write(const uint8_t data);

void SerialPort_WriteArray(const uint8_t * const data, const uint32_t length);

uint8_t SerialPort_IsRequestAvailable(Packet* self);

/** @} */
/** @} */
/** @} */

#endif
