//protocol.h


#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "nuc_can.h"
#include "global.h"
#include "protocol_def.h"
#include "data_def.h"

#define TRANS_COMTEST                   0x0A

#define SYN_BYTE	0xAA		//ͬ����
#define ERR_BYTE	0xA5		//������


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
	uint16 hostAddr;         //��ǰ�������ݹ�����CAN��������ID��
	uint16 localAddr;        //�ӻ�ID�ţ�����һ��CAN���ߵ�CAN������IDֵ
	uint8  cmd;				 //��������
	uint8  dat[];				 //��չ����
}DATA,*PDATA;

typedef struct TRANSMITER
{
	uint8 T1;				//0xFA
	uint8 T2;				//0xFB
	uint8 cnt;				//sizeof(data) ���ݰ�data�Ĵ�С ��󲻳���128
	DATA  data;				//���ݰ�
	uint16 crc;				//crcУ����
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
 * �Ӵ������д��������������Ϣ�ṹ
 */
typedef struct TProData
{
    uint8  type;
    uint8  TTcnt;
    uint16 addr;
    union{
        uint16  state;
        struct{
            uint16  Zstate :1;        /*���翪��״̬ 1�� 0��*/
            uint16  Bstate :1;        /*���翪��״̬ 1�� 0��*/
            uint16  Tstate :1;        /*������״̬  0 ���� 1 ����*/
            uint16  Wstate :1;        /*�¶�״̬    0 ���� 1 ����*/
            uint16  Za     :2;        /*����A��״̬ 0 ���� 1��ѹ 2Ƿѹ 3�ϵ�*/
            uint16  Zb     :2;        /*����B��״̬*/
            uint16  Zc     :2;        /*����C��״̬*/
            uint16  Ba     :2;        /*����A��״̬*/
            uint16  Bb     :2;        /*����B��״̬*/
            uint16  Bc     :2;        /*����C��״̬*/
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
        uint16 data;             /*������ֵ*/
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

//��������
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


