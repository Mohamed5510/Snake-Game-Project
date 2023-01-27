 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART0 of Tiva C TM4C123G
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "../headers/std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART0_init(uint32 clk, uint32 baudrate);

/*
 * Description :
 * Functional responsible for print a char.
 */
void UART0_putchar(uint8 c);

/*
 * Description :
 * Functional responsible for get a char.
 */
uint8 UART0_getchar(void);

#endif /* UART_H_ */
