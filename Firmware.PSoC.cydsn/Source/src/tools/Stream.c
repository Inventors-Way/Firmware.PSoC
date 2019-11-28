#include <tools/Stream.h>
#include <string.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void Stream_InsertUint8(uint8_t * const self, uint32_t pos, uint8_t data)
{
	self[pos] = data;
}

void Stream_InsertInt8(uint8_t * const self, uint32_t pos, int8_t data)
{
	self[pos] = *((uint8_t*) &data);
}

void Stream_InsertUint16(uint8_t * const self, uint32_t pos, uint16_t data)
{
	self[pos]   = (uint8_t) (data & 0x00FF);
	self[pos+1] = (uint8_t) ((data & 0xFF00) >> 8);
}

void Stream_InsertInt16(uint8_t * const self, uint32_t pos, int16_t data)
{
	uint16_t tmp = *((uint16_t*) &data);
	
	self[pos]   = (uint8_t)  (tmp & 0x00FF);
	self[pos+1] = (uint8_t) ((tmp & 0xFF00) >> 8);
}

void Stream_InsertUint32(uint8_t * const self, uint32_t pos, uint32_t data)
{
	self[pos]   = (uint8_t)  (data & 0x000000FF);
	self[pos+1] = (uint8_t) ((data & 0x0000FF00) >> 8);
	self[pos+2] = (uint8_t) ((data & 0x00FF0000) >> 16);
	self[pos+3] = (uint8_t) ((data & 0xFF000000) >> 24);
}

void Stream_InsertInt32(uint8_t * const self, uint32_t pos, int32_t data)
{
	uint32_t tmp = *((uint32_t*) &data);
	
	self[pos]   = (uint8_t)  (tmp & 0x000000FF);
	self[pos+1] = (uint8_t) ((tmp & 0x0000FF00) >> 8);
	self[pos+2] = (uint8_t) ((tmp & 0x00FF0000) >> 16);
	self[pos+3] = (uint8_t) ((tmp & 0xFF000000) >> 24);
}

void Stream_InsertString(uint8_t * const self, uint32_t pos, int8_t size, char* str)
{
	uint8_t len = strlen(str);
	
	for (uint8_t n = 0; n < size; ++n)
	{
		self[n + pos] = n < len ? str[n] : 0;	
	}
}


uint8_t  Stream_GetUint8(const uint8_t * const self, uint32_t pos)
{
	return self[pos];
}

int8_t   Stream_GetInt8(const uint8_t * const self, uint32_t pos)
{
	return *((int8_t *) &self[pos]);
}

uint16_t Stream_GetUint16(const uint8_t * const self, uint32_t pos)
{
    return ((uint16_t) self[pos]) + 
           (((uint16_t) self[pos+1]) << 8);
}

int16_t  Stream_GetInt16(const uint8_t * const self, uint32_t pos)
{
    uint16_t tmp = ((uint16_t) self[pos]) + 
                   (((uint16_t) self[pos+1]) << 8);
                
	return *((int16_t *) &tmp);
}

uint32_t Stream_GetUint32(const uint8_t * const self, uint32_t pos)
{
    return ((uint16_t) self[pos]) + 
           (((uint16_t) self[pos+1]) << 8) +
           (((uint16_t) self[pos+2]) << 16) +
           (((uint16_t) self[pos+3]) << 24);
}

int32_t  Stream_GetInt32(const uint8_t * const self, uint32_t pos)
{
    uint32_t tmp = ((uint16_t) self[pos]) + 
                   (((uint16_t) self[pos+1]) << 8) +
                   (((uint16_t) self[pos+2]) << 16) +
                   (((uint16_t) self[pos+3]) << 24);

    return *((int32_t *) &tmp);
}

/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

