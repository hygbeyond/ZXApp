
#include "mytype.h"
#include "I2C.h"
#include "app_cfg.h"
#include "at24xx.h"

#define  AT24_ID     0xA0
#include "MicroOS.h"

//#define  EE_DEBUG

static uint16 AT_Addr=0;

void   AT24_Init(void)
{
    uint8 i;
    uint8 buff[2];
    uint8 id = AT24_ID;
    AT_Addr = 0;
    i2c_init( I2C_200K );
    for(i=0; i<8; i++)
    {
        if( !i2c_ReadExt( id, 0, buff, 1)) 
            break;
        id      += 0x02;
        AT_Addr += 256;
    }
    pr_debug("at %d %d\n", i, AT_Addr);
}

#define  __size( addr, cnt )     (((addr+cnt) & (~(cnt-1)))-(addr))

static int at24_writepage( int addr, uint8 *buff, int size )
{
    uint8 id  = (addr >> 7) | AT24_ID;
    int   ret = __size(addr, 16);
    ret = MIN( ret, size );
    
    if( addr + ret > AT_Addr )
        return 0;

    if( i2c_WriteExt( id, addr & 0xFF, buff, ret ))
    {
        return ret;
    }
    return 0;
}


static int AT24_readpage( int addr, uint8 *buff, int size )
{
    int   ret = size>256? 256: size;
    uint8 id  = (addr >> 7) | AT24_ID;
    if( (ret + (addr&0xFF) ) & 0x100)
        ret = 0x100-(addr&0xFF);
    if( addr + ret > AT_Addr )
        return 0;

    if( i2c_ReadExt( id, addr&0xFF, buff, ret) )
    {
        return ret;
    }
    return 0;
}

int AT24_Read( int addr, uint8 *buff, int size )
{
    int ret = 0;
    int time = 3;
    while( size )
    {
        int tmp = AT24_readpage( addr, buff, size );
        if( tmp )
        {
            ret  += tmp;
            size -= tmp;
            buff += tmp;
            addr += tmp;
            time = 3;
        }
        if( time == 0 )
            break;
        //OSTimeDly(1);
        time--;
    }
    return ret;
}

int AT24_Write( int addr, uint8 *buff, int size )
{
    int ret = 0;
    int time = 3;
    while( size )
    {
        int tmp = at24_writepage( addr, buff, size );
        if( tmp )
        {
            //pr_debug("ee w %d\t%d\n", addr, tmp );
            size -= tmp;
            buff += tmp;
            addr += tmp;
            ret  += tmp;
            time = 3;
        }
        if( time == 0 )
            break;
        OSTimeDly( HZ/20 + 1);
        time--;
    }
    return ret;
}


#if defined( EE_DEBUG )
#include "pcommand.h"
static int UserCmd(int args, char **argv)
{
    uint8 buff[16];
    int addr = atoi( argv[2] );
    int cnt  = atoi( argv[3] );
    int dat  = atoi( argv[4] );
    if(args > 1)
    {
        if( !strcmp( argv[1], "r" ) )
        {
            eeprom_read( addr, buff, sizeof(buff));
            for(int i=0; i<sizeof(buff); i++)
            {
                printk("%02x ", buff[i] );
            }
            printk("\n");
        }else if( !strcmp( argv[1], "w") )
        {
            dat = MIN( dat, sizeof(buff));
            for(int i=0; i<dat; i++)
            {
                buff[i] = cnt;
            }
            eeprom_write( addr, buff, dat );
        }
    }
    return 0;
}
PSHELL_CMD(at, UserCmd, "at [r|w] addr cnt data");
#endif



