

#include "mytype.h"
#include "pcommand.h"
#include "printk.h"
#include "MicroOS.h"
#include "OSstring.h"
#include "bsp.h"
#include "app_cfg.h"
#include "nuc_uart0.h"


uint32 debug_en = 3;
void wdt_reset( void );
int dbg_print( const char *fmt, ... )
{
	va_list ap;
	int i;
	static char buff[64];
	uint8 *str = (uint8*)buff;
	va_start(ap, fmt);
	i = vsnprintf(buff, 64, fmt, ap);
	va_end(ap);

	for(int a=0; a<HZ/5; a++)
	{
		wdt_reset();
		int ret = Uart_SendBuff(str, i);
		str += ret;
		i   -= ret;
		if( i==0 ) break;
		OSTimeDly(5);
	}
	return 1;
}

static int debug_cmd( int args, char **argv )
{
    if( args > 2 )
    {
        int num = atoi( argv[2] );
        if(!strcmp(argv[1], "en"))
        {
            if( num > 31 )
                num = 31;
            debug_en |= (1<<num);
        }else if(!strcmp(argv[1], "dis"))
        {
            if( num > 31 )
                num = 31;
            debug_en &= ~(1<<num);
        }else if(!strcmp(argv[1], "set"))
        {
            debug_en = num;
        }
    }else
    {
    	dbg_print("debug [en|dis|set] num 0x%08x\n", debug_en);
    }
    return 0;
}
PSHELL_CMD( debug, debug_cmd, "debug [en|dis|set] numb");

void OS_cmd( void );
void OS_cmd( void )
 {
	int i, j;
	/* 打印MicroOS的版本号 */
	dbg_print("  MicroOS  : V%d.%02d\n", OS_VERSION, OS_PATCHLEVEL);

	/* 打印滴答（心跳）速率，即每秒多少次心跳 */
	dbg_print("  TickRate  : %d\n", OS_TICKS_PER_SEC);

#if OS_STAT_EN > 0
	/* 打印CPU占用率 */
	dbg_print("  CPU Usage : %02d%%\n", OSCPUUsage);
#endif

	/* 打印CPU速度 */
	dbg_print("  CPU Speed : %d MHz\n", (int)(FPCLK / 1000000L));

	/* 打印CPU已运行时间（单位：滴答) */
	dbg_print("  #Ticks    : %d\n", OSTimeCnt);

	/* 打印上下文切换次数 (context switches) */
	dbg_print("  #CtxSw    : %d\n", OSCtxSwCtr);

	dbg_print("名称\tID号\t堆栈总数\t堆栈使用\t运行次数\n");
	for (i = 0; i <= OS_TASKS; i++) {
		OS_STK *stk = OS_TCBTbl[i].OSTaskStackEnd;
		int size = OS_TCBTbl[i].StkSize;
		if (size) {
			dbg_print("%-5s\t%-4d\t", OS_TCBTbl[i].name, OS_TCBTbl[i].ID);
			for (j = 0; j < size; j++) {
				if (*stk++ != 0)
					break;
			}
			dbg_print("%-5d\t%5d%%\t%-5d\n", size, (size - j) * 100 / size,
					OS_TCBTbl[i].TaskSchedCnt);
		}
	}
}
PSHELL_CMD( os, OS_cmd, "os");

void OSTaskStatHook(void)
{
//	static int i=0;
//	const char *str = "-\\|/";
	DEBUG9("CPU Usage : %02d%%\n", OSCPUUsage);
//	if( i>3 ) i=0;
}

//void OSTaskSwHook(void)
//{
//	OS_TCBCur->TaskSchedCnt++;
//	OSCtxSwCtr++;
//}

/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/
#ifndef BFSR        
#define BFSR          ((volatile uint32 *) 0xE000ED29)
#define UFSR          ((volatile uint32 *) 0xE000ED2A)
#define HFSR          ((volatile uint32 *) 0xE000ED2C)
#define DFSR          ((volatile uint32 *) 0xE000ED30)
#define AFSR          ((volatile uint32 *) 0xE000ED3C)
#endif
#if 0
void HardFaultHandle_c( uint32 *hardfault_args, uint32 i);
void HardFaultHandle_c( uint32 *hardfault_args, uint32 i)
{
    switch( i)
    {
        case 0:
            pr_debug("Handle hard fault ISR\n");
            break;
        case 1:
            pr_debug("Handle memory fault ISR\n");
            break;
        case 2:
            pr_debug("handle bus fault ISR\n");
            break;
        case 3:
            pr_debug( "Handle usage fault ISR\n");
            break;
        case 4:
            pr_debug("Handle spurious interrupt\n");
            break;
    }
    pr_debug("R0 = 0x%08x\n", hardfault_args[0]);
    pr_debug("R1 = 0x%08x\n", hardfault_args[1]);
    pr_debug("R2 = 0x%08x\n", hardfault_args[2]);
    pr_debug("R3 = 0x%08x\n", hardfault_args[3]);
    pr_debug("R12= 0x%08x\n", hardfault_args[4]);
    pr_debug("LR = 0x%08x\n", hardfault_args[5]);
    pr_debug("PC = 0x%08x\n", hardfault_args[6]);
    pr_debug("PSR= 0x%08x\n", hardfault_args[7]);
    pr_debug("BFAR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED38)));
    pr_debug("CFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED28)));
    pr_debug("HFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED2C)));
    pr_debug("DFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED30)));
    pr_debug("AFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED3C)));
    while(1);
}
#endif

__AFTER__
void HardFault_Handler(void );
void HardFault_Handler(void )
{
    uint32 *hardfault_args;

    pr_debug("Handle hard fault ISR\n");
    asm volatile( "mrs %0, MSP\n\t" :"=r"(hardfault_args) );
    if( hardfault_args == 0 )
        asm volatile( "mrs %0, PSP\n\t":"=r"(hardfault_args) );

    pr_debug("R0 = 0x%08x\n", hardfault_args[0]);
    pr_debug("R1 = 0x%08x\n", hardfault_args[1]);
    pr_debug("R2 = 0x%08x\n", hardfault_args[2]);
    pr_debug("R3 = 0x%08x\n", hardfault_args[3]);
    printk("R12= 0x%08x\n", hardfault_args[4]);
    printk("LR = 0x%08x\n", hardfault_args[5]);
    printk("PC = 0x%08x\n", hardfault_args[6]);
    printk("PSR= 0x%08x\n", hardfault_args[7]);
    pr_debug("BFAR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED38)));
    pr_debug("CFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED28)));
    pr_debug("HFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED2C)));
    pr_debug("DFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED30)));
    pr_debug("AFSR= 0x%08x\n", (*((volatile uint32 *) 0xE000ED3C)));
    while(1);
}

