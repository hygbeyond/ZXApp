
#define COMMAND_GLOBAL
#include "mytype.h"
#include "pcommand.h"
#include <string.h>
#include "printk.h"
#include "microOS.h"
#include "bsp.h"



#define NUL		0x00	//
#define SOH 	0x01	//01H 数据块开始
#define STX		0x02	//正文开始
#define ETX		0x03	//正文结束
#define EOT 	0x04	//04H 发送结束
#define ENQ 	0x05 	//请求
#define ACK 	0x06 	//06H 认可响应
#define BEL		0x07	//响铃
#define BS		0x08	// 退格
#define HT		0x09	// 水平制表符
#define FF		0x0C	//换页键
#define CR		0x0D	// 回车键
#define SO		0x0E	// 不用切换
#define SI		0x0F	// 启用切换
#define DLE 	0x10	//10H 中止数据连接
#define X_ON 	0x11	//11H 数据传送启动 当通信双方的速度不一致时，
						//可采用该字符来调节通信速度，比如接收方速度
						//太慢而导致接收缓冲器满时，
						//发送“X-off”给发送方，使发送方暂停发送数据。
						//相当于RS232接口的DSR,CTS等信号。
#define X_OFF 	0x13	//13H 数据传送停止
#define NAK 	0x15	//15H 不认可响应 对于CRC校验的协议软件，本信号用字母“C”（43H）代替。
#define SYN 	0x16	//16H 同步
#define ETB		0x17	//传输块结束
#define CAN 	0x18	//18H 撤销传送取消
#define ESC		0x1B	//
#define DEL		0x7F	//删除

#define EXIT_SHELL   0xFFAA   //退出shell

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




