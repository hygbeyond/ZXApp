
#define COMMAND_GLOBAL
#include "mytype.h"
#include "pcommand.h"
#include <string.h>
#include "printk.h"
#include "microOS.h"
#include "bsp.h"



#define NUL		0x00	//
#define SOH 	0x01	//01H ���ݿ鿪ʼ
#define STX		0x02	//���Ŀ�ʼ
#define ETX		0x03	//���Ľ���
#define EOT 	0x04	//04H ���ͽ���
#define ENQ 	0x05 	//����
#define ACK 	0x06 	//06H �Ͽ���Ӧ
#define BEL		0x07	//����
#define BS		0x08	// �˸�
#define HT		0x09	// ˮƽ�Ʊ��
#define FF		0x0C	//��ҳ��
#define CR		0x0D	// �س���
#define SO		0x0E	// �����л�
#define SI		0x0F	// �����л�
#define DLE 	0x10	//10H ��ֹ��������
#define X_ON 	0x11	//11H ���ݴ������� ��ͨ��˫�����ٶȲ�һ��ʱ��
						//�ɲ��ø��ַ�������ͨ���ٶȣ�������շ��ٶ�
						//̫�������½��ջ�������ʱ��
						//���͡�X-off�������ͷ���ʹ���ͷ���ͣ�������ݡ�
						//�൱��RS232�ӿڵ�DSR,CTS���źš�
#define X_OFF 	0x13	//13H ���ݴ���ֹͣ
#define NAK 	0x15	//15H ���Ͽ���Ӧ ����CRCУ���Э����������ź�����ĸ��C����43H�����档
#define SYN 	0x16	//16H ͬ��
#define ETB		0x17	//��������
#define CAN 	0x18	//18H ��������ȡ��
#define ESC		0x1B	//
#define DEL		0x7F	//ɾ��

#define EXIT_SHELL   0xFFAA   //�˳�shell

#define SHELL_EN


#ifdef SHELL_EN

static char str[128] ;
static int  cnt = 0;


void shell(int);


static uint8 GetKey( uint8 dat )
{
    if( cnt < sizeof(str) )
    {
        char temp = dat;
        switch(temp)
        {
            case 0:
                break;
            case '\r':
                goto GetKey_end;
            case BS:
                if(cnt)
                {
                    cnt--;
                    PutChar(temp);
                    PutChar(' ');
                    PutChar(temp);
                }
                break;
            default:
                if(temp >= ' ')
                {
                    str[cnt++] = temp;
                    PutChar(temp);
                }
                break;
        }
    }else
    {
GetKey_end:
        str[cnt] = '\0';
        PutChar('\n');
        cnt = 0;
        return(TRUE);
    }
    return(FALSE);
}

void shell(int ch)
{
    if(ch < 0 )
        return;

    if(GetKey(ch))
    {
        PCMD_string(str);
        printk("cmd>");
    }
}

#endif




