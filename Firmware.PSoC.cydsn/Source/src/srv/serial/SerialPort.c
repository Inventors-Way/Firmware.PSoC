#include "SerialPort.h"
#include <srv/serial/SerialHandler.h>
#include <project.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/


uint8_t SerialPort_Read(uint8_t* data);

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void SerialPort_WriteArray(const uint8_t * const data, const uint32_t length)
{
    switch (port)
    {
        case PORT_DEBUG:
            for (uint32_t n = 0; n < length; ++n)
            {
                UART_UartPutChar(data[n]);
            }
            break;
        default:
            break;
    }
}

void SerialPort_Write(const uint8_t data)
{
    switch (port)
    {
        case PORT_DEBUG:
            UART_UartPutChar(data);
            break;
        default:
            break;
    }
}

uint8_t SerialPort_IsRequestAvailable(Packet* packet)
{
   uint8_t retValue = 0;
   uint8_t data;
   
   while (SerialPort_Read(&data) && !retValue)
   {
      retValue = Packet_Add(packet, data);
   }
   
   return retValue;
}


/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

uint32_t SerialPort_ReadData(const enum Port port)
{
    uint32_t retValue = UART_UART_RX_UNDERFLOW;
    
    switch (port)
    {
        case PORT_DEBUG:
            retValue = UART_UartGetByte();  
            break;
        default:
            break;
    }
    
    return retValue;
}

uint8_t SerialPort_Read(uint8_t* data)
{
    const uint32_t input = SerialPort_ReadData(port);
    uint8_t retValue = 0;
        
    if (!SerialPort_RxFramingError(input) && 
        !SerialPort_RxParityError(input) &&
        !SerialPort_RxOverflow(input) &&
        !SerialPort_RxUnderflow(input))
    {
        retValue = 1;
        *data = (uint8_t) (0x000000FF & input);
    }
    
    return retValue;
}
