/*
 * DAD_Timer.h
 *
 *  Created on: Dec 31, 2022
 *      Author: Maximilien Engel


    Description :   Asynchronous timer
                    Timer is initialized with timer length
                    Timer is then started.
                    If timer stops before timer expires, nothing happens. Program continues as if timer was never set.
                    Else, set timer expired flag high

                    Only one timer can be initialized at a time (software limitation)
 */

#ifndef DAD_TIMER_H_
#define DAD_TIMER_H_

//DriverLib Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//Standard includes
#include <stdint.h>
#include <stdbool.h>

//Timer Defines
#define DAD_DEFAULT_TIMER_TICKS    0x2DC6


//Initialize timer
void DAD_Timer_Initialize(uint16_t period_ms, uint32_t timer_base);

//Start Timer
void DAD_Timer_Start(uint32_t timer_base);

//Stop Timer
void DAD_Timer_Stop(uint32_t timer_base);

#endif /* DAD_TIMER_H_ */
