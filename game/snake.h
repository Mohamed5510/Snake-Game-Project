 /******************************************************************************
 *
 * Module: application
 *
 * File Name: snake.h
 *
 * Description: Header file for the snake
 *
 *******************************************************************************/

#ifndef SNAKE_H_
#define SNAKE_H_

#include "../headers/std_types.h"

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
    uint16 limit_x;
    uint16 limit_y;
    int isAlive;
} Snake;

/******************************************************************************
 *                       Functions Prototype                                  *
 ******************************************************************************/

SnakeNode* createSnakeNode (uint16 x, uint16 y);
Snake* createSnake (uint16 startx, uint16 starty);
void enqueue_snake(SnakeNode** head, SnakeNode** newNode);
void deqeue_snake(SnakeNode** tail);
void move_snake(char* dir, Snake* snake);

#endif /* SNAKE_H_ */