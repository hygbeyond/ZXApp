
#ifndef __NXPMCU_H__
#define __NXPMCU_H__


#define CMD_SUCCESS      0	//成功执行命令。只有成功执行了主机发出的命令后，才由ISP 处理器发送该代码。
#define INVALID_COMMAND  1	//无效命令
#define SRC_ADDR_ERROR   2	//源地址没有以字为边界
#define DST_ADDR_ERROR	3	//目标地址的边界错误
#define SRC_ADDR_NOT_MAPPED  4	//源地址的映射不在存储器映射中。无法考证运算值适用之处。
#define DST_ADDR_NOT_MAPPED  5	//目标地址的映射不在存储器映射中。无法考证运算值适用之处。
#define COUNT_ERROR      6	//字节计数值不是4 的倍数或是一个非法值
#define INVALID_SECTOR   7	//扇区号无效或结束扇区号大于起始扇区号
#define SECTOR_NOT_BLANK 8	//扇区非空
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION	9	//为写操作准备扇区的命令未执行
#define COMPARE_ERROR    10	//源和目标数据不相等
#define BUSY             11	//Flash 编程硬件接口忙
#define PARAM_ERROR      12	//参数不足或无效参数
#define ADDR_ERROR       13	//地址没有以字为边界
#define ADDR_NOT_MAPPED  14	//地址的映射不在存储器映射中。无法考证运算值的适用之处
#define CMD_LOCKED       15	//命令被锁定
#define INVALID_CODE     16	//解锁代码无效
#define INVALID_BAUD_RATE   17	//无效波特率设定
#define INVALID_STOP_BIT    18	//无效停止位设定
#define CODE_READ_PROTECTION_ENABLED 19	 //代码读保护使能


#define LPC_RAMSTART_LPC43XX    0x10000000L
#define LPC_RAMBASE_LPC43XX     0x10000200L

#define LPC_RAMSTART_LPC2XXX    0x40000000L
#define LPC_RAMBASE_LPC2XXX     0x40000200L

#define LPC_RAMSTART_LPC18XX    0x10000000L
#define LPC_RAMBASE_LPC18XX     0x10000200L

#define LPC_RAMSTART_LPC17XX    0x10000000L
#define LPC_RAMBASE_LPC17XX     0x10000200L

#define LPC_RAMSTART_LPC13XX    0x10000000L
#define LPC_RAMBASE_LPC13XX     0x10000300L

#define LPC_RAMSTART_LPC11XX    0x10000000L
#define LPC_RAMBASE_LPC11XX     0x10000300L

#define LPC_RAMSTART_LPC8XX     0x10000000L
#define LPC_RAMBASE_LPC8XX      0x10000270L

/* Return values used by NxpDownload(): reserving all values from 0x1000 to 0x1FFF */

#define NO_ANSWER_WDT       0x1000
#define NO_ANSWER_QM        0x1001
#define NO_ANSWER_SYNC      0x1002
#define NO_ANSWER_OSC       0x1003
#define NO_ANSWER_RBV       0x1004
#define NO_ANSWER_RPID      0x1005
#define ERROR_WRITE_DATA    0x1006
#define ERROR_WRITE_CRC     0x1007
#define ERROR_WRITE_CRC2    0x1008
#define PROGRAM_TOO_LARGE   0x1009

#define USER_ABORT_SYNC     0x100A   /* User aborted synchronisation process */

#define UNKNOWN_LPC         0x100B   /* Unknown LPC detected */

#define UNLOCK_ERROR        0x1100   /* return value is 0x1100 + NXP ISP returned value (0 to 255) */
#define WRONG_ANSWER_PREP   0x1200   /* return value is 0x1200 + NXP ISP returned value (0 to 255) */
#define WRONG_ANSWER_ERAS   0x1300   /* return value is 0x1300 + NXP ISP returned value (0 to 255) */
#define WRONG_ANSWER_WRIT   0x1400   /* return value is 0x1400 + NXP ISP returned value (0 to 255) */
#define WRONG_ANSWER_PREP2  0x1500   /* return value is 0x1500 + NXP ISP returned value (0 to 255) */
#define WRONG_ANSWER_COPY   0x1600   /* return value is 0x1600 + NXP ISP returned value (0 to 255) */
#define FAILED_RUN          0x1700   /* return value is 0x1700 + NXP ISP returned value (0 to 255) */
#define WRONG_ANSWER_BTBNK  0x1800   /* return value is 0x1800 + NXP ISP returned value (0 to 255) */

#if defined COMPILE_FOR_LPC21
#ifndef WIN32
#define LPC_BSL_PIN         13
#define LPC_RESET_PIN       47
#define LPC_RESET(in)       NAsetGPIOpin(LPC_RESET_PIN, (in))
#define LPC_BSL(in)         NAsetGPIOpin(LPC_BSL_PIN,   (in))
#endif // WIN32
#endif // COMPILE_FOR_LPC21


/* LPC_FLASHMASK
*
* LPC_FLASHMASK - bitmask to define the maximum size of the Filesize to download.
*                 LoadFile() will check any new segment address record (03) or extended linear
*                 address record (04) to see if the addressed 64 kByte data block still falls
*                 in the max. flash size.
*                 LoadFile() will not load any files that are larger than this size.
*/
#define LPC_FLASHMASK  0xFFC00000 /* 22 bits = 4 MB */

typedef enum CHIP_VARIANT
{
	CHIP_VARIANT_NONE,
		CHIP_VARIANT_LPC43XX,
		CHIP_VARIANT_LPC2XXX,
		CHIP_VARIANT_LPC18XX,
		CHIP_VARIANT_LPC17XX,
		CHIP_VARIANT_LPC13XX,
		CHIP_VARIANT_LPC11XX,
		CHIP_VARIANT_LPC8XX
} CHIP_VARIANT;

typedef struct LPC_DEVICE_TYPE
{
	unsigned long  id;
	unsigned long  id2;
	unsigned int   EvalId2;
	const char    *Product;
	unsigned int   FlashSize;     /* in kiB, for informational purposes only */
	unsigned int   RAMSize;       /* in kiB, for informational purposes only */
	unsigned int   FlashSectors;  /* total number of sectors */
	unsigned int   MaxCopySize;   /* maximum size that can be copied to Flash in a single command */
	const unsigned int  *SectorTable;   /* pointer to a sector table with constant the sector sizes */
	CHIP_VARIANT   ChipVariant;
} LPC_DEVICE_TYPE;

extern 
const LPC_DEVICE_TYPE LPCtypes[];

unsigned long ReturnValueLpcRamStartExt(const LPC_DEVICE_TYPE *pLPC);
unsigned long ReturnValueLpcRamBaseExt( const LPC_DEVICE_TYPE *pLPC );

const LPC_DEVICE_TYPE *FindID( unsigned long id );
const LPC_DEVICE_TYPE *FindIDExt( unsigned long id, unsigned long id2 );

unsigned long GetSectorStart(  const LPC_DEVICE_TYPE *pLPC, int sector );
unsigned long NxpGetSector( const LPC_DEVICE_TYPE *pLPC, unsigned long addr );

int NxpSetBinCRC( const LPC_DEVICE_TYPE *pLPC, unsigned char *buff  );

#endif 





