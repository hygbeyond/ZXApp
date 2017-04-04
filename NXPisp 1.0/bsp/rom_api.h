

#ifndef __ROM_API_H__
#define __ROM_API_H__

#include "mytype.h"

/* set_pll mode options */
#define CPU_FREQ_EQU 0
#define CPU_FREQ_LTE 1
#define CPU_FREQ_GTE 2
#define CPU_FREQ_APPROX 3
/* set_pll result0 options */
#define PLL_CMD_SUCCESS 0
#define PLL_INVALID_FREQ 1
#define PLL_INVALID_MODE 2
#define PLL_FREQ_NOT_FOUND 3
#define PLL_NOT_LOCKED 4

typedef struct _PWRD {
    void (*set_pll)(unsigned int cmd[], unsigned int resp[]);
    void (*set_power)(unsigned int cmd[], unsigned int resp[]);
} PWRD;


/*********************************************************************************************************
  CAN 报文结构与变量定义
 *********************************************************************************************************/
#define CAN_MSGOBJ_STD 0x00000000UL                                     /* CAN 2.0a 11-bit ID           */
#define CAN_MSGOBJ_EXT 0x20000000UL                                     /* CAN 2.0b 29-bit ID           */
#define CAN_MSGOBJ_DAT 0x00000000UL                                     /* data frame	                */
#define CAN_MSGOBJ_RTR 0x40000000UL                                     /* rtr frame	                */

// error status bits
#define CAN_ERROR_NONE 0x00000000UL
#define CAN_ERROR_PASS 0x00000001UL
#define CAN_ERROR_WARN 0x00000002UL
#define CAN_ERROR_BOFF 0x00000004UL
#define CAN_ERROR_STUF 0x00000008UL
#define CAN_ERROR_FORM 0x00000010UL
#define CAN_ERROR_ACK 0x00000020UL
#define CAN_ERROR_BIT1 0x00000040UL
#define CAN_ERROR_BIT0 0x00000080UL
#define CAN_ERROR_CRC 0x00000100UL


/* 
 *  CAN报文结构体定义
 */
typedef struct _CAN_MSG_OBJ {
    union{
        uint32 mode_id;        /* 29 bit identifier */
        uint32 id;
        struct{//通信协议中数据分布
            uint32 IDTK     :11;  /*探测器地址     0  - 10 */
            uint32 IDTT     :8;   /*首位探头地址   11 - 18 */
            uint32 IDcmd    :8;   /*数据命令       19 - 26 */
            uint32 IDstate  :2;   /*扩展位状态     27 - 28 */
        };

        struct{
            uint32 IDtype  : 5;              //数据串类型
            uint32 IDRaddr : 8;             //发送本机地址
            uint32 IDSaddr : 8;             //发送目标板接收地址
            uint32 IDmod   : 8;             //数据发送模式
            uint32 IDnc    : 3;
        };
    };
    uint32 mask;
    union
    {
        struct __PACKED__
        {//29位ID发送数据格式
            uint8  Dtype;
            uint8  Dstate;
            uint16 Dvalue;
            uint8  ZKaddr;
        };
        struct  __PACKED__
        {
            uint32 DataA;
            uint32 DataB;
        };
        struct {
            uint16 value :13;
            uint16 type  :3;
        }Data[4];
        uint16 buff16[4];
        uint8  buff[8];           /* Data field */
    }; 
    union{
        uint8 dlc;
        uint8 len;
        uint8 cnt;
    };
    uint8 msgobj;
}CAN_MSG_OBJ;

/*********************************************************************************************************
  CANOpen 报文结构与变量定义
 *********************************************************************************************************/
typedef struct _CAN_ODCONSTENTRY {
    uint16 index;
    uint8 subindex;
    uint8 len;
    uint32 val;
}CAN_ODCONSTENTRY;

/*
 * upper-nibble values for CAN_ODENTRY.entrytype_len 
 */
#define OD_NONE   0x00                                  /* Object Dictionary entry doesn't exist        */
#define OD_EXP_RO 0x10                                  /* Object Dictionary entry expedited, read-only */
#define OD_EXP_WO 0x20                                  /* Object Dictionary entry expedited, write-only*/
#define OD_EXP_RW 0x30                                  /* Object Dictionary entry expedited, read-write*/
#define OD_SEG_RO 0x40                                  /* Object Dictionary entry segmented, read-only */
#define OD_SEG_WO 0x50                                  /* Object Dictionary entry segmented, write-only*/
#define OD_SEG_RW 0x60                                  /* Object Dictionary entry segmented, read-write*/

typedef struct _CAN_ODENTRY {
    uint16 index;
    uint8 subindex;
    uint8 entrytype_len;
    uint8 *val;
}CAN_ODENTRY;

typedef struct _CAN_CANOPENCFG {
    uint8 node_id;
    uint8 msgobj_rx;
    uint8 msgobj_tx;
    uint32 od_const_num;
    CAN_ODCONSTENTRY *od_const_table;
    uint32 od_num;
    CAN_ODENTRY *od_table;
}CAN_CANOPENCFG;

/* 
 *  CAN回调函数声明
 */
typedef struct _CAN_CALLBACKS {
    void (*CAN_rx)(uint8 msg_obj);
    void (*CAN_tx)(uint8 msg_obj);
    void (*CAN_error)(uint32 error_info);
    uint32 (*CANOPEN_sdo_read)(uint16 index, uint8 subindex);
    uint32 (*CANOPEN_sdo_write)(
            uint16 index, uint8 subindex, uint8 *dat_ptr);
    uint32 (*CANOPEN_sdo_seg_read)(
            uint16 index, uint8 subindex, uint8 openclose,
            uint8 *length, uint8 *data, uint8 *last);
    uint32 (*CANOPEN_sdo_seg_write)(
            uint16 index, uint8 subindex, uint8 openclose,
            uint8 length, uint8 *data, uint8 *fast_resp);
    uint8 (*CANOPEN_sdo_req)(
            uint8 length_req, uint8 *req_ptr, uint8 *length_resp,
            uint8 *resp_ptr);
}CAN_CALLBACKS;

/* 
 *  CAN固件函数结构体
 */
typedef struct _CAND {
    void (*init_can) (uint32 * can_cfg);
    void (*isr) (void);
    void (*config_rxmsgobj) (const CAN_MSG_OBJ * msg_obj);
    uint8 (*can_receive) (CAN_MSG_OBJ * msg_obj);
    void (*can_transmit) (CAN_MSG_OBJ * msg_obj);
    void (*config_canopen) (const CAN_CANOPENCFG * canopen_cfg);
    void (*canopen_handler) (void);
    void (*config_calb) (const CAN_CALLBACKS * callback_cfg);
}CAND;

/* 
 *  ROM中CAN固件地址
 */
typedef	struct _ROM {
    const unsigned p_usbd;
    const unsigned p_clib;
    const CAND * pCANAPI;
    const PWRD * pWRD;
    const unsigned p_dev1;
    const unsigned p_dev2;
    const unsigned p_dev3;
    const unsigned p_dev4; 
}ROM;

#define PROM     (*(volatile ROM **)0x1FFF1FF8)
//ROM ** rom = (ROM **) 0x1FFF1FF8;






#endif



