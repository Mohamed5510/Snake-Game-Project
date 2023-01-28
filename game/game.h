 /******************************************************************************
 *
 * Module: application
 *
 * File Name: game.h
 *
 * Description: Header file for the snake game
 *
 *******************************************************************************/

#ifndef GAME_H_
#define GAME_H_
#include "../drivers/uart.h"
#include <stdlib.h>
#include <stdbool.h>
#define defaultLineDrawChar '*'
#define defaultBackgroundDrawChar ' '

typedef struct Point
{
    int x;
    int y;
} Point;

typedef struct Size
{
    int width;
    int height;
} Size;

void Game_init(void);
bool draw(void (*print_func)(const char), const Point topLeftPos, const Size rectangleSize, const char lineDrawChar, const char backgroundDrawChar);


#endif /* GAME_H_ */