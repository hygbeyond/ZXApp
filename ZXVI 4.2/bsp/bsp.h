


#ifndef __BSP_H__
#define __BSP_H__

#include "NUC131.h"
#include "NUC131_mac.h"
#include "system_NUC131.h"

/*********************************************************************************************************
  系统时钟配置
*********************************************************************************************************/
#define MAIN_CLKSRCSEL_VALUE        1                                   /*  0x00:选择IRC振荡器          */
                                                                        /*  0x01:选择系统振器           */
                                                                        /*  0x02:选择WDT振荡器          */

#define FOSC                        F_CPU                               /*  振荡器频率                  */

#define FCCLK                       SystemCoreClock                     /*  主时钟频率<=50Mhz           */
#define SYS_AHB_DIV_VALUE           1
#define FAHBCLK                    (FCCLK / SYS_AHB_DIV_VALUE)          /*  AHB CLK分频或与FCCLK相同    */
#define FPCLK                       SystemCoreClock
                                             

#define Sleep()          __asm__ __volatile__ ("WFI")
#define NOP()            __asm__ __volatile__ ("nop")
#define nop()            NOP()

void BSP_Init      (void);
void BSP_Reset     (void);
#define resetMCU    BSP_Reset

#endif //__BSP_H__



