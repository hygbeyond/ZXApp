

#ifndef __UART_H__
#define __UART_H__


#ifndef UART_GLOBAL
#define EXT 
#else
#define EXT   extern 
#endif


EXT void  Uart_Init (uint32 baudrate );
EXT int   Uart_GetChar_nb( void );
EXT void  Uart_SendByte(uint8 dat);
EXT void  Uart_SendString(uint8 *pstr);
EXT uint8 Uart_SendBuff( uint8 *buff, int size );
void Uart_clean(void);



#undef EXT

#endif

