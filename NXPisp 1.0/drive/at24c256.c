
#include "mytype.h"
#include "I2C.h"
#include "app_cfg.h"
#include "at24c256.h"

//#define  EE_DEBUG

#define  MAX_ADDRESS 0x8000
#define  BUFF_SIZE   64
#define  AT24_ID     0xA0
#include "MicroOS.h"



void AT24C256_Init(void)
{
    i2c_init( I2C_200K );
}

/*
 * 地址的边界最小为16位，64位是一个区间
 */
#define  __size( addr, cnt )     (((addr+cnt) & (~(cnt-1)))-(addr))
static int AT24C256_writepage( int addr, const uint8 *buff, int size )
{
    static uint8 pd[BUFF_SIZE+2];
    int   ret = __size(addr, BUFF_SIZE);
    ret = MIN( ret, size );

    pd[0] = addr/256;
    pd[1] = addr%256;

    for(int i=0; i<ret; i++)
        pd[i+2] = *buff++;

    if( i2c_Write( AT24_ID, pd, ret+2 ))
        return ret;
    return 0;
}

static int AT24C256_readpage( int addr, uint8 *buff, int Nbyte)
{
    uint8 a[2];
    int ret = __size(addr, 128);
    Nbyte   = MIN( Nbyte, ret );
    a[0] = addr/256;
    a[1] = addr%256;

    if( i2c_Write( AT24_ID, a, 2 ) )
    {
        if( i2c_Read( AT24_ID, buff, Nbyte))
        {
            return Nbyte;
        }
    }
    return 0;
}



int AT24C256_Write( int addr, const uint8 *buff, int size )
{
    int ret = 0;
    int time = 3;
    while( size )
    {
        int tmp = AT24C256_writepage( addr, buff, size );
        if( tmp )
        {
//        	dbg_print("ee w %d\t%d\n", addr, tmp );
            size -= tmp;
            buff += tmp;
            addr += tmp;
            ret  += tmp;
            time = 3;
        }
        if( time == 0 )
            break;
#if HZ >= 200
        OSTimeDly(  HZ/200 +1 );
#else
        OSTimeDly(1);
#endif
        time--;
    }
    if( size == 0 ){ 
//    	dbg_print("w %d\n", ret );
    }
    return ret;
}



int AT24C256_Read( int addr, uint8 *buff, int size )
{
    int ret = 0;
    int time = 3;
    while( size )
    {
        int tmp = AT24C256_readpage( addr, buff, size );
        if( tmp )
        {
            size -= tmp;
            buff += tmp;
            addr += tmp;
            ret  += tmp;
            time = 3;
            continue;
        }
        if( time == 0 )
            break;
        //OSTimeDly(1);
        time--;
    }
    return ret;
}



#if defined( EE_DEBUG )
#include "pcommand.h"


uint8 eebuff[128];

static void ee_read(int addr, int size)
{
    size = MIN( size, sizeof( eebuff ));
    if( AT24C256_Read( addr, eebuff, size) )
    {
        for( int i=0; i<size; i++)
        {
        	SH_PRINT("%02x ", eebuff[i] );
        }
        SH_PRINT("read 0x%04x\n", addr);
    }
}

static void ee_write(int addr, int size )
{
    size = MIN( size, sizeof( eebuff ));
    for(int i=0; i<size; i++)
    {
        eebuff[i] = i;
    }
    if( AT24C256_Write( addr, eebuff, size) )
    {
    	SH_PRINT("write 0x%04x\n", addr);
    }
}

static int ee_cmd(int args, char **argv )
{
    int addr = atoi( argv[2] );
    if( !strcmp( argv[1], "i" ) )
    {
        AT24C256_Init();
    }else if( !strcmp( argv[1], "r"))
    {
        ee_read( addr, sizeof( eebuff ));
    }else if( !strcmp( argv[1], "w" ))
    {
        ee_write( addr, sizeof( eebuff));
    }else if( !strcmp( argv[1], "t" ))
    {
		addr=0;
    	while(addr<32768)
    	{
            ee_write( addr, sizeof( eebuff));
    		addr+=128;
    	}
    }
    return 0;
}
PSHELL_CMD( ee, ee_cmd, "ee [i|r|w] address" );
#endif

