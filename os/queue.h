 /******************************************************************************
 *
 * Module: OS
 *
 * File Name: queue.h
 *
 * Description: Header file for queue data structure
 *
 *******************************************************************************/

#ifndef QUEUE_H_
#define QUEUE_H_

#include "../headers/std_types.h"

/******************************************************************************
 *                              Data Types                                    *
 ******************************************************************************/
typedef enum{
    READY,
    RUNNING,
    BLOCKED
} TaskState;

typedef struct {
    uint32 task_id;
    uint32 priority;
    TaskState task_state;
    void *sp; /* stack pointer */
} TCB;

// Node
typedef struct { 
    TCB* data;
    uint32 priority; 
    struct node* next; 
} Node; 

/******************************************************************************
 *                       Functions Prototype                                  *
 ******************************************************************************/
TCB* peek(Node** head);
void Dequeue(Node** head);
void Enqueue(Node** head, TCB* d, uint32 p);
boolean isEmpty(Node** head);

#endif /* QUEUE_H_ */