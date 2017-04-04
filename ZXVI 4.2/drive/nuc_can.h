

#ifndef __MYCAN_H__
#define __MYCAN_H__


#define CAN_BAUDRATE              BPS_20K

#define STANDARD_FORMAT  0
#define EXTENDED_FORMAT  1

#define DATA_FRAME       0
#define REMOTE_FRAME     1

#define CAN_MSG_STD 0x00000000UL                                     /* CAN 2.0a 11-bit ID           */
#define CAN_MSG_EXT 0x20000000UL                                     /* CAN 2.0b 29-bit ID           */
#define CAN_MSG_DAT 0x00000000UL                                     /* data frame	                */
#define CAN_MSG_RTR 0x40000000UL                                     /* rtr frame	                */

#define __ID( state, cmd, tt, tk )             (((uint32)(state)<<27) | ((uint32)(cmd) <<19 )  | ((uint32)(tt)<<11 ) | ((tk)) | CAN_MSG_EXT)
#define __DATA( value, type )                  (((uint16)(value) & 0x1FFF) | ((uint16)(type) <<13))

#define CAN_TYPE_NORMAL   0
#define CAN_TYPE_S        1
#define CAN_TYPE_I        2
#define CAN_TYPE_W        3
#define CAN_TYPE_MS       4 
#define CAN_TYPE_MW       5

#define CAN_ID_NORMAL     0
#define CAN_ID_DATA       1

/* 
 *  CAN���Ľṹ�嶨��
 */
typedef struct _CAN_OBJ {
    union{
        uint32 mode_id;        /* 29 bit identifier */
        uint32 id;
        struct{//ͨ��Э�������ݷֲ�
            uint32 IDTK     :11;  /*̽������ַ     0  - 10 */
            uint32 IDTT     :8;   /*��λ̽ͷ��ַ   11 - 18 */
            uint32 IDcmd    :8;   /*��������       19 - 26 */
            uint32 IDstate  :2;   /*��չλ״̬     27 - 28 */
        };

        struct{
            uint32 IDtype  : 5;              //���ݴ�����
            uint32 IDRaddr : 8;             //���ͱ�����ַ
            uint32 IDSaddr : 8;             //����Ŀ�����յ�ַ
            uint32 IDmod   : 8;             //���ݷ���ģʽ
            uint32 IDnc    : 3;
        };
        
    };
    union
    {
         struct __PACKED__
        {
            uint8 DJsign;             //������Ϣ
            uint8 DJloop;             //��·���ַ
            uint8 DJaddr;             //�ƾߵ�ַ
            uint8 DJtype;             //��������
            uint8 DJncBuff[4];
        };
        
        struct  __PACKED__
        {
            uint8  cmd;
            union{
                uint16 addr;
                struct {
                    uint16 TKaddr: 11;
                    uint16 ZKaddr:  5;
                };
            };
            uint8  TTaddr;
            uint16 value;
            uint8  type;
            uint8  state;
        };
        struct __PACKED__
        {//29λID�������ݸ�ʽ
            uint8  Dtype;
            uint8  Dstate;
            int16  Dvalue;
            uint32 DTTstate;
        };
        struct  __PACKED__
        {
            uint32 DataA;
            uint32 DataB;
        };

        union{
            uint16 data;
            struct 
            {
                uint16 value :13;
                uint16 type  :3;
            };
        }Data[4];
        uint16 buff16[4];
        uint8  buff[8];           /* Data field */
    }; 

    union{
        uint8 dlc;
        uint8 len;
        uint8 cnt;
    };
}CAN_OBJ;
typedef CAN_OBJ CAN_msg;

void CAN_Init(uint32 addr, uint32 baudrate);

int CAN_init( uint32 baudrate);
/*
 * CAN_ReadMsg
 * ��can���ߵĽ��ջ����ж�ȡһ�����ݡ�
 */
int  CAN_rdMSG( CAN_msg *buff );
int  CAN_wrMSG( CAN_msg *pMsg );
void CAN_Clean( void );
int  CAN_GetErr( int en );

void CAN_wrMsg (CAN_msg *msg);

void CAN_start         (void);
void CAN_waitReady     (void);
void CAN_stMsg         (CAN_msg *msg);
void CAN_wrFilter      (uint32 id, uint32 msk, uint8 filter_type);

void CAN_testmode      (void);
void CAN_GetErrCnt( uint8 *pRxCnt, uint8 *pTxCnt);

#endif //__CAN_H__






