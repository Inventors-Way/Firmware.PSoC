#include <os/Timer.h>
#include "TimerInt.h"
#include <stdlib.h>
#include <project.h>
#include <tools/Stream.h>

/******************************************************************************
*                                                                            *
*                       Private Function Declaration                         *
*                                                                            *
******************************************************************************/

uint32_t timerCount;
struct TimerItem;

struct Timer
{
   uint32_t period;
   enum TimerType  type;   
   uint8_t  running;
   uint32_t startTime;      
    
   void* owner;
   void (*func)(void* self);
};

struct TimerItem
{    
    struct TimerItem* next;
    struct Timer* timer;
};

struct TimerItem* timers;
uint32_t currentTimerTicks = 0;

uint32_t Timer_GetTicks(void);

uint32_t Timer_GetElapsedTime(const uint32_t time);

uint8_t Timer_Check(struct Timer* const self);

void Timer_Add(struct Timer* const self);

void SysTickISRCallback(void);

/******************************************************************************
*                                                                            *
*                       Public Function Implementation                       *
*                                                                            *
******************************************************************************/

void Timer_Initialize(void)
{
    timers = NULL;
    timerCount = 0;
    uint8 i;
       
    CySysTickStart();
    for (i = 0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; ++i)
    {
        if (CySysTickGetCallback(i) == NULL)
        {
            CySysTickSetCallback(i, SysTickISRCallback);
            break;
        }
    }
}


void Timer_Run(void)
{
    struct TimerItem* iterator = timers;
    currentTimerTicks = Timer_GetTicks();

    while (iterator != NULL)
    {
        if (Timer_Check(iterator->timer))
        {
            iterator->timer->func(iterator->timer->owner);
        }
        
        iterator = iterator->next;
    }
}

uint8_t Timer_Check(struct Timer* const self)
{
   uint8_t retValue = 0;
   
   if (self->running)
   {
      if (Timer_GetElapsedTime(self->startTime) > self->period)
      {
         retValue = 1U;
         
         if (self->type == TIMER_PERIODIC)
            self->startTime += self->period;
         else
            Timer_Stop(self);
      }
   }
   
   return retValue;
}

struct Timer* Timer_Create(void* owner, void (*func)(void* owner))
{
    struct Timer* retValue = (struct Timer*) malloc(sizeof(struct Timer));
    retValue->type = TIMER_ONESHOT;
    retValue->period = 0;
    retValue->running = 0;
    retValue->startTime = 0;
    retValue->owner = owner;
    retValue->func = func;
    Timer_Add(retValue);
    
    return retValue;
}

void Timer_Start(struct Timer* self, enum TimerType type, uint32_t period)
{
    self->type = type;
    self->period = period;
    self->running = 1;
    self->startTime = Timer_GetTicks();    
}

void Timer_Stop(struct Timer* self)
{
   self->running = 0U;
   self->startTime = 0U;       
}

/******************************************************************************
 *                                                                            *
 *                       Private Function Implementation                      *
 *                                                                            *
 ******************************************************************************/

void SysTickISRCallback(void)
{
    ++timerCount;
}

uint32_t Timer_GetElapsedTime(const uint32_t time)
{
   return time <= currentTimerTicks ? currentTimerTicks - time : (UINT32_MAX - time) + currentTimerTicks;
}

uint32_t Timer_GetTicks(void)
{
    uint32_t retValue = 0;
    const uint8_t state = CyEnterCriticalSection();
    retValue = timerCount;
    CyExitCriticalSection(state);
    
    return retValue;
}

void Timer_Add(struct Timer* const self)
{
    struct TimerItem* item = (struct TimerItem*) malloc(sizeof(struct TimerItem));
    item->timer = self;
    item->next = NULL;

    if (timers != NULL)
    {
        item->next = timers;
        timers = item;
    }
    else
    {
        timers = item;
    }    
}
