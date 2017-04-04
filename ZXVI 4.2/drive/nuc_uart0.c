

#define UART_GLOBAL
#include "mytype.h"
#include "bsp.h"
#include "nuc_uart0.h"


#include "myFifo.h"

typedef uint8 Tser;
TFIFO_TYPE( Tser, 64 );
typedef FIFO_TYPE( Tser ) Tserial;

static Tserial UartList_Rx,UartList_Tx;
static int TxEn;


/*---------------------------------------------------------------------------------------------------------*/
/* UART Callback function                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
__WEAK__
void U1_Isr()
{
}
__WEAK__
void U0_Isr()
{
    uint32_t u32IntSts = UART0->ISR;
    if(u32IntSts & UART_ISR_RDA_INT_Msk)
    {
        /* Get all the input characters */
        while(UART0->ISR & UART_ISR_RDA_IF_Msk)
        {
            /* Get the character from UART Buffer */
            FIFO_PUT( UartList_Rx, UART0->RBR);
        }
    }

    if(u32IntSts & UART_ISR_THRE_INT_Msk)
    {
		if (!FIFO_Get(UartList_Tx, UART0->THR)) {
		    UART0->IER &= ~UART_IER_THRE_IEN_Msk;
			TxEn = 1;
		}
    }
}


void UART02_IRQHandler(void)
{
    U0_Isr();
    U1_Isr();
}

void Uart_Init (uint32 baudrate )
{
    FIFO_CLEAN( UartList_Rx );
    FIFO_CLEAN( UartList_Tx );
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable UART module clock */
    CLK->APBCLK |= CLK_APBCLK_UART0_EN_Msk;

    /* Select UART module clock source */
    CLK->CLKSEL1 &= ~CLK_CLKSEL1_UART_S_Msk;
    CLK->CLKSEL1 |= CLK_CLKSEL1_UART_S_HXT;

    /* Lock protected registers */
    SYS_LockReg();

    /* Reset UART IP */
    SYS->IPRSTC2 |=  SYS_IPRSTC2_UART0_RST_Msk;
    SYS->IPRSTC2 &= ~SYS_IPRSTC2_UART0_RST_Msk;

    /* Configure UART0 and set UART0 Baudrate */
    UART0->BAUD = UART_BAUD_MODE2 | UART_BAUD_MODE2_DIVIDER(__HXT, baudrate);
    UART0->LCR = UART_WORD_LEN_8 | UART_PARITY_NONE | UART_STOP_BIT_1;

    /* Enable Interrupt and install the call back function */
    UART0->IER |= UART_IER_RDA_IEN_Msk /*| UART_IER_THRE_IEN_Msk*/ | UART_IER_TOUT_IEN_Msk ;
    NVIC_EnableIRQ(UART02_IRQn);
    TxEn = 1;
}

int Uart_GetChar_nb( void )
{
	int r;
    if( FIFO_Get( UartList_Rx, r ) )
    {
        return r;
    }
    return -1;
//    if((UART0->FSR & UART_FSR_RX_EMPTY_Msk) == 0 ) {
//        return (UART0->DATA);
//    }
//    return -1;
}

void Uart_SendByte (uint8 ucDat)
{
    UART0->IER &= ~UART_IER_THRE_IEN_Msk;
//	TxEn = 0;
    UART0->THR = ucDat;                          /*  写入数据                    */
    while(UART0->FSR & UART_FSR_TX_FULL_Msk);     /*  等待数据发送完毕            */
    UART0->IER |= UART_IER_THRE_IEN_Msk;
//    while( TxEn == 0 );
}


void Uart_SendString(uint8 *pstr)
{
    while(*pstr)
    {
        Uart_SendByte(*pstr++);
    }
}

uint8 Uart_SendBuff( uint8 *buff, int size )
{
	int i=0;
	if( TxEn )
	{
		UART0->THR = buff[i++];
	}
	while( i<size )
	{
		if( !FIFO_PUT( UartList_Tx, buff[i]) )
			break;
		i++;
	}
	TxEn = 0;
    UART0->IER |= UART_IER_THRE_IEN_Msk;
	return i;
}
