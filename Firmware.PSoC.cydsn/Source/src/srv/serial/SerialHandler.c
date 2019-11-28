#include <srv/serial/SerialHandler.h>
#include "Packet.h"
#include <os/System.h>
#include <stdlib.h>
#include <os/Timer.h>
#include "SerialPort.h"
#include <project.h>
#include <string.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/


enum CommError
{
   COMM_ERR_UNKNOWN_FUNCTION = 0x01,
};

struct SerialHandler
{
   uint8_t cableCount;

   Packet mResponse;
   Packet mRequest;

   struct Timer* functionTimer;
};

void SerialHandler_Initialize(void* vself);
void SerialHandler_Run(void* vself );
void SerialHandler_ProcessMessages(void* vself, const enum MessageID  id, const uint32_t data);
void SerialHandler_DeviceIdentification(struct SerialHandler* self);
void SerialHandler_Ping(struct SerialHandler* self);
void SerialHandler_GetEndianness(struct SerialHandler* self);

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void SerialHandler_Create(struct Task* task)
{
   struct SerialHandler* self = (struct SerialHandler *) malloc(sizeof(struct SerialHandler));  
   self->functionTimer = Timer_Create(self, SerialHandler_Run);

   task->vself = self;
   task->id = HID_SERIAL_HANDLER;
   task->initialize = SerialHandler_Initialize;
   task->process = SerialHandler_ProcessMessages;
}

void SerialHandler_Initialize(void* vself)
{
   struct SerialHandler* self = (struct SerialHandler*) vself;
   
   Packet_Initialize(&self->mRequest);
   Packet_Initialize(&self->mResponse);   
   Timer_Start(self->functionTimer, TIMER_PERIODIC, 20);

   //UART_Start();
}

void SerialHandler_Printf(void* vself, char* str)
{	
    struct SerialHandler* self = (struct SerialHandler*) vself;	
	const uint8_t length = strlen(str);
	Packet message;

	Packet_Initialize(&message);
	Packet_CreateMessage(&message, PRINTF_MSG, length);

	for (uint8_t n = 0; n < length; ++n)
	{
		Packet_InsertUint8(&message, n, str[n]);
	}
				
	Packet_Send(&message);   
}   

void SerialHandler_Run(void* vself)
{
    struct SerialHandler* self = (struct SerialHandler*) vself;	
	
	while (SerialPort_IsRequestAvailable(&self->mRequest))
	{
	  switch (self->mRequest.code)
	  {
		 case NACK:
			break;
		 case DEVICE_IDENTIFICATION:
			SerialHandler_DeviceIdentification(self);
			break;
		 case SYSTEM_PING:
			SerialHandler_Ping(self);
			break;
		 case GET_ENDIANNESS:
			SerialHandler_GetEndianness(self);
			break;
		 default:
			Packet_SendNotAcknowledge(COMM_ERR_UNKNOWN_FUNCTION);
			break;
	  }    
	
	  Packet_Initialize(&self->mRequest);        
	}        
}

void SerialHandler_ProcessMessages(void* vself, const enum MessageID  id, const uint32_t data)
{
	struct SerialHandler* self = (struct SerialHandler*) vself;	
}

/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

void SerialHandler_DeviceIdentification(struct SerialHandler* self)
{
   Packet_CreateResponse(&self->mResponse, &self->mRequest, 64U);
   Packet_InsertUint32(&self->mResponse, 0, MANUFACTURER_ID);
   Packet_InsertUint16(&self->mResponse, 4, DEVICE_TYPE);        // Device Type
   Packet_InsertUint32(&self->mResponse, 6, 0); // Serial number
   Packet_InsertUint8(&self->mResponse, 10, MAJOR_REVISION);     // Major Revision
   Packet_InsertUint8(&self->mResponse, 11, MINOR_REVISION);     
   Packet_InsertUint8(&self->mResponse, 12, PATCH_REVISION);     
   Packet_InsertUint8(&self->mResponse, 13, ENGINEERING_REVISION);     
   Packet_InsertUint16(&self->mResponse, 14, 0xAA00);    // Program checksum
   Packet_InsertString(&self->mResponse, 16, 24, MANUFACTORER_NAME);
   Packet_InsertString(&self->mResponse, 40, 24, DEVICE_NAME);     
   Packet_Send(&self->mResponse);
}

void SerialHandler_Ping(struct SerialHandler* self)
{
   static uint32_t pingCount = 0;

   Packet_CreateResponse(&self->mResponse, &self->mRequest, 4U);
   Packet_InsertUint32(&self->mResponse, 0, pingCount);   
   Packet_Send(&self->mResponse);
   ++pingCount;
}

void SerialHandler_GetEndianness(struct SerialHandler* self)
{
	Packet_CreateResponse(&self->mResponse, &self->mRequest, 2U);
	Packet_InsertUint16(&self->mResponse, 0, 1);
	Packet_Send(&self->mResponse);
}
