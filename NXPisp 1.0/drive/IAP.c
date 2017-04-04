/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           IAP.c
** Last modified Date:  2010-02-04
** Last Version:        V1.0
** Descriptions:        IAP升级-IAP函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Lanwuqiang
** Created date:        2010-02-05
** Version:             V1.0
** Descriptions:        添加用户应用程序
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
** Rechecked by:        
*********************************************************************************************************/
#include "IAP.h"
#include "app_cfg.h"
#include "pcommand.h"

/* 
 *  定义CCLK值大小，单位为KHz 
 */
#define  IAP_FCCLK            (25000)
#define  IAP_ENTER_ADR        0x1FFF1FF1                                /* IAP入口地址定义              */

typedef void (*IAP)(uint32 [],uint32 []);


/*
 *  定义函数指针  
 */
#define IAP_Entry(a, b)  ((IAP) IAP_ENTER_ADR)(a, b)

#if 0
uint32 command[5];
uint32 result[4];

/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP操作扇区选择，命令代码50
** input parameters:    sec1:           起始扇区
**                      sec2:           终止扇区
** output parameters:   result[0]:    IAP操作状态码,IAP返回值
** Returned value:      result[0]:    IAP操作状态码,IAP返回值
*********************************************************************************************************/
uint32  sectorPrepare (uint8 sec1, uint8 sec2)
{  
    command[0] = IAP_Prepare;                                           /* 设置命令字                   */
    command[1] = sec1;                                                  /* 设置参数                     */
    command[2] = sec2;                            
    IAP_Entry(command, result);                                    /* 调用IAP服务程序              */
   
    return (result[0]);                                               /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        复制RAM的数据到FLASH，命令代码51
** input parameters:    dst:            目标地址，即FLASH起始地址。以512字节为分界
**                      src:            源地址，即RAM地址。地址必须字对齐
**                      no:             复制字节个数，为512/1024/4096/8192
** output parameters:   result[0]:    IAP操作状态码,IAP返回值
** Returned value:      result[0]:    IAP操作状态码,IAP返回值
*********************************************************************************************************/
uint32  ramToFlash (uint32 dst, uint32 src, uint32 no)
{  
    command[0] = IAP_RAMTOFLASH;                                        /* 设置命令字                   */
    command[1] = dst;                                                   /* 设置参数                     */
    command[2] = src;
    command[3] = no;
    command[4] = IAP_FCCLK;
    IAP_Entry(command, result);                                    /* 调用IAP服务程序              */
    
    return (result[0]);                                               /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        扇区擦除，命令代码52
** input parameters:    sec1            起始扇区
**                      sec2            终止扇区92
** output parameters:   result[0]:    IAP操作状态码,IAP返回值
** Returned value:      result[0]:    IAP操作状态码,IAP返回值
*********************************************************************************************************/
uint32  sectorErase (uint8 sec1, uint8 sec2)
{  
    command[0] = IAP_ERASESECTOR;                                       /* 设置命令字                   */
    command[1] = sec1;                                                  /* 设置参数                     */
    command[2] = sec2;
    command[3] = IAP_FCCLK;
    IAP_Entry(command, result);                                    /* 调用IAP服务程序              */
   
    return (result[0]);                                               /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       blankChk
** Descriptions:        扇区查空，命令代码53
** input parameters:    sec1:           起始扇区
**                      sec2:           终止扇区92
** output parameters:   result[0]:    IAP操作状态码,IAP返回值
** Returned value:      result[0]:    IAP操作状态码,IAP返回值
*********************************************************************************************************/
uint32  blankChk (uint8 sec1, uint8 sec2)
{  
    command[0] = IAP_BLANKCHK;                                          /* 设置命令字                   */
    command[1] = sec1;                                                  /* 设置参数                     */
    command[2] = sec2;
    IAP_Entry(command, result);                                    /* 调用IAP服务程序              */

    return (result[0]);                                               /* 返回状态码                   */
}

/*********************************************************************************************************
** Function name:       dataCompare
** Descriptions:        校验数据，命令代码56
** input parameters:    dst:            目标地址，即RAM/FLASH起始地址。地址必须字对齐
**                      src:            源地址，即FLASH/RAM地址。地址必须字对齐
**                      no:             复制字节个数，必须能被4整除
** output parameters:   result[0]:    IAP操作状态码,IAP返回值
** Returned value:      result[0]:    IAP操作状态码,IAP返回值
*********************************************************************************************************/
uint32  dataCompare (uint32 dst, uint32 src, uint32 no)
{  
    command[0] = IAP_COMPARE;                                           /* 设置命令字                   */
    command[1] = dst;                                                   /* 设置参数                     */
    command[2] = src;
    command[3] = no;
    IAP_Entry(command, result);                                    /* 调用IAP服务程序              */

    return (result[0]);                                               /* 返回状态码                   */
}


/*
 * CPU ID
 */
uint32  parIdRead (void)
{  
    command[0] = IAP_READPARTID;                                        /* 设置命令字                   */
    IAP_Entry(command, result);                                  /* 调用IAP服务程序              */

    return (result[1]);                                               /* 返回状态码                   */
}

/*
 * bootload 版本
 */
uint32  codeIdBoot (void)
{  
    command[0] = IAP_BOOTCODEID;                                        /* 设置命令字                   */
    IAP_Entry (command, result);                                  /* 调用IAP服务程序              */

    return (result[1]);                                                /* 返回状态码                   */
}
#endif

uint32 __NOINIT__ isp_en;
void reset_cmd();
void   GotoISP(void)
{
    uint32 command[4];
    switch( isp_en )
    {
    case 0x55AA55AAUL:
        isp_en = 0x55AA55ABUL;
        command[0] = IAP_ReinvokeISP;                                  /* 设置命令字                   */
        IAP_Entry (command, NULL);                                   /* 调用IAP服务程序              */
        while(1);
    	break;
    case 0:
    	break;
    default:
    //case 0x55AA55ABUL:
    	isp_en = 0;
    	reset_cmd();
    	break;
    }
}

#include "bsp.h"

static int isp_cmd( int args, char **argv )
{
    printk("goto isp \n");
    isp_en = 0x55AA55AAUL;
    BSP_Reset();
    return 0;
}
PSHELL_CMD( isp, isp_cmd, "isp");


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/

