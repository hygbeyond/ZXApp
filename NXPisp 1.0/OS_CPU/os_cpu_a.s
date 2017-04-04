@/*
@ ********************************************************************************
@ **				                   MicroOS(ARM)
@ **                      The Real-Time Kernel(For ARM cortex-m0)
@ **
@ **                     (c) Copyright 2006-2007, hygbeyond
@ **                             All Rights Reserved
@ **
@ **                                    V0.9
@ **                       基于周立功TinyM0  LPC1114/301
@ ********************************************************************************
@ **创 建 人：胡勇刚
@ **文 件 名：os_cpu_c.c
@ **创建日期：2010-11-23
@ **描    述：根据micrum移植过来的，进行了小量的修改。
@ */



@********************************************************************************************************
@                                               uC/OS-II
@                                         The Real-Time Kernel
@
@                               (c) Copyright 1992-2006, Micrium, Weston, FL
@                                          All Rights Reserved
@
@                                           ARM Cortex-M3 Port
@
@ File      : OS_CPU_A.S
@ Version   : V2.89
@ By        : Jean J. Labrosse
@             Brian Nagel
@
@ For       : ARMv7M Cortex-M3 Cortex-M0
@ Mode      : Thumb2 armv6-m
@ Toolchain : arm-none-eabi-gcc
@ 2010-11-23 移植到 Cortex-m0 LPC1114/301
@           运行环境 MicroOS 0.9
@ 移植人：  胡勇刚
@********************************************************************************************************

@********************************************************************************************************
@                                           PUBLIC FUNCTIONS
@********************************************************************************************************

    .extern  OSRunning                                           @ External references
    .extern  OS_TCBCur
    .extern  OS_TCBHighRdy
    .extern  OSIntExit
    .extern  OSTaskSwHook


    .global  OS_CPU_SR_Save                                      @ Functions declared in this file
    .global  OS_CPU_SR_Restore
    .global  OSStartHighRdy
    .global  OSCtxSw
    .global  OSIntCtxSw
    .global  OS_CPU_PendSVHandler

@********************************************************************************************************
@                                                EQUATES
@********************************************************************************************************

.equ NVIC_INT_CTRL   ,     0xE000ED04                              @ Interrupt control state register.
.equ NVIC_SYSPRI14   ,     0xE000ED22                              @ System priority register (priority 14).
.equ NVIC_PENDSV_PRI ,           0xFF                              @ PendSV priority value (lowest).
.equ NVIC_PENDSVSET  ,     0x10000000                              @ Value to trigger PendSV exception.

@********************************************************************************************************
@                                      CODE GENERATION DIRECTIVES
@********************************************************************************************************

.text
.code 16 
.syntax unified

@********************************************************************************************************
@                                          START MULTITASKING
@                                       void OSStartHighRdy(void)
@
@ Note(s) : 1) This function triggers a PendSV exception (essentially, causes a context switch) to cause
@              the first task to start.
@
@           2) OSStartHighRdy() MUST:
@              a) Setup PendSV exception priority to lowest@
@              b) Set initial PSP to 0, to tell context switcher this is first run@
@              c) Set OSRunning to TRUE@
@              d) Trigger PendSV exception@
@              e) Enable interrupts (tasks will run with interrupts enabled).
@********************************************************************************************************


.global  OSStartHighRdy
.type    OSStartHighRdy, function
OSStartHighRdy:
    LDR     R0, =NVIC_SYSPRI14                                  @ Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]

    MOVS    R0, #0                                              @ Set the PSP to 0 for initial context switch call
    MSR     PSP, R0

    LDR     R0, =OSRunning                                      @ OSRunning = TRUE
    MOVS    R1, #1
    STRB    R1, [R0]

    LDR     R0, =NVIC_INT_CTRL                                  @ Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                                                   @ Enable interrupts at processor level

OSStartHang:
    B       OSStartHang                                         @ Should never get here


@********************************************************************************************************
@                                         HANDLE PendSV EXCEPTION
@                                     void OS_CPU_PendSVHandler(void)
@
@ Note(s) : 1) PendSV is used to cause a context switch.  This is a recommended method for performing
@              context switches with Cortex-M3.  This is because the Cortex-M3 auto-saves half of the
@              processor context on any exception, and restores same on return from exception.  So only
@              saving of R4-R11 is required and fixing up the stack pointers.  Using the PendSV exception
@              this way means that context saving and restoring is identical whether it is initiated from
@              a thread or occurs due to an interrupt or exception.
@
@           2) Pseudo-code is:
@              a) Get the process SP, if 0 then skip (goto d) the saving part (first context switch)@
@              b) Save remaining regs r4-r11 on process stack@
@              c) Save the process SP in its TCB, OSTCBCur->OSTCBStkPtr = SP@
@              d) Call OSTaskSwHook()@
@              e) Get current high priority, OSPrioCur = OSPrioHighRdy@
@              f) Get current ready thread TCB, OSTCBCur = OSTCBHighRdy@
@              g) Get new process SP from TCB, SP = OSTCBHighRdy->OSTCBStkPtr@
@              h) Restore R4-R11 from new process stack@
@              i) Perform exception return which will restore remaining context.
@
@           3) On entry into PendSV handler:
@              a) The following have been saved on the process stack (by processor):
@                 xPSR, PC, LR, R12, R0-R3
@              b) Processor mode is switched to Handler mode (from Thread mode)
@              c) Stack is Main stack (switched from Process stack)
@              d) OSTCBCur      points to the OS_TCB of the task to suspend
@                 OSTCBHighRdy  points to the OS_TCB of the task to resume
@
@           4) Since PendSV is set to lowest priority in the system (by OSStartHighRdy() above), we
@              know that it will only be run when no other exception or interrupt is active, and
@              therefore safe to assume that context being switched out was using the process stack (PSP).
@********************************************************************************************************
.section .after_vectors
.global  PendSV_Handler
.type    PendSV_Handler, function
PendSV_Handler:
.global  OS_CPU_PendSVHandler
.type    OS_CPU_PendSVHandler, function
OS_CPU_PendSVHandler:
	CPSID   I                                                   @ Prevent interruption during context switch
    MRS     R0, PSP                                             @ PSP is process stack pointer
    CMP     R0, #0x00
	BEQ     OS_CPU_PendSVHandler_nosave                         @; Skip register save the first time

    SUBS    R0, R0, #16                                         @; Save remaining regs r4-7 on process stack
	
    LDR     R1, =OS_TCBCur                                      @; OSTCBCur->OSTCBStkPtr = SP;
    LDR     R1, [R1]
    STR     R0, [R1]                                            @; R0 is SP of process being switched out
    
	STM     R0!, {R4-R7}
	
                                                                @; At this point, entire context of process has been saved
OS_CPU_PendSVHandler_nosave:
    @PUSH    {R14}                                               @ Save LR exc_return value
    @LDR     R0, =OSTaskSwHook                                   @ OSTaskSwHook();
    @BX      R0
	@POP     {R0}
	@MOV     R14, R0
	bl       OSTaskSwHook

    LDR     R0, =OS_TCBCur                                      @; OSTCBCur  = OSTCBHighRdy;
    LDR     R1, =OS_TCBHighRdy
    LDR     R2, [R1]
    STR     R2, [R0]

    LDR     R0, [R2]                                            @; R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
    LDM     R0!, {R4-R7}                                        @; Restore r4-11 from new process stack
    MSR     PSP, R0                                             @; Load PSP with new process SP
    CPSIE   I
	
	MOVS	r0, #0x03                                           @; r0 = 0xFFFFFFFD 返回使用PSP栈
	RSBS	r0, #0
	BX		r0

    .end 
