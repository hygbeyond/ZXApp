
#include "mytype.h"
#include "app_cfg.h"

typedef uint32 (*Tsvc_func)(uint32, uint32, uint32, uint32);

__WEAK__
const void * svc_function[MAX_SVC_SIZE]={NULL};

/*
 * pwdSF[0] = R0,   pwdSF[1] = R1
 * pwdSF[2] = R2,   pwdSF[3] = R3
 * pwdSF[4] = R12,  pwdSF[5] = LR
 * pwdSF[6] = 返回地址（入栈的PC）
 * pwdSF[7] = xPSR
 */
uint32 svc_handler_export( uint32 * pwdSF);
uint32 svc_handler_export( uint32 * pwdSF)
{
    Tsvc_func svc_func = NULL;
	uint32 svc_number;
    /*
	uint32 svc_r0;
	uint32 svc_r1;
	uint32 svc_r2;
	uint32 svc_r3;
	svc_r0 = pwdSF[0];
	svc_r1 = pwdSF[1];
	svc_r2 = pwdSF[2];
	svc_r3 = pwdSF[3];	
	*/
    svc_number = ((char*) pwdSF[6])[-2];
	
    //pr_debug("SVC number = %d\n", svc_number);
    if( svc_number < MAX_SVC_SIZE )
        svc_func = svc_function[svc_number];

    if( svc_func != NULL)
        pwdSF[0] = svc_func( pwdSF[0], pwdSF[1], pwdSF[2], pwdSF[3]);
    else
        pwdSF[0] = 0;

    return 0;
}


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


