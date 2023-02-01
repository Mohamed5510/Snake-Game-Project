/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART0 of Tiva C TM4C123G
 *
 *******************************************************************************/
 
 #include "uart.h"
 #include "../headers/tm4c123gh6pm.h"
 #include "../headers/common_macros.h"
 
 /*******************************************************************************
 *                      Functions Difinitions                                  *
 *******************************************************************************/
 
 /*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Enable the UART.
 * 2. Configure UART interrupts and GPIO.
 * 3. Setup the UART baud rate.
 */
void UART0_init(uint32 clk, uint32 baudrate)
{
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
	SYSCTL_RCGCUART_R |= SYSCTL_RCGCGPIO_R0;
	
	SET_BIT(SYSCTL_RCGCUART_R, SYSCTL_RCGCUART_R0);
	SET_BIT(SYSCTL_RCGCUART_R, SYSCTL_RCGCGPIO_R0);

	CLEAR_BIT(UART0_CTL_R, UART_CTL_UARTEN);
        // BRD = 16000000 / (16 * 9600) = 104.167
	UART0_IBRD_R = 104;     // integer(BRD)
	UART0_FBRD_R = 11;      // (BRD - integer(BRD)) * 64 + 0.5

	// Interrupts Configuration
	SET_BIT(UART0_ICR_R, 4);
	SET_BIT(UART0_IM_R, 4);
	
	UART0_LCRH_R |= (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	UART0_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

    // GPIO Configuration
	UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
    GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
    GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
    GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA

    // Configure PA1-0 Alternate function as UART
    GPIO_PORTA_PCTL_R |= (1 << GPIO_PCTL_PA0_U0RX) | (1 << GPIO_PCTL_PA1_U0TX);
}

/*
 * Description :
 * Functional responsible for print a char.
 */
void UART0_putchar(uint8 c)
{
	while((UART0_FR_R & UART_FR_TXFF)==UART_FR_TXFF){};
	UART0_DR_R = c;
}

/*
 * Description :
 * Functional responsible for get a char.
 */
uint8 UART0_getchar(void)
{
	while((UART0_FR_R & 0x0010) != 0 );     // wait until RXFE is 0
        return((char)(UART0_DR_R));
}