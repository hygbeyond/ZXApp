

/*
 * wdt.c
 */

#include "bsp.h"
#include "nuc_wdt.h"
#include "app_cfg.h"
#include "MicroOS.h"


__AFTER__
void WDT_IRQHandler(void);
void WDT_IRQHandler(void)
{
    if(WDT_GET_TIMEOUT_INT_FLAG() == 1)
    {
        /* Clear WDT time-out interrupt flag */
        WDT_CLEAR_TIMEOUT_INT_FLAG();
        printk("wdt reset\n");
        BSP_Reset();
    }
}


void wdt_init( void )
{
    SYS_UnlockReg();

    /* Enable IRC 10 kHz */
    CLK->PWRCON |= CLK_PWRCON_OSC10K_EN_Msk;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_IRC10K_STB_Msk));
    
    /* Enable module clock */
    CLK->APBCLK  |=  CLK_APBCLK_WDT_EN_Msk; 
    /* Peripheral clock source */
    CLK->CLKSEL1 |= CLK_CLKSEL1_WDT_S_LIRC;
	
    WDT->WTCR = WDT_TIMEOUT_2POW14 | WDT_WTCR_WTIE_Msk | WDT_WTCR_WTE_Msk | WDT_WTCR_WTRE_Msk;

    /* Enable WDT interrupt function */
    WDT_EnableInt();

    /* Enable WDT NVIC */
    NVIC_EnableIRQ(WDT_IRQn);
    SYS_LockReg();
    pr_debug("wdt init\n");
}

void wdt_reset( void )
{
    SYS_UnlockReg();
    WDT_RESET_COUNTER();
    SYS_LockReg();
}


