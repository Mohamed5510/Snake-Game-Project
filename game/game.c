/******************************************************************************
*
* Module: application
*
* File Name: game.c
*
* Description: Source file for the snake game
*
*******************************************************************************/
#include "game.h"
#include "../os/os.h"
#include <stdlib.h>

// Global Variables
#define frameWidth 40
#define frameHeight 20
extern uint8 nextDir;
Snake* snake;
uint8 board [frameHeight][frameWidth];

uint32 seed = 350; //random initialization seed

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
void print_string(const uint8* str)
{
    uint8 i = 0;
    while (str[i] != '\0')
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

void print_game(void)
{
    uint8 i,j;
    while(1)
    {
        for(i = 0; i < frameHeight; i++)
        {
            for(j = 0; j < frameWidth; j++)
            {
                print_char(board[i][j]);
            }
        }

        TaskDelay(500);
    }
}

/*
 * Description :
 * Function responsible for generating pseudorandom number
 */
static uint32 generatePseudorandomNumber(int max) {
    uint32 result = seed;
    result ^= result << 13;
    result ^= result >> 17;
    result ^= result << 5;
    seed = result;
    return result % max;
}

static void generatePoint(uint8 c)
{
    uint8 x, y;
    do
    {
        x = generatePseudorandomNumber(frameWidth);
        y = generatePseudorandomNumber(frameHeight);
    }while(board[x][y] != EMPTY);
    board[x][y] = c;
}

/*
 * Description :
 * Function responsible for generating pseudorandomly positioned powerup
 */
void generatePowerup(void) {
    while(1)
    {
        TakeSemaphore();
        generatePoint(POWERUP);
    }
}

/*
 * Description :
 * Function responsible for generating pseudorandomly positioned enemy
 */
void generateEnemy(void) {
    while(1)
    {
        generatePoint(BORDER);
        TaskDelay(30000);
    }
}

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
void move_snake() {
    while(1)
    {
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

    if(board[next_y][next_x] == POWERUP)
    {
        GiveSemaphore();
    }

    SnakeNode* newNode = createSnakeNode(next_x, next_y);
    enqueue_snake(&(snake -> head), &newNode);
    if (board[next_y][next_x] != POWERUP) {
        board[snake -> tail -> y_coordinate][snake -> tail -> x_coordinate] = EMPTY;
        deqeue_snake(&(snake -> tail));
    }
    board[next_y][next_x] = SNAKE;

    TaskDelay(500);
    }
}
