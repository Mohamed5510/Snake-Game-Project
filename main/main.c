 /******************************************************************************
 *
 * Module: Main
 *
 * File Name: main.c
 *
 * Description: source file for the main function
 *
 *******************************************************************************/
#include "../headers/tm4c123gh6pm.h"
#include "../os/os.h"
#include "../os/tasks.h"

int main(void)
{
  OS_init();
  Init_Task();
  TasksCreation();
  print_game();
  //TaskStartScheduler();
  //while(1){}
}
