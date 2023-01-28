
#include "snake.h"
#include <stdlib.h>




// Creates a new Snake with head and tail
Snake* createSnake (uint16 startx, uint16 starty) {
    Snake* snake = malloc(sizeof(Snake));
    snake -> tail = snake -> head = createSnakeNode(startx, starty);
    snake -> lastDir = 'w';
    snake -> isAlive = 1;
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
    if(nextDir == 'w' && snake -> lastDir != 's') snake -> lastDir = 'w';
    else if(nextDir == 'a' && snake -> lastDir != 'd') snake -> lastDir = 'a';
    else if(nextDir == 's' && snake -> lastDir != 'w') snake -> lastDir = 's';
    else if(nextDir == 'd' && snake -> lastDir != 'a') snake -> lastDir = 'd';
    
    if(snake -> lastDir == 'w')      xm = 0, ym = -1;
    else if(snake -> lastDir == 'a') xm = -1, ym = 0;
    else if(snake -> lastDir == 's') xm = 0, ym = 1;
    else if(snake -> lastDir == 'd') xm = 1, ym = 0;

    uint16 cur_x = snake -> head -> x_coordinate, cur_y = snake -> head -> y_coordinate;

    // Some checking, border, snake eating itself

    uint16 next_x = xm + cur_x, next_y = ym + cur_y;

    if (board[next_y][next_x] == BORDER || board[next_y][next_x] == SNAKE) {
        snake -> isAlive = 0;
        return;
    }

    SnakeNode* newNode = createSnakeNode(next_x, next_y);
    enqueue_snake(&(snake -> head), &newNode);
    if (board[next_y][next_x] != POWERUP) {
        board[snake -> tail -> y_coordinate][snake -> tail -> x_coordinate] = EMPTY;
        deqeue_snake(&(snake -> tail));
    }
    board[next_y][next_x] = SNAKE;
}