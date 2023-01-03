/*
 * DAD_Timer.c
 *
 *  Created on: Dec 31, 2022
 *      Author: Maximilien Engel
 */

#include <DAD_Timer.h>


//Timer Variables
volatile bool DAD_timerHasExpired = true;


Timer_A_UpModeConfig DAD_Timer_Config =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64,         // SMCLK/1 = 3MHz
        DAD_DEFAULT_TIMER_TICKS,                // 5000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};


//Initialize timer with default function
void DAD_Timer_Initialize(uint16_t period_ms){
    //Set timer period
    DAD_Timer_Config.timerPeriod = period_ms*10;

    //Configure timer mode
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &DAD_Timer_Config);


    //Enable interrupts
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_Interrupt_enableInterrupt(INT_TA0_0);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
}

//Start Timer
void DAD_Timer_Start(){
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    //Set Timer Flag
    DAD_timerHasExpired = false;
}

//Stop Timer
void DAD_Timer_Stop(){
    //Stop timer
    Timer_A_stopTimer(TIMER_A0_BASE);

    //Set Timer Flag
    DAD_timerHasExpired = true;
}

//Interrupt handle
void TA0_0_IRQHandler(void)
{
    //Debug - toggle LED
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);

    //Set Timer Flag
    DAD_timerHasExpired = true;
}
