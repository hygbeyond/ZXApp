
#ifndef __DATA_H__
#define __DATA_H__

#include "global.h"

#define GETBUFF_SIZE   10

typedef struct {
    uint16 value;
    uint16 tmp;
    uint16 value_tmp;
    uint16 vbuff[GETBUFF_SIZE];
    uint8  state;
    uint8  type;
    uint8  trans;
    uint8  delay;
    uint8  bj_delay;
    uint8  yj_delay;
}TData;

typedef struct
{
    union{
		uint32 s32;
		struct {
			uint16 state;
			uint8 state2;
			int8 wValue;
		};
		struct {
			uint32 state_nc :4;
			uint32 Zs :6;
			uint32 Bs :6;
		};
		struct {
			uint32 :4;
			uint32 Tstate :16;      //所有探头故障信息
		};
		struct {
			uint32 Zstate :1; /*主电开关状态*/
			uint32 Bstate :1; /*备电开关状态*/
			uint32 Wstate :2; /*温度状态*/

			uint32 Za :2; /*主电A相状态 0 正常 1过压 2欠压 3断电*/
			uint32 Zb :2; /*主电B相状态*/
			uint32 Zc :2; /*主电C相状态*/
			uint32 Ba :2; /*备电A相状态*/
			uint32 Bb :2; /*备电B相状态*/
			uint32 Bc :2; /*备电C相状态*/

			uint32 t21 :2;
			uint32 t22 :2;
			uint32 t23 :2;
			uint32 t24 :2;
		};
    };
    uint16  trans;
}TDataState;

extern TData data[MAX_POINT_COUNT];
extern TDataState MKState;
extern int    wValue;

void Data_Init( uint8 );
void Data_CJ( void );
int  Data_PD( void );
void Data_Trans( void );

int GetWJZValue( int tmp );   //温度校正后采集的数据

void createList(int w );
void createWJZ(int wV);

int WValue( uint16 value );

int GetValueV( int Vtime, int Vjz );
int GetValueVD( int Value, int Vjz );
int GetValueWV( int Value, int Vjz );


void UserWDJZ( int16 value );
void UserLDJZ( int16 value );
void UserVJZ( int16 value );

const char *GetObjName( int type );

#if 0

void   sort(uint32 v[],int n);
uint32 pingjun( uint32 v[], int n );
int    GetValue( int Vtime, int Vjz );
#endif

#endif

