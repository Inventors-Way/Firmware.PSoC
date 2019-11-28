#ifndef __SERVICES_OS_TIMER_H
#define __SERVICES_OS_TIMER_H

#include <stdint.h>

/** \addtogroup Timer
 *  @{
 */

enum TimerType
{
    TIMER_PERIODIC = 0, ///< The timer elapses periodically
    TIMER_ONESHOT       ///< The timer elapses only once, after which it is stopped and must be started again
};

struct Timer;

/**
 * \brief Create a timer
 * This functions creates a new timer and returns a pointer to the newly created
 * timer object. 
 * 
 * \param[in] owner the owner of the timer
 * \param[in] func the function that should be called on the owner when the timer elapsed.
 * \return a pointer to the created timer object
 */
struct Timer* Timer_Create(void* owner, void (*func)(void* owner));

/**
 * \brief Start a timer
 * This function starts the timer, if the timer is allready started this will stop
 * the current run of the timer.
 * 
 * \param[in] self a pointer to the timer that should be started
 * \param[in] type the type of the timer (TIMER_PERIODIC or TIMER_ONESHOT)
 * \param[in] period the period of the timer.
 */
void Timer_Start(struct Timer* self, enum TimerType type, uint32_t period);

/**
 * \brief Stops a currently running timer
 * This stops a currently running timer. If the timer is allready stopped this
 * function has no effect.
 * 
 * \param[in] self a pointer to the timer that should be stopped
 */
void Timer_Stop(struct Timer* self);

/** @}*/

#endif
