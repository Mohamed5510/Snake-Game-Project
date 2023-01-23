 /******************************************************************************
 *
 * Module: SysTick Timer
 *
 * File Name: systick.h
 *
 * Description: header file for TM4C123GH6PM Microcontroller - SysTick Timer Driver.
 *
 ******************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include "../headers/Std_Types.h"

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: SysTick_Start
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Tick_Time - Time in miliseconds
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer configuration to count n miliseconds:
*              - Set the Reload value
*              - Enable SysTick Timer with System clock 16Mhz
*              - Enable SysTick Timer Interrupt and set its priority
************************************************************************************/	
void SysTick_Start(uint16 Tick_Time);

/************************************************************************************
* Service Name: SysTick_Stop
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Stop the SysTick Timer.
************************************************************************************/
void SysTick_Stop(void);

#endif /* SYSTICK_H */