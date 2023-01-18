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

                    Only 4 timers can be initialized at a time
                    Timer runs at 1024Hz, which is approx 1ms period.
                    Max period of about 65535ms
                    If more precision is needed, use a DCO
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
void DAD_Timer_Initialize(uint16_t period_ms, uint32_t timerBase, Timer_A_UpModeConfig *timerConfig);

//Start Timer
void DAD_Timer_Start(uint32_t timerBase);

bool DAD_Timer_Has_Finished(uint32_t timerBase);

//Stop Timer
void DAD_Timer_Stop(uint32_t timerBase);

#endif /* DAD_TIMER_H_ */
