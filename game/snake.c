
#include "snake.h"
#include <stdlib.h>




// Creates a new Snake with head and tail
Snake* createSnake (uint16 startx, uint16 starty) {
    Snake* snake = malloc(sizeof(Snake));
    snake -> tail = snake -> head = createSnakeNode(startx, starty);
    return snake;
}

// Create a new snake node
SnakeNode* createSnakeNode (uint16 x, uint16 y) {
    SnakeNode* current = (SnakeNode*)malloc(sizeof(SnakeNode));
    current -> x_coordinate = x;
    current -> y_coordinate = y;
    current -> next = NULL;
    current -> back = NULL;
    return current;
}

// Adds a new node to the head of the snake
void enqueue_snake(SnakeNode** head, SnakeNode** newNode) {
    (*newNode) -> next = *head;
    (*head) -> back = (*newNode);
    *head = *newNode;
}

// Removes a node from the tail of the snake
void deqeue_snake(SnakeNode** tail) {

    SnakeNode* cur = (*tail);

    (*tail) = cur -> back;
    (*tail) -> next = NULL;
    free(cur);
}

// Moves the snake in the specified direction through the arguments 
void move_snake(char* dir, Snake* snake) {
    uint16 xm = 0 , ym = 0;
    if(*dir == 'w') xm = 0, ym = 1;
    else if(*dir == 'a') xm = -1, ym = 0;
    else if(*dir == 's') xm = 0, ym = -1;
    else if(*dir == 'd') xm = 1, ym = 0;

    uint16 cur_x = snake -> head -> x_coordinate, cur_y = snake -> head -> y_coordinate;

    // Some checking, border, snake eating itself

    SnakeNode* newNode = createSnakeNode(xm + cur_x, ym + cur_y);
    enqueue_snake(&(snake -> head), &newNode);
    deqeue_snake(&(snake -> tail));
}