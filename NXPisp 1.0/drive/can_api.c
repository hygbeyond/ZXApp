

#define CAN_API_GLOBAL
#include "mytype.h"
#include "rom_api.h"
#include "bsp.h"
#include "app_cfg.h"
#include "microOS.h"
#include "can.h"


#define  CAN_TX_NUM             5

#define  LIST_BUF_SIZE         16
#include "mylist.h"

LIST_TYPE_EXT( TCANlist, CAN_MSG_OBJ, 16 );

TCANlist canList_rx;

static uint32   CANID;
static uint32   sendEn;
static uint32   CANErrCnt;
static uint32   CANErr;

#define CAN_BAUD            5000

#define SET_CFG             1


#if CAN_BAUD == 5000
/*
 * pCAN_clk = 48MHz/8
 */
//{15, 8, 4, 50}
//15, 4, 1, 60
//5K = pCAN_clk/(TSEG2+TSGE1+1)/BRP
#define TSEG2            8
#define TSEG1           15
#define BRP             50
#define SJW              1
#define CLKDIV           7

#elif CAN_BAUD == 20000
/*
 * pCAN_clk = 48MHz/2
 */
//{15, 8, 4, 50}
//15, 4, 1, 60
//20K = pCAN_clk/(TSEG2+TSGE1+1)/BRP
#define TSEG2            8
#define TSEG1           15
#define BRP             50
#define SJW              1
#define CLKDIV           1
#else
/*
 * pCAN_clk = 48MHz/2
 */
//{15, 8, 4, 50}
//15, 4, 1, 60
//20K = pCAN_clk/(TSEG2+TSGE1+1)/BRP
#define TSEG2            4
#define TSEG1            7
#define BRP             50
#define SJW              1
#define CLKDIV           2
#endif


#if (TSEG2 > 8) || (TSEG1 > 16) || (BRP > 64) || (SJW > 4)
#error  "CAN 波特率设置错误"
#endif 

uint32 CanClkInit[2] = {
    CLKDIV,	                                                    /* CANCLKDIV                    */
	((TSEG2-1)<<12) | ((TSEG1-1)<<8) | ((SJW-1)<<6) | (BRP-1)   /* CANBTR                       */
};


/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
void CAN_rx(uint8 msg_obj_num);
void CAN_tx(uint8 msg_obj_num);
void CAN_error(uint32 error_info);


/* 
*  注册回调函数
*/
const CAN_CALLBACKS callbacks = {
    CAN_rx,
	CAN_tx,
    CAN_error
};




/*********************************************************************************************************
** Function name:       CAN_IRQHandler
** Descriptions:        CAN中断
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void CAN_IRQHandler(void);
void CAN_IRQHandler(void)
{
    PROM->pCANAPI->isr();
}

/*********************************************************************************************************
** Function name:       CAN_rx
** Descriptions:        数据接收处理，将接收到的报文再发送出去
** input parameters:    msg_obj_num  待读取报文编号
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void CAN_rx(uint8 msg_obj_num)
{
    static CAN_MSG_OBJ  msg_obj;
	msg_obj.msgobj = msg_obj_num;
	PROM->pCANAPI->can_receive(&msg_obj);
   
    if( !LIST_BUF_FULL( canList_rx ) ) // && ( msg_obj.id == 0 || msg_obj.id == CANID ))
    {
        LIST_BUF_WR( canList_rx, msg_obj);
    }
}

/*********************************************************************************************************
** Function name:       CAN_tx
** Descriptions:        发送数据处理
** input parameters:    msg_obj_num  待发送报文编号
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void CAN_tx(uint8 msg_obj_num)
{
    //DEBUG2("can tx%d ok\n", msg_obj_num );
    if( msg_obj_num == CAN_TX_NUM )
    {
        sendEn = 1;
    }
}
/* Private Variables ---------------------------------------------------------- */

/*********************************************************************//**
 * @brief 		Setting CAN baud rate (bps)
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @return 		None
 ***********************************************************************/
#if SET_CFG > 0
static void can_SetBaudrate ( uint32_t baudrate)
{
	uint32  result;
	uint8   NT, T1, T2;
    int    i;
	uint32 brp;

	result = FPCLK / baudrate;
    //pr_debug("result %d\n", result );
	/* Calculate suitable nominal time value
	 * NT (nominal time) = (TSEG1 + TSEG2 + 3)
	 * NT <= 24
	 * TSEG1 >= 2*TSEG2
	 */
	for(NT=24; NT>3; NT=NT-2)
	{
		if ((result%NT)==0)
        {
            brp = result / NT;
            T2 = NT/3;
            T2--;
            T1 = NT-T2-3;
            for(i=1; i<=16; i++ )
            {
                uint32 a = brp/i;
                if( (brp%i) == 0 && a <= 64 )
                {
                    //pr_debug("T1  %d\tT2  %d\tNT  %d\tBRP  %d\n", T1, T2, NT, brp );
                    //pr_debug("div %d\tBRP %d\n", i, a );
                    CanClkInit[0] = i-1;
                    CanClkInit[1] =  (((T2)<<12) | ((T1)<<8)) | ((SJW-1)<<6) | (a-1) ;
                    return;
                }
            }
        }
	}
    pr_debug("can set baudrate error\n");
}
#endif

static void CAN_reset(void)
{    
    CAN_MSG_OBJ  msg_obj;
    OS_SREG_VAR;
    OS_ENTER_CRITICAL();

    PROM->pCANAPI->init_can(  CanClkInit); 
    PROM->pCANAPI->config_calb( &callbacks);

    msg_obj.mode_id = 0 | CAN_MSGOBJ_EXT;
    msg_obj.mask    = 0;
    msg_obj.msgobj  = 1;
    PROM->pCANAPI->config_rxmsgobj(&msg_obj);

    msg_obj.mode_id = 0 ;
    msg_obj.mask    = 0;
    msg_obj.msgobj  = 2;
    PROM->pCANAPI->config_rxmsgobj(&msg_obj);
#if 0
    msg_obj.mode_id = CANID ;
    msg_obj.mask    = 0x7FF;
    msg_obj.msgobj  = 3;
    PROM->pCANAPI->config_rxmsgobj(&msg_obj);
#endif

    sendEn = 1;
    CANErr = 0;
    CANErrCnt = 0;

    OS_EXIT_CRITICAL();
    //pr_debug("CAN reset\n");
}

void CAN_error( uint32 error_info )
{
    CANErr |= error_info;
    DEBUG2("can err 0x%08x\n", CANErr);

}

void CAN_Init(uint32 addr, uint32 baudrate)
{
#if SET_CFG > 0 
    can_SetBaudrate( baudrate );
#endif

    LIST_BUF_RESET( canList_rx );
    SYSAHBCLKCTRL_SET( AHBCLK_CAN );

    CANID = addr;
    CAN_reset();

    /* Enable the CAN Interrupt */
    NVIC_EnableIRQ(CAN_IRQn);

    pr_debug("can init %d\n", baudrate);
}




/*
 * CAN_ReadMsg
 * 从can总线的接收缓存中读取一组数据。
 */
int CAN_rdMSG(CAN_msg *pMsg)
{
    if( LIST_BUF_COUNT( canList_rx ) )
    {
        LIST_BUF_RD( canList_rx , *pMsg);
        return TRUE;
    }
    return FALSE;
}


void CAN_Clean( void )
{
    CANErrCnt = 0;
}

int CAN_GetErr( int en )
{
    uint32 ret;
    ret = CANErr;
    if( ret & CAN_ERROR_BOFF || en )
    {
        CANErrCnt = 0;
        //pr_debug("error:0x%08x ", ret);
        CAN_reset();
    }
    return ret;
}

int CAN_wrMSG( CAN_msg *pMsg )
{
    OS_SREG_VAR;
    if( sendEn == 0 || pMsg == NULL )
        return FALSE;
    sendEn = 0;
    pMsg->mask   = 0;
    pMsg->msgobj = CAN_TX_NUM;
    OS_ENTER_CRITICAL();
    PROM->pCANAPI->can_transmit(pMsg);	                            /* 将接收到的数据回发出去       */
    OS_EXIT_CRITICAL();
    return TRUE;
}


//  file end

