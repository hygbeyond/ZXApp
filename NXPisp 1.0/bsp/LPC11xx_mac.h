

#ifndef __LPC11XX_MAC_H__
#define __LPC11XX_MAC_H__

#include "LPC11xx.h"
#include "mytype.h"

/*
 * 位操作宏定义区
 */
#ifndef BIT
#define BIT(a)         (1<<(a))
#endif

#ifndef SBI
#define SBI(port, bit) ((port) |=  (1 << (bit)))
#endif /*SBI*/

#ifndef CBI
#define CBI(port, bit) ((port) &= ~(1 << (bit)))
#endif /*CBI*/

#ifndef PBI
#define PBI(pin,  bit) (((pin) &   (1 << (bit))))
#endif /*PBI*/

#ifndef NBI
#define NBI(port, bit) ((port) ^=  (1 << (bit)))
#endif /*NBI*/



/*
 * GPIO 位操作宏定义区,不在位带操作范围内，通过C语言位操作方式操作
 */
#ifndef MYLIB_CONCAT           
#define MYLIB_CONCAT(a, b)            a ## b
#define MYLIB_CONCAT_EXPANDED(a, b)   MYLIB_CONCAT(a, b)
#endif

#ifndef SBI_P
#define DATA_PORT(name)               MYLIB_CONCAT(LPC_GPIO, name)->DATA
#define DDRP(name)                    MYLIB_CONCAT(LPC_GPIO, name)->DIR
#define DDRPORT( name )               DDRP( name )

#define PIN(name )                    DATA_PORT(name)
#define PORT( name )                  DATA_PORT( name )

#define PORT_IN( pin, bit )           CBI( DDRP( pin ), bit)
#define PORT_OUT( pin, bit)           SBI( DDRP( pin ), bit)
#define PBI_P(pin, bit)               PBI( PIN( pin ), bit)
#define NBI_P(pin, bit)               if(PBI_P(pin,bit)) { CBI_P(pin,bit); } else { SBI_P(pin,bit); }
#define SBI_P(pin, bit)               SBI( DATA_PORT(pin), bit )
#define CBI_P(pin, bit)               CBI( DATA_PORT(pin), bit )


#define SETPORT( pin, bit )           SBI_P( pin, bit )
#define CLRPORT( pin, bit )           CBI_P( pin, bit )
#endif


/*
 * GPIO 位操作宏定义结束
 */
#define SYSAHBCLKCTRL_SET( bit )     SBI( LPC_SYSCON->SYSAHBCLKCTRL, bit )
#define SYSAHBCLKCTRL_CLR( bit )     CBI( LPC_SYSCON->SYSAHBCLKCTRL, bit )

#define AHBCLK_SYS                           0 
#define AHBCLK_ROM                           1
#define AHBCLK_RAM                           2 
#define AHBCLK_FLASH1                        3 
#define AHBCLK_FLASH2                        4
#define AHBCLK_I2C                           5 
#define AHBCLK_GPIO                          6
#define AHBCLK_CT16B0                        7
#define AHBCLK_CT16B1                        8
#define AHBCLK_CT32B0                        9
#define AHBCLK_CT32B1                        10
#define AHBCLK_SSP0                          11
#define AHBCLK_UART                          12
#define AHBCLK_ADC                           13
#define AHBCLK_USB_RGG                       14
#define AHBCLK_WDT                           15
#define AHBCLK_IOCON                         16
#define AHBCLK_SSP1                          18


/*********************************************************************************************************
**  NVIC Interrupt Priority   NVIC中断优先级
*********************************************************************************************************/
#ifndef PRIO_ZERO
#define PRIO_ZERO            (0x00)                                     /* 优先级为0                    */
#define PRIO_ONE             (0x01 << 5)                                /* 优先级为1                    */
#define PRIO_TWO             (0x02 << 5)                                /* 优先级为2                    */
#define PRIO_THREE           (0x03 << 5)                                /* 优先级为3                    */
#define PRIO_FOUR            (0x04 << 5)                                /* 优先级为4                    */
#define PRIO_FIVE            (0x05 << 5)                                /* 优先级为5                    */
#define PRIO_SIX             (0x06 << 5)                                /* 优先级为6                    */
#define PRIO_SEVEN           (0x07 << 5)                                /* 优先级为7                    */
#endif
               


#endif



