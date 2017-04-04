
//protocol.c
//通信协议实现文件
#define PROTOCOL_GLOBALS

#include "mytype.h"
#include "../drive/nuc_can.h"
#include "protocol.h"
#include "MicroOS.h"
#include "data.h"
#include "user.h"
#include "crc16.h"
#include "app_cfg.h"
#include "bsp.h"
#include "hardware.h"


#define COMOFF()    
#define COMON()     


#undef  CAN_LOCK
#undef  CAN_UNLOCK

#define CAN_LOCK()         OSSemPend( Can_LockSem, 0 )
#define CAN_UNLOCK()       OSSemPost( Can_LockSem )


#define BUFFER_SIZE        8

static CAN_msg  can_buff[BUFFER_SIZE];
static uint8 in,out,cnt;
static uint8 TransAddr;
static uint8 TransTTAddr;
static uint8 CanTransEn;
static uint8 CanAutoDis;    //自动发送关闭

static CAN_msg Txmsg;
int SetAddress;
int ProCanError;

void Pro_Init(void )
{
    in = out = cnt = 0;
    Txmsg.cnt = 0;
    TransAddr = 0;
    CanTransEn = 0;
    ProCanError = 0;
    SetAddress = 0;

    TransTTAddr = 0;
    CanAutoDis  = 1;
}

static int Set_Crc( TProtocol *pPro )
{
    if( pPro == NULL )
        return 0;

    int size; 
    uint16 crc;
    pPro->T1 = TPROC_T1;
    pPro->T2 = TPROC_T2;

    size = pPro->Tcnt+3;
    crc  = modbus_crc( 0, &pPro->T1, size);
    pPro->buff[pPro->Tcnt]   = crc & 0xFF;
    pPro->buff[pPro->Tcnt+1] = crc>>8;
    return size+2;
}

uint8 BJ_Trans( uint8 addr )
{
    CAN_msg *pMSG;
    uint8 ret = 0;
    OS_SREG_VAR;
    if( CanTransEn == 0)
        return 0;
    OS_ENTER_CRITICAL();
    if( cnt < BUFFER_SIZE   )
    {
        cnt++;
        pMSG = &can_buff[in++];
        if( in >= BUFFER_SIZE )
            in = 0;

        pMSG->id = __ID( CAN_ID_DATA, TCMDDATA_BJ, addr+1, pUsrCfg->address);
        pMSG->cnt    = 4;
        pMSG->Dtype  = data[addr].type;
        pMSG->Dstate = DATA_BJ;
        pMSG->Dvalue = data[addr].value ;

        ret++;
    }
    OS_EXIT_CRITICAL();
    return ret;
}


/*
 * 发送故障或故障恢复
 */
uint8 GZ_Trans( uint8 addr, uint8 cmd, uint8 state )
{
    CAN_msg *pMSG;
    uint8 ret = 0;
    OS_SREG_VAR;
    if( CanTransEn == 0)
        return 0;
    OS_ENTER_CRITICAL();
    if( cnt < BUFFER_SIZE  )
    {
        cnt++;
        pMSG = &can_buff[in++];
        if( in >= BUFFER_SIZE )
            in = 0;

        pMSG->id = __ID( CAN_ID_DATA, cmd, addr+1, pUsrCfg->address);
        pMSG->cnt    = 4;
        pMSG->Dtype  = data[addr].type;
        pMSG->Dstate = state;
        pMSG->Dvalue = data[addr].value;
        ret++;
    }
    OS_EXIT_CRITICAL();
    return ret;
}

//extern uint8 TKstate;
uint8 TK_Trans( uint8 cmd )
{
    CAN_msg *pMSG;
    uint8 ret = 0;
    TMKState *ps;
    OS_SREG_VAR;
    if( CanTransEn == 0)
        return 0;
    OS_ENTER_CRITICAL();
    if( cnt < BUFFER_SIZE )
    {
        cnt++;
        pMSG = &can_buff[in++];
        if( in >= BUFFER_SIZE )
            in = 0;

        ps = (TMKState*)pMSG->buff;

        pMSG->id    = __ID( CAN_ID_DATA, cmd, 1, pUsrCfg->address);
        pMSG->cnt   = 8;

		ps->TKtype   = pUsrCfg->type;
		ps->TTcnt    = pUsrCfg->count;
		ps->TKstate  = MKState.state;
		ps->TKstate2 = MKState.state2;
		ps->wValue   = wValue;
		ps->ZKaddr   = 0;
		ps->version  = MKVERSION;
        ret++;
    }
    OS_EXIT_CRITICAL();
    return ret;
}


void AutoTrans(void)
{
	if( CanAutoDis ) return;
    TK_Trans( TCMDDATA_TK );
}

uint8 TT_Trans( uint8 addr )
{
    if( addr > pUsrCfg->count  )
        return FALSE;

    CAN_msg *pMSG;
    uint8 ret = 0;
    OS_SREG_VAR;
    if( CanTransEn == 0)
        return 0;
    OS_ENTER_CRITICAL();
    if( cnt < BUFFER_SIZE )
    {
        cnt++;
        pMSG = &can_buff[in++];
        if( in >= BUFFER_SIZE )
            in = 0; 
        pMSG->id     = __ID( CAN_ID_DATA, TCMDDATA_TT, addr+1, pUsrCfg->address);
        pMSG->cnt    = 4;
        pMSG->Dtype  = data[addr].type;
        pMSG->Dstate = data[addr].state;
        pMSG->Dvalue = data[addr].type == eTTvd? data[addr].value/10 :data[addr].value;
        ret++;
    }
    OS_EXIT_CRITICAL();
    return ret;
}

static void CAN_TransAll(void)
{
    if( Txmsg.cnt == 0 && TransAddr != 0)
    {
        uint8 i,j;
        uint8 len=0;
        uint8 tmp = TransAddr;
        for(i=0; i<4; i++)
        {
            if( TransAddr > pUsrCfg->count )
            {
                TransAddr = 0;
                break;
            }
            j = TransAddr-1;
            Txmsg.Data[i].value = data[j].type == eTTvd? data[j].value/10 :data[j].value;
            Txmsg.Data[i].type  = data[j].type;
            TransAddr++;
            len += 2;
        }
        if( len )
        {
            Txmsg.cnt = len;
            Txmsg.id = __ID( 1, TCMDDATA_MK, tmp, pUsrCfg->address);
        }
    }

	if (Txmsg.cnt == 0 && TransTTAddr != 0) {
		uint8 i, j;
		uint8 len = 0;
		uint8 tmp = TransTTAddr;
		TMKAllValue *pv = (TMKAllValue*)Txmsg.buff;

		for (i = 0; i < 2; i++) {
			if (TransTTAddr > pUsrCfg->count) {
				TransTTAddr = 0;
				break;
			}
			j = TransTTAddr - 1;
			int v = data[j].type == eTTvd ? data[j].value / 10 : data[j].value;

			pv->Data[i].d32 =MKAllValue12(v, data[j].type, GetWJZValue(data[j].tmp), data[j].state );
			TransTTAddr++;
			len += 4;
		}
		if (len) {
			Txmsg.cnt = len;
			Txmsg.id = __ID(1, TCMDDATA_MKALL12, tmp, pUsrCfg->address);
		}
	}
}

uint8 CAN_WriteAll( void )
{
    COMOFF();
    if( CanTransEn == 0)
        return 0;
    if( Txmsg.cnt == 0 )
    {
        if( cnt )
        {
            cnt--;
            Txmsg = can_buff[out++];
            if( out >= BUFFER_SIZE )
                out = 0;
        }
    }

    CAN_TransAll();

    if( Txmsg.cnt )
    {
        COMON();
        int8 ret = CAN_wrMSG( &Txmsg );
        if( ret > 0)
        {//数据发送完成
//            DEBUG2("tx %08x: %08x %08x\n", Txmsg.id, Txmsg.DataA, Txmsg.DataB);
            ProCanError = 0;
            Txmsg.cnt = 0;
        }else
        {
            if( ProCanError > 30 )
                CAN_GetErr(0);
            if( ProCanError > 1000 )
            {
                ProCanError = 0;
                CAN_GetErr(1);
            }
        }
        return ret;
    }

    return FALSE;
}


//#define OEM_ZHHA           0x55AA
//#define OEM_TANDA          0x55A0
//#define OEM_O              0x55A1

extern void reset_cmd(void);
void jz_cmd( int args, char **argv );
//extern uint8 sound;
void CanCmdMsg( CAN_msg *pMsg )
{
    DEBUG2("can %x-%x-%x\n", pMsg->IDstate, pMsg->IDcmd, pMsg->Dtype);
    if( pMsg->IDstate == 0 )
    {
        return;
    }

    switch (pMsg->IDcmd) {
	case TPROTRANS_GETTK:
		if (pMsg->IDTK == pUsrCfg->address || pMsg->IDTK == 0) {
			TK_Trans( TCMDDATA_TK);
			if (pMsg->Dvalue == OEM_NUM) {
				CanTransEn = 1;
				CanAutoDis = 1;
			}
		}
        break;
    case TPROTRANS_SETADDR:
        DEBUG2("SetAddress ");
        if( pMsg->Dstate == 0x55 && pMsg->Dtype == 0xAA )
        {
            DEBUG2("SetAddress:%d", pMsg->Dvalue);
            SetAddress = pMsg->Dvalue;
        }
    	return;
    case TPROTRANS_RESETALL:
    	if( pMsg->Dvalue == 0x55AA )
    	{
    		UserSetDef();
    	}
        reset_cmd();
        return;
    	break;
	case TPROTRANS_HAIPPO3:
		CanTransEn = 0;
		CanAutoDis = 0;
		if (pMsg->Dtype == 0xAA) {
			if( pMsg->Dvalue == OEM_NUM )
			{
				CanTransEn = 1;
			}
			return;
		}
		break;
	case TPROTRANS_JZ:
		if (pMsg->IDTK == pUsrCfg->address || pMsg->IDTK == 0) {
			switch (pMsg->Dtype) {
			case 0xAA:
				jz_cmd(0, 0);
				return;
			case 0xAB:
				UserVJZ(pMsg->Dvalue);
				return;
			}
		}
	}

    if( pMsg->Dtype == 0xAA && ( pMsg->IDTK == pUsrCfg->address || pMsg->IDTK == 0 ))
    {
        switch( pMsg->IDcmd )
        {
			case TPROTRANS_SETWARNING_QYV:
				UsrSetVqy( pMsg->Dvalue );
				return;
            case TPROTRANS_SETWARNING_V:
                UsrSetVbj( pMsg->Dvalue );
                return;
            case TPROTRANS_SETWARNING_I:
                UsrSetIbj( pMsg->Dvalue );
                return;
            case TPROTRANS_SETED_I:
                UsrSetIed( pMsg->Dvalue );
                return;
            case TPROTRANS_SETDELAYI :
                UsrSetDelayI( pMsg->Dvalue );
                return;
            case TPROTRANS_SETDELAY :
                UsrSetDelay( pMsg->Dvalue );
                return;
            case TPROTRANS_SETWARNING:
                UsrSetSbj( pMsg->Dvalue );
                return;
            case TPROTRANS_SETWARNING_W:
                UsrSetWbj( pMsg->Dvalue );
                return;
            case TPROTRANS_SETBDDIS:
                UsrSetBD( pMsg->Dvalue );
                return;
            case TPROTRANS_TTCNT:
                UsrSetCnt( pMsg->Dvalue );
                return;
            case TPROTRANS_SETTYPE:
                UsrSetType( pMsg->Dvalue );
            	return ;
        }
    }

    if( pMsg->IDTK == pUsrCfg->address )
    {
        switch( pMsg->IDcmd )
        {
            case TPROTRANS_GETTT:
                TT_Trans( pMsg->IDTT );
                break;
//            case TPROTRANS_GETTK:
//                TK_Trans( TCMDDATA_TK );
//                break;
            case TPROTRANS_NORMAL:
                if( pMsg->IDTT )
                {
                    TT_Trans( pMsg->IDTT -1 );
                }
                break;
            case TPROTRANS_TKALL:
                {
                    TK_Trans( TCMDDATA_TK );
                    if( TransAddr == 0 )
                        TransAddr = 1;
                }
                break;
            case TPROTRANS_TTALL:
            {
                TK_Trans( TCMDDATA_TK );
                if( TransTTAddr == 0 )
                    TransTTAddr = 1;
            	break;
            }
        }
    }
}

#include "pcommand.h"
#include "../drive/nuc_uart0.h"
static  int sendBuff( uint8 *buff, int size )
{
	int i=size;
	for(int a=0; a<HZ; a++)
	{
		wdt_reset();
		int ret = Uart_SendBuff(buff, i);
		buff += ret;
		i   -= ret;
		if( i==0 ) break;
		OSTimeDly(1);
	}
//    while( size--)
//    {
//        Uart_SendByte( *buff++);
//    }
    return 1;
}

TProtocol pro;
static 
int get_cmd(int args, char **argv )
{
    TProData *pData = (TProData*)&pro.buff;
    int i;

    pData->TTcnt =  pUsrCfg->count;
    pData->type  =  pUsrCfg->type;
    pData->addr  = pUsrCfg->address;
    pData->state = MKState.state;
    pData->state2 = MKState.state2;
    for(i=0; i<pUsrCfg->count; i++)
    {
        pData->TTbuff[i].value = data[i].value;
        pData->TTbuff[i].type  = data[i].type;
    }

    pro.Tcnt =  pUsrCfg->count*2 + 8;

    int size = Set_Crc( &pro );
    return sendBuff( (uint8*)&pro, size);
    return 0;
}
PSHELL_CMD( get, get_cmd, "get");


