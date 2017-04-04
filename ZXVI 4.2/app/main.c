/*
********************************************************************************
**				                   MicroOS(ARM)
**                      The Real-Time Kernel(For ARM cortex-m0)
**
**                     (c) Copyright 2006-2007, hygbeyond
**                             All Rights Reserved
**
**                                    V0.9
**                       ����������TinyM0  LPC1114/301
********************************************************************************
**�� �� �ˣ����¸�
**�� �� ����main.c
**�������ڣ�2010-11-23
**��    ������ֲ��LPC1114��ʵ���������л���shell�ӿڡ�
**  mainԴ���룬ʵ���˻���MicroOS��ARM������������
**  OSTASK(Dis_Task);  ��ʾ��������
**  OSTASK(Key_Task);  ����ɨ������
**  OSTASK(BJ_Task);   ������������
**  OSTASK(GetValue_Task);  �ɼ���������
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
  �궨��
*********************************************************************************************************/
/*
 * �������ȼ�����
 */
#define LCD_PRO             7
#define SHELL_PRO           6
#define DIS_PRO             5
#define CANTRANS_PRO        4
#define BJ_PRO              3
#define GET_PRO             2
#define FILE_PRO            1
#define KEY_PRO             0


OSTASK_STK( Dis,   256 );     /* Һ����ʾ���˻����� */
OSTASK_STK( Can,   192 );     /* can���ߴ���        */
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
 * ����: shell�ӿ�
 */
void shell(int);
#define LCDLED_DLY        (30*2)                       /*����Һ��������ʾʱ��*/
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
 * ��ʼ��Ӳ��
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
 * ��������Ӻ�������Ҫ�ǵ͹��Ŀ���
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

