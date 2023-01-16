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
 #include "tm4c123gh6pm.h"
 #include "common_macros.h"
 
 /*******************************************************************************
 *                      Functions Difinitions                                  *
 *******************************************************************************/
 
 /*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 2. Enable the UART.
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
	
	UART0_LCRH_R |= (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	UART0_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
        // GPIO Configuration
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R &= ~0xFF)|(GPIO_PCTL_PA1_U0TX|GPIO_PCTL_PA0_U0RX);
	GPIO_PORTA_DEN_R |= 0x03;
	GPIO_PORTA_AMSEL_R &= ~0x03;
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
 * Function responsible for print a charcter on the terminal using UART.
 */
void print_char(uint8 c)
{
      UART0_putchar(c);
}

/*
 * Description :
 * Function responsible for print a string on the terminal using UART return when encountering a newline.
 */
void print_string(const uint8 *str)
{
	uint8 i = 0;
	while(str[i] != '\0')
	{
		if (str[i] == '\n')
		{
                        UART0_putchar('\r');
		}
		UART0_putchar(str[i++]);
	}
}