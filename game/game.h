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

void Game_init(void);
bool draw(void print_func, const Point topLeftPos, const Size rectangleSize, const char lineDrawChar = "*", const char backgroundDrawChar = " ");


#endif /* GAME_H_ */