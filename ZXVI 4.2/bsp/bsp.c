


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
 * BSP_Reset   通过软件复位单片机
 */
void BSP_Reset (void)
{   /*  此系统上电复位等同硬件复位  */
    NVIC_SystemReset();
}



/*
 * end bsp.c
 */


