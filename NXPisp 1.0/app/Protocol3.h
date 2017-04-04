

//protocol3.h


#ifndef __PROTOCOL3_H__
#define __PROTOCOL3_H__

/*
三代通信协议

一、通信数据发送格式：
	FA ＋ FB ＋ dataSize ＋（data……）＋ crcL ＋ crcH。
	所有通信数据是以0xFA＋0xFB开始，dataSize是后面发送数据的大小（≤255），data发送数据，crc为16位校验码。

二、通信模块控制命令
*/
#define RCOM_CMD_DATA        0xA0   //读取状态及数值
#define RCOM_CMD_SY          0xA1   //发送试验控制信号
#define RCOM_CMD_TYPE	     0xA2   //读取探头类型
#define RCOM_CMD_RESET       0xA3   //复位探头,关闭总线5秒即可
#define RCOM_CMD_GETADDR     0xA4   //读取地址
#define RCOM_CMD_TK          0xA5   //脱扣
#define RCOM_CMD_CLOSE       0xA6   //关闭总线
#define RCOM_CMD_OPEN        0xA7   //打开总线
#define RCOM_CMD_SETCNT      0xA8   //设置探头数量
#define RCOM_CMD_TRANSTYPE   0xA9   //控制板读取探头类型
#define RCOM_CMD_JZL         0xAA   //校正低
#define RCOM_CMD_JZH         0xAB   //校正高
#define RCOM_CMD_SETADDR     0xAC   //设置地址
#define RCOM_CMD_SETWANING   0xAD   //设置报警值
#define RCOM_CMD_SETTK       0xAE   //设置脱扣值
#define RCOM_CMD_VERSION     0xAF   //读取软件版本



#define RCOM_ENTER           0xAB
#define RCOM_ERROR           0xAA
#define RCOM_ERR_BUS         0x01   //总线短路或总线故障
#define RCOM_ERR_SETCNT      0x02   //设置探头数量不在范围内
#define RCOM_ERR_NOTCMD      0x03   //无效的命令

/*
1、RCOM_CMD_DATA(0xA0)
	RCOM_CMD_DATA读取探头状态及数据，发送数据格式：
	1、	MKaddr ＋ A0
	2、	MKaddr ＋ A0 ＋ TTaddr
	格式1：读取所有的探头数据，返回信息：所有探头状态＋探头数值＋探头总数
	State1 ＋ value1 ＋ type1 ＋ state2 ＋ value2 ＋ type2 ＋ …… ＋ 
    staten ＋ valuen ＋ typen ＋ n
	格式2：读取单个探头的数据，返回信息：探头状态＋探头数值＋探头地址
	State ＋ value ＋ type ＋ address

    错误:
    AA  ＋ 01 （总线短路）
2、ROCM_CMD_SY（0xA1）
	RCOM_CMD_SY发送试验控制信号，发送数据格式
	1、	MKaddr ＋ A1
	2、	MKaddr ＋ A1 ＋ TTaddr
3、RCOM_CMD_TYPE（0xA2）   //
	读取探头类型
	1、	MKaddr ＋ A2
	2、	MKaddr ＋ A2 ＋ TTaddr
4、RCOM_CMD_RESET（0xA3）   //
	复位探头,关闭总线5秒即可
	MKaddr ＋ A3
5、RCOM_CMD_GETADDR（0xA4）
    读取探头地址
    MKaddr ＋ A4
    返回格式
    1 ＋ 2 ＋ 3 + …… ＋ n（地址错误返回 FF）
6、RCOM_CMD_TK（0xA5）   //
	脱扣
	1、	MKaddr ＋ A5
	2、	MKaddr ＋ A5 ＋ TTaddr
*/

//探测器状态
#define  TK_STATE_FAIL         (1<<0)
#define  TK_STATE_WARNING      (1<<1)
#define  TK_STATE_POWERCUT     (1<<2)

//探头返回状态信息
#define  TT_STATE_FAIL         0x00    //故障
#define  TT_STATE_NOMAL        0x01    //正常
#define  TT_STATE_WARNING      0x02    //报警
#define  TT_STATE_POWERCUT     0x03    //断电
#define  TT_STATE_TEST         0x04	   //试验
#define  TT_STATE_ADDR         0x05    //返回地址
#define  TT_STATE_TYPE         0x07    //探头类型

#define  TT_FAIL_SYCG          0x55    //试验成功
#define  TT_FAIL_SYSB          0xAA    //试验失败
#define  TT_FAIL_DD            0xAB    //探头断电
#define  TT_FAIL_TK            0xAC    //探头脱扣


#define  TT_STATE_TESTCG       0x10    //试验成功
#define  TT_STATE_TESTSB       0x11    //试验失败
#define  TT_STATE_TK           0x12    //脱扣
#define  TT_STATE_DD           0x13    //断电



/*
state 探头状态
value 探头数值(剩余电流值 ＝ value × 10)
*/
/*
1、TT_STATE_FAIL（0x00）
	TT_STATE_FAIL探头故障，探头信息格式
	state（00） ＋ value （00）：value信息无效
2、TT_STATE_NOMAL（0x01）
	TT_STATE_NOMAL探头正常状态，探头信息格式：
	state（01） ＋ value（探头实际工作值）
3、TT_STATE_WARNING(0x02)
	TT_STATE_WARNING探头报警，探头信息格式
	state（02）＋ value（探头实际工作值）
4、TT_STATE_POWERCUT（0x03）
	TT_STATE_POWERCUT探头断电，探头信息格式
	state（03）＋ state2（0xAA、0xA5）0xAA脱扣断电，0xA5手动断电。
5、TT_STATE_TEST（0x04）
	TT_STATE_TEST探头试验，信息格式
	state（04）＋ state2（0xAA、0xA5）0xAA试验成功，0xA5试验失败
6、TT_STATE_ADDR（0x05）
	TT_STATE_ADDR探头地址，信息格式
	state（05）＋ address
7、TT_STATE_TYPE（0x07）
	TT_STATE_TYPE探头类型，信息格式
	state（07）＋ type（探头类型）
*/

/*探头控制命令*/
//兼容第一代命令
// ------------------------
//|  4   3   2   1   |  0   |
//|   扩展控制命令   | 地址 |
// ------------------------
//地址为1时，探头返回地址
//地址为0时，扩展命令有效

//扩展控制命令
#define CODE_NOMAL       0x00
//发送试验命令
#define CODE_SY          0x01
//发送脱扣命令
#define CODE_TK          0x02
//发送打开脱扣功能命令
#define CODE_TKON        0x03
//发送关闭脱扣功能命令
#define CODE_TKOFF       0x04
//发送打开剩余电流脱扣功能命令
#define CODE_LDTKON      0x05
//发送关闭剩余电流脱扣功能命令
#define CODE_LDTKOFF     0x06
//设置工作级数为一级
#define CODE_SET1J       0x07
//设置工作级数为二级
#define CODE_SET2J       0x08
//设置工作级数为三级
#define CODE_SET3J       0x09

//二代扩展命令
//设置当前模块地址
#define CODE_SET_ADDR    0x0A
//读取当前工作电流或温度数据
#define CODE_READ_COUNT  0x0B
//读取当前探头的类型
#define CODE_READ_TYPE   0x0C
//读取当前探头的状态
#define CODE_READ_STATE  0x0D

//三代扩展命令
//设置报警值
#define CODE_SET_WARNING 0x0E
//设置脱扣值
#define CODE_SET_TK      0x0F

/*
1、CODE_NOMAL(0x00)
	读取探头信息
	CMD(00) + address(探头地址)
2、CODE_SET_ADDR（0x0A）
	设置探头地址
	(CMD(0A)<<1) ＋ address(设置的地号)
3、CODE_SET_WARNING(0x0E)
	设置探头报警值
	(CMD(0E)<<1) ＋ BJvalue (剩余电流BJvalue ＝ BJvalue ÷ 10)
4、CODE_SET_TK(0x0F)
	设置探头脱扣值【只有剩余电流探头有】
	(CMD(0F)<<1) ＋ TKvalue (TKvalue ＝ TKvalue ÷ 10)
*/


#endif /*__PROTOCOL_H__*/



