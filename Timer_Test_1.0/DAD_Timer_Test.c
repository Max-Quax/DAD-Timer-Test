/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// HAL includes
#include <DAD_Timer.h>
#include <DAD_SW_Timer.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// Test config
    // SIMPLE_TIMER_TEST
    // SW_TIMER_TEST
#define SW_TIMER_TEST

int main(void)
{
    /* Stop WDT  */
    MAP_WDT_A_holdTimer();

    #ifdef SW_TIMER_TEST

    int currentTimeMS = 0;
    // Initialize software timer.
    DAD_SW_Timer_initHardware();

    // Wait 12 seconds, then turn on LED
    while(currentTimeMS <= 120000){
        currentTimeMS = DAD_SW_Timer_getMS();       // Get ms since starting (max of 32 bits)
    }
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // Test Finished
    while(true);


    #endif

    #ifdef SIMPLE_TIMER_TEST

    // Timer start
    Timer_A_UpModeConfig config;
    DAD_Timer_Initialize_ms(1000, TIMER_A3_BASE, &config);
    DAD_Timer_Initialize_ms(1000, TIMER_A2_BASE, &config);
    DAD_Timer_Start(TIMER_A3_BASE);

    // Test timer restart
//    while(){                                           // Forever loop
//        while(!DAD_Timer_Has_Finished(TIMER_A3_BASE)); // Wait until timer expires
//        DAD_Timer_Restart(TIMER_A3_BASE, &config);
//        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
//    }
    //
//
    //DAD_Timer_Stop(TIMER_A0_BASE);
    int freq = CS_getACLK() / 32;
    int x = 0;
    while(true){
        if(DAD_Timer_Has_Finished(TIMER_A3_BASE) && x < 5){
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            DAD_Timer_Start(TIMER_A3_BASE);
            x++;
        }
        else if(x >= 5){
            DAD_Timer_Stop(TIMER_A3_BASE, &config);
        }
    }
    #endif
}
