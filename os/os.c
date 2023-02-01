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
#define PENDSV_PRIORITY_MASK 0xFF1FFFFF
#define PENDSV_INTERRUPT_PRIORITY 7
#define PENDSV_PRIORITY_BITS_POS   21

static uint32 volatile Ticks;
Node* Ready_Tasks ;
TCB* Blocked_Tasks[20] = {NULL_PTR};
TCB* Semaphore_Blocked_Task = NULL_PTR;
TCB* volatile Running_Task = NULL_PTR;
TCB* volatile Scheduled_Task;
static volatile void (*Idle_Task_Callback_Ptr)(void) = NULL_PTR;
binarySemaphore semaphore = TRUE;

void OS_init(void) {
    __asm("CPSID I");
    /* Assign priority level 0 to the SysTick Interrupt */
    NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & SYSTICK_PRIORITY_MASK) | (SYSTICK_INTERRUPT_PRIORITY << SYSTICK_PRIORITY_BITS_POS);
    /* set the PendSV interrupt priority to the lowest level 7 */
    NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & PENDSV_PRIORITY_MASK) | (PENDSV_INTERRUPT_PRIORITY << PENDSV_PRIORITY_BITS_POS);
    __asm("CPSIE I");
}

void PendSV_Handler(void)
{
__asm(
    "CPSID         I\n\t"
    "LDR           r1,=Running_Task\n\t"
    "LDR           r1,[r1,#0x00]\n\t"
    "CBZ           r1,PendSV_restore\n\t"
    "PUSH          {r4-r11}\n\t"
    "LDR           r1,=Running_Task\n\t"
    "LDR           r1,[r1,#0x00]\n\t"
    "STR           sp,[r1,#0x00]\n"
"PendSV_restore:\n\t"
    "LDR           r1,=Scheduled_Task\n\t"
    "LDR           r1,[r1,#0x00]\n\t"
    "LDR           sp,[r1,#0x00]\n\t"
    "LDR           r1,=Scheduled_Task\n\t"
    "LDR           r1,[r1,#0x00]\n\t"
    "LDR           r2,=Running_Task\n\t"
    "STR           r1,[r2,#0x00]\n\t"
    "POP           {r4-r11}\n\t"
    "CPSIE         I\n\t"
    "BX            lr\n");
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
        if(Semaphore_Blocked_Task == NULL_PTR)
        {
            for(uint8 i = 0; i < 20; i++)
            {
                if(Blocked_Tasks[i] == NULL_PTR)
                {
                    Blocked_Tasks[i] = Running_Task;
                    break;
                }
            }
        }
        Scheduled_Task->task_state= RUNNING;
        Running_Task = Scheduled_Task;
        PendSV_Handler();
    }
    else if ( Running_Task == NULL_PTR)
    {
        Scheduled_Task = peek(&Ready_Tasks);
        Dequeue(&Ready_Tasks);
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
        if(Blocked_Tasks[i] != NULL_PTR)
        {
            (Blocked_Tasks[i]->blocking_ticks)--;
            if(Blocked_Tasks[i]->blocking_ticks == 0)
            {
                Blocked_Tasks[i]->task_state = READY;
                Enqueue(&Ready_Tasks, Blocked_Tasks[i], Blocked_Tasks[i]->priority);
                Blocked_Tasks[i] = NULL_PTR;
            }
        }
    }
    OS_scheduler();
}

void TaskCreate(uint32 id, uint32 priority, uint32 stk_size, TaskImplementer taskImplementer, void* parameters)
{
  uint32 task_stk[40]={0};
    TCB me = {
   id,
   priority,
   READY,
   0,
     task_stk
    };

    /* round down the stack top to the 8-byte boundary
    * NOTE: ARM Cortex-M stack grows down from hi -> low memory
    */
    //uint32 *sp = (uint32 *)((((uint32)task_stk + 40) / 8) * 8);
    uint32 *sp =  (uint32 *)task_stk + 40;
    
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
    me.sp = sp;
    
    /* round up the bottom of the stack to the 8-byte boundary */
    uint32 * stk_limit = (uint32 *)(((((uint32)task_stk - 1U) / 8) + 1U) * 8);

    /* pre-fill the unused part of the stack with 0xDEADBEEF */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }

    Enqueue(&Ready_Tasks, &me, priority);

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

void TakeSemaphore()
{
    if(semaphore == TRUE)
    {
        semaphore = FALSE;
        return;
    }
    else
    {
        Running_Task->task_state = BLOCKED;
        Semaphore_Blocked_Task = Running_Task;
        OS_scheduler();
    }
}

void GiveSemaphore()
{
    if(semaphore == FALSE)
    {
        semaphore = TRUE;
        Enqueue(&Ready_Tasks, Semaphore_Blocked_Task, Semaphore_Blocked_Task->priority);
        Semaphore_Blocked_Task = NULL_PTR;
        OS_scheduler();
    }
    else
    {
        OS_scheduler();
    }
}