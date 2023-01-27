 /******************************************************************************
 *
 * Module: OS
 *
 * File Name: os.h
 *
 * Description: Header file for OS
 *
 *******************************************************************************/
#include "os.h"
#include "queue.h"
#include "../headers/tm4c123gh6pm.h"
#include "../drivers/systick.h"

#define SYSTICK_PRIORITY_MASK  0x1FFFFFFF
#define SYSTICK_INTERRUPT_PRIORITY  0
#define SYSTICK_PRIORITY_BITS_POS   29
#define PENDSV_PRIORITY_MASK 0xFF1FFFFFF
#define PENDSV_INTERRUPT_PRIORITY 7
#define PENDSV_PRIORITY_BITS_POS   21

static uint32 volatile Ticks;
Node* Ready_Tasks;
TCB* Blocked_Tasks[20] = {NULL_PTR};
TCB* volatile Running_Task;
TCB* volatile Scheduled_Task;
static volatile void (*Idle_Task_Callback_Ptr)(void) = NULL_PTR;

void OS_init(void) {
    __disable_irq();
    /* Assign priority level 0 to the SysTick Interrupt */
    NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & SYSTICK_PRIORITY_MASK) | (SYSTICK_INTERRUPT_PRIORITY << SYSTICK_PRIORITY_BITS_POS);
    /* set the PendSV interrupt priority to the lowest level 7 */
    NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & PENDSV_PRIORITY_MASK) | (PENDSV_INTERRUPT_PRIORITY << PENDSV_PRIORITY_BITS_POS);
    *(uint32 volatile *)0xE000ED20 |= (0xFFU << 16);
    __enable_irq();
}

void PendSV_Handler(void) {
  __asm("\
    IMPORT  Running_Task\
    IMPORT  Scheduled_Task\
    CPSID         I\
    LDR           r1,=OS_curr\
    LDR           r1,[r1,#0x00]\
    CBZ           r1,PendSV_restore\
    PUSH          {r4-r11}\
    LDR           r1,=OS_curr\
    LDR           r1,[r1,#0x00]\
    STR           sp,[r1,#0x00]\
PendSV_restore\
    LDR           r1,=OS_next\
    LDR           r1,[r1,#0x00]\
    LDR           sp,[r1,#0x00]\
    LDR           r1,=OS_next\
    LDR           r1,[r1,#0x00]\
    LDR           r2,=OS_curr\
    STR           r1,[r2,#0x00]\
    POP           {r4-r11}\
    CPSIE         I\
    BX            lr\
  ");
}

static void OS_scheduler(void) {
    // Scheduling Algorithm
    if(Running_Task != NULL_PTR && Running_Task->task_state != BLOCKED)
    {
        if(peek(&Ready_Tasks)->priority>=Running_Task->priority)
        {
            Scheduled_Task = peek(&Ready_Tasks);
        }
        if (Running_Task != Scheduled_Task) {
        Dequeue(&Ready_Tasks);
        Enqueue(&Ready_Tasks,Running_Task,Running_Task->priority);
        Running_Task->task_state = READY;
        Scheduled_Task->task_state= RUNNING;
        Running_Task = Scheduled_Task;
        PendSV_Handler();
        }
    }
    else if(Running_Task != NULL_PTR && Running_Task->task_state == BLOCKED)
    {
        Scheduled_Task = peek(&Ready_Tasks);
        Dequeue(&Ready_Tasks);
        for(uint8 i = 0; i < 20; i++)
        {
            if(Blocked_Tasks[i] == NULL_PTR)
            {
                Blocked_Tasks[i] = Running_Task;
            }
        }
        Scheduled_Task->task_state= RUNNING;
        Running_Task = Scheduled_Task;
        PendSV_Handler();
    }
}

void SysTick_Handler(void) {
    ++Ticks;
    uint8 i;
    for(i = 0; i < 20; i++)
    {
        (Blocked_Tasks[i]->blocking_ticks)--;
        if(Blocked_Tasks[i]->blocking_ticks == 0)
        {
            Blocked_Tasks[i]->task_state = READY;
            Enqueue(&Ready_Tasks, Blocked_Tasks[i], Blocked_Tasks[i]->priority);
            Blocked_Tasks[i] = NULL_PTR;
        }
    }
    OS_scheduler();
}

void TaskCreate(uint32 id, uint32 priority, uint32 stk_size, TaskImplementer taskImplementer, void* parameters)
{
    uint32 task_stk[40];
    TCB* me;
    me->task_id = id;
    me->priority = priority;
    me->task_state = READY;
    me->sp = task_stk;
    me->blocking_ticks = 0;

    /* round down the stack top to the 8-byte boundary
    * NOTE: ARM Cortex-M stack grows down from hi -> low memory
    */
    uint32 *sp = (uint32 *)((((uint32)task_stk + 40) / 8) * 8);
    
    *(--sp) = (1U << 24);  /* xPSR */
    *(--sp) = (uint32)taskImplementer; /* PC */
    *(--sp) = 0x0000000EU; /* LR  */
    *(--sp) = 0x0000000CU; /* R12 */
    *(--sp) = 0x00000003U; /* R3  */
    *(--sp) = 0x00000002U; /* R2  */
    *(--sp) = 0x00000001U; /* R1  */
    *(--sp) = 0x00000000U; /* R0  */
    /* additionally, fake registers R4-R11 */
    *(--sp) = 0x0000000BU; /* R11 */
    *(--sp) = 0x0000000AU; /* R10 */
    *(--sp) = 0x00000009U; /* R9 */
    *(--sp) = 0x00000008U; /* R8 */
    *(--sp) = 0x00000007U; /* R7 */
    *(--sp) = 0x00000006U; /* R6 */
    *(--sp) = 0x00000005U; /* R5 */
    *(--sp) = 0x00000004U; /* R4 */

    /* save the top of the stack in the thread's attibute */
    me->sp = sp;
    
    /* round up the bottom of the stack to the 8-byte boundary */
    uint32 * stk_limit = (uint32 *)(((((uint32)task_stk - 1U) / 8) + 1U) * 8);

    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }

    Enqueue(&Ready_Tasks, me, priority);

}

static void IdleTask(void)
{
    if(Idle_Task_Callback_Ptr != NULL_PTR)
    {
        Idle_Task_Callback_Ptr();
    }
    else
    {
        __asm("WFI");
    }
}

void IdleTaskSetCallback(void(*fn_ptr)(void))
{
    //Idle_Task_Callback_Ptr = fn_ptr;
}

void TaskStartScheduler(void)
{
    TaskCreate(555, 0, 20, IdleTask, NULL_PTR);
    SysTick_Start(1);
    OS_scheduler();
}

void TaskDelay(uint32 ticks)
{
    Running_Task->blocking_ticks = ticks;
    Running_Task->task_state = BLOCKED;
    OS_scheduler();
}
