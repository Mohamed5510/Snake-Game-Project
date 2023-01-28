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

#include "../headers/std_types.h"
#include "../drivers/uart.h"
#include <stdlib.h>

#define SNAKE '0'
#define POWERUP 'z'
#define BORDER '#'
#define EMPTY ' '

/******************************************************************************
 *                              Data Types                                    *
 ******************************************************************************/

typedef struct snakeN {  
    uint16 x_coordinate;
    uint16 y_coordinate;
    struct snakeN* next;
    struct snakeN* back;
} SnakeNode;

typedef struct {
    SnakeNode* head;
    SnakeNode* tail;
    char lastDir;
    int isAlive;
} Snake;

/******************************************************************************
 *                       Functions Prototype                                  *
 ******************************************************************************/
void Game_init(void);
void print_game(void);
void generatePowerup(void);
void generateEnemy(void);
SnakeNode* createSnakeNode (uint16 x, uint16 y);
Snake* createSnake (uint16 startx, uint16 starty);
void enqueue_snake(SnakeNode** head, SnakeNode** newNode);
void deqeue_snake(SnakeNode** tail);
void move_snake();

#endif /* GAME_H_ */