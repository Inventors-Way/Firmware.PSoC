#include <os/Buffer.h>
#include <stdlib.h>

/*****************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

/**
 * \addtogroup Buffer
 * @{
 * \addtogroup PBuffer Buffer (Private)
 * @{
 */



/** @} @} */

/*****************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void Buffer_Create(Buffer* const self, const uint32_t size, const uint32_t elementSize)
{
   self->size = size;
   self->elementSize = elementSize;
   self->start = 0U;
   self->end = 0U;
   self->count = 0U;
   self->data = malloc(size*elementSize);
}

void Buffer_Initialize(Buffer* const self)
{
   self->start = 0U;
   self->end = 0U;
   self->count = 0U;
}

uint8_t Buffer_IsEmpty(const Buffer* const self)
{
   return self->count == 0;
}

uint8_t Buffer_IsFull(const Buffer* const self)
{
   return self->count == self->size;
}

void* Buffer_Write(Buffer* const self)
{
    void* retValue = 0U;

   if (!Buffer_IsFull(self))
   {
      retValue = (void*) (&((uint8_t *)self->data)[self->end *  self->elementSize]);
      self->end = (self->end + 1) % self->size;
      ++self->count;
   }

   return retValue;
}

void* Buffer_Read(Buffer* const self)
{
   void* retValue = 0U;
   
   if (!Buffer_IsEmpty(self))
   {
      retValue = (void*) (&((uint8_t *)self->data)[self->start *  self->elementSize]);
      self->start = (self->start + 1) % self->size;
      --self->count;
   }
   
   return retValue;
}

/*****************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/