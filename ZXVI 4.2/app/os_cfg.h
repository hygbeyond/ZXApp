/*
********************************************************************************
**				                   MicroOS(AVR)
**                      The Real-Time Kernel(For WinAVR)
**
**                     (c) Copyright 2006-2010, hygbeyond
**                             All Rights Reserved
**
**                                    V0.9
**                      
********************************************************************************
**�� �� �ˣ����¸�
**�� �� ����OS_CFG.h
**�������ڣ�2006-6-18 15:57
**��    ����ϵͳ����״̬
********************************************************************************
*/


#ifndef __OSCFG_H__
#define __OSCFG_H__

#define OS_TASKS                8      //�趨�������������
										//�����������

#define OS_TIME_PRO             OS_TASKS-1 
#define OS_TIME_STK_SIZE        64

#define OS_TASK_HOOK_EN         1
#define OS_TASK_IDLE_STK_SIZE   32

#define OS_TICKS_PER_SEC        200      
#define HZ                      OS_TICKS_PER_SEC
 
#define OS_EVENT_SIZE           8

#define OS_ARG_CHK_EN			0    /* Enable (1) or Disable (0) argument checking                  */

#define OS_SEM_EN               1
#define OS_MBOX_EN              1

#define OS_Q_EN                 0
#define OS_Q_BUFFER_SIZE		32
#define OS_Q_SIZE				1
#define OS_Q_EXT_EN				0	/*��Ϣ������չ ���Ե���������пռ�*/

#define OS_MEM_EN				0
#define OS_MAX_MEM_PART			3


#define OS_TIME_EN              0
#define OS_TIME_SIZE            8


#define  OSMAP_INFLASH          0  /*����flash��*/

#if OS_TASKS > 16
	#error "�������������������������!!!!"
#endif

#endif /*__OSCFG_H__*/
