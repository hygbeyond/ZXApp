

/*
 * wdt.c
 */

#include "bsp.h"
#include "wdt.h"
#include "app_cfg.h"
#include "MicroOS.h"

#define WDEN              (0x1<<0)
#define WDRESET           (0x1<<1)
#define WDTOF             (0x1<<2)
#define WDINT             (0x1<<3)
#define WDPROTECT         (0x1<<4)

#ifndef WDT_FEED_VALUE
#define WDT_FEED_VALUE		0x003FFFFF
#endif

#define WINDOW_MODE       0
#define PROTECT_MODE      1
#define WATCHDOG_RESET    0

__AFTER__
void WDT_IRQHandler(void);
void WDT_IRQHandler(void)
{
    DEBUG1( "wdt IRQ\n" );
                                /* For WDPROTECT test */
    LPC_WDT->FEED = 0xAA;		/* Feeding sequence */
    LPC_WDT->FEED = 0x55;    

    LPC_WDT->MOD &= ~WDTOF;		/* clear the time-out flag and interrupt flag */
    LPC_WDT->MOD &= ~WDINT;		/* clear the time-out flag and interrupt flag */
}

void wdt_init( void )
{
    OS_SREG_VAR;

    LPC_SYSCON->WDTOSCCTRL = 0x03F; /* ~8kHz */
    LPC_SYSCON->PDRUNCFG &= ~(0x1<<6);

    LPC_SYSCON->WDTCLKSEL = 0x02;		/* Select watchdog osc */
    LPC_SYSCON->WDTCLKUEN = 0x01;		/* Update clock */
    LPC_SYSCON->WDTCLKUEN = 0x00;		/* Toggle update register once */
    LPC_SYSCON->WDTCLKUEN = 0x01;
    while ( !(LPC_SYSCON->WDTCLKUEN & 0x01) );		/* Wait until updated */
    LPC_SYSCON->WDTCLKDIV = 1;		/* Divided by 1 */

    OS_ENTER_CRITICAL();
    SYSAHBCLKCTRL_SET( AHBCLK_WDT );
    //LPC_SYSCON->SYSAHBCLKCTRL |= (1<<15);                               /* 打开WDT模块                  */
    LPC_WDT->TC          = 0x0FFF;                                       /* 定时时间                     */
    LPC_WDT->MOD         = (0x01 << 0)                                  /* 使能WDT                      */
                          |(0x01 << 1);                                 /* 使能WDT中断                  */
    LPC_WDT->FEED        = 0xAA;                                        /* 喂狗                         */
    LPC_WDT->FEED        = 0x55;
    OS_EXIT_CRITICAL();
}

void wdt_reset( void )
{
    OS_SREG_VAR;
    OS_ENTER_CRITICAL();
    LPC_WDT->FEED = 0xAA;		/* Feeding sequence */
    LPC_WDT->FEED = 0x55;
    OS_EXIT_CRITICAL();
}


