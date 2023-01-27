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

}