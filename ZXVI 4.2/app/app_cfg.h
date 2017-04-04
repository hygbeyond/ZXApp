
#ifndef __APP_CFG_H__
#define __APP_CFG_H__

#include "mytype.h"
#include "printk.h"

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


extern uint32 debug_en;
int dbg_print( const char *fmt, ... );
//#define dbg_print           printk
#define DEBUG_PRINT                      dbg_print
#define TRACE                            dbg_print

#define DEBUG0(fmt, args...)       do{if( debug_en & DEF_BIT_00 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG1(fmt, args...)       do{if( debug_en & DEF_BIT_01 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG2(fmt, args...)       do{if( debug_en & DEF_BIT_02 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG3(fmt, args...)       do{if( debug_en & DEF_BIT_03 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG4(fmt, args...)       do{if( debug_en & DEF_BIT_04 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG5(fmt, args...)       do{if( debug_en & DEF_BIT_05 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG6(fmt, args...)       do{if( debug_en & DEF_BIT_06 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG7(fmt, args...)       do{if( debug_en & DEF_BIT_07 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG8(fmt, args...)       do{if( debug_en & DEF_BIT_08 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG9(fmt, args...)       do{if( debug_en & DEF_BIT_09 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG10(fmt, args...)      do{if( debug_en & DEF_BIT_10 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG11(fmt, args...)      do{if( debug_en & DEF_BIT_11 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG12(fmt, args...)      do{if( debug_en & DEF_BIT_12 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG13(fmt, args...)      do{if( debug_en & DEF_BIT_13 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG14(fmt, args...)      do{if( debug_en & DEF_BIT_14 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG15(fmt, args...)      do{if( debug_en & DEF_BIT_15 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG16(fmt, args...)      do{if( debug_en & DEF_BIT_16 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG17(fmt, args...)      do{if( debug_en & DEF_BIT_17 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG18(fmt, args...)      do{if( debug_en & DEF_BIT_18 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG19(fmt, args...)      do{if( debug_en & DEF_BIT_19 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG20(fmt, args...)      do{if( debug_en & DEF_BIT_20 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG21(fmt, args...)      do{if( debug_en & DEF_BIT_21 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG22(fmt, args...)      do{if( debug_en & DEF_BIT_22 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG23(fmt, args...)      do{if( debug_en & DEF_BIT_23 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG24(fmt, args...)      do{if( debug_en & DEF_BIT_24 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG25(fmt, args...)      do{if( debug_en & DEF_BIT_25 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG26(fmt, args...)      do{if( debug_en & DEF_BIT_26 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG27(fmt, args...)      do{if( debug_en & DEF_BIT_27 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG28(fmt, args...)      do{if( debug_en & DEF_BIT_28 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG29(fmt, args...)      do{if( debug_en & DEF_BIT_29 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG30(fmt, args...)      do{if( debug_en & DEF_BIT_30 ) DEBUG_PRINT(fmt, ## args);}while(0);
#define DEBUG31(fmt, args...)      do{if( debug_en & DEF_BIT_31 ) DEBUG_PRINT(fmt, ## args);}while(0);


#define  SHELL_CMD_TIME                 1
#define  SHELL_CMD_STK                  1
#define  SHELL_CMD_FILE                 1
#define  SHELL_CMD_USER                 1
#define  SHELL_CMD_CAN                  1
#define  SHELL_CMD_IAP                  0
#define  SHELL_CMD_KEY                  1
#define  SHELL_CMD_BJ                   1
#define  SHELL_CMD_LIST_TEST            0
#define  SHELL_CMD_TCAN                 1

#define  FLASH_DEBUG                    1


#define   UART_BPS                   115200
#define   MAX_SVC_SIZE               128

#define   MAX_CAN_RX_SIZE            32



#endif



 
 
 
 
