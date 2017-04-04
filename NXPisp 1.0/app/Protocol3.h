

//protocol3.h


#ifndef __PROTOCOL3_H__
#define __PROTOCOL3_H__

/*
����ͨ��Э��

һ��ͨ�����ݷ��͸�ʽ��
	FA �� FB �� dataSize ����data�������� crcL �� crcH��
	����ͨ����������0xFA��0xFB��ʼ��dataSize�Ǻ��淢�����ݵĴ�С����255����data�������ݣ�crcΪ16λУ���롣

����ͨ��ģ���������
*/
#define RCOM_CMD_DATA        0xA0   //��ȡ״̬����ֵ
#define RCOM_CMD_SY          0xA1   //������������ź�
#define RCOM_CMD_TYPE	     0xA2   //��ȡ̽ͷ����
#define RCOM_CMD_RESET       0xA3   //��λ̽ͷ,�ر�����5�뼴��
#define RCOM_CMD_GETADDR     0xA4   //��ȡ��ַ
#define RCOM_CMD_TK          0xA5   //�ѿ�
#define RCOM_CMD_CLOSE       0xA6   //�ر�����
#define RCOM_CMD_OPEN        0xA7   //������
#define RCOM_CMD_SETCNT      0xA8   //����̽ͷ����
#define RCOM_CMD_TRANSTYPE   0xA9   //���ư��ȡ̽ͷ����
#define RCOM_CMD_JZL         0xAA   //У����
#define RCOM_CMD_JZH         0xAB   //У����
#define RCOM_CMD_SETADDR     0xAC   //���õ�ַ
#define RCOM_CMD_SETWANING   0xAD   //���ñ���ֵ
#define RCOM_CMD_SETTK       0xAE   //�����ѿ�ֵ
#define RCOM_CMD_VERSION     0xAF   //��ȡ����汾



#define RCOM_ENTER           0xAB
#define RCOM_ERROR           0xAA
#define RCOM_ERR_BUS         0x01   //���߶�·�����߹���
#define RCOM_ERR_SETCNT      0x02   //����̽ͷ�������ڷ�Χ��
#define RCOM_ERR_NOTCMD      0x03   //��Ч������

/*
1��RCOM_CMD_DATA(0xA0)
	RCOM_CMD_DATA��ȡ̽ͷ״̬�����ݣ��������ݸ�ʽ��
	1��	MKaddr �� A0
	2��	MKaddr �� A0 �� TTaddr
	��ʽ1����ȡ���е�̽ͷ���ݣ�������Ϣ������̽ͷ״̬��̽ͷ��ֵ��̽ͷ����
	State1 �� value1 �� type1 �� state2 �� value2 �� type2 �� ���� �� 
    staten �� valuen �� typen �� n
	��ʽ2����ȡ����̽ͷ�����ݣ�������Ϣ��̽ͷ״̬��̽ͷ��ֵ��̽ͷ��ַ
	State �� value �� type �� address

    ����:
    AA  �� 01 �����߶�·��
2��ROCM_CMD_SY��0xA1��
	RCOM_CMD_SY������������źţ��������ݸ�ʽ
	1��	MKaddr �� A1
	2��	MKaddr �� A1 �� TTaddr
3��RCOM_CMD_TYPE��0xA2��   //
	��ȡ̽ͷ����
	1��	MKaddr �� A2
	2��	MKaddr �� A2 �� TTaddr
4��RCOM_CMD_RESET��0xA3��   //
	��λ̽ͷ,�ر�����5�뼴��
	MKaddr �� A3
5��RCOM_CMD_GETADDR��0xA4��
    ��ȡ̽ͷ��ַ
    MKaddr �� A4
    ���ظ�ʽ
    1 �� 2 �� 3 + ���� �� n����ַ���󷵻� FF��
6��RCOM_CMD_TK��0xA5��   //
	�ѿ�
	1��	MKaddr �� A5
	2��	MKaddr �� A5 �� TTaddr
*/

//̽����״̬
#define  TK_STATE_FAIL         (1<<0)
#define  TK_STATE_WARNING      (1<<1)
#define  TK_STATE_POWERCUT     (1<<2)

//̽ͷ����״̬��Ϣ
#define  TT_STATE_FAIL         0x00    //����
#define  TT_STATE_NOMAL        0x01    //����
#define  TT_STATE_WARNING      0x02    //����
#define  TT_STATE_POWERCUT     0x03    //�ϵ�
#define  TT_STATE_TEST         0x04	   //����
#define  TT_STATE_ADDR         0x05    //���ص�ַ
#define  TT_STATE_TYPE         0x07    //̽ͷ����

#define  TT_FAIL_SYCG          0x55    //����ɹ�
#define  TT_FAIL_SYSB          0xAA    //����ʧ��
#define  TT_FAIL_DD            0xAB    //̽ͷ�ϵ�
#define  TT_FAIL_TK            0xAC    //̽ͷ�ѿ�


#define  TT_STATE_TESTCG       0x10    //����ɹ�
#define  TT_STATE_TESTSB       0x11    //����ʧ��
#define  TT_STATE_TK           0x12    //�ѿ�
#define  TT_STATE_DD           0x13    //�ϵ�



/*
state ̽ͷ״̬
value ̽ͷ��ֵ(ʣ�����ֵ �� value �� 10)
*/
/*
1��TT_STATE_FAIL��0x00��
	TT_STATE_FAIL̽ͷ���ϣ�̽ͷ��Ϣ��ʽ
	state��00�� �� value ��00����value��Ϣ��Ч
2��TT_STATE_NOMAL��0x01��
	TT_STATE_NOMAL̽ͷ����״̬��̽ͷ��Ϣ��ʽ��
	state��01�� �� value��̽ͷʵ�ʹ���ֵ��
3��TT_STATE_WARNING(0x02)
	TT_STATE_WARNING̽ͷ������̽ͷ��Ϣ��ʽ
	state��02���� value��̽ͷʵ�ʹ���ֵ��
4��TT_STATE_POWERCUT��0x03��
	TT_STATE_POWERCUT̽ͷ�ϵ磬̽ͷ��Ϣ��ʽ
	state��03���� state2��0xAA��0xA5��0xAA�ѿ۶ϵ磬0xA5�ֶ��ϵ硣
5��TT_STATE_TEST��0x04��
	TT_STATE_TEST̽ͷ���飬��Ϣ��ʽ
	state��04���� state2��0xAA��0xA5��0xAA����ɹ���0xA5����ʧ��
6��TT_STATE_ADDR��0x05��
	TT_STATE_ADDR̽ͷ��ַ����Ϣ��ʽ
	state��05���� address
7��TT_STATE_TYPE��0x07��
	TT_STATE_TYPE̽ͷ���ͣ���Ϣ��ʽ
	state��07���� type��̽ͷ���ͣ�
*/

/*̽ͷ��������*/
//���ݵ�һ������
// ------------------------
//|  4   3   2   1   |  0   |
//|   ��չ��������   | ��ַ |
// ------------------------
//��ַΪ1ʱ��̽ͷ���ص�ַ
//��ַΪ0ʱ����չ������Ч

//��չ��������
#define CODE_NOMAL       0x00
//������������
#define CODE_SY          0x01
//�����ѿ�����
#define CODE_TK          0x02
//���ʹ��ѿ۹�������
#define CODE_TKON        0x03
//���͹ر��ѿ۹�������
#define CODE_TKOFF       0x04
//���ʹ�ʣ������ѿ۹�������
#define CODE_LDTKON      0x05
//���͹ر�ʣ������ѿ۹�������
#define CODE_LDTKOFF     0x06
//���ù�������Ϊһ��
#define CODE_SET1J       0x07
//���ù�������Ϊ����
#define CODE_SET2J       0x08
//���ù�������Ϊ����
#define CODE_SET3J       0x09

//������չ����
//���õ�ǰģ���ַ
#define CODE_SET_ADDR    0x0A
//��ȡ��ǰ�����������¶�����
#define CODE_READ_COUNT  0x0B
//��ȡ��ǰ̽ͷ������
#define CODE_READ_TYPE   0x0C
//��ȡ��ǰ̽ͷ��״̬
#define CODE_READ_STATE  0x0D

//������չ����
//���ñ���ֵ
#define CODE_SET_WARNING 0x0E
//�����ѿ�ֵ
#define CODE_SET_TK      0x0F

/*
1��CODE_NOMAL(0x00)
	��ȡ̽ͷ��Ϣ
	CMD(00) + address(̽ͷ��ַ)
2��CODE_SET_ADDR��0x0A��
	����̽ͷ��ַ
	(CMD(0A)<<1) �� address(���õĵغ�)
3��CODE_SET_WARNING(0x0E)
	����̽ͷ����ֵ
	(CMD(0E)<<1) �� BJvalue (ʣ�����BJvalue �� BJvalue �� 10)
4��CODE_SET_TK(0x0F)
	����̽ͷ�ѿ�ֵ��ֻ��ʣ�����̽ͷ�С�
	(CMD(0F)<<1) �� TKvalue (TKvalue �� TKvalue �� 10)
*/


#endif /*__PROTOCOL_H__*/



