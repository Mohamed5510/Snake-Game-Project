#include "game.h"
#include "../drivers/uart.h"

void Game_init(void)
{
    UART0_init(16000000, 9600);
}

