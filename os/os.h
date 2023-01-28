 /******************************************************************************
 *
 * Module: OS
 *
 * File Name: os.h
 *
 * Description: Header file for OS
 *
 *******************************************************************************/
#ifndef OS_H_
#define OS_H_

#include "../headers/std_types.h"

/******************************************************************************
 *                              Data Types                                    *
 ******************************************************************************/
typedef void (*TaskImplementer)();
typedef boolean binarySemaphore;

/******************************************************************************
 *                       Functions Prototype                                  *
 ******************************************************************************/
void OS_init(void);
void TaskCreate();
void TaskStartScheduler(void);
void TaskDelay(uint32 ticks);
void IdleTaskSetCallback(void(*fn_ptr)(void));
void TakeSemaphor();
void GiveSemaphore();

#endif /* OS_H_ */