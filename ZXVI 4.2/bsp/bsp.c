


#define BS_GLOBAL
#include "mytype.h"
#include "bsp.h"
#include "bsp_int.h"
#include "app_cfg.h"


extern void *__GpvVectorTable[];

void BSP_Init(void)
{
    SystemInit();
}

/*
 * BSP_Reset   ͨ�������λ��Ƭ��
 */
void BSP_Reset (void)
{   /*  ��ϵͳ�ϵ縴λ��ͬӲ����λ  */
    NVIC_SystemReset();
}



/*
 * end bsp.c
 */


