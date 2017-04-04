


#define BS_GLOBAL
#include "mytype.h"
#include "bsp.h"
#include "app_cfg.h"
#include "rom_api.h"
#include "system_LPC11xx.h"


//static void pinInit (void);
extern void *__GpvVectorTable[];

void BSP_Init(void)
{
    SystemInit();
    LPC_SYSCON->SYSAHBCLKCTRL |= (1ul << 16)                            /*  ʹ��AHB����ʱ��             */
                                                                        /*  �����޷�����ICON            */
                               | (1ul << 6);                            /*  ��GPIO                    */
    SystemCoreClockUpdate();
}

#if 0
void pinInit (void)
{
    LPC_GPIO3->DIR = 0;
    LPC_GPIO2->DIR = 0;
    LPC_GPIO1->DIR = 0;
    LPC_GPIO0->DIR = 0;
    
    LPC_GPIO0->DATA = 0x3FF;
    LPC_GPIO1->DATA = 0x3FF;
    LPC_GPIO2->DATA = 0x3FF;
    LPC_GPIO0->DATA = 0x3FF;
    
    LPC_IOCON->RESET_PIO0_0  = PIO0_0_CON;                                         /*  PIO0_0 ��������             */
    LPC_IOCON->PIO0_1  = PIO0_1_CON;                                         /*  PIO0_1 ��������             */
    LPC_IOCON->PIO0_2  = PIO0_2_CON;                                         /*  PIO0_2 ��������             */
    LPC_IOCON->PIO0_3  = PIO0_3_CON;                                         /*  PIO0_3 ��������             */
    LPC_IOCON->PIO0_4  = PIO0_4_CON;                                         /*  PIO0_4 ��������             */
    LPC_IOCON->PIO0_5  = PIO0_5_CON;                                         /*  PIO0_5 ��������             */
    LPC_IOCON->PIO0_6  = PIO0_6_CON;                                         /*  PIO0_6 ��������             */
    LPC_IOCON->PIO0_7  = PIO0_7_CON;                                         /*  PIO0_7 ��������             */
    LPC_IOCON->PIO0_8  = PIO0_8_CON;                                         /*  PIO0_8 ��������             */
    LPC_IOCON->PIO0_9  = PIO0_9_CON;                                         /*  PIO0_9 ��������             */
    LPC_IOCON->SWCLK_PIO0_10 = PIO0_10_CON;                                        /*  PIO0_10��������             */
    LPC_IOCON->R_PIO0_11 = PIO0_11_CON;                                        /*  PIO0_11��������             */

    LPC_IOCON->R_PIO1_0  = PIO1_0_CON;                                         /*  PIO1_0 ��������             */
    LPC_IOCON->R_PIO1_1  = PIO1_1_CON;                                         /*  PIO1_1 ��������             */
    LPC_IOCON->R_PIO1_2  = PIO1_2_CON;                                         /*  PIO1_2 ��������             */
    LPC_IOCON->SWDIO_PIO1_3  = PIO1_3_CON;                                         /*  PIO1_3 ��������             */
    LPC_IOCON->PIO1_4  = PIO1_4_CON;                                         /*  PIO1_4 ��������             */
    LPC_IOCON->PIO1_5  = PIO1_5_CON;                                         /*  PIO1_5 ��������             */
    LPC_IOCON->PIO1_6  = PIO1_6_CON;                                         /*  PIO1_6 ��������             */
    LPC_IOCON->PIO1_7  = PIO1_7_CON;                                         /*  PIO1_7 ��������             */
    LPC_IOCON->PIO1_8  = PIO1_8_CON;                                         /*  PIO1_8 ��������             */
    LPC_IOCON->PIO1_9  = PIO1_9_CON;                                         /*  PIO1_9 ��������             */
    LPC_IOCON->PIO1_10 = PIO1_10_CON;                                        /*  PIO1_10��������             */
    LPC_IOCON->PIO1_11 = PIO1_11_CON;                                        /*  PIO1_11��������             */

    LPC_IOCON->PIO2_0  = PIO2_0_CON;                                         /*  PIO2_0 ��������             */
    LPC_IOCON->PIO2_1  = PIO2_1_CON;                                         /*  PIO2_1 ��������             */
    LPC_IOCON->PIO2_2  = PIO2_2_CON;                                         /*  PIO2_2 ��������             */
    LPC_IOCON->PIO2_3  = PIO2_3_CON;                                         /*  PIO2_3 ��������             */
    LPC_IOCON->PIO2_4  = PIO2_4_CON;                                         /*  PIO2_4 ��������             */
    LPC_IOCON->PIO2_5  = PIO2_5_CON;                                         /*  PIO2_5 ��������             */
    LPC_IOCON->PIO2_6  = PIO2_6_CON;                                         /*  PIO2_6 ��������             */
    LPC_IOCON->PIO2_7  = PIO2_7_CON;                                         /*  PIO2_7 ��������             */
    LPC_IOCON->PIO2_8  = PIO2_8_CON;                                         /*  PIO2_8 ��������             */
    LPC_IOCON->PIO2_9  = PIO2_9_CON;                                         /*  PIO2_9 ��������             */
    LPC_IOCON->PIO2_10 = PIO2_10_CON;                                        /*  PIO2_10��������             */
    LPC_IOCON->PIO2_11 = PIO2_11_CON;                                        /*  PIO2_11��������             */

    LPC_IOCON->PIO3_0  = PIO3_0_CON;                                         /*  PIO3_0 ��������             */
    LPC_IOCON->PIO3_1  = PIO3_1_CON;                                         /*  PIO3_1 ��������             */
    LPC_IOCON->PIO3_2  = PIO3_2_CON;                                         /*  PIO3_2 ��������             */
    LPC_IOCON->PIO3_3  = PIO3_3_CON;                                         /*  PIO3_3 ��������             */
    LPC_IOCON->PIO3_4  = PIO3_4_CON;                                         /*  PIO3_4 ��������             */
    LPC_IOCON->PIO3_5  = PIO3_5_CON;                                         /*  PIO3_5 ��������             */
}
#endif

/*
 * BSP_Reset   ͨ�������λ��Ƭ��
 */
void BSP_Reset (void)
{   /*  ��ϵͳ�ϵ縴λ��ͬӲ����λ  */
    NVIC_SystemReset();
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

void  OS_CPU_SysTickInit (uint32  cnts)
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


/*
 * end bsp.c
 */


