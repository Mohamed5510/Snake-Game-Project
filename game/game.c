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

// Global Variables
int frameWidth = 40;
int frameHeight = 20;
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

/*
* Example for calling this function :
* draw(std::cout, Point{6,4}, Size{10,10}, 'O', '-'}
*/
bool draw(void (*print_func)(const char), const Point topLeftPos, const Size rectangleSize, const char lineDrawChar, const char backgroundDrawChar)
{
    if (topLeftPos.x < 0 || topLeftPos.y < 0 || rectangleSize.width < 1 || rectangleSize.height < 0) {
        return false;
    }
    const int rectRightXIndex = {
        topLeftPos.x + rectangleSize.width - 1
    };
    const int rectLeftXIndex = {
        topLeftPos.x
    };
    const int rectBottomYIndex = {
        topLeftPos.y + rectangleSize.height - 1
    };
    const int rectTopYIndex = {
        topLeftPos.y
    };

    bool isXYOnRectangleBorder = false;

    for (int y = 0; y < rectBottomYIndex + 1; ++y)
    {
        for (int x = 0; x < rectRightXIndex + 1; ++x)
        {
            if (x >= rectLeftXIndex && x <= rectRightXIndex && y >= rectTopYIndex && y <= rectBottomYIndex)
            {
                if (y == rectTopYIndex || y == rectBottomYIndex)
                {
                    isXYOnRectangleBorder = true;
                }
                else if (x == rectLeftXIndex || x == rectRightXIndex)
                {
                    isXYOnRectangleBorder = true;
                }
            }
            if (isXYOnRectangleBorder)
            {
                print_func(lineDrawChar);
                isXYOnRectangleBorder = false;
            }
            else
            {
                print_func(backgroundDrawChar);
            }
        }
        print_func('\n');
    }
    return true;
}

/*
 * Description :
 * Function responsible for generating pseudorandom number
 */
int generatePseudorandomNumber(int max) {
    uint32 result = seed;
    result ^= result << 13;
    result ^= result >> 17;
    result ^= result << 5;
    seed = result;
    return result % max;
}

/*
 * Description :
 * Function responsible for generating pseudorandomly positioned powerup
 */
struct Point generatePowerup() {
    int x = generatePseudorandomNumber(frameWidth);
    int y = generatePseudorandomNumber(frameHeight);
    struct Point point = { x, y };
    return point;
}

/*
 * Description :
 * Function responsible for generating pseudorandomly positioned enemy
 */
struct Point generateEnemy() {
    return generatePowerup();
}
