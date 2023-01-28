/******************************************************************************
 *
 * Module: Interrupts
 *
 * File Name: interrupts.c
 *
 * Description: Source file for the interrupt serive routines
 *
 *******************************************************************************/
#include "../headers/tm4c123gh6pm.h"
 #include "../headers/std_types.h"
 #include "../headers/common_macros.h"

uint8 nextDir;
void UART0_Handler(void)
{
	SET_BIT(UART0_ICR_R, 4);	// clear RXIC bit
	nextDir = UART0_DR_R;		// get the received data byte
}