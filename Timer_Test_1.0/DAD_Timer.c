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
void DAD_Timer_Initialize(uint16_t period_ms, uint32_t timer_base){
    //Set timer period
    DAD_Timer_Config.timerPeriod = period_ms*10;

    //Configure timer mode
    MAP_Timer_A_configureUpMode(timer_base, &DAD_Timer_Config);

    //Decide which interrupt
    uint32_t interruptNum;
    switch(timer_base){
    case TIMER_A0_BASE:
        interruptNum = INT_TA0_0;
        break;
    case TIMER_A1_BASE:
        interruptNum = INT_TA1_0;
        break;
    case TIMER_A2_BASE:
        interruptNum = INT_TA2_0;
        break;
    case TIMER_A3_BASE:
        interruptNum = INT_TA3_0;
    }
    MAP_Interrupt_enableInterrupt(interruptNum);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();
}

//Start Timer
void DAD_Timer_Start(uint32_t timer_base){
    MAP_Timer_A_startCounter(timer_base, TIMER_A_UP_MODE);

    //Set Timer Flag
    DAD_timerHasExpired = false;
}

//Stop Timer
void DAD_Timer_Stop(uint32_t timer_base){
    //Stop timer
    Timer_A_stopTimer(timer_base);

    //Set Timer Flag
    DAD_timerHasExpired = true;
}

//Interrupt handle
void TA0_0_IRQHandler(void)
{
    //Debug - toggle LED
    //MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_0);
    //Timer_A_clearInterruptFlag(TIMER_A0_BASE);                                //Trying to clear general interrupt flag
    if(Timer_A_getInterruptStatus(TIMER_A0_BASE) == TIMER_A_INTERRUPT_PENDING)  //Tests to see that interrupt was cleared
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //If interrupt not cleared, turn on light

    //Set Timer Flag
    DAD_timerHasExpired = true;
}
