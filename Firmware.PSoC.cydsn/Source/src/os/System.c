#include <stdlib.h>
#include <os/System.h>
#include <os/Buffer.h>
#include <os/Timer.h>
#include <os/Message.h>
#include <srv/serial/SerialHandler.h>
#include "TimerInt.h"

#include <stdio.h>
#include <stdarg.h>
#include <project.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

/**
 * \addtogroup System
 * @{
 * \addtogroup PSystem System (Private)
 * @{
 */

#define MESSAGE_QUEUE_SIZE 512

Buffer msgQueue;

struct Task* tasks = 0U;

void System_CreateHandlers(void);

void System_RunHandlers(void);

void System_InitializeHandlers(void);

void System_ZeroString(char* str, uint8_t len);

void System_Notify(void);

struct Task* System_CreateTask(void);

struct Task* System_InsertTask(struct Task* const self);

void System_OnTimer(void* vself);

struct Timer* timer;

/** @} @} */

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/


struct Task* System_CreateTask(void)
{
    struct Task* retValue = (struct Task*) malloc(sizeof(struct Task));

    retValue->id= HID_EOL;   
    retValue->initialize = 0U;
	retValue->run = 0U;
    retValue->process = 0U;
    
    return System_InsertTask(retValue);
}

void System_Initialize(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */  
    Buffer_Create(&msgQueue, 512, sizeof(uint64_t));
    Timer_Initialize();
    System_CreateHandlers();
	System_InitializeHandlers();
	timer = Timer_Create(0U, System_OnTimer);
	Timer_Start(timer, TIMER_PERIODIC, 500);
}

void System_Run(void)
{   
    Timer_Run();
    System_Notify();    
	System_RunHandlers();
}

#define MAX_STRING_LENGTH 255

void System_Printf(const char* format, ...)
{
	struct Task* task = tasks;     
	char str[MAX_STRING_LENGTH];
	System_ZeroString(str, MAX_STRING_LENGTH);
	va_list arguments;
	va_start(arguments, format);
	vsnprintf(str, MAX_STRING_LENGTH, format, arguments);
	va_end(arguments);
	
	while (task != 0U)
	{
	    if (task->id == HID_SERIAL_HANDLER)
	    {
	        SerialHandler_Printf(task->vself, str);   
	    }
	
	    task = task->next;
	}
}   

void System_DispatchMessage(const uint64_t msg)
{
    uint64_t *element = (uint64_t *) Buffer_Write(&msgQueue);

    if (element != 0U)
    {
        *element = msg;
    }
}

/******************************************************************************
*                                                                            *
*                       Private Function Implementation                      *
*                                                                            *
******************************************************************************/

void System_InitializeHandlers(void)
{    
    struct Task* task = tasks;
    
    while (task != 0U)
    {     
		if (task->initialize != 0U)
		{
			task->initialize(task->vself);
		}
		
        task = task->next;
    }
}

void System_RunHandlers(void)
{
    struct Task* task = tasks;
    
    while (task != 0U)
    {   
		if (task->run != 0)
		{
			task->run(task->vself);
		}
		
        task = task->next;
    }
}


void System_ProcessMessages(const enum MessageID  id, const uint32_t data)
{
    switch (id)
    {			
        default:
            break;
    }
}


void System_Notify(void)
{   
    while(!Buffer_IsEmpty(&msgQueue))
    {
        const uint64_t * const msg = (uint64_t *) Buffer_Read(&msgQueue);

        if (msg != 0U)
        {     
            const enum MessageID id = Message_GetID(*msg);
            const uint32_t data = Message_GetData(*msg);
            struct Task* task = tasks; 

            while (task != 0U)
            {
				if (task->process != 0U)
				{
                	task->process(task->vself, id, data);
				}
				
                task = task->next;
            }

            System_ProcessMessages(id, data);
        }
    }
}

void System_CreateHandlers(void)
{
    SerialHandler_Create(System_CreateTask());    
}

void System_ZeroString(char* str, uint8_t len)
{
	uint8_t n;

	for (n = 0; n < len; ++n)
	{
		str[n] = 0;
	}
}

struct Task* System_InsertTask(struct Task* const self)
{
    self->next = NULL;

    if (tasks != NULL)
    {
        self->next = tasks;
        tasks = self;
    }
    else
    {
        tasks = self;
    }    
    
    return self;
}

void System_OnTimer(void* vself)
{
	static uint8_t toogle = 0;
	LED01_Write(toogle);
	
	toogle = !toogle ? 0xFF : 0;
}
