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

void Game_init(void)
{
    UART0_init(16000000, 9600);
}

