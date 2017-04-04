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


OSTASK_STK( Dis,   512);     /* Һ����ʾ���˻����� */
//OSTASK_STK( Key,   256 );

//volatile int __STACK__ testStack;

OS_EVENT *Key_Mbox;			//����������Ϣ
OS_EVENT *ProcessingMbox;	//���鴦����Ϣ

uint16 __NOINIT__  MemStart;
uint8  __NOINIT__  Sound;
volatile int ADC_Lock;      /* ��AD��ȡ */

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
 * ����: shell�ӿ�
 */
void shell(int);
#define LCDLED_DLY        (30*2)                       /*����Һ��������ʾʱ��*/
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
 * ��ʼ��Ӳ��
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
 * ��������Ӻ�������Ҫ�ǵ͹��Ŀ���
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


