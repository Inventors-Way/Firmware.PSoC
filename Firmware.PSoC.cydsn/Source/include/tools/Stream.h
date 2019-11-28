#ifndef STREAM_H_
#define STREAM_H_
   
/** 
 * \addtogroup Stream
 * @{
 */

   
#include <stdint.h>

void Stream_InsertUint8(uint8_t * const self, uint32_t pos, uint8_t data);
void Stream_InsertInt8(uint8_t * const self, uint32_t pos, int8_t data);
void Stream_InsertUint16(uint8_t * const self, uint32_t pos, uint16_t data);
void Stream_InsertInt16(uint8_t * const self, uint32_t pos, int16_t data);
void Stream_InsertUint32(uint8_t * const self, uint32_t pos, uint32_t data);
void Stream_InsertInt32(uint8_t * const self, uint32_t pos, int32_t data);
void Stream_InsertString(uint8_t * const self, uint32_t pos, int8_t size, char* str);

uint8_t  Stream_GetUint8(const uint8_t * const self, uint32_t pos);
int8_t   Stream_GetInt8(const uint8_t * const self, uint32_t pos);
uint16_t Stream_GetUint16(const uint8_t * const self, uint32_t pos);
int16_t  Stream_GetInt16(const uint8_t * const self, uint32_t pos);
uint32_t Stream_GetUint32(const uint8_t * const self, uint32_t pos);
int32_t  Stream_GetInt32(const uint8_t * const self, uint32_t pos);

/** @} */

#endif 