/*
 * dataW.c
 *
 *  Created on: 2016年7月24日
 *      Author: beyond
 */


#include "data.h"
#include "user.h"
#include "protocol.h"

typedef struct Tpt100list
{
    int16  count;
    uint16 value;
}Tpt100list;

const Tpt100list PROGMEM ptlist[] =
{
    {-10,1518},
    {  0,1459},
    { 10,1406},
    { 20,1356},
    { 30,1320},
    { 40,1244},
    { 50,1191},
    { 60,1138},
    { 70,1085},
    { 80,1032},
    { 90,979 },
    {100,926 },
};

/*
 * 根据电阻值计算Pt100温度值
 * 通过查表法计算
 */
static int16 GetWvalue( uint16 value )
{
    int i;
    int16 wL;
    uint16 aL,aH;
    int32 tmp;
    for( i=1; i<ARRAY_SIZE( ptlist ); i++)
    {
        if( value > ptlist[i].value )
        {
            wL = ptlist[i-1].count ;
            aL = ptlist[i-1].value ;
            aH = ptlist[i].value ;
            tmp = (value-aL)*10/(aH-aL) + wL;
//            tmp  = value-aL;
//            tmp *= 10;
//            tmp /= (aH- aL );
//            tmp += wL;
            return (int16)tmp;
        }else if( value == ptlist[i].value )
        	return ptlist[i].count;
    }
    return 0;
}

/*
 * 根据温度值读取Pt100电阻值
 */
static uint16 GetPtValue( int16 value )
{
    int i;
    int32 tmp;
    for( i=1; i<ARRAY_SIZE( ptlist ); i++)
    {
        if( value <= ptlist[i].count )
        {/* V = (V2-V1)*(ax - a1) / (a2-a1) + V1 */
            tmp = (ptlist[i].value - ptlist[i-1].value );
            tmp *= value - ptlist[i-1].count;
            tmp /= ptlist[i].count - ptlist[i-1].count;
            tmp += ptlist[i-1].value;
            return (int16)tmp;
        }
    }
    return 0;
}

uint16 JZPtValue( int16 value, uint16 dat )
{
    uint32 tmp;
    tmp = GetPtValue( value );
    tmp = tmp * 1024 / dat;
    return tmp;
}


int WValue( uint16 value )
{
    uint32 tmp = value;
    //tmp = tmp * pUsrCfg->WDCfg.jzH / 1024;
    return GetWvalue( tmp );
}


void UserWDJZ( int16 value )
{
    int i=7;
    pr_debug("jz w  %d\n", value );
    //for(i=0; i<MAX_POINT_COUNT; i++)
    {
        if( data[i].type == eTTw )
        {
            uint16 ret = JZPtValue( value, data[i].tmp );
            pr_debug("W %d %d %d\n", i+1, data[i].tmp, ret );
            if( ret && data[i].tmp > 200 && data[i].tmp < 700)
            {
                pUsrCfg->WDCfg.jzH = ret;
                pr_debug("wd :%d\n", ret);
                UsrWrite();
                return;
            }
        }
    }
}

