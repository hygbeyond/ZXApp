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
**创 建 人：胡勇刚
**文 件 名：OS_CFG.h
**创建日期：2006-6-18 15:57
**描    述：系统配置状态
********************************************************************************
*/


#ifndef __OSCFG_H__
#define __OSCFG_H__

#define OS_TASKS                8      //设定运行任务的数量
										//不算空闲任务

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
#define OS_Q_EXT_EN				0	/*消息队列扩展 可以单独分配队列空间*/

#define OS_MEM_EN				0
#define OS_MAX_MEM_PART			3


#define OS_TIME_EN              0
#define OS_TIME_SIZE            8


#define  OSMAP_INFLASH          0  /*表在flash中*/

#if OS_TASKS > 16
	#error "任务的数量大于最大的任务数量!!!!"
#endif

#endif /*__OSCFG_H__*/
