

#include "mytype.h"
#include "bsp.h"
#include "app_cfg.h"
#include "nuc_can.h"


/*------- LPC11Cxx  CAN Register Definitions ---------------------------------*/

/*- CAN CMDREQ register ------------------------------------------------------*/
#define _CMDREQ_BUSY       (1UL << 15)

/*- CAN CTRL register --------------------------------------------------------*/
#define _CNTL_INIT         (1UL <<  0)
#define _CNTL_IE           (1UL <<  1) 
#define _CNTL_SIE          (1UL <<  2)
#define _CNTL_EIE          (1UL <<  3) 
#define _CNTL_DAR          (1UL <<  5)
#define _CNTL_CCE          (1UL <<  6) 
#define _CNTL_TEST         (1UL <<  7)

/*- CAN TEST register --------------------------------------------------------*/
#define _TEST_BASIC        (1UL <<  2)
#define _TEST_SILENT       (1UL <<  3)
#define _TEST_LBACK        (1UL <<  4)

/*- CAN STAT register --------------------------------------------------------*/
#define _STAT_LEC          (7UL <<  0)
#define _STAT_TXOK         (1UL <<  3) 
#define _STAT_RXOK         (1UL <<  4)
#define _STAT_EPASS        (1UL <<  5) 
#define _STAT_EWARN        (1UL <<  6)
#define _STAT_BOFF         (1UL <<  7)

/*- CAN CMDMASK register -----------------------------------------------------*/
#define	_CMDMASK_DATAB     (1UL <<  0)
#define	_CMDMASK_DATAA     (1UL <<  1)
#define	_CMDMASK_TREQ      (1UL <<  2)
#define	_CMDMASK_INTPND    (1UL <<  3)
#define	_CMDMASK_CTRL      (1UL <<  4)
#define	_CMDMASK_ARB       (1UL <<  5)
#define	_CMDMASK_MASK      (1UL <<  6)
#define	_CMDMASK_WR        (1UL <<  7)
#define _CMDMASK_RD        (0UL <<  7)

/*- CAN MSK1 register --------------------------------------------------------*/
#define _MSK1_MSK          (0xFFFF)

/*- CAN MSK2 register --------------------------------------------------------*/
#define _MSK2_MSK          (0x1FFF)
#define	_MSK2_MXTD         (1UL << 15)
#define	_MSK2_MDIR         (1UL << 14)

/*- CAN ARB1 register --------------------------------------------------------*/
#define _ARB1_ID           (0xFFFF)

/*- CAN ARB2 register --------------------------------------------------------*/
#define _ARB2_ID           (0x1FFF)
#define	_ARB2_DIR          (1UL << 13)
#define	_ARB2_XTD          (1UL << 14)
#define	_ARB2_MSGVAL       (1UL << 15)

/*- CAN MCTRL register -------------------------------------------------------*/
#define	_MCTRL_DLC         (0x0F)   
#define	_MCTRL_EOB         (1UL <<  7)
#define _MCTRL_TXRQST      (1UL <<  8)
#define	_MCTRL_RMTEN       (1UL <<  9)
#define	_MCTRL_RXIE        (1UL << 10)
#define	_MCTRL_TXIE        (1UL << 11)
#define _MCTRL_UMASK       (1UL << 12)
#define	_MCTRL_INTPND      (1UL << 13)
#define	_MCTRL_MSGLST      (1UL << 14)
#define	_MCTRL_NEWDAT      (1UL << 15)


#define SJW                 2

#define _MSG_OBJ_MAX        0x0020         /* 1..32 */

#define _STD_FORMAT         0x000007FF     /* 11 bit standard format */	
#define _EXT_FORMAT         0x1FFFFFFF     /* 29 bit extended format */
/* -- end LPC11Cxx  CAN Register Definitions -------------------------------- */

#define  CAN_TX_NUM             5

#define  LIST_BUF_SIZE         16
#include "mylist.h"

LIST_TYPE_EXT( TCANlist, CAN_msg, 16 );

TCANlist canList_rx;


#define tCAN     CAN0
#define LPC_CAN  CAN0


static uint32 can_baudrate;

CAN_msg       CAN_TxMsg;                         /* CAN message for sending */
CAN_msg       CAN_RxMsg;                         /* CAN message for receiving */   

unsigned int  CAN_TxRdy = 0;                     /* CAN HW ready to transmit a message */

/*----------------------------------------------------------------------------*/
/*  Get current bit rate                                                      */
/*----------------------------------------------------------------------------*/
static uint32_t GetCANBitRate(void)
{
    uint8_t u8Tseg1, u8Tseg2;
    uint32_t u32Bpr;

    u8Tseg1 = (tCAN->BTIME & CAN_BTIME_TSEG1_Msk) >> CAN_BTIME_TSEG1_Pos;
    u8Tseg2 = (tCAN->BTIME & CAN_BTIME_TSEG2_Msk) >> CAN_BTIME_TSEG2_Pos;
    u32Bpr  = (tCAN->BTIME & CAN_BTIME_BRP_Msk) | (tCAN->BRPE << 6);

    return (SystemCoreClock / (u32Bpr + 1) / (u8Tseg1 + u8Tseg2 + 3));
}


static int CAN_setup( void )
{
    uint8_t u8Tseg1, u8Tseg2;
    uint32_t u32Brp;
    uint32_t u32Value;

    /* Set the CAN to enter initialization mode and enable access bit timing register */
    tCAN->CON |= CAN_CON_INIT_Msk;
    tCAN->CON |= CAN_CON_CCE_Msk;


    SystemCoreClockUpdate();

    u8Tseg1 = 2;
    u8Tseg2 = 1;

    u32Value = SystemCoreClock / can_baudrate;

    while(1)
    {
        if(((u32Value % (u8Tseg1 + u8Tseg2 + 3)) == 0))
            break;
        if(u8Tseg1 < 7)
            u8Tseg2++;

        if((u32Value % (u8Tseg1 + u8Tseg2 + 3)) == 0)
            break;
        if(u8Tseg1 < 15)
            u8Tseg1++;
        else
        {
            u8Tseg1 = 2;
            u8Tseg2 = 1;
            break;
        }
    }

    u32Brp  = SystemCoreClock / (can_baudrate) / (u8Tseg1 + u8Tseg2 + 3) - 1;

    u32Value = ((uint32_t)u8Tseg2 << CAN_BTIME_TSEG2_Pos) | ((uint32_t)u8Tseg1 << CAN_BTIME_TSEG1_Pos) |
               (u32Brp & CAN_BTIME_BRP_Msk) | (tCAN->BTIME & CAN_BTIME_SJW_Msk);
    tCAN->BTIME = u32Value;
    tCAN->BRPE     = (u32Brp >> 6) & 0x0F;

    /* pre-initialize CAN message objects */ 
    for (int i = 0; i < _MSG_OBJ_MAX; i++ )
    {
        tCAN->IF[0].CMASK    = _CMDMASK_WR    | _CMDMASK_MASK | _CMDMASK_ARB | _CMDMASK_CTRL | _CMDMASK_DATAA | _CMDMASK_DATAB;
        tCAN->IF[0].MASK1    = 0x0;          // useless in basic mode
        tCAN->IF[0].MASK2    = 0x0;          // useless in basic mode
        tCAN->IF[0].ARB1     = 0x0;          // ID15~0
        tCAN->IF[0].ARB2     = 0x0;          // MsgVal, eXt, xmt, ID28~16
        tCAN->IF[0].MCON     = 0x0;          // DLC
        tCAN->IF[0].DAT_A1   = 0x0;          // data0,1
        tCAN->IF[0].DAT_A2   = 0x0;          // data2,3
        tCAN->IF[0].DAT_B1   = 0x0;          // data4,5
        tCAN->IF[0].DAT_B2   = 0x0;          // data6,7
        tCAN->IF[0].CREQ     = i+1;          // set bit15 for sending

        while (tCAN->IF[0].CREQ & _CMDREQ_BUSY);
    }

    tCAN->STATUS = 0;                            /* reset CAN status register */

    tCAN->CON = (tCAN->CON & 0xF1) 
                |  CAN_CON_IE_Msk
                // |  CAN_CON_SIE_Msk
                |  CAN_CON_EIE_Msk;

    /* Set the CAN to leave initialization mode */
    tCAN->CON &= (~(CAN_CON_INIT_Msk | CAN_CON_CCE_Msk));
    while(tCAN->CON & CAN_CON_INIT_Msk); /* Check INIT bit is released */

    NVIC_EnableIRQ(CAN0_IRQn);                     /* enable CAN interrupt */

    CAN_TxRdy  = 1;  

    CAN_wrFilter( 0, 0, EXTENDED_FORMAT); 
    CAN_wrFilter( 0, 0, STANDARD_FORMAT); 

    return (GetCANBitRate());
}

int CAN_init( uint32 baud )
{
    can_baudrate = baud;
    return CAN_setup();
}

void CAN_Init(uint32 addr, uint32 baud)
{
    LIST_BUF_RESET( canList_rx );
    CAN_init( baud );
}

/*----------------------------------------------------------------------------
  set the testmode
 *----------------------------------------------------------------------------*/
void CAN_testmode (void) {

    //tCAN->CON |=  _CNTL_TEST;                 /* enable Test mode     */ // important to set this register first!
    //LPC_CAN->TEST  =  _TEST_LBACK;                 /* enable loopback mode */
}

/*----------------------------------------------------------------------------
  leave initialisation mode.
 *----------------------------------------------------------------------------*/
void CAN_start (void)  {

    //tCAN->CON &= ~_CNTL_INIT;                     /* Enter normal operating mode */
}

/*----------------------------------------------------------------------------
  check if transmit mailbox is empty
 *----------------------------------------------------------------------------*/
void CAN_waitReady (void)  {

    CAN_TxRdy = 1;
}

int CAN_GetErr( int en )
{

    static int errCnt = 0;
    uint32_t state = tCAN->STATUS;
	volatile int i, time = 100;
	
    if( state & _STAT_BOFF )
    {
        pr_debug("error:0x%08lx ", state);
        errCnt++;
        if( errCnt > 30 )
        {
            errCnt = 0;
			CAN_setup();
			CAN_TxRdy = 1;
            return state;
        }
		while( time-- )
		{
			tCAN->CON &= ~_CNTL_INIT;
			for(i=0; i<10000;i++);
			if(!( tCAN->STATUS & _STAT_BOFF )) break;
		}
        return state;
    }
    errCnt = 0;
    return 0;
}

void CAN_GetErrCnt( uint8 *pRxCnt, uint8 *pTxCnt)
{
    *pRxCnt = (LPC_CAN->ERR>>8) & 0x7F;
	*pTxCnt = (LPC_CAN->ERR   )& 0xFF;
}

/*----------------------------------------------------------------------------
  write a message to CAN peripheral and transmit it.
 *----------------------------------------------------------------------------*/
void CAN_wrMsg (CAN_msg *msg)  
{
    uint8_t u8MsgObj;
    int32_t i;
    uint32_t can_msgv;

    can_msgv = (tCAN->MVLD2 << 16) | tCAN->MVLD1;

    for (i = _MSG_OBJ_MAX - 1; i > -1; i--) {
        if ((can_msgv & (1 << i)) == 0)
            break;
    }
    u8MsgObj = i>-1? i+1: 0 ;

    if( msg->id & CAN_MSG_RTR )
    {
        tCAN->IF[0].CMASK = _CMDMASK_WR   | _CMDMASK_MASK  | _CMDMASK_ARB | _CMDMASK_CTRL | _CMDMASK_DATAA | _CMDMASK_DATAB;
        tCAN->IF[0].MCON  = _MCTRL_UMASK  /*| _MCTRL_TXRQST */ | _MCTRL_RMTEN | _MCTRL_TXIE  | _MCTRL_EOB    | (msg->len & _MCTRL_DLC);
    }else
    {
        tCAN->IF[0].CMASK = _CMDMASK_WR   | _CMDMASK_ARB   | _CMDMASK_CTRL | _CMDMASK_TREQ | _CMDMASK_DATAA | _CMDMASK_DATAB;
        tCAN->IF[0].MCON  =                 _MCTRL_TXRQST  | _MCTRL_TXIE  | _MCTRL_EOB    | (msg->len & _MCTRL_DLC);
    }

    if ( !( msg->id & CAN_MSG_EXT ) ) {   /* handle standard format */
        tCAN->IF[0].ARB1  = 0;
        tCAN->IF[0].ARB2  = ((msg->id & _STD_FORMAT) << 2)           | _ARB2_DIR | _ARB2_MSGVAL;
    }
    else {                                  /* handle extended format */
        tCAN->IF[0].ARB1 = ( msg->id        & _ARB1_ID);
        tCAN->IF[0].ARB2 = ((msg->id >> 16) & _ARB2_ID)  | _ARB2_XTD | _ARB2_DIR | _ARB2_MSGVAL;
    }

    /* Set the data for IFn Data Register */
    tCAN->IF[0].DAT_A1 = msg->buff16[0];
    tCAN->IF[0].DAT_A2 = msg->buff16[1];
    tCAN->IF[0].DAT_B1 = msg->buff16[2];
    tCAN->IF[0].DAT_B2 = msg->buff16[3];

    if (msg->type == REMOTE_FRAME) {
        tCAN->IF[0].CMASK  &= ~(_CMDMASK_DATAA | _CMDMASK_DATAB);
        tCAN->IF[0].ARB2   &= ~(_ARB2_DIR);                 /* trasnmit a Remote Frame Request */
    }

    tCAN->IF[0].CREQ  = u8MsgObj;   /* Transfer message object data to message RAM */
    while (tCAN->IF[0].CREQ & _CMDREQ_BUSY);
}

/*----------------------------------------------------------------------------
  read a message from CAN peripheral and release it.
 *----------------------------------------------------------------------------*/
static void CAN_ISRmsg (uint32_t can_msgObj)  
{
    while (tCAN->IF[1].CREQ & _CMDREQ_BUSY);
    tCAN->IF[1].CMASK = _CMDMASK_RD     | _CMDMASK_MASK | _CMDMASK_ARB   | _CMDMASK_CTRL |
        _CMDMASK_INTPND | _CMDMASK_TREQ | _CMDMASK_DATAA | _CMDMASK_DATAB;	
    tCAN->IF[1].CREQ = can_msgObj;        /* Transfer message object data from message RAM */
    while (tCAN->IF[1].CREQ & _CMDREQ_BUSY);

    /* check the message object whether it is receive or transmit*/
    if (tCAN->IF[1].MCON & _MCTRL_TXIE) {
        CAN_TxRdy = 1;                          /*  set transmit flag   */
        /* release message obect */
        tCAN->IF[1].CMASK = _CMDMASK_WR    | _CMDMASK_MASK | _CMDMASK_ARB | _CMDMASK_CTRL |
            _CMDMASK_DATAA | _CMDMASK_DATAB;
        tCAN->IF[1].MCON  = 0x0000;

        tCAN->IF[1].MASK1   = 0x0000;
        tCAN->IF[1].MASK2   = 0x0000;

        tCAN->IF[1].ARB1   = 0x0000;
        tCAN->IF[1].ARB2   = 0x0000;

        tCAN->IF[1].DAT_A1    = 0x0000;
        tCAN->IF[1].DAT_A2    = 0x0000;
        tCAN->IF[1].DAT_B1    = 0x0000;
        tCAN->IF[1].DAT_B2    = 0x0000;

        tCAN->IF[1].CREQ = can_msgObj;	   	/* Transfer message object data to message RAM */
        while( tCAN->IF[1].CREQ & _CMDREQ_BUSY );
    }
    else {
        if (tCAN->IF[1].ARB2 & (1UL << 14)) {  /* check XTD bit (extended format) */
            CAN_RxMsg.id = ((tCAN->IF[1].ARB2 & _ARB2_ID) << 16) | tCAN->IF[1].ARB1;
        }
        else {
            CAN_RxMsg.id = (tCAN->IF[1].ARB2 >> 2) & _STD_FORMAT; /* bit 28-18 is 11-bit standard frame */
        }

        CAN_RxMsg.len = tCAN->IF[1].MCON & _MCTRL_DLC;	         /* get message object data length */
        CAN_RxMsg.buff16[0] = tCAN->IF[1].DAT_A1;
        CAN_RxMsg.buff16[1] = tCAN->IF[1].DAT_A2;
        CAN_RxMsg.buff16[2] = tCAN->IF[1].DAT_B1;
        CAN_RxMsg.buff16[3] = tCAN->IF[1].DAT_B2;

        if( !LIST_BUF_FULL( canList_rx ) ) // && ( msg_obj.id == 0 || msg_obj.id == CANID ))
        {
            LIST_BUF_WR( canList_rx, CAN_RxMsg);
        }
    }
}

/*----------------------------------------------------------------------------
  setup acceptance filter.
 *----------------------------------------------------------------------------*/
void CAN_wrFilter (uint32 id, uint32 msk, uint8 format)  {
    /* Set the Message Buffer Register */
    uint8_t u8MsgObj = 0;
    int32_t i;
    uint32_t can_msgv;


    can_msgv = (tCAN->MVLD2 << 16) | tCAN->MVLD1;

    for (i = _MSG_OBJ_MAX - 1; i > -1; i--) {
        if ((can_msgv & (1 << i)) == 0)
            break;
    }
    u8MsgObj = i>-1? i+1: 0 ;


    tCAN->IF[0].CMASK = _CMDMASK_WR  | _CMDMASK_MASK | _CMDMASK_ARB | _CMDMASK_CTRL;
    tCAN->IF[0].MCON  = _MCTRL_UMASK | _MCTRL_RXIE   | _MCTRL_EOB   | _MCTRL_DLC;

    if (format == STANDARD_FORMAT) {       /* handle standard format */
        id = id & _STD_FORMAT;               

        tCAN->IF[0].MASK1  =  0;
        tCAN->IF[0].MASK2  = (msk << 2);

        tCAN->IF[0].ARB1  =  0;
        tCAN->IF[0].ARB2  = (id << 2)            | _ARB2_MSGVAL;      /* id is stored left-aligned */
    }
    else {                                  /* handle extended format */
        id = id & _EXT_FORMAT;               

        tCAN->IF[0].MASK1 = (msk        & _MSK1_MSK);
        tCAN->IF[0].MASK2 = (msk >> 16)             | _MSK2_MXTD;

        tCAN->IF[0].ARB1 = (id        & _ARB1_ID);
        tCAN->IF[0].ARB2 = (id >> 16)             | _ARB2_XTD | _ARB2_MSGVAL;
    }

    tCAN->IF[0].CREQ = u8MsgObj;         /* Transfer message object data to message RAM */
    while (tCAN->IF[0].CREQ & _CMDREQ_BUSY);
}


__AFTER__
void CAN0_IRQHandler(void);
void CAN0_IRQHandler(void)
{
   
    volatile uint32_t can_int, can_stat;
    uint32_t can_msgObj;

    can_int = tCAN->IIDR;                          /* read interrupt status */
    switch (can_int) {
        case 0x0000:                                   /* no interrupt */
            ;
            break;
        case 0x8000:                                   /* status interrupt */
            can_stat = tCAN->STATUS;

            if (can_stat & _STAT_TXOK) {                   /* TXOK       */
                tCAN->STATUS &= ~_STAT_TXOK;                /* reset TXOK */
            }	  
            if (can_stat & _STAT_RXOK) {                   /* RXOK       */	 // got this never to work!
                tCAN->STATUS &= ~_STAT_RXOK;                /* reset RXOK */
            }	  
            break;
        default:                                      /* message object interrupt */
            can_msgObj = can_int & 0x7FFF;
            if ((can_msgObj >=  1) &&   (can_msgObj <= 32)   )
            {                /*   valid msgObj number */
                CAN_ISRmsg (can_msgObj);      /*  read the message  */
            }
            break;
    }
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

int CAN_wrMSG( CAN_msg *pMsg )
{
    if( CAN_TxRdy == 0 || pMsg == NULL )
        return FALSE;
    CAN_TxRdy = 0;
    CAN_wrMsg( pMsg );
    return TRUE;
}

