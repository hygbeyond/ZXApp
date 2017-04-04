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

#include "../drive/nuc_can.h"
#include "app_cfg.h"
#include "bsp.h"
#include "hardware.h"
#include "protocol.h"
#include "data.h"
#include "user.h"
#include "nuc_wdt.h"




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


OSTASK_STK( Dis,   256 );     /* 液晶显示，人机交互 */
OSTASK_STK( Can,   192 );     /* can总线处理        */
OSTASK_STK( DataCJ, 96 );     /*                    */

uint16   MemStart __NOINIT__;

#define VERSION_STR    __stringify(HNAME) " " __stringify(VERSION) "." __stringify( PATCHLEVEL )

//******************************************************************************
void Init_Devices(void);
extern void GotoISP(void );
void data_cmd( void );

static int printk_put( int ch, FILE * p_file)
{
    if( ch == '\n' )
        Uart_SendByte( '\r' );
    Uart_SendByte( ch );
    return 0;
}


int main (void)
{
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
    int dly;

    int delay;
    Init_Devices();

    Pro_Init();
    UsrInit();

    if(MemStart != 0x55AA)
    {
        Data_Init(1);
        MemStart = 0x55AA;
    }else
    	Data_Init(0);

    dly = OSTASK_CREATE( Can,   0 );
    ASSERT(dly == OS_NO_ERR);
   
    dly = OSTASK_CREATE( DataCJ, 4 );
    ASSERT( dly == OS_NO_ERR );

    dly = HZ/10;

    TRACE( "\n%s\n"
            VERSION_STR \
            "\ncmd>"
            , GetObjName( pUsrCfg->type )
          );
    while(1)
    {
        wdt_reset();
        shell( Uart_GetChar_nb() );
        OSTimeDly(HZ/10);
        if( --dly == 0 )
        {
//        	LED_NOT;
            dly = HZ/10;
            if( Data_PD() )
            {
                LED_ON;
            }else
            	LED_OFF;
            Data_Trans();
        }

        if( KEY_IN == 0)
        {
        	data_cmd();
            LED_OFF; //LED_H;
            OSTimeDly(10);
            if( KEY_IN == 0 )
            {
                LED_ON;
                delay = 0;
                while( KEY_IN == 0 )
                {
                    wdt_reset();
                    OSTimeDly(10);
                    delay++;
                    if( delay > 20 )
                    {
                        LED_OFF;
                        if( SetAddress )
                        {
                            UsrSetAddr( SetAddress );
                            CAN_Init( UsrSetAddr( SetAddress ), 10000);
                            TK_Trans( TCMDDATA_SETADDR );
                            SetAddress = 0;
                            wdt_reset();
                            OSTimeDly(HZ);
                            wdt_reset();
                            OSTimeDly(HZ);
                            wdt_reset();
                            break;
                        }
                    }
                }
            }
            LED_OFF;
        }

    }
}

void Can_Task(void)
{
    CAN_msg msg;
    int dly = 0;

    while(1)
    {
		CAN_GetErr(0);

		if (++dly > HZ * 5) {
			dly = 0;
            TK_AutoTrans();
		}

		if (CAN_WriteAll())
			dly = 0;

		if (CAN_rdMSG(&msg)) {
			DEBUG2("can %08x, %08x %08x\n", msg.id, msg.DataA, msg.DataB);
			CanCmdMsg(&msg);
		}
		OSTimeDly(1);
	}
}

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

    IO_Init(); 
   
    Uart_Init( 9600 );
    printk_set( printk_put, NULL );

    wdt_init();
    CAN_Init( 1 , 10000 );
}

#include "pcommand.h"

/*
 * 最低任务钩子函数，主要是低功耗控制
 */
#if OS_TASK_HOOK_EN > 0
void OS_TaskIdle_Hook(void)
{
    __WFI();
}
#endif 


int reset_cmd(void);
int reset_cmd(void)
{
    MemStart = 0;
    BSP_Reset();
	return 0;
}
PSHELL_CMD(reset, reset_cmd, "reset");

