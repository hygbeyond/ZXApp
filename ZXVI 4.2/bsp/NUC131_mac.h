

#ifndef __LPC11XX_MAC_H__
#define __LPC11XX_MAC_H__

#include "NUC131.h"
#include "mytype.h"



/*
 * λ�����궨����
 */
#ifndef BIT
#define BIT(a)         (1<<(a))
#endif

#ifndef SBI
#define SBI(port, bit) ((port) |=  (1 << (bit)))
#endif /*SBI*/

#ifndef CBI
#define CBI(port, bit) ((port) &= ~(1 << (bit)))
#endif /*CBI*/

#ifndef PBI
#define PBI(pin,  bit) (((pin) &   (1 << (bit))))
#endif /*PBI*/

#ifndef NBI
#define NBI(port, bit) ((port) ^=  (1 << (bit)))
#endif /*NBI*/



/*
 * GPIO λ�����궨����,����λ��������Χ�ڣ�ͨ��C����λ������ʽ����
 */
#ifndef MYLIB_CONCAT           
#define MYLIB_CONCAT(a, b)            a ## b
#define MYLIB_CONCAT3(a, b, c)        a ## b ## c
#define MYLIB_CONCAT_EXPANDED(a, b)   MYLIB_CONCAT(a, b)
#endif


#ifndef SBI_P
#define DATA_PORT(name)               MYLIB_CONCAT(P, name)->DOUT
#define DDRP(name)                    MYLIB_CONCAT(P, name)->PMD
#define DDRPORT( name )               DDRP( name )

#define DDRMOD( pin, mod )            ((mod&3)<<(pin*2))

#define PIN(name )                    MYLIB_CONCAT(P, name)->PIN
#define PORT( name )                  MYLIB_CONCAT(P, name)->DOUT

#define PORT_IN( pin, bit )           DDRP( pin ) &= ~( DDRMOD(bit,3) )
#define PORT_OUT( pin, bit)           DDRP( pin ) =   (DDRP(pin) & (~DDRMOD(bit,3))) + DDRMOD(bit,1)

#if 0
#define PBI_P(pin, bit)               PBI( PIN( pin ), bit)
#define NBI_P(pin, bit)               if(PBI_P(pin,bit)) { CBI_P(pin,bit); } else { SBI_P(pin,bit); }
#define SBI_P(pin, bit)               SBI( DATA_PORT(pin), bit )
#define CBI_P(pin, bit)               CBI( DATA_PORT(pin), bit )
#else
#define PBI_P(pin, bit)               MYLIB_CONCAT3( P, pin, bit )
#define NBI_P(pin, bit)               MYLIB_CONCAT3( P, pin, bit ) ^= 1
#define SBI_P(pin, bit)               MYLIB_CONCAT3( P, pin, bit )  = 1
#define CBI_P(pin, bit)               MYLIB_CONCAT3( P, pin, bit )  = 0
#endif

#define SETPORT( pin, bit )           SBI_P( pin, bit )
#define CLRPORT( pin, bit )           CBI_P( pin, bit )
#endif


/*********************************************************************************************************
**  NVIC Interrupt Priority   NVIC�ж����ȼ�
*********************************************************************************************************/
#ifndef PRIO_ZERO
#define PRIO_ZERO            (0x00)                                     /* ���ȼ�Ϊ0                    */
#define PRIO_ONE             (0x01 << 5)                                /* ���ȼ�Ϊ1                    */
#define PRIO_TWO             (0x02 << 5)                                /* ���ȼ�Ϊ2                    */
#define PRIO_THREE           (0x03 << 5)                                /* ���ȼ�Ϊ3                    */
#define PRIO_FOUR            (0x04 << 5)                                /* ���ȼ�Ϊ4                    */
#define PRIO_FIVE            (0x05 << 5)                                /* ���ȼ�Ϊ5                    */
#define PRIO_SIX             (0x06 << 5)                                /* ���ȼ�Ϊ6                    */
#define PRIO_SEVEN           (0x07 << 5)                                /* ���ȼ�Ϊ7                    */
#endif
               


#endif



