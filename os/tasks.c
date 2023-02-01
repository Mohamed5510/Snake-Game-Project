 /******************************************************************************
 *
 * Module: OS
 *
 * File Name: tasks.c
 *
 * Description: source file for the tasks will be run on CPU
 *
 *******************************************************************************/
#include "tasks.h"
#include "os.h"
#include "../game/game.h"

void Init_Task(void)
{
    Game_init();
}

void TasksCreation(void)
{
    // Powerup Task
    TaskCreate((uint32)4, (uint32)4, (uint32)40, (TaskImplementer)generatePowerup, NULL_PTR);

    // Print Task
    TaskCreate(3, 3, 40, print_game, NULL_PTR);

    // Move Snake Task
    TaskCreate(2, 2, 40, move_snake, NULL_PTR);

    // Enemy Task
    TaskCreate(2, 3, 40, move_snake, NULL_PTR);

}