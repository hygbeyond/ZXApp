/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           IAP.c
** Last modified Date:  2010-02-04
** Last Version:        V1.0
** Descriptions:        IAP����-IAP����
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Lanwuqiang
** Created date:        2010-02-05
** Version:             V1.0
** Descriptions:        ����û�Ӧ�ó���
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
 *  ����CCLKֵ��С����λΪKHz 
 */
#define  IAP_FCCLK            (25000)
#define  IAP_ENTER_ADR        0x1FFF1FF1                                /* IAP��ڵ�ַ����              */

typedef void (*IAP)(uint32 [],uint32 []);


/*
 *  ���庯��ָ��  
 */
#define IAP_Entry(a, b)  ((IAP) IAP_ENTER_ADR)(a, b)

#if 0
uint32 command[5];
uint32 result[4];

/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP��������ѡ���������50
** input parameters:    sec1:           ��ʼ����
**                      sec2:           ��ֹ����
** output parameters:   result[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      result[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32  sectorPrepare (uint8 sec1, uint8 sec2)
{  
    command[0] = IAP_Prepare;                                           /* ����������                   */
    command[1] = sec1;                                                  /* ���ò���                     */
    command[2] = sec2;                            
    IAP_Entry(command, result);                                    /* ����IAP�������              */
   
    return (result[0]);                                               /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        ����RAM�����ݵ�FLASH���������51
** input parameters:    dst:            Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**                      src:            Դ��ַ����RAM��ַ����ַ�����ֶ���
**                      no:             �����ֽڸ�����Ϊ512/1024/4096/8192
** output parameters:   result[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      result[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32  ramToFlash (uint32 dst, uint32 src, uint32 no)
{  
    command[0] = IAP_RAMTOFLASH;                                        /* ����������                   */
    command[1] = dst;                                                   /* ���ò���                     */
    command[2] = src;
    command[3] = no;
    command[4] = IAP_FCCLK;
    IAP_Entry(command, result);                                    /* ����IAP�������              */
    
    return (result[0]);                                               /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        �����������������52
** input parameters:    sec1            ��ʼ����
**                      sec2            ��ֹ����92
** output parameters:   result[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      result[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32  sectorErase (uint8 sec1, uint8 sec2)
{  
    command[0] = IAP_ERASESECTOR;                                       /* ����������                   */
    command[1] = sec1;                                                  /* ���ò���                     */
    command[2] = sec2;
    command[3] = IAP_FCCLK;
    IAP_Entry(command, result);                                    /* ����IAP�������              */
   
    return (result[0]);                                               /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       blankChk
** Descriptions:        ������գ��������53
** input parameters:    sec1:           ��ʼ����
**                      sec2:           ��ֹ����92
** output parameters:   result[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      result[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32  blankChk (uint8 sec1, uint8 sec2)
{  
    command[0] = IAP_BLANKCHK;                                          /* ����������                   */
    command[1] = sec1;                                                  /* ���ò���                     */
    command[2] = sec2;
    IAP_Entry(command, result);                                    /* ����IAP�������              */

    return (result[0]);                                               /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       dataCompare
** Descriptions:        У�����ݣ��������56
** input parameters:    dst:            Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���
**                      src:            Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
**                      no:             �����ֽڸ����������ܱ�4����
** output parameters:   result[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      result[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32  dataCompare (uint32 dst, uint32 src, uint32 no)
{  
    command[0] = IAP_COMPARE;                                           /* ����������                   */
    command[1] = dst;                                                   /* ���ò���                     */
    command[2] = src;
    command[3] = no;
    IAP_Entry(command, result);                                    /* ����IAP�������              */

    return (result[0]);                                               /* ����״̬��                   */
}


/*
 * CPU ID
 */
uint32  parIdRead (void)
{  
    command[0] = IAP_READPARTID;                                        /* ����������                   */
    IAP_Entry(command, result);                                  /* ����IAP�������              */

    return (result[1]);                                               /* ����״̬��                   */
}

/*
 * bootload �汾
 */
uint32  codeIdBoot (void)
{  
    command[0] = IAP_BOOTCODEID;                                        /* ����������                   */
    IAP_Entry (command, result);                                  /* ����IAP�������              */

    return (result[1]);                                                /* ����״̬��                   */
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
        command[0] = IAP_ReinvokeISP;                                  /* ����������                   */
        IAP_Entry (command, NULL);                                   /* ����IAP�������              */
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

