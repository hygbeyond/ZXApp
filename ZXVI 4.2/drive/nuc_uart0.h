

#ifndef __UART0_H__
#define __UART0_H__


void  Uart_Init (uint32 baudrate );
int   Uart_GetChar_nb( void );
void  Uart_SendByte(uint8 dat);
void  Uart_SendString(uint8 *pstr);
uint8 Uart_SendBuff( uint8 *buff, int size );

#endif

