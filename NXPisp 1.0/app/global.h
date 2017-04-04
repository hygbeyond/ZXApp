//global.h

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "printk.h"


#define HBTK_NAME          __stringify(HNAME)

#ifdef __cplusplus
extern "C" {
#endif

#define TT_STATE_NORMAL		0	//����
#define TT_STATE_BJ			1	//����
#define TT_STATE_YJ			2	//Ԥ��
#define TT_STATE_TK			3	//�ѿ�
#define TT_STATE_DD			4	//�ϵ�
#define TT_STATE_GZ         5


// ̽ͷ״̬����
#define TT_CMD_NORMAL		0x00	//����״̬
#define TT_CMD_SY_SUCCESS	0x0A	//����ɹ�
#define TT_CMD_SY_FAILED	0x0B	//����ʧ��
#define TT_CMD_POWER_OFF	0x05	//�ⲿ�ϵ�
#define TT_CMD_TK			0x06	//���ѿ�
#define TT_CMD_LOST_TWI		0x07	//ʧȥ����������
#define TT_CMD_TWI_ERROR	0x08	//������ͨ��ʧ��
#define TT_CMD_ERROR		0x09	//Xϵ��̽ͷ����


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


#define MAX_MJ_ADDR				200			//������������ַ����
#define MAX_MN_ADDR				110			//���������ӵ�ַ����
#define MAX_POINT_COUNT			MAXPOINT	//һ�����������ɴ����������
#define MAX_DELAY				30

#define MAX_ED_LD				1600		//���ʣ�����
#define MIN_ED_LD				200
#define INC_LD_CNT				10
#define INC_TKLD_CNT			50

#define MAX_ED_WD				145			//��󱨾�
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
	A1  //��ͨ��ת��ģ�飩��һ��̽����
	A2  //D��̽����
	A3  //һ����Ͷ�̽����HBTK-1000X
	A4  //һ����Ͷ˸���̽����
	A5	//new HBTK-1000QX
*/
#define HBTK_TYPE	HBTK_1000XQ

#define HBTT_TYPE	HBTT_TYPE_LD

#ifndef KEY_JY					//������
#define KEY_JY			0xA1
#endif

typedef struct _USER_CONFIG{
	uint16	edld;	//�©����	//�ʣ�����
	uint16	bjld;	//����©����	//����ʣ�����
	uint16	gzdl;	//�ѿ�©����	//��������
	uint16 	eddl;	//�����		//�����
	uint8 	bjwd;	//�����¶�
	uint8	cfg;	//����
}USER_CONFIG,*LPUSER_CONFIG;

typedef struct MENU_SET{
	uint16 edld;	//�©��
	uint16 bjld;	//����©��
	uint16 bjwd;
	uint16 addr;	//��ַ
	uint8  bjys;	//������ʱ
	uint8  cnt;		//̽ͷ����
	uint8  enable;
	uint8  bj_en;               //�Ƿ񱨾�
}MENU_SET,*LPMENU_SET;

typedef struct PointType
{
	uint8  type;	//����
	uint8  state;	//״̬
	int16  value;	//����
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
