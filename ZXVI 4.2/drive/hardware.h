

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "bsp.h"
#include "app_cfg.h"
#include "MicroOS.h"
#include "app_cfg.h"
#include "nuc_uart0.h"
#include "nuc_wdt.h"


#define KEY_IN         ( PBI_P(C, 0) )


#define LED_ON        CBI_P( B, 4)
#define LED_OFF       SBI_P( B, 4)
#define LED_NOT       NBI_P( B, 4)


extern OS_EVENT *SPI_Sem;
#define OSSPI_Lock()               OSSemPend( SPI_Sem, 0 )
#define OSSPI_Unlock()             OSSemPost( SPI_Sem )


void IO_Init(void);
int GetAddr(void);






#endif
