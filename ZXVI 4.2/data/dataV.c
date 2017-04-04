/*
 * dataV.c
 *
 *  Created on: 2016年7月24日
 *      Author: beyond
 *
 *  20161108 增加了温度补偿，重新编写了220V、380V查表算法的真值表
 */

#include "data.h"
#include "pcommand.h"
#include "protocol.h"
#include "user.h"


typedef struct{
    uint16  Vout;   // 实际电压值
    uint16  Vin;    // AD采集值
}TDatalist;


#define VJZ_V     120
#define VDATALIST_220              4
const TDatalist CVDataList[13] =
{
	{260,  1697},   //0
	{250,  1600},   //1
	{240,  1505},   //2
	{230,  1414},   //3
	{220,  1329},   //4
	{210,  1233},   //5
	{200,  1138},   //6
	{190,  1053},   //7
	{180,   961},   //8
	{170,   880},   //9
	{160,   794},   //10
	{140,   637},   //11
	{100,   355},   //12
};

#define WVDATALIST_380              4
const TDatalist WVDataList[] =
{
	{449,   546*4},   //0  
	{432,   515*4},   //1  
	{415,   484*4},   //2  
	{397,   456*4},   //3  
	{380,   425*4},   //4  
	{363,   396*4},   //5  
	{345,   370*4},   //6  
	{328,   341*4},   //7  
	{311,   315*4},   //8  
	{294,   289*4},   //9  
	{276,   261*4},   //10 
	{242,   210*4},   //11 
	{173,   120*4},   //12 
};

/*
 * 10V ~ 60V 直流查表表格
 */
const TDatalist VDDataList[] =
{
    {400, 797},
    {240, 395},
    {100, 82},
};

typedef struct{  //温度补偿系数
    int16   wd;      // 温度
    uint16  wVJZ;    // 220V 温度补偿
    uint16  wWVJZ;   // 380V 温度补偿
    uint16  wDVJZ;   // 24V  温度补偿
}TWDlist;

const
TWDlist WDlist[]={
		{-10, 1267  , 1179, 1116 },
		{  0, 1182  , 1119, 1082 },
		{ 10, 1111  , 1054, 1041 },
		{ 20, 1024  , 1000, 1000 },
		{ 30, 943	,  931,  938 },
		{ 40, 853	,  847,  875 },
};

static
int wJZ,wVJZ, wDVJZ;  // 220V 380V 温度补偿值

#define DataList   CVDataList
#define WVDataList WVDataList

/*
 * 根据实际温度计算温度补偿值
 */
void createWJZ(int wV)
{
	int i;
	wJZ  = 1024;
	wVJZ = 1000;
	wDVJZ = 1000;
	if( 0 == pUsrCfg->WDJZen ) return;//温度校正关闭
	for (i = 1; i < 6; i++) {
		if (wV < WDlist[i].wd) {
			int t = WDlist[i].wd - wV;

			wJZ = WDlist[i].wVJZ
					+ (WDlist[i - 1].wVJZ - WDlist[i].wVJZ) * t / 10;
			wVJZ = WDlist[i].wWVJZ
					+ (WDlist[i - 1].wWVJZ - WDlist[i].wWVJZ) * t / 10;
			wDVJZ = WDlist[i].wDVJZ
					+ (WDlist[i - 1].wDVJZ - WDlist[i].wDVJZ) * t / 10;
			break;
		} else if (wV == WDlist[i].wd) {
			wJZ   = WDlist[i].wVJZ;;
			wVJZ  = WDlist[i].wWVJZ;
			wDVJZ = WDlist[i].wDVJZ;
		}
	}
}


int GetWJZValue( int tmp )
{
#if DEBUG
	return tmp;
#else
	if( wJZ==1024 )
	{
		return tmp;
	}else
	{
		return tmp * 1024 / wJZ;
	}
#endif
}

/*
 * 通过采集的数据，计算380V电压值
 */
int GetValueWV( int Value, int Vjz )
{
    TDatalist list1, list2;
    int i;

    Value = Value * Vjz / wVJZ;
    for(i=1; i<ARRAY_SIZE( WVDataList ); i++)
    {
        if(  Value >= WVDataList[i].Vin )
        {
            list1 = WVDataList[i-1];
            list2 = WVDataList[i];
            int32 a;
            a = (Value - list2.Vin) * ( list1.Vout - list2.Vout ) / ( list1.Vin - list2.Vin ) + list2.Vout;
            return a;
        }
    }
    return 0;
}

/*
 * 计算
 */
int GetValueWV_JZ( int Value )
{
    TDatalist list1, list2;
    int i;

    for(i=1; i<ARRAY_SIZE( WVDataList ); i++)
    {
        if(  Value >= WVDataList[i].Vout )
        {
            list1 = WVDataList[i-1];
            list2 = WVDataList[i];
            int32 a;
            a = (Value - list2.Vout) * ( list1.Vin - list2.Vin ) / ( list1.Vout - list2.Vout ) + list2.Vin;
            return a;
        }
    }
    return 0;
}

/*
 * 通过采集的数据，计算24V电压值
 */
int GetValueVD( int Value, int Vjz )
{
    Value = Value * Vjz / wDVJZ;
    if( Value > VDDataList[1].Vin )
    {
        Value = ( (Value - VDDataList[1].Vin) * ( VDDataList[0].Vout - VDDataList[1].Vout ) * 10 + 5)
            / ( VDDataList[0].Vin - VDDataList[1].Vin ) / 10 + VDDataList[1].Vout;
    }else
    {
        Value = ((Value - VDDataList[2].Vin) * ( VDDataList[1].Vout - VDDataList[2].Vout ) * 10 + 5)
            / ( VDDataList[1].Vin - VDDataList[2].Vin ) / 10 + VDDataList[2].Vout;
    }
    if( Value > 500 )
        Value = 500;

    if( Value < 8 )
        Value = 0;
    return Value;
}


/*
 * 通过采集的数据，计算220V电压值
 */
int GetValueV( int Vtime, int Vjz )
{
    TDatalist list1, list2;
    int i;
    Vtime = Vtime * Vjz  / wJZ;
    for(i=1; i<ARRAY_SIZE( DataList ); i++)
    {
        if(  Vtime >= DataList[i].Vin )
        {
            list1 = DataList[i-1];
            list2 = DataList[i];
            int32 a;
            a = ((Vtime - list2.Vin)* 20 /(list1.Vin - list2.Vin) + 1 )/2  + list2.Vout;
            return a;
        }
    }
    return 0;
}

int GetValueV_JZ( int Value )
{
    TDatalist list1, list2;
    int i;

    for(i=1; i<ARRAY_SIZE( DataList ); i++)
    {
        if(  Value >= DataList[i].Vout )
        {
            list1 = DataList[i-1];
            list2 = DataList[i];
            int32 a;
            a = (Value - list2.Vout) * ( list1.Vin - list2.Vin ) / ( list1.Vout - list2.Vout ) + list2.Vin;
            return a;
        }
    }
    return 0;
}


void UserVJZ( int16 value )
{
    int i;
    for(i=0; i<MAX_DATA_SIZE; i++)
    {
        switch( data[i].type )
        {
            case eTTwv:
            	if( value == 0 ){
            		value = 380;
            	}
                if( value > 350 && value < 450 && data[i].tmp > 300 )
                {
                    pUsrCfg->JZ[i] = GetValueWV_JZ(value ) * wVJZ / data[i].tmp ;
                    pr_debug("%d jzWV %d: %d\n", i, value, data[i].tmp);
                }
                break;
            case eTTv:
            	if( value == 0 ){
            		value = 220;
            	}
                if( value > 180 && value < 256 && data[i].tmp > 180 )
                {
                    pUsrCfg->JZ[i] = GetValueV_JZ( value ) * wJZ / data[i].tmp;
                    pr_debug("%d jzV %d: %d\t%d\n", i, value, data[i].tmp, pUsrCfg->JZ[i]);
                }
                break;
        }
    }
    UsrWrite();
}

void jz_cmd( int args, char **argv );
void jz_cmd( int args, char **argv )
{
    int i;
    for(i=0; i<MAX_DATA_SIZE; i++)
    {
        switch( data[i].type )
        {
            case TT_VD:
                if( data[i].tmp > 100 )
                {
                    pUsrCfg->JZ[i] = VDDataList[1].Vin * wDVJZ / data[i].tmp ;
                }
                break;
            case TT_WV:
                if( data[i].tmp > 200 )
                {
                    pUsrCfg->JZ[i] = WVDataList[WVDATALIST_380].Vin * wVJZ / data[i].tmp ;
                }
                break;
            case TT_V:
                if( data[i].tmp > 200 )
                {
                    pUsrCfg->JZ[i] = DataList[VDATALIST_220].Vin * wJZ / data[i].tmp;
                }
                break;
            case TT_ID:
            case TT_I:
                if( data[i].tmp > 100 )
                    pUsrCfg->JZ[i] = data[i].tmp;
                break;
        }
    }
    UsrWrite();
}
PSHELL_CMD( jz, jz_cmd, "jz" );

static void jzV_cmd( int args, char **argv )
{
    int a;
    if( args >= 2 )
    {
        a = atoi( argv[1] );
        pr_debug("jzV %d\n", a);
        UserVJZ( a );
    }
}
PSHELL_CMD(jzV, jzV_cmd, "jzV V");
