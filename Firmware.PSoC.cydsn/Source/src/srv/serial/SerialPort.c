#include "SerialPort.h"
#include <srv/serial/SerialHandler.h>
#include <os/Buffer.h>
#include <project.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

enum PortState
{
	PORT_IDLE = 0,
	PORT_TRANSMIT_ZERO_PACKET
};

#define USBFS_DEVICE    (0u)

/* The buffer size is equal to the maximum packet size of the IN and OUT bulk
* endpoints.
*/
#define USBUART_BUFFER_SIZE (64u)
#define PORT_BUFFER_SIZE    (512u)

uint8_t SerialPort_Read(uint8_t* data);

void SerialPort_ReadUSB(void);

void SerialPort_WriteUSB(void);

Buffer txBuffer;
Buffer rxBuffer;
enum PortState portState;

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void SerialPort_Initialize(void)
{
	Buffer_Create(&txBuffer, PORT_BUFFER_SIZE, 1);
	Buffer_Create(&rxBuffer, PORT_BUFFER_SIZE, 1);
	
    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);   
	portState = PORT_IDLE;
}

void SerialPort_Run(void)
{
    if (0u != USBUART_IsConfigurationChanged())
    {
        /* Initialize IN endpoints when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Enumeration is done, enable OUT endpoint to receive data 
             * from host. */
            USBUART_CDC_Init();
			Buffer_Initialize(&txBuffer);
			Buffer_Initialize(&rxBuffer);
        }
    }
	
    if (0u != USBUART_GetConfiguration())
	{
		SerialPort_ReadUSB();
		SerialPort_WriteUSB();
	}
}

void SerialPort_Write(const uint8_t data)
{
	if (!Buffer_IsFull(&txBuffer))
	{
		uint8_t* ptr = (uint8_t *) Buffer_Write(&txBuffer);
		*ptr = data;
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

uint8_t SerialPort_Read(uint8_t* data)
{
    uint8_t retValue = 0;
        
	if (!Buffer_IsEmpty(&rxBuffer))
	{
        *data = *((uint8_t *) Buffer_Read(&rxBuffer));
        retValue = 1;
    }
    
    return retValue;
}


void SerialPort_ReadUSB(void)
{
	static uint8_t buffer[USBUART_BUFFER_SIZE];
	
    if (0u != USBUART_DataIsReady())
    {
        /* Read received data and re-enable OUT endpoint. */
        const uint32_t count = USBUART_GetAll(buffer);

        if (0u != count)
        {
			for (uint8_t n = 0; n < count; ++n)
			{
				if (!Buffer_IsFull(&rxBuffer))
				{
					uint8_t* ptr = (uint8_t *) Buffer_Write(&rxBuffer);
					*ptr = buffer[n];
				}
			}
		}
	}			
}

void SerialPort_WriteUSB(void)
{
	static uint8_t buffer[USBUART_BUFFER_SIZE];
	uint8_t count = 0;

	switch (portState)
	{
		case PORT_IDLE:
			if (!Buffer_IsEmpty(&txBuffer))
			{
				while (!Buffer_IsEmpty(&txBuffer) && count < USBUART_BUFFER_SIZE)
				{
					buffer[count] = *((uint8_t *) Buffer_Read(&txBuffer));
					++count;
				}
				
	            USBUART_PutData(buffer, count);

	            /* If the last sent packet is exactly the maximum packet 
	            *  size, it is followed by a zero-length packet to assure
	            *  that the end of the segment is properly identified by 
	            *  the terminal.
	            */
	            portState = USBUART_BUFFER_SIZE == count ? PORT_TRANSMIT_ZERO_PACKET : PORT_IDLE;
			}
			break;
		case PORT_TRANSMIT_ZERO_PACKET:
			if (USBUART_CDCIsReady())
			{
				/* Send zero-length packet to PC. */
				USBUART_PutData(NULL, 0u);		
				portState = PORT_IDLE;
			}
			break;
	}	
}
