/*
********************************************************************************
**				                   MicroOS(ARM)
**                      The Real-Time Kernel(For ARM cortex-m0)
**
**                     (c) Copyright 2006-2007, hygbeyond
**                             All Rights Reserved
**
**                                    V0.9
**                       基于周立功TinyM0  LPC1114/301
********************************************************************************
**创 建 人：胡勇刚
**文 件 名：main.c
**创建日期：2010-11-23
**描    述：移植到LPC1114，实现了任务切换，shell接口。
**  main源代码，实现了基于MicroOS（ARM）的任务函数。
**  OSTASK(Dis_Task);  显示处理任务
**  OSTASK(Key_Task);  键盘扫描任务
**  OSTASK(BJ_Task);   报警处理任务
**  OSTASK(GetValue_Task);  采集数据任务
********************************************************************************
********************************************************************************
*/

#include "MicroOS.h"
#include "app_cfg.h"
#include "bsp.h"
#include "uart.h"
#include "gui_config.h"
#include "wdt.h"
#include "hardware.h"
#include "uudata.h"
#include "key.h"

#include "lpcprog.h"

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
/*
 * 任务优先级定义
 */
#define LCD_PRO             7
#define SHELL_PRO           6
#define DIS_PRO             5
#define CANTRANS_PRO        4
#define BJ_PRO              3
#define GET_PRO             2
#define FILE_PRO            1
#define KEY_PRO             0


OSTASK_STK( Dis,   512);     /* 液晶显示，人机交互 */
//OSTASK_STK( Key,   256 );

//volatile int __STACK__ testStack;

OS_EVENT *Key_Mbox;			//按键输入消息
OS_EVENT *ProcessingMbox;	//试验处理消息

uint16 __NOINIT__  MemStart;
uint8  __NOINIT__  Sound;
volatile int ADC_Lock;      /* 锁AD读取 */

#define VERSION_STR    __stringify(HNAME) " " __stringify(VERSION) "." __stringify( PATCHLEVEL )

//******************************************************************************
void Init_Devices(void);
extern void GotoISP(void );

static int printk_put( int ch, FILE * p_file)
{
    if( ch == '\n' )
        Uart_SendByte( '\r' );
    Uart_SendByte( ch );
    return 0;
}


int main (void)
{
	GotoISP();
	OSInit();
    OSTASK_CREATE( Dis, 5 );
    OSStart();
	return 0;
}

/*
 * 功能: shell接口
 */
void shell(int);
#define LCDLED_DLY        (30*2)                       /*设置液晶背光显示时间*/
void Dis_Task(void)
{
    int ret;
    uint8 key = 0;
    uint8 err;
    uint8 *pd;

    Init_Devices();
    uuInit();

    GUI_Init();

    ProcessingMbox   = OSMboxCreate(NULL);	
    Key_Mbox         = OSMboxCreate(NULL);

    if(MemStart != 0x55AA)
    {
        MemStart = 0x55AA;
    }

//    ret = OSTASK_CREATE( Key,   0 );
//    ASSERT( ret == OS_NO_ERR );

    GUI_Goto( NULL );
    ret = 20;
    NxpDownloadExt();

    GUI_Goto( NULL );

    while(1)
    {
    	while( (ret = Uart_GetChar_nb()) >= 0 ){
    		shell( ret );
    	}
    	OSTimeDly(1);
	}
}

//void Key_Task( void )
//{
//    static uint8 key;
//    int tmp;
//    int dly;
//    while( 1 )
//    {
//    	while( (tmp = Uart_GetChar_nb()) >= 0 ){
//    		shell( tmp );
//    	}
//    	OSTimeDly(1);
//    }
//}

/*
 * 初始化硬件
 */
void Init_Devices(void)
{
    OS_SREG_VAR;

    OS_ENTER_CRITICAL();
    BSP_Init();
    OS_CPU_SysTickInit( FCCLK/HZ );
    OS_EXIT_CRITICAL();

#if OS_STAT_EN > 0
    OSStatInit();
#endif

    IO_Init();

    Uart_Init( UART_BPS );
    printk_set( printk_put, NULL );
}

#include "pcommand.h"

/*
 * 最低任务钩子函数，主要是低功耗控制
 */
#if OS_TASK_HOOK_EN > 0
void OS_TaskIdle_Hook(void)
{
#if OS_STAT_EN > 0
#else
    __WFI();
#endif
}
#endif 

int reset_cmd(void);
int reset_cmd(void)
{
    wdt_reset();
    MemStart = 0;
    BSP_Reset();
	return 0;
}
PSHELL_CMD(reset, reset_cmd, "reset");


