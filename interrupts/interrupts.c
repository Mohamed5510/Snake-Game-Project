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
 #include "../game/game.h"

void UART0_Handler(void)
{
	uint8 rx_data = 0;
	SET_BIT(UART0_ICR_R, 4);	// clear RXIC bit
	rx_data = UART0_DR_R;		// get the received data byte
	if(rx_data == 'w' || rx_data == 'W' ||\
	   rx_data == 'a' || rx_data == 'A' ||\
	   rx_data == 's' || rx_data == 'S' ||\
	   rx_data == 'd' || rx_data == 'D')
	{
		//move snake(rx_data);
	}
}