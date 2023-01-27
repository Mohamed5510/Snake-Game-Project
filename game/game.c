 /******************************************************************************
 *
 * Module: application
 *
 * File Name: game.h
 *
 * Description: Source file for the snake game
 *
 *******************************************************************************/
#include "game.h"
#include "../drivers/uart.h"

/*
 * Description :
 * Function responsible for print a charcter on the terminal.
 */
void print_char(uint8 c)
{
      UART0_putchar(c);
}

/*
 * Description :
 * Function responsible for get a charcter from the terminal.
 */
uint8 get_char()
{
      return UART0_getchar();
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

void Game_init(void)
{
    UART0_init(16000000, 9600);
}

