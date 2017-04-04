//global.h

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "printk.h"


#define HBTK_NAME          __stringify(HNAME)

#ifdef __cplusplus
extern "C" {
#endif

#define TT_STATE_NORMAL		0	//正常
#define TT_STATE_BJ			1	//报警
#define TT_STATE_YJ			2	//预警
#define TT_STATE_TK			3	//脱扣
#define TT_STATE_DD			4	//断电
#define TT_STATE_GZ         5


// 探头状态命令
#define TT_CMD_NORMAL		0x00	//正常状态
#define TT_CMD_SY_SUCCESS	0x0A	//试验成功
#define TT_CMD_SY_FAILED	0x0B	//试验失败
#define TT_CMD_POWER_OFF	0x05	//外部断电
#define TT_CMD_TK			0x06	//自脱扣
#define TT_CMD_LOST_TWI		0x07	//失去二总线连接
#define TT_CMD_TWI_ERROR	0x08	//二总线通信失败
#define TT_CMD_ERROR		0x09	//X系列探头故障


#define HOST_SY	1
#define LOCAL_SY 2

#define VOLUME_ON	1
#define VOLUME_OFF	0

#define BACK_LIGHT_ON	1
#define BACK_LIGHT_OFF	0

#define CAN_ENABLE		(1<<0)
#define SOUND_ENABLE	(1<<1)



#define LDJZ            0
#define WDJZH			1
#define WDJZL			2

#ifndef MAXPOINT
#define MAXPOINT          8
#endif


#define MAX_MJ_ADDR				200			//最大控制器主地址定义
#define MAX_MN_ADDR				110			//最大控制器从地址定义
#define MAX_POINT_COUNT			MAXPOINT	//一个控制器最大可带点个数定义
#define MAX_DELAY				30

#define MAX_ED_LD				1600		//最大额定剩余电流
#define MIN_ED_LD				200
#define INC_LD_CNT				10
#define INC_TKLD_CNT			50

#define MAX_ED_WD				145			//最大报警
#define MIN_ED_WD				40
#define INC_WD_CNT				5

#define MAX_SETWD               150
#define MIN_SETWD               50

#define MAX_SETLD               1600
#define MIN_SETLD               10    

#define MAX_WD_VALUE            1000
#define MIN_WD_VALUE            50

#define SET_WD_L				15
#define SET_WD_H				100



/*
	A1  //（通信转换模块）第一代探测器
	A2  //D型探测器
	A3  //一带多低端探测器HBTK-1000X
	A4  //一带多低端感温探测器
	A5	//new HBTK-1000QX
*/
#define HBTK_TYPE	HBTK_1000XQ

#define HBTT_TYPE	HBTT_TYPE_LD

#ifndef KEY_JY					//静音键
#define KEY_JY			0xA1
#endif

typedef struct _USER_CONFIG{
	uint16	edld;	//额定漏电流	//额定剩余电流
	uint16	bjld;	//报警漏电流	//报警剩余电流
	uint16	gzdl;	//脱扣漏电流	//工作电流
	uint16 	eddl;	//额定电流		//额定电流
	uint8 	bjwd;	//报警温度
	uint8	cfg;	//配置
}USER_CONFIG,*LPUSER_CONFIG;

typedef struct MENU_SET{
	uint16 edld;	//额定漏电
	uint16 bjld;	//报警漏电
	uint16 bjwd;
	uint16 addr;	//地址
	uint8  bjys;	//报警延时
	uint8  cnt;		//探头数量
	uint8  enable;
	uint8  bj_en;               //是否报警
}MENU_SET,*LPMENU_SET;

typedef struct PointType
{
	uint8  type;	//类型
	uint8  state;	//状态
	int16  value;	//数据
    uint16 d;
    uint8  trans;
    uint8  bj_delay;
    uint8  yj_delay;
    uint8  state_tmp;
}PointType,*PPointType;



#ifdef __cplusplus
}
#endif



#endif
