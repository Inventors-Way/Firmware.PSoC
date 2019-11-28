#include "SerialPort.h"
#include <srv/serial/SerialHandler.h>
#include <os/Buffer.h>
#include <project.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

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
    
    for(;;)
    {
        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data 
                 * from host. */
                USBUART_CDC_Init();
            }
        }
	}	
}

void SerialPort_Run(void)
{
	SerialPort_ReadUSB();
	SerialPort_WriteUSB();
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
	uint8_t buffer[USBUART_BUFFER_SIZE];
	
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
	
}
