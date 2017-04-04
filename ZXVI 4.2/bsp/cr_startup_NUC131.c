//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009-10 Code Red Technologies Ltd.
//
// Microcontroller Startup code for use with Red Suite
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 
//
//*****************************************************************************
#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
	extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

#include "app_cfg.h"
#include "bsp.h"
//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will 
// automatically take precedence over these weak definitions
//
//*****************************************************************************
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVCall_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);
//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
//*****************************************************************************

                                                            
void     BOD_IRQHandler    (void) ALIAS( IntDefaultHandler )  ;
void     WDT_IRQHandler    (void) ALIAS( IntDefaultHandler )  ;
void     EINT0_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     EINT1_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     GPAB_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     GPCDEF_IRQHandler (void) ALIAS( IntDefaultHandler )  ;
void     Default_Handler   (void) ALIAS( IntDefaultHandler )  ;
void     TMR0_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     TMR1_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     TMR2_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     TMR3_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     UART02_IRQHandler (void) ALIAS( IntDefaultHandler )  ;
void     UART1_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     SPI0_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     UART3_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     UART4_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     UART5_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     I2C0_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     I2C1_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     CAN0_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     CAN1_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     PWM0_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     PWM1_IRQHandler   (void) ALIAS( IntDefaultHandler )  ;
void     BPWM0_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     BPWM1_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     BRAKE0_IRQHandler (void) ALIAS( IntDefaultHandler )  ;
void     BRAKE1_IRQHandler (void) ALIAS( IntDefaultHandler )  ;
void     PWRWU_IRQHandler  (void) ALIAS( IntDefaultHandler )  ;
void     ADC_IRQHandler    (void) ALIAS( IntDefaultHandler )  ;
void     RTC_IRQHandler    (void) ALIAS( IntDefaultHandler )  ;

//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for redlib based applications
// main() is the entry point for newlib based applications
//
//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);
//*****************************************************************************
//
// External declaration for the pointer to the stack top from the Linker Script
//
//*****************************************************************************

#ifndef STACK_SIZE
#define STACK_SIZE                              64
#endif
__attribute__ ((section(".stack")))
static unsigned long pulMainStack[STACK_SIZE];


//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
extern void (* const g_pfnVectors[])(void);
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void*)&pulMainStack[STACK_SIZE],		// The initial stack pointer
    ResetISR,                               // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
    0,                      				// Reserved
    0,                      				// Reserved
    0,                      				// Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVCall_Handler,                      	// SVCall handler
    0,                      				// Reserved
    0,                                      // Reserved
    PendSV_Handler,                      	// The PendSV handler
    SysTick_Handler,                      	// The SysTick handler

     BOD_IRQHandler    ,
     WDT_IRQHandler    ,
     EINT0_IRQHandler  ,
     EINT1_IRQHandler  ,
     GPAB_IRQHandler   ,
     GPCDEF_IRQHandler ,
     Default_Handler   ,
     Default_Handler   ,
     TMR0_IRQHandler   ,
     TMR1_IRQHandler   ,
     TMR2_IRQHandler   ,
     TMR3_IRQHandler   ,
     UART02_IRQHandler ,
     UART1_IRQHandler  ,
     SPI0_IRQHandler   ,
     UART3_IRQHandler  ,
     UART4_IRQHandler  ,
     UART5_IRQHandler  ,
     I2C0_IRQHandler   ,
     I2C1_IRQHandler   ,
     CAN0_IRQHandler   ,
     CAN1_IRQHandler   ,
     PWM0_IRQHandler   ,
     PWM1_IRQHandler   ,
     BPWM0_IRQHandler  ,
     BPWM1_IRQHandler  ,
     BRAKE0_IRQHandler ,
     BRAKE1_IRQHandler ,
     PWRWU_IRQHandler  ,
     ADC_IRQHandler    ,
     Default_Handler   ,
     RTC_IRQHandler    ,

};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void ResetISR(void)
{

    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    for(pulDest = &_bss; pulDest < &_ebss; )
    {
        *pulDest++ = 0;
    }

    /* Unlock protected registers */
    SYS_UnlockReg();
 
                // Init POR
    asm volatile (
                "LDR     R2, =0x50000024  \r\n"
                "LDR     R1, =0x00005AA5  \r\n"
                "STR     R1, [R2]         \r\n"
                ::
                );

    /* Lock protected registers */
    SYS_LockReg();
   
    SystemInit();
    //
    // Call the application's entry point.
    //
    main();
	while(1);
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void NMI_Handler(void)
{
    //
    // Enter an infinite loop.
    //
    pr_debug("NMI_Handler\n");
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void HardFault_Handler(void)
{
    //
    // Enter an infinite loop.
    //
    pr_debug("HardFault_Handler\n");
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void SVCall_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void PendSV_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void SysTick_Handler(void)
{
    while(1)
    {
    }
}

//*****************************************************************************
//
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void)
{
    pr_debug("IntDefaultHandler\n");
    while(1)
    {
    }
}

