

/*
   文件名：data.c
   功能：HBTK-1000XQ增强型，数据处理部分
   数据采集后判断处理，及通信设置部分设置数据转换
   */
#define DATA_GLOBAL

#define DATA_DEFDIS

#include <string.h>
#include "mytype.h"
#include "global.h"
#include "data.h"
#include "MicroOS.h"
#include "user.h"
#include "protocol.h"
#include "hardware.h"
#include "app_cfg.h"
#include "nuc_adc.h"
#include "pcommand.h"
#include "shellSort.h"

TData       data[MAX_DATA_SIZE] __NOINIT__;
extern uint8 Sound;
TDataState  MKState __NOINIT__;

uint8 BD_disable;
int    wValue;

void Data_Init( uint8 en )
{
    int i;
    int dis = 0;
    for(i=0; i<ARRAY_SIZE(data); i++)
    {
        data[i].type  = TT_V;
        data[i].value = 0;
        data[i].state = 0;
        data[i].trans = 0;
        data[i].value_tmp = 0;
        data[i].bj_delay = pUsrCfg->delay;
        data[i].yj_delay = pUsrCfg->delay;
    }

    BD_disable = 0;
    dis = PBI( pUsrCfg->enable, USER_TYPE_BD);

    // TODO 更新了通道类型
#if OEM == 2
    data[6].type = eTTwv;
#else
    data[6].type = eTTv;
#endif
    data[7].type = eTTw;
    pUsrCfg->count = 8;
    switch( pUsrCfg->type )
    {
    	case CGQ_TYPE_VA_D6:
    	    data[6].type = 0;
    		break;
        case CGQ_TYPE_VA_S2:
            BD_disable = dis;
            break;
        case eCGQvaw2:
            BD_disable    = dis;
            data[0].type  = TT_WV;
            data[1].type  = TT_WV;
            data[2].type  = TT_WV;
            data[3].type  = TT_WV;
            data[4].type  = TT_WV;
            data[5].type  = TT_WV;
            data[6].type  = eTTwv;
            break;
        case eCGQvad:
            data[0].type  = 0;
            data[1].type  = 0;
            data[2].type  = TT_V;
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = 0;
            break;
        case eCGQvd:
            data[0].type  = 0;
            data[1].type  = 0;
            data[2].type  = TT_VD;
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = 0;
            data[6].type  = TT_VD;
            break;
        case eCGQvdi:
            data[0].type  = 0;
            data[1].type  = 0;
            data[2].type  = TT_VD;
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = TT_ID;
            data[6].type  = TT_VD;
            break;
        case CGQ_TYPE_VA_W:
            data[0].type  = TT_WV;
            data[1].type  = TT_WV;
            data[2].type  = TT_WV;
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = 0;
            data[6].type  = eTTwv;
            break;
        case CGQ_TYPE_VI_W:
            data[0].type  = TT_WV;
            data[1].type  = TT_WV;
            data[2].type  = TT_WV;
            data[3].type  = TT_I;
            data[4].type  = TT_I;
            data[5].type  = TT_I;
            data[6].type  = eTTwv;
            break;
        case CGQ_TYPE_VA_D2:
            data[0].type  = 0;
            data[1].type  = 0;
            data[2].type  = TT_V;
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = TT_V;
            break;
        case CGQ_TYPE_VI_D:
            BD_disable = dis;
            data[0].type  = 0;
            data[1].type  = 0;
            data[2].type  = TT_V;
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = 0;
            break;
        case CGQ_TYPE_VI_S:
            data[3].type  = TT_I;
            data[4].type  = TT_I;
            data[5].type  = TT_I;
            break;
        case CGQ_TYPE_VA_S1:
            data[3].type  = 0;
            data[4].type  = 0;
            data[5].type  = 0;
            break;
//        case eCGQvis2:
//            pUsrCfg->count = 8;
//        	break;
        default:
            break;
    }
    MKState.state = 0;
    MKState.state2 = 0;
    MKState.trans = 0;
}


__STATIC_INLINE
void GetADC_Enable(void)
{   //同时采集8路AD，等待结束
    int value;
	OSTimeDly( HZ/20 );
    ADC_Enable( 0 );
    while( (value = ADC_Flag() ) < 0 )
    {
        OSTimeDly(1 );
    }
}

__STATIC_INLINE
int16 GetADCValue( int channel, int wd )
{   //得到单路AD采集的值
    return ADC_GetExt( channel, wd );
}

/*
 * 每个数据采集10次，
 * 通过shell排序法排序
 * 去掉2个最大、最小值
 * 其余6个数取平均值
 */
void DataCJgetValue(void)
{
	int i,j;
	int value;
	for( j=0; j<GETBUFF_SIZE; j++ )
	{
        GetADC_Enable();
        for(i=0; i<ARRAY_SIZE(data); i++)
        {
            value = 0;
            switch( data[i].type )
            {
                case eTTs:
                case eTTi:
                case eTTv:
                case eTTwv:
                    value = GetADCValue(i, 1 );
                    break;
                case eTTw:
                case eTTvd:
                case eTTid:
                    value = GetADCValue(i, 0 );
                    break;
                default:
                    value = 0;
                    break;
            }
            data[i].vbuff[j] = value;
        }
	}
    for(i=0; i<ARRAY_SIZE(data); i++)
    {//通过shell排序，去掉2个最大值，去掉2个最小值，取平均值。
    	shellSort16( data[i].vbuff, GETBUFF_SIZE);
    	value = 0;
    	for(j=2; j<GETBUFF_SIZE-2; j++)
    	{
    		value += data[i].vbuff[j];
    	}
		value /= (GETBUFF_SIZE - 4);
		//第7路温度采用12位AD值，其他采用10位AD
		data[i].tmp = value;
    }
}

void DataCJ_Task( void );
void DataCJ_Task( void )
{
    int i;
    int value;
    wValue=20;
    pr_debug("data cj task\n");
    while( 1 )
    {
    	DataCJgetValue();
    	createWJZ(wValue);
        for(i=0; i<ARRAY_SIZE(data); i++)
        {
            value = data[i].tmp;
            switch( data[i].type )
            {
                case TT_VD:
                    value = GetValueVD( value, pUsrCfg->JZ[i] );
                    break;
                case TT_WV:
                    value = GetValueWV( value, pUsrCfg->JZ[i] );
                    break;
                case TT_V:
                    value = GetValueV( value, pUsrCfg->JZ[i] );
                    break;
                case eTTw:
                	value = WValue(value);
                	wValue = value;
                	if( value < 0 )
                		value = 0;
                	break;
                case TT_ID:
                case TT_I:
                    value = value * pUsrCfg->Aed / pUsrCfg->JZ[i];
                    value = value < 5 ? 0: value;
                    break;
                default:
                    value = 0;
                    break;
            }
            data[i].value = value;
	}
        OSTimeDly(1);
    }
}

int Data_PD( void )
{
    int i;
    int ret = 0;
    int gy,qy,qx;
    uint8 disable=0;
    gy = pUsrCfg->Vbj;
    qy = pUsrCfg->Vqy;
    qx = qy / 2;

    if( BD_disable )
    {
        if( MKState.Zs == 0x3F )
            disable = 0;
        else
            disable = 1;
    }
    for(i=0; i<pUsrCfg->count; i++)
    {
        if( i>2 )
        {
            if( ( data[i].type == eTTv || data[i].type == eTTwv ) && disable )
            {
                data[i].bj_delay = pUsrCfg->delay;
                data[i].yj_delay = pUsrCfg->delay;
                continue;
            }
        }
        switch( data[i].type )
        {
            case TT_WV:
			if( gy < 300 )
			{
				gy = gy * 380 / 220;
				qy = qy * 380 / 220;
				qx = qy / 2;
			}
            case TT_V:
            case TT_VD:
                if( data[i].value <= qx && data[i].value_tmp <= qx )
                {
                    data[i].state = 3;
                    data[i].bj_delay = pUsrCfg->delay;
                    data[i].yj_delay = pUsrCfg->delay;
                }else if( data[i].value > gy && data[i].value_tmp > gy )
                {
                    if( data[i].bj_delay == 0 )
                    {
                        data[i].state = 1;
                    }else
                        data[i].bj_delay--;
                    data[i].yj_delay = pUsrCfg->delay;
                }else if( data[i].value < qy && data[i].value_tmp < qy )
                {
                    if( data[i].yj_delay == 0 )
                    {
                        data[i].state = 2;
                    }else
                    {
                        data[i].yj_delay--;
                    }
                    data[i].bj_delay = pUsrCfg->delay;
                }else
                {
                    data[i].bj_delay = pUsrCfg->delay;
                    data[i].yj_delay = pUsrCfg->delay;
                    data[i].state = 0;
                }
                break;
            case TT_ID:
            case TT_I:
                if( data[i].value > pUsrCfg->Abj && data[i].value_tmp > pUsrCfg->Abj )
                {
                    if( data[i].bj_delay == 0 )
                    {
                        data[i].state = 1;
                    }else
                        data[i].bj_delay--;
                    data[i].yj_delay = pUsrCfg->delayI;
                }else if( data[i].value < pUsrCfg->Abj/2 && data[i].value_tmp < pUsrCfg->Abj/2 )
                {
                    data[i].bj_delay = pUsrCfg->delayI;
                    data[i].yj_delay = pUsrCfg->delayI;
                    data[i].state = 0;
                }
                break;
        }
        if( data[i].state )
        {
            ret++;
        }
        data[i].value_tmp = data[i].value;
    }

    MKState.Za  = data[0].state;
    MKState.Zb  = data[1].state;
    MKState.Zc  = data[2].state;
    MKState.Ba  = data[3].state;
    MKState.Bb  = data[4].state;
    MKState.Bc  = data[5].state;
    MKState.t21 = data[6].state;
    MKState.t22 = data[7].state;
    return ret;
}



#define TRANS_ZDGD            0            /*中断供电*/
#define TRANS_GY              1            /*过压*/
#define TRANS_QY              2            /*欠压*/
#define TRANS_DD              3            /*断电 缺相*/

void Data_Trans( void )
{
    int i;

	switch (pUsrCfg->type) {
	case eCGQvaw2:
	case eCGQvis2:
	case CGQ_TYPE_VA_S2:
		if (MKState.Ba == 3 && MKState.Bb == 3 && MKState.Bc == 3) {
			if ((MKState.trans & 2) == 0) {
				if (GZ_Trans(3, TCMDDATA_ZDGD, DATA_GZ_ZDGD))
					SBI(MKState.trans, 1);
			}
		} else {
			if (MKState.trans & 2) {
				if (GZ_Trans(3, TCMDDATA_NOZDGD, DATA_GZ_ZDGD))
					CBI(MKState.trans, 1);
			}
		}
	case CGQ_TYPE_VA_W:
	case CGQ_TYPE_VI_W:
	case CGQ_TYPE_VI_S:
	case CGQ_TYPE_VA_S1:
		if (MKState.Za == 3 && MKState.Zb == 3 && MKState.Zc == 3) {
			if ((MKState.trans & 1) == 0) {
				if (GZ_Trans(0, TCMDDATA_ZDGD, DATA_GZ_ZDGD))
					SBI(MKState.trans, 0);
			}
		} else {
			if (MKState.trans & 1) {
				if (GZ_Trans(0, TCMDDATA_NOZDGD, DATA_GZ_ZDGD))
					CBI(MKState.trans, 0);
			}
		}
		break;
	}

	for (i = 0; i < ARRAY_SIZE(data); i++) {
		int BJtype = DATA_GZ_GY;
		int QXtype = DATA_GZ_QX;
		uint8 cmd;

		if (data[i].type == TT_I || data[i].type == TT_ID)
			BJtype = DATA_GZ_GL;

		switch (pUsrCfg->type) {
		case eCGQvd:
		case eCGQvdi:
		case eCGQvad6:
		case eCGQvad2:
		case eCGQvid:
		case eCGQvad:
			QXtype = DATA_GZ_ZDGD;
			break;
		}

		switch (data[i].state) {
		case 0:
			if (PBI(data[i].trans, TRANS_GY)) {
				if (GZ_Trans(i, TCMDDATA_NOGZ, BJtype))
					CBI(data[i].trans, TRANS_GY);
			}
			if (PBI(data[i].trans, TRANS_QY)) {
				if (GZ_Trans(i, TCMDDATA_NOGZ, DATA_GZ_QY))
					CBI(data[i].trans, TRANS_QY);
			}
			if (PBI(data[i].trans, TRANS_DD)) {  //判断是缺相，还是中断供电恢复
				if (i >= 6)
					cmd = TCMDDATA_NOZDGD;
				else
					cmd = QXtype == DATA_GZ_ZDGD ?
							TCMDDATA_NOZDGD : TCMDDATA_NOGZ;

				if (GZ_Trans(i, cmd, QXtype)) {
					CBI(data[i].trans, TRANS_DD);
				}
			}
			break;
		case 1:
			if (!PBI(data[i].trans, TRANS_GY)) {
				if (GZ_Trans(i, TCMDDATA_GZ, BJtype)) {
					SBI(data[i].trans, TRANS_GY);
				}
			}
			break;
		case 2:
			if (!PBI(data[i].trans, TRANS_QY)) {
				if (GZ_Trans(i, TCMDDATA_GZ, DATA_GZ_QY)) {
					SBI(data[i].trans, TRANS_QY);
				}
			}
			break;
		case 3:
			if( i<3 )
			{
				switch (pUsrCfg->type) {
				case eCGQvaw2:
				case eCGQvis2:
				case CGQ_TYPE_VA_S2:

				case CGQ_TYPE_VA_W:
				case CGQ_TYPE_VI_W:
				case CGQ_TYPE_VI_S:
				case CGQ_TYPE_VA_S1:
					if (MKState.Za == 3 && MKState.Zb == 3 && MKState.Zc == 3) {
						goto Data_Trans12;
					}
					break;
				}
			}else if( i<6 )
			{
				switch( pUsrCfg->type )
				{
				case eCGQvaw2:
				case eCGQvis2:
				case CGQ_TYPE_VA_S2:
					if (MKState.Ba == 3 && MKState.Bb == 3 && MKState.Bc == 3) {
						goto Data_Trans12;
					}
					break;
				}
			}

			if (!PBI(data[i].trans, TRANS_DD)) {
				if (i >= 6)
					cmd = TCMDDATA_ZDGD;
				else
					cmd = QXtype == DATA_GZ_ZDGD ? TCMDDATA_ZDGD : TCMDDATA_GZ;
				if (GZ_Trans(i, cmd, QXtype)) {
					SBI(data[i].trans, TRANS_DD);
				}
			}
			break;
		}
Data_Trans12: ;
	}
}

const TOBJname OBJname[]={
    {eCGQvas2,  _T("ZXVA-S2")},
    {eCGQvad2, _T("ZXVA-D2")},
	{eCGQvis, _T("ZXVI-S")},
	{eCGQvas1, _T("ZXVA-S1")},
	{eCGQvad6, _T("ZXVA-D6")},
	{eCGQvd, _T("ZXVD")},
	{eCGQvid, _T("ZXVI-D")},
	{eCGQviw, _T("ZXVI-W")},
	{eCGQvaw, _T("ZXVA-W")},
	{eCGQvas2f, _T("ZXVA-S2F")},
	{eCGQvad, _T("ZXVA-D")},
	{eCGQvaw2, _T("ZXVA-W2")},
	{eCGQvdi, _T("ZXVD-I")},
	{eCGQld, _T("ZXLW")},
	{eCGQvis2, _T("ZXVI-S2")},
};

const char *GetObjName( int type )
{
    int i;
    for(i=0; i<ARRAY_SIZE( OBJname ); i++)
    {
        if( OBJname[i].type == type )
            return OBJname[i].name;
    }
    return " ";
}



void data_cmd( int args, char **argv );
void data_cmd( int args, char **argv )
{
    int i;
    TRACE("T n\tvalue\tdata\tstate\n" );
    for(i=0; i<ARRAY_SIZE(data); i++)
    {
        switch( data[i].type )
        {
            case eTTi:
            case eTTid:
            	TRACE("I");
                break;
            case eTTw:
            	TRACE("W");
                break;
//            case eTTs:
//                printk("S");
//                break;
            case eTTv:
            	TRACE("V");
                break;
            case eTTwv:
            	TRACE("WV");
                break;
            case eTTvd:
            	TRACE("VD");
                break;
        }
        TRACE(" %d.\t%d\t%d\t%d\n", i+1, data[i].value, data[i].tmp, data[i].state);
    }
}
PSHELL_CMD(data, data_cmd, "data");




