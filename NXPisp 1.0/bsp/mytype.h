

#ifndef __MY_TYPE_H__
#define __MY_TYPE_H__

#include <stdint.h>

//This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL (void*)0
#endif

typedef unsigned char  uint8;          /* defined for unsigned 8-bits integer variable      无符号8位整型变量  */
typedef signed   char  int8;           /* defined for signed 8-bits integer variable        有符号8位整型变量  */
typedef unsigned short uint16;         /* defined for unsigned 16-bits integer variable     无符号16位整型变量 */
typedef signed   short int16;          /* defined for signed 16-bits integer variable       有符号16位整型变量 */
typedef unsigned int   uint32;         /* defined for unsigned 32-bits integer variable     无符号32位整型变量 */
typedef signed   int   int32;          /* defined for signed 32-bits integer variable       有符号32位整型变量 */
typedef float          fp32;           /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;           /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
#ifndef BOOL
typedef unsigned int   BOOL;
#endif

#define bool           BOOL
//typedef uint8 	BYTE;
//typedef uint16  WORD;
//typedef uint32  DWORD;

/* These types must be 16-bit, 32-bit or larger integer */
typedef int				INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef signed char		CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types must be 32-bit integer */
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

/* Boolean type */
//typedef enum { FALSE = 0, TRUE } BOOL;

typedef int  _int;
typedef uint32  size_t;

#define CONST_UINT8 		const char
#define CONST_UINT16 		const uint16

typedef void  (*FUNC)       (void);
typedef uint8 (*DRIVE_WRITE)(uint32,uint16,uint8*,uint16);
typedef void  (*DRIVE_READ )(uint32,uint16,uint8*,uint16);
typedef void  (*PUT)        (uint8);

#define ARRAY_SIZE(x)       (sizeof((x))/sizeof((x)[0]))
#define ZERO_BUFF(x)        (memset((x),0,sizeof(x)))


#ifndef PSTR
#define pgm_read_byte(a)	((CONST_UINT8*)a)[0]
#define pgm_read_word(a)    ((uint16)((CONST_UINT8*)a)[0] + (((CONST_UINT8*)a)[1]<<8))
#define PSTR(a)				(CONST_UINT8*)(a)
#define FSTR(a)				(CONST_UINT8*)(a)
#define PROGMEM 			
#endif

#define GET_PVOID(a)        (*(a))


#define ResetMCU()          (SCB->AIRCR = (0x05fa << 16) + 4)

#define __UDATA__     __attribute__((section(".udata")))
#define __NDATA__     __attribute__((section(".ndata"))) 
#define __NOINIT__    __attribute__((section(".noinit")))
#define __AFTER__     __attribute__ ((section(".after_vectors")))
#define __ROM_DATA__  __attribute__ ((section(".rom_data")))

#define WEAK          __attribute__ ((weak))                         /*符号重复定义,当前函数可以被替换*/
#define ALIAS(f)      __attribute__ ((weak, alias (#f)))             /*符号重复定义,f函数重新定义一个别名*/
#define __PACKED__    __attribute__ ((packed))                       /*取消结构在编译过程中的优化对齐,按照实际占用字节数进行对齐*/
#define __WEAK__      WEAK

#define OSTASK(_signame)                          \
    void _signame (void) __attribute__ ((noreturn))

/* Indirect stringification.  Doing two levels allows the parameter to be a
 * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
 * converts to "bar".
 */
#ifndef __stringify
#define __stringify_1(x)	#x
#define __stringify(x)		__stringify_1(x)
#endif


#ifndef __PRINTF
#define __PRINTF(a,b)      __attribute__ ((__format__ (__printf__,(a),(b))))
#endif

#ifndef MAX
#define MAX(a,b)      ( (a)>(b)? (a): (b) )
#endif

#ifndef MIN
#define MIN(a,b)      ( (a)<(b)? (a): (b) )
#endif

#ifndef BIT
#define BIT(a)         (1<<(a))
#endif

#ifndef SBI
#define SBI(port, bit) (port) |=  (1 << (bit))
#endif /*SBI*/

#ifndef CBI
#define CBI(port, bit) (port) &= ~(1 << (bit))
#endif /*CBI*/

#ifndef PBI
#define PBI(pin,  bit) ((pin) &   (1 << (bit)))
#endif /*PBI*/

#ifndef NBI
#define NBI(port, bit) (port) ^=  (1 << (bit))
#endif /*NBI*/


#ifndef offset_of
#define offset_of(_type, _memb)  ((unsigned long)(&((_type *)0)->_memb))
#endif

#ifndef container_of
#define container_of(_obj, _type, _memb) ((_type *)(((char *)_obj) - offset_of(_type, _memb)))
#endif


#ifndef __PRINT__
#define __PRINT__                     __attribute__ ((__format__ (__printf__, 1, 0)))
#endif


#if _end > __stack_end__
#error "内存错误溢出"
#endif


#define  DEF_DISABLED                                      0
#define  DEF_ENABLED                                       1

#define  DEF_FALSE                                         0
#define  DEF_TRUE                                          1

#define  DEF_NO                                            0
#define  DEF_YES                                           1

#define  DEF_OFF                                           0
#define  DEF_ON                                            1

#define  DEF_CLR                                           0
#define  DEF_SET                                           1

#define  DEF_ACTIVE                                        0
#define  DEF_INACTIVE                                      1

#define  DEF_FAIL                                          0
#define  DEF_OK                                            1


/* -------------------- BIT DEFINES ------------------- */
#define  DEF_BIT_NONE                                   0x00

#define  DEF_BIT_00                                     0x01
#define  DEF_BIT_01                                     0x02
#define  DEF_BIT_02                                     0x04
#define  DEF_BIT_03                                     0x08
#define  DEF_BIT_04                                     0x10
#define  DEF_BIT_05                                     0x20
#define  DEF_BIT_06                                     0x40
#define  DEF_BIT_07                                     0x80

#define  DEF_BIT_08                                   0x0100
#define  DEF_BIT_09                                   0x0200
#define  DEF_BIT_10                                   0x0400
#define  DEF_BIT_11                                   0x0800
#define  DEF_BIT_12                                   0x1000
#define  DEF_BIT_13                                   0x2000
#define  DEF_BIT_14                                   0x4000
#define  DEF_BIT_15                                   0x8000

#define  DEF_BIT_16                               0x00010000
#define  DEF_BIT_17                               0x00020000
#define  DEF_BIT_18                               0x00040000
#define  DEF_BIT_19                               0x00080000
#define  DEF_BIT_20                               0x00100000
#define  DEF_BIT_21                               0x00200000
#define  DEF_BIT_22                               0x00400000
#define  DEF_BIT_23                               0x00800000

#define  DEF_BIT_24                               0x01000000
#define  DEF_BIT_25                               0x02000000
#define  DEF_BIT_26                               0x04000000
#define  DEF_BIT_27                               0x08000000
#define  DEF_BIT_28                               0x10000000
#define  DEF_BIT_29                               0x20000000
#define  DEF_BIT_30                               0x40000000
#define  DEF_BIT_31                               0x80000000


#endif /*__MY_TYPE_H__*/


