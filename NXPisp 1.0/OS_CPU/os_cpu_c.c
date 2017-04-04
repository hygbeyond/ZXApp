/*
 ********************************************************************************
 **				                   MicroOS(ARM)
 **                      The Real-Time Kernel(For ARM cortex-m0)
 **
 **                     (c) Copyright 2006-2007, hygbeyond
 **                             All Rights Reserved
 **
 **                                    V0.9
 **                       基于周立功TinyM0  LPC1114/301
 ********************************************************************************
 **创 建 人：胡勇刚
 **文 件 名：os_cpu_c.c
 **创建日期：2010-11-23
 **描    述：根据micrum移植过来的，进行了小量的修改。
 */

/*
 *********************************************************************************************************
 *                                               uC/OS-II
 *                                         The Real-Time Kernel
 *
 *
 *                                (c) Copyright 2006, Micrium, Weston, FL
 *                                          All Rights Reserved
 *
 *                                           ARM Cortex-M3 Port
 *
 * File      : OS_CPU_C.C
 * Version   : V2.89
 * By        : Jean J. Labrosse
 *             Brian Nagel
 *
 * For       : ARMv7M Cortex-M3
 * Mode      : Thumb2
 * Toolchain : IAR EWARM
 *********************************************************************************************************
 */

#define  OS_CPU_GLOBALS
#include <MicroOs.h>
#include "bsp.h"

/*
 *********************************************************************************************************
 *                                          LOCAL VARIABLES
 *********************************************************************************************************
 */

#if OS_TMR_EN > 0u
static  INT16U  OSTmrCtr;
#endif

/*
 *********************************************************************************************************
 *                                          SYS TICK DEFINES
 *********************************************************************************************************
 */

static void OS_TaskReturn(void)
{
    while(1)
    {
        OSTimeDly(HZ);
    }
}

/*
 * 初始化线程堆栈
 * task：线程函数指针
 * ptos：堆栈栈顶
 * 返回：初始化后的堆栈指针地址
 */

OS_STK *OSTaskStkInit (void (*task)(void ), OS_STK *ptos)
{
    OS_STK *stk;

    stk       = ptos;                            /* Load stack pointer                                 */

    /* Registers stacked as if auto-saved on exception    */
    *(stk)    = (INT32U)0x01000000uL;            /* xPSR                                               */
    *(--stk)  = (INT32U)task;                    /* Entry Point                                        */
    *(--stk)  = (INT32U)OS_TaskReturn;           /* R14 (LR)                                           */
    *(--stk)  = (INT32U)0x12;            /* R12                                                */
    *(--stk)  = (INT32U)0x03;            /* R3                                                 */
    *(--stk)  = (INT32U)0x02;            /* R2                                                 */
    *(--stk)  = (INT32U)0x01;            /* R1                                                 */
    *(--stk)  = (INT32U)0x00;            /* R0 : argument                                      */

    /* Remaining registers saved on process stack         */
    *(--stk)  = (INT32U)0x07;            /* R7                                                 */
    *(--stk)  = (INT32U)0x06;            /* R6                                                 */
    *(--stk)  = (INT32U)0x05;            /* R5                                                 */
    *(--stk)  = (INT32U)0x04;            /* R4                                                 */

    return (stk);
}

/*
 *********************************************************************************************************
 *                                          SYS TICK HANDLER
 *
 * Description: Handle the system tick (SysTick) interrupt, which is used to generate the uC/OS-II tick
 *              interrupt.
 *
 * Arguments  : none.
 *
 * Note(s)    : 1) This function MUST be placed on entry 15 of the Cortex-M3 vector table.
 *********************************************************************************************************
 */
__AFTER__
         void  SysTick_Handler (void);
void  SysTick_Handler (void)
{
    OS_CPU_SR  cpu_sr;

    OSTimeTickSize++;

    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */

    OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}

/*
 *********************************************************************************************************
 *                                          INITIALIZE SYS TICK
 *
 * Description: Initialize the SysTick.
 *
 * Arguments  : cnts          is the number of SysTick counts between two OS tick interrupts.
 *
 * Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
 *********************************************************************************************************
 */

void  OS_CPU_SysTickInit (INT32U  cnts)
{
    NVIC_EnableIRQ( PendSV_IRQn );
    //NVIC_SetPriority( PendSV_IRQn, 0xFF);
    /* Make PendSV the same priroity as SysTick and the kernel. */
    NVIC_SetPriority (PendSV_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

    NVIC_EnableIRQ( SysTick_IRQn );
    NVIC_SetPriority(SysTick_IRQn, 0x80);

    SysTick->LOAD = cnts;
    SysTick->CTRL   = 0x07;
}


void OSIntCtxSw (void)
{
    SCB->ICSR = 1ul << 28;
}


OS_CPU_SR  OS_CPU_SR_Save(void)
{
    OS_CPU_SR cpu_sr;
    asm volatile( "MRS %0, PRIMASK\n\t"
            "CPSID  I\n\t": "=r"(cpu_sr) );
    return cpu_sr;
}

void  OS_CPU_SR_Restore(OS_CPU_SR cpu_sr)
{
    asm volatile( "MSR     PRIMASK, %0" :: "r"(cpu_sr));
}

