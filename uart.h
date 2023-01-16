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

#include "std_types.h"

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
 * Function responsible for print a charcter on the terminal using UART.
 */
void print_char(uint8 c);

/*
 * Description :
 * Function responsible for print a string on the terminal using UART return when encountering a newline.
 */
void print_string(const uint8 *str);

#endif /* UART_H_ */
