

#include "hardware.h"
#include "MicroOS.h"


#define LCD_CS_H	CS0_H
#define LCD_CS_L	CS0_L
#define LCD_A0_H	CD_H
#define LCD_A0_L	CD_L

#define LCD_SPICLK     500000
#define LED_SPICLK     1000000

OS_EVENT *SPI_Sem;
uint8 led_out;

#define IOSET(a)           ( a | (2<<3) | (1<<7) )

void IO_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable module clock */
    CLK->APBCLK = CLK_APBCLK_UART0_EN_Msk
        | CLK_APBCLK_TMR0_EN_Msk
        | CLK_APBCLK_ADC_EN_Msk
        | CLK_APBCLK_WDT_EN_Msk
        | CLK_APBCLK_CAN0_EN_Msk;

    /* Peripheral clock source */
    CLK->CLKSEL1 = CLK_CLKSEL1_UART_S_HXT | CLK_CLKSEL1_TMR0_S_HXT | CLK_CLKSEL1_ADC_S_HXT | CLK_CLKSEL1_WDT_S_LIRC;
	
    /* ADC clock source is 12MHz, set divider to 6, ADC clock is 12/6 MHz */
    CLK->CLKDIV  = (CLK->CLKDIV & ~CLK_CLKDIV_ADC_N_Msk) | (((6) - 1) << CLK_CLKDIV_ADC_N_Pos);

    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    GPIO_DISABLE_DIGITAL_PATH(PA, 0xFF);
    SYS->VREFCR = (1<<4);  //ADC 参考电压源来自AVDD

    SYS->GPA_MFP = SYS_GPA_MFP_PA7_ADC7 | SYS_GPA_MFP_PA6_ADC6 | SYS_GPA_MFP_PA5_ADC5 | SYS_GPA_MFP_PA4_ADC4
        | SYS_GPA_MFP_PA3_ADC3 | SYS_GPA_MFP_PA2_ADC2 | SYS_GPA_MFP_PA1_ADC1 | SYS_GPA_MFP_PA0_ADC0;

    SYS->GPB_MFP = SYS_GPB_MFP_PB1_UART0_TXD | SYS_GPB_MFP_PB0_UART0_RXD;
    SYS->GPC_MFP = 0x00000000;
    SYS->GPD_MFP = SYS_GPD_MFP_PD7_CAN0_TXD | SYS_GPD_MFP_PD6_CAN0_RXD;
    SYS->GPE_MFP = 0x00000000;
    SYS->GPF_MFP = SYS_GPF_MFP_PF7_ICE_DAT | SYS_GPF_MFP_PF6_ICE_CLK | SYS_GPF_MFP_PF5_I2C0_SCL 
        | SYS_GPF_MFP_PF4_I2C0_SDA | SYS_GPF_MFP_PF1_XT1_IN | SYS_GPF_MFP_PF0_XT1_OUT;

    /* Disable the GPA0 - GPA7 digital input path to avoid the leakage current. */
    DDRP( B ) |= DDRMOD( 4, 1) | DDRMOD( 5, 1) | DDRMOD( 15, 1)  ;

    /* Lock protected registers */
    SYS_LockReg();
}

