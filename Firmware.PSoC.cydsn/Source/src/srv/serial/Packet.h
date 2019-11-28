#ifndef PACKET_H_
#define PACKET_H_

#include <os/System.h>
#include <tools/Stream.h>

/** 
 * \addtogroup comm
 * @{
 * \addtogroup pcomm Private Classes
 * @{
 * \addtogroup Packet
 *  @{
 */

   
#include <stdint.h>
#include <srv/serial/SerialHandler.h>    

#define MAX_PACKET_SIZE 196

typedef struct
{
   uint8_t code;
   uint8_t length;
   uint8_t data[MAX_PACKET_SIZE];
   
   // Private variables
   uint8_t state;
   uint8_t count;
} Packet; 

void Packet_Initialize(Packet* self);

/**
 * \brief Add a newly received byte to the packet
 * This function adds a newly received byte to the packet. This involves
 * destuffing and inserting it into the correct place in the packet.
 *
 * \param[in] self a reference to self
 * \param[in] data the received byte from the master to process
 * \return 1 if a packet has been received, otherwise 0
 */
uint8_t Packet_Add(Packet* self, const uint8_t data);

void Packet_CreateResponse(Packet* self, const Packet* request, const uint8_t length);

void Packet_CreateMessage(Packet* self, const uint8_t code, const uint8_t length);

void Packet_InsertUint8(Packet* self, uint8_t pos, uint8_t data); 
void Packet_InsertInt8(Packet* self, uint8_t pos, int8_t data); 
void Packet_InsertUint16(Packet* self, uint8_t pos, uint16_t data);
void Packet_InsertInt16(Packet* self, uint8_t pos, int16_t data);
void Packet_InsertUint32(Packet* self, uint8_t pos, uint32_t data);
void Packet_InsertInt32(Packet* self, uint8_t pos, int32_t data);
void Packet_InsertString(Packet* self, uint8_t pos, int8_t size, char* str);

uint8_t Packet_GetUint8(Packet* self, uint8_t pos);
int8_t Packet_GetInt8(Packet* self, uint8_t pos);
uint16_t Packet_GetUint16(Packet* self, uint8_t pos);
int16_t Packet_GetInt16(Packet* self, uint8_t pos);
uint32_t Packet_GetUint32(Packet* self, uint8_t pos);
int32_t Packet_GetInt32(Packet* self, uint8_t pos);

void Packet_Send(Packet* self);

void Packet_SendNotAcknowledge(const uint8_t errorCode);

/** @} */
/** @} */
/** @} */

#endif /* PACKET_H_ */