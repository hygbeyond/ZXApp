

#define UART_GLOBAL
#include "mytype.h"
#include "bsp.h"
#include "uart.h"


#define IER_RBR         (0x01<<0)
#define IER_THRE        (0x01<<1)
#define IER_RLS         (0x01<<2)

#define IIR_PEND        0x01
#define IIR_RLS         0x03
#define IIR_RDA         0x02
#define IIR_CTI         0x06
#define IIR_THRE        0x01

#define LSR_RDR         (0x01<<0)
#define LSR_OE          (0x01<<1)
#define LSR_PE          (0x01<<2)
#define LSR_FE          (0x01<<3)
#define LSR_BI          (0x01<<4)
#define LSR_THRE        (0x01<<5)
#define LSR_TEMT        (0x01<<6)
#define LSR_RXFE        (0x01<<7)

#define BUFSIZE         0x40

/* RS485 mode definition. */
#define RS485_NMMEN		(0x1<<0)
#define RS485_RXDIS		(0x1<<1)
#define RS485_AADEN		(0x1<<2)
#define RS485_SEL		(0x1<<3)
#define RS485_DCTRL		(0x1<<4)
#define RS485_OINV		(0x1<<5)



#include "myFifo.h"

typedef uint8 Tser;
TFIFO_TYPE( Tser, 128 );
typedef FIFO_TYPE( Tser ) Tserial;

static Tserial UartList_Rx;


void UART_IRQHandler (void)
{
    uint8_t IIRValue;
    uint8_t Dummy = Dummy;

    IIRValue = LPC_UART->IIR;

    IIRValue >>= 1;			/* skip pending bit in IIR */
    IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
    if (IIRValue == IIR_RDA)	/* Receive Data Available */
    {
        FIFO_PUT( UartList_Rx, LPC_UART->RBR);
    }
    else if (IIRValue == IIR_RLS)		/* Receive Line Status */
    {
        uint8_t LSRValue = LPC_UART->LSR;
        /* Receive Line Status */
        if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
        {
            /* There are errors or break interrupt */
            /* Read LSR will clear the interrupt */
            Dummy = LPC_UART->RBR;	/* Dummy read on RX to clear
                                       interrupt, then bail out */
            return;
        }
        if (LSRValue & LSR_RDR)	/* Receive Data Ready */
        {
            /* If no error on RLS, normal ready, save into the data buffer. */
            /* Note: read RBR will clear the interrupt */
            FIFO_PUT( UartList_Rx, LPC_UART->RBR);
        }
    }
    else if (IIRValue == IIR_CTI)	/* Character timeout indicator */
    {
        /* Character Time-out indicator */
        //UARTStatus0 |= 0x100;		/* Bit 9 as the CTI error */
    }
    else if (IIRValue == IIR_THRE)	/* THRE, transmit holding register empty */
    {
/* THRE interrupt */
//        LSRValue = LPC_UART->LSR;		/* Check status in the LSR to see if
//                                           valid data in U0THR or not */
//        if (LSRValue & LSR_THRE)
//        {
//            UARTTxEmpty0 = 1;
//        }
//        else
//        {
//            UARTTxEmpty0 = 0;
//        }
    }
}

void Uart_Init (uint32 baudrate )
{
    uint16 usFdiv;

    FIFO_CLEAN( UartList_Rx );

    NVIC_DisableIRQ(UART_IRQn);
    SYSAHBCLKCTRL_SET( AHBCLK_UART );                                   /*  打开UART功能部件时钟        */

    LPC_IOCON->PIO1_6 |= 0x01;                                          /*  RXD  */
    LPC_IOCON->PIO1_7 |= 0x01;                                          /*  TXD  */

    LPC_SYSCON->UARTCLKDIV     = 0x01;                                  /*  UART时钟分频                */

    LPC_UART->LCR  = 0x83;                                              /*  允许设置波特率              */

    usFdiv = (FPCLK /LPC_SYSCON->UARTCLKDIV / 16) / baudrate;        /*  设置波特率                  */
    LPC_UART->DLM  = (usFdiv >> 8);
    LPC_UART->DLL  = (usFdiv & 0xFF); 
    LPC_UART->LCR  = 0x03;                                              /*  锁定波特率                  */
    LPC_UART->FCR  = 0x07;
    NVIC_EnableIRQ(UART_IRQn);
    LPC_UART->IER  = IER_RBR | IER_RLS;	/* Enable UART interrupt */
}

int Uart_GetChar_nb( void )
{
	int r;
    if( FIFO_Get( UartList_Rx, r ) )
    {
        return r;
    }
    return -1;
}


void Uart_clean(void)
{
    FIFO_CLEAN( UartList_Rx );
}

void Uart_SendByte (uint8 ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  写入数据                    */
    while ((LPC_UART->LSR & 0x40) == 0) {                               /*  等待数据发送完毕            */
    }
}

uint8 Uart_SendBuff( uint8 *buff, int size )
{
	int c = MIN(16, size );
	if( (LPC_UART->LSR & 0x40) == 0 ) return 0;
	for(int i=0; i<c; i++)
	{
		LPC_UART->THR = buff[i];
	}
	return c;
}

void Uart_SendString(uint8 *pstr)
{
    while(*pstr)
    {
        Uart_SendByte(*pstr++);
    }
}

