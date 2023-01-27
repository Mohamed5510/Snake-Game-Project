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
#include <stdlib.h>

struct Point
{
    Point() = default;
    explicit Point(int x_, int y_) : x{ x_ }, y{ y_ }
    {

    }
    int x;
    int y;
};

struct Size
{
    Size() = default;
    explicit Size(int width_, int height_) : width{ width_ }, height{ height_ }
    {

    }
    int width;
    int height;
};

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
void print_string(const uint8 *str)
{
	uint8 i = 0;
	while(str[i] != '\0')
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


// example for calling this function 

// draw(std::cout, Point{6,4}, Size{10,10}, 'O', '-'}

bool draw(void print_func, const Point topLeftPos, const Size rectangleSize, const char lineDrawChar = "*", const char backgroundDrawChar =" ")
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


    bool isXYOnRectangleBorder{ false };

    for (int y = 0 ; y < rectBottomYIndex + 1; ++y)
    {
        for (int x = 0; x < rectRightXIndex + 1; ++x)
        {
            if (x >= rectLeftXIndex && x <= rectRightXIndex && y >= rectTopYIndex && y <= rectBottomYIndex)
            {
                if (y == rectTopYIndex || y == rectBottomYIndex)
                {
                    isXYOnRectangleBorder = true;
                }
                else if ( x == rectLeftXIndex || x == rectRightXIndex)
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

