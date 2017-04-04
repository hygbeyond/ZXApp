//protocol.h


#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "nuc_can.h"
#include "global.h"
#include "protocol_def.h"
#include "data_def.h"

#define TRANS_COMTEST                   0x0A

#define SYN_BYTE	0xAA		//同步码
#define ERR_BYTE	0xA5		//错误码


extern const char *strCGQtype[];


typedef union TTKValue
{
	uint8 buff[8];
    struct
    {
		uint8  TKtype;
		uint8  TTcnt;
		uint16 TKstate;
		uint16 TKstate2;
		uint8  ZKaddr;
		union {
			uint8 version;
			struct {
				uint8 p :5;
				uint8 v :3;
			};
		};
    };
}__PACKED__ TTKValue;



typedef struct DATA
{
	uint16 hostAddr;         //当前发送数据过来的CAN控制器的ID号
	uint16 localAddr;        //从机ID号，不在一条CAN总线的CAN控制器ID值
	uint8  cmd;				 //控制命令
	uint8  dat[];				 //扩展数据
}DATA,*PDATA;

typedef struct TRANSMITER
{
	uint8 T1;				//0xFA
	uint8 T2;				//0xFB
	uint8 cnt;				//sizeof(data) 数据包data的大小 最大不超过128
	DATA  data;				//数据包
	uint16 crc;				//crc校验码
}TRANSMITER,*PTRANSMITER;

#define TPROC_T1     0xFA
#define TPROC_T2     0xFB
#define TPROC_SIZE   129

typedef struct TProtocol
{
    uint8 T1;
    uint8 T2;
    uint8 Tcnt;
    uint8 buff[TPROC_SIZE];
    uint16 crc;
	uint8  state;
    uint8  size;
}__PACKED__ TProtocol;

/*
 * 从传感器中传输过来的数据信息结构
 */
typedef struct TProData
{
    uint8  type;
    uint8  TTcnt;
    uint16 addr;
    union{
        uint16  state;
        struct{
            uint16  Zstate :1;        /*主电开关状态 1开 0关*/
            uint16  Bstate :1;        /*备电开关状态 1开 0关*/
            uint16  Tstate :1;        /*传感器状态  0 正常 1 掉线*/
            uint16  Wstate :1;        /*温度状态    0 正常 1 过温*/
            uint16  Za     :2;        /*主电A相状态 0 正常 1过压 2欠压 3断电*/
            uint16  Zb     :2;        /*主电B相状态*/
            uint16  Zc     :2;        /*主电C相状态*/
            uint16  Ba     :2;        /*备电A相状态*/
            uint16  Bb     :2;        /*备电B相状态*/
            uint16  Bc     :2;        /*备电C相状态*/
        };
        struct{
            uint16  t1:2;
            uint16  t2:2;
            uint16  t3:2;
            uint16  t4:2;
            uint16  t5:2;
            uint16  t6:2;
            uint16  t7:2;
            uint16  t8:2;
        };
    };
    union{
        uint16 state2;
        struct{
            uint16  t21:2;
            uint16  t22:2;
            uint16  t23:2;
            uint16  t24:2;
            uint16  t25:2;
            uint16  t26:2;
            uint16  t27:2;
            uint16  t28:2;
        };
    };
    union{
        uint16 data;             /*故障数值*/
        struct{
            uint16 value  : 13;
            uint16 type   : 3;
        };
    }TTbuff[12];
} __PACKED__ TProData;


/*
#ifdef __cplusplus
extern "C" {
#endif
*/
extern int SetAddress;
extern int ProCanError;

void Pro_Init(void );

//函数声明
void CanCmdMsg( CAN_msg *pMsg );
uint8 BJ_Trans( uint8 addr);
uint8 TK_Trans( uint8 cmd );
uint8 TT_Trans( uint8 addr );
uint8 GZ_Trans( uint8 addr, uint8 en, uint8 state );
uint8 CAN_WriteAll( void );
void  AutoTrans(void);

#define TK_AutoTrans()      TK_Trans( TCMDDATA_TK )

/*
#ifdef __cplusplus
}
#endif
*/

#undef PROTOCOL_EXT
#endif /*__PROTOCOL_H__*/


