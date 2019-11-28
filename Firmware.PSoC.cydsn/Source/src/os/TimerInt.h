#ifndef TIMER_INT_H
#define TIMER_INT_H

/**
 * \brief Initialize the timer system 
 * This function initializes Timer 1 for use with the general purpose
 * timer system. This allows Timer objects to be constructed (Timer_Construct)
 * and used to implement timer with a resolution of 1ms.
 *
 * \return void
 */
void Timer_Initialize(void);

/*
 * \brief Updates the current ticks
 * This function shall be called once and only once in the main loop of the program
 * in order for the timer system to work correctly.                                                                  
 */
void Timer_Run(void);

#endif

