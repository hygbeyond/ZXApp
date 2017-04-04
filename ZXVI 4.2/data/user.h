/*
文件名：user.h
*/


#ifndef __USER_H__
#define __USER_H__

#include "global.h"


typedef struct SSetWD
{
	uint16  bj;
	uint16  jzH;
	uint16  jzL;
}TSETWD,*LPTSETWD;

typedef struct SSetLD
{
	uint16  ed;
	uint16  bj;
	uint16  jz;
}TSETLD,*LPTSETLD;


typedef struct SSetDL
{
	uint16  ed;
	uint16  bj;
	uint16  jz;
}TSETDL,*LPTSETDL;


#define  USER_TYPE_BD              0   /*  冷备、热备  */

#define MAX_DATA_SIZE           8

typedef struct
{
    uint16 address;  /*通信地址*/
    uint8  count;
    uint8  type;     /*传感器类型*/
    uint8  delay;
    uint8  delayI;
    union{
		uint8 enable; /**/
		struct {
			uint8 BDenable: 1;      //冷备系统打
			uint8 WDJZen:   1;      //温度校正打开
		};
    };
    uint16 Aed;      /*额定电流*/
    uint16 Abj;      /*报警电流*/
    uint16 Vqy;      /*报警温度*/
    uint16 Vbj;
    uint16 JZ[MAX_DATA_SIZE];

    TSETLD LDCfg;           //剩余电流配置文件
	TSETWD WDCfg;           //温度配置文件
    TSETDL DLCfg;
}__PACKED__ TUser;


//----------------------------------------------------------------------
//全局变量

extern TUser *pUsrCfg;

void UsrInit( void );
void UsrDefault( void );
void UserSetDef( void );
void UsrWrite( void );

int UsrSetAddr( int addr );
int UsrSetDelay( int delay );
int UsrSetDelayI( int delay );
int UsrSetIed( int ed );
int UsrSetIbj( int bj );
int UsrSetVbj( int bj );
int UsrSetVqy( int bj );
int UsrSetType( int type );

int UsrSetWbj( int bj );
int UsrSetSbj( int bj );
int UsrSetCnt( int cnt );
int UsrSetBD( int cnt );


#endif /*__USER_H__*/

