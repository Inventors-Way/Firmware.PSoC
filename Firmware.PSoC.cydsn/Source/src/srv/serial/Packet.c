#include "Packet.h"
#include <string.h>
#include "SerialPort.h"
#include <srv/serial/SerialHandler.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

/** 
 * \addtogroup Packet
 * @{
 * \addtogroup PPacket Packet (Private)
 * @{
 */
#define DLE 0xFF
#define STX 0xF1
#define ETX 0xF2

#define WAITING_FOR_DLE   0x01
#define WAITING_FOR_STX   0x02
#define RECEIVING_DATA    0x03
#define WAITING_FOR_ETX   0x04

/**
 * \brief Handle waiting for the data length escape
 * A more detailed description goes here.
 * 
 * \param[in] self reference to self
 * \param[in] data the data to parse
 */
void Packet_HandleWaitingForDLE(Packet* self, uint8_t data);

void Packet_HandleWaitingForSTX(Packet* self, uint8_t data);
void Packet_HandleReceivingData(Packet* self, uint8_t data);
uint8_t Packet_HandleWaitingForETX(Packet* self, uint8_t data);
void Packet_HandleAddData(Packet* self, uint8_t data);

void Packet_SendDataByte(uint8_t data);

#define PACKET_BUFFER_SIZE 256
uint8_t buffer[PACKET_BUFFER_SIZE];

/** @} @} */

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void Packet_Initialize(Packet* self)
{
    self->count = 0;
    self->state = WAITING_FOR_DLE;
}

uint8_t Packet_Add(Packet* self, const uint8_t data)
{
	uint8_t retValue = 0;
	
	switch (self->state)
	{
		case WAITING_FOR_DLE:
		Packet_HandleWaitingForDLE(self, data);
		break;

		case WAITING_FOR_STX:
		Packet_HandleWaitingForSTX(self, data);
		break;

		case RECEIVING_DATA:
		Packet_HandleReceivingData(self, data);
		break;
		case WAITING_FOR_ETX:
		retValue = Packet_HandleWaitingForETX(self, data);
		break;
	}
	
	return retValue;
}

void Packet_CreateResponse(Packet* self, const Packet* request, const uint8_t length)
{
	self->code = request->code;
	self->length = length;
}

void Packet_CreateMessage(Packet* self, const uint8_t code, const uint8_t length)
{
	self->code = code;
	self->length = length;
}

void Packet_Send(Packet* self)
{
	uint32_t length = 0;
	uint8_t i;
	uint8_t data;
	
	buffer[length] = DLE; ++length;
	buffer[length] = STX; ++length;

	if (self->code == DLE)
	{
		buffer[length] = DLE; ++length;
		buffer[length] = DLE; ++length;
	}
	else
	{
		buffer[length] = self->code; ++length;
	}

	if (self->length == DLE)
	{
		buffer[length] = DLE; ++length;
		buffer[length] = DLE; ++length;
	}
	else
	{
		buffer[length] = self->length; ++length;
	}

	for (i = 0; i < self->length; ++i)
	{
		data = self->data[i];

		if (data == DLE)
		{
			buffer[length] = DLE; ++length;
			buffer[length] = DLE; ++length;
		}
		else
		{
			buffer[length] = data; ++length;
		}
	}

	buffer[length] = DLE; ++length;
	buffer[length] = ETX; ++length;

	SerialPort_WriteArray(buffer, length);
}

void Packet_SendNotAcknowledge(const uint8_t errorCode)
{
	SerialPort_Write(DLE);
	SerialPort_Write(STX);
	
	Packet_SendDataByte(0U);
	Packet_SendDataByte(1U);
    Packet_SendDataByte(errorCode);
	
	SerialPort_Write(DLE);
	SerialPort_Write(ETX);
}


/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

void Packet_SendDataByte(uint8_t data)
{
	if (data == DLE)
	{
		SerialPort_Write(DLE);
		SerialPort_Write(DLE);
	}
	else
	{
		SerialPort_Write(data);
	}
}

void Packet_HandleWaitingForDLE(Packet* self, uint8_t data)
{
	if (data == DLE)
	{
		self->state = WAITING_FOR_STX;
	}
}

void Packet_HandleWaitingForSTX(Packet* self, uint8_t data)
{
	if (data == STX)
	{
		self->state = RECEIVING_DATA;
		self->count = 0;
	}
	else if (data != DLE)
	{
		self->state = WAITING_FOR_DLE;
	}
}

void Packet_HandleReceivingData(Packet* self, uint8_t data)
{
	if (data != DLE)
	{
		Packet_HandleAddData(self, data);
	}
	else
	{
		self->state = WAITING_FOR_ETX;
	}
}

uint8_t Packet_HandleWaitingForETX(Packet* self, uint8_t data)
{
	uint8_t retValue = 0U;
	
	if (data == DLE)
	{
		Packet_HandleAddData(self, DLE);
		self->state = RECEIVING_DATA;
	}
	else if (data == ETX)
	{
		self->state = WAITING_FOR_DLE;
		retValue = 1U;
	}
	else if (data == STX)
	{
		self->state = RECEIVING_DATA;
		self->count = 0;
	}
	else
	{
		self->state = WAITING_FOR_DLE;
	}
	
	return retValue;
}

void Packet_HandleAddData(Packet* self, uint8_t data)
{
	if (self->count == 0)
	{
		self->code = data;
	}
	else if (self->count == 1)
	{
		self->length = data;
	}
	else if (self->count < self->length + 2)
	{
		self->data[self->count - 2] = data;
	}

	++self->count;
}
