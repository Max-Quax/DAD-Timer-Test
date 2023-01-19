/*
 * DAD_Timer.c
 *
 *  Created on: Dec 31, 2022
 *      Author: Maximilien Engel
 */

#include <DAD_Timer.h>

//Timer Variables
static volatile bool DAD_timerHasExpired0 = true;
static volatile bool DAD_timerHasExpired1 = true;
static volatile bool DAD_timerHasExpired2 = true;
static volatile bool DAD_timerHasExpired3 = true;

//Initialize timer with default function
void DAD_Timer_Initialize(uint16_t period_ms, uint32_t timerBase, Timer_A_UpModeConfig *timerConfig){
    //Set config
    timerConfig->clockSource = TIMER_A_CLOCKSOURCE_ACLK,                                        // ACLK Clock Source
    timerConfig->clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_32,                           // 1024Hz =~1ms period
    timerConfig->timerPeriod = period_ms,                                                    // Period in ticks
    timerConfig->timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                    // Disable Timer interrupt
    timerConfig->captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;  // Enable CCR0 interrupt
    timerConfig->timerClear = TIMER_A_DO_CLEAR;                                                 // Clear value

    //Configure timer mode
    MAP_Timer_A_configureUpMode(timerBase, timerConfig);

    //Decide which interrupt
    uint32_t interruptNum;
    switch(timerBase){
    case TIMER_A0_BASE:
        interruptNum = INT_TA0_0;
        DAD_timerHasExpired0 = false;
        break;
    case TIMER_A1_BASE:
        interruptNum = INT_TA1_0;
        DAD_timerHasExpired1 = false;
        break;
    case TIMER_A2_BASE:
        interruptNum = INT_TA2_0;
        DAD_timerHasExpired2 = false;
        break;
    case TIMER_A3_BASE:
        interruptNum = INT_TA3_0;
        DAD_timerHasExpired3 = false;
    }

    MAP_Interrupt_enableInterrupt(interruptNum);    //Enable timer interrupt
    MAP_Interrupt_enableMaster();                   //Enable interrupts
    MAP_Interrupt_disableSleepOnIsrExit();          //Don't sleep, boi
}

//Start Timer
void DAD_Timer_Start(uint32_t timerBase){
    MAP_Timer_A_startCounter(timerBase, TIMER_A_UP_MODE);

    //Set Timer Flag
    switch(timerBase){
    case TIMER_A0_BASE:
        DAD_timerHasExpired0 = false;
        break;
    case TIMER_A1_BASE:
        DAD_timerHasExpired1 = false;
        break;
    case TIMER_A2_BASE:
        DAD_timerHasExpired2 = false;
        break;
    case TIMER_A3_BASE:
        DAD_timerHasExpired3 = false;
    }
}

bool DAD_Timer_Has_Finished(uint32_t timerBase){
    //Return Timer Flag
    switch(timerBase){
    case TIMER_A0_BASE:
        return DAD_timerHasExpired0;
    case TIMER_A1_BASE:
        return DAD_timerHasExpired1;
    case TIMER_A2_BASE:
        return DAD_timerHasExpired2;
    }
    return DAD_timerHasExpired3;
}

//Stop Timer
void DAD_Timer_Stop(uint32_t timerBase){
    //Stop timer
    MAP_Timer_A_stopTimer(timerBase);

    //Set Timer Flag
    switch(timerBase){
        case TIMER_A0_BASE:
            DAD_timerHasExpired0 = true;
            break;
        case TIMER_A1_BASE:
            DAD_timerHasExpired1 = true;
            break;
        case TIMER_A2_BASE:
            DAD_timerHasExpired2 = true;
            break;
        case TIMER_A3_BASE:
            DAD_timerHasExpired3 = true;
    }
}

//Interrupt handlers
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_stopTimer(TIMER_A0_BASE);                                           //Stop timer

    //Clear interrupts
    MAP_Timer_A_clearInterruptFlag(TIMER_A0_BASE);                                  //Clear general timer interrupt flag
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,                         //Clear capture interrupt flag
                TIMER_A_CAPTURECOMPARE_REGISTER_0);

    //Debug - toggle LED to indicate interrupt not cleared
    if(Timer_A_getInterruptStatus(TIMER_A0_BASE) == TIMER_A_INTERRUPT_PENDING)      //Tests to see that interrupt was cleared
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //If interrupt not cleared, turn on light

    //Set Timer Flag
    DAD_timerHasExpired0 = true;
}

void TA1_0_IRQHandler(void)
{
    MAP_Timer_A_stopTimer(TIMER_A1_BASE);                                           //Stop timer

    //Clear interrupt
    MAP_Timer_A_clearInterruptFlag(TIMER_A1_BASE);
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,                         //Clear capture interrupt flag
                        TIMER_A_CAPTURECOMPARE_REGISTER_0);

    //Debug - toggle LED to indicate interrupt not cleared
    if(Timer_A_getInterruptStatus(TIMER_A1_BASE) == TIMER_A_INTERRUPT_PENDING)      //Tests to see that interrupt was cleared
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //If interrupt not cleared, turn on light

    //Set Timer Flag
    DAD_timerHasExpired1 = true;
}


void TA2_0_IRQHandler(void)
{
    MAP_Timer_A_stopTimer(TIMER_A2_BASE);                                           //Stop timer

    //Clear interrupt
    MAP_Timer_A_clearInterruptFlag(TIMER_A2_BASE);
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,                         //Clear capture interrupt flag
                        TIMER_A_CAPTURECOMPARE_REGISTER_0);

    //Debug - toggle LED to indicate interrupt not cleared
    if(Timer_A_getInterruptStatus(TIMER_A2_BASE) == TIMER_A_INTERRUPT_PENDING)      //Tests to see that interrupt was cleared
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //If interrupt not cleared, turn on light

    //Set Timer Flag
    DAD_timerHasExpired2 = true;
}


void TA3_0_IRQHandler(void)
{
    MAP_Timer_A_stopTimer(TIMER_A3_BASE);                                           //Stop timer

    //Clear interrupt
    MAP_Timer_A_clearInterruptFlag(TIMER_A3_BASE);
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,                         //Clear capture interrupt flag
                    TIMER_A_CAPTURECOMPARE_REGISTER_0);

    //Debug - toggle LED to indicate interrupt not cleared
    if(Timer_A_getInterruptStatus(TIMER_A3_BASE) == TIMER_A_INTERRUPT_PENDING)      //Tests to see that interrupt was cleared
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);                    //If interrupt not cleared, turn on light

    //Set Timer Flag
    DAD_timerHasExpired3 = true;
}
