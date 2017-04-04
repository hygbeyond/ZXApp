


#ifndef __BSP_H__
#define __BSP_H__

#include "NUC131.h"
#include "NUC131_mac.h"
#include "system_NUC131.h"

/*********************************************************************************************************
  ϵͳʱ������
*********************************************************************************************************/
#define MAIN_CLKSRCSEL_VALUE        1                                   /*  0x00:ѡ��IRC����          */
                                                                        /*  0x01:ѡ��ϵͳ����           */
                                                                        /*  0x02:ѡ��WDT����          */

#define FOSC                        F_CPU                               /*  ����Ƶ��                  */

#define FCCLK                       SystemCoreClock                     /*  ��ʱ��Ƶ��<=50Mhz           */
#define SYS_AHB_DIV_VALUE           1
#define FAHBCLK                    (FCCLK / SYS_AHB_DIV_VALUE)          /*  AHB CLK��Ƶ����FCCLK��ͬ    */
#define FPCLK                       SystemCoreClock
                                             

#define Sleep()          __asm__ __volatile__ ("WFI")
#define NOP()            __asm__ __volatile__ ("nop")
#define nop()            NOP()

void BSP_Init      (void);
void BSP_Reset     (void);
#define resetMCU    BSP_Reset

#endif //__BSP_H__



