
@********************************************************************************************************
@                                           ARM Cortex-M3 Port
@
@ File      : bsp_int_asm.S
@ For       : ARMv7M Cortex-M3
@ Mode      : Thumb2
@ Toolchain : arm-none-eabi-gcc
@********************************************************************************************************


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

.section .after_vectors
.extern  svc_handler_export
.global  SVCall_Handler
.type    SVCall_Handler, function
SVCall_Handler:
    tst   lr, #4
    ite   EQ
    mrseq r0, MSP
    mrsne r0, PSP
    b     svc_handler_export

.extern  HardFaultHandle_c
.global  NMI_Handler
.type    NMI_Handler, function   
NMI_Handler:    
    tst   lr, #4
    ite   EQ
    mrseq r0, MSP
    mrsne r0, PSP
    mov   r1, #0
    b     HardFaultHandle_c

.global  HardFault_Handler
.type    HardFault_Handler, function   
HardFault_Handler:   
    tst   lr, #4
    ite   EQ
    mrseq r0, MSP
    mrsne r0, PSP
    mov   r1, #1
    b     HardFaultHandle_c

.global  MemManage_Handler
.type    MemManage_Handler, function   
MemManage_Handler:
    tst   lr, #4
    ite   EQ
    mrseq r0, MSP
    mrsne r0, PSP
    mov   r1, #2
    b     HardFaultHandle_c


.global  BusFault_Handler
.type    BusFault_Handler, function   
BusFault_Handler:
    tst   lr, #4
    ite   EQ
    mrseq r0, MSP
    mrsne r0, PSP
    mov   r1, #3
    b     HardFaultHandle_c

.global  UsageFault_Handler
.type    UsageFault_Handler, function   
UsageFault_Handler:
    tst   lr, #4
    ite   EQ
    mrseq r0, MSP
    mrsne r0, PSP
    mov   r1, #4
    b     HardFaultHandle_c

    .end





