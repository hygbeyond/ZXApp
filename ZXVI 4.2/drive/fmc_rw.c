


#include "hardware.h"
#include "fmc_rw.h"
#include "MicroOS.h"


typedef union{
    uint32 dat;
    uint8  buff[4];
} __PACKED__ FVoid;

//typedef __PACKED__  uint32 FUint32;


__STATIC_INLINE
void fmc_open(void){
    /* Unlock protected registers */
    SYS_UnlockReg();
    FMC->ISPCON |=  FMC_ISPCON_ISPEN_Msk | FMC_ISPCON_APUEN_Msk;
}

__STATIC_INLINE
void fmc_close(void){
    FMC->ISPCON &= ~(FMC_ISPCON_ISPEN_Msk | FMC_ISPCON_APUEN_Msk);   
    SYS_LockReg();
}

__STATIC_INLINE
int32_t fmc_WriteConfig(uint32_t *u32Config, uint32_t u32Count)
{
    int32_t i;

    for(i = 0; i < u32Count; i++)
    {
        FMC_Write(FMC_CONFIG_BASE + i * 4, u32Config[i]);
        if(FMC_Read(FMC_CONFIG_BASE + i * 4) != u32Config[i])
            return -1;
    }

    return 0;
}

__STATIC_INLINE 
void fmc_CfgDataFlash( void )
{
    OS_SREG_VAR;
    OS_ENTER_CRITICAL();
    fmc_open();
    //uint32 uData = FMC_Read(FMC_CONFIG_BASE + 4);
    uint32 uData = FMC->DFBADR;

    if( uData != (uint32) __DataFlash )
    {
        FMC->ISPCON |= FMC_ISPCON_CFGUEN_Msk;

        uData = FMC_Read(FMC_CONFIG_BASE);

        uData &=    ~(BIT(0)|BIT(2));
        FMC_Erase( FMC_CONFIG_BASE );


        FMC_Write( FMC_CONFIG_BASE, uData );
        //uData = (uint32) __DataFlash;
        FMC_Write( FMC_CONFIG_BASE+4, (uint32) __DataFlash );


        pr_debug("\nSet Data Flash base as 0x%x.\n",  (int)FMC->DFBADR);

        /* Perform chip reset to make new User Config take effect */
        SYS->IPRSTC1 |= SYS_IPRSTC1_CHIP_RST_Msk;

        CBI( FMC->ISPCON, FMC_ISPCON_CFGUEN_Pos );

    }
    pr_debug("DataFlash Cfg %08x\n", uData );

    fmc_close();        
    OS_EXIT_CRITICAL();
}

__STATIC_INLINE
int fmc_WriteData( uint32 Addr, uint8 *buff, int len )
{
    int i;
    for( i=0; i<len; )
    {
        uint32 tmp = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);
        FMC_Write( Addr + i , tmp );
        buff += 4;
        i    += 4;
    }
    return i;
}


void fmc_Init(void)
{
    fmc_CfgDataFlash();
}


int fmc_Write( uint8 *ToFlash, uint8 *From, int size )
{
    uint32 u32Addr = (uint32)ToFlash;
    int i=0;
    int cnt;        
    OS_SREG_VAR;
    FVoid *pd = (FVoid *)From;

    pr_debug("fmc erase %08x\n", u32Addr);
    OS_ENTER_CRITICAL();
    fmc_open();

    while( size )
    {
        cnt = MIN( size, FMC_FLASH_PAGE_SIZE );
        FMC_Erase( u32Addr);

        for(i=0; i<cnt; i +=4 )
        {
            FMC_Write( u32Addr + i, pd->dat );
//            pr_debug("FMC Write %08x: %08x\n", u32Addr + i, pd->dat  );
            pd++;
        }
        
        size    -= cnt;
        u32Addr += cnt;
    }
    //fmc_WriteData( u32Addr, From, i );
    fmc_close(); 
    OS_EXIT_CRITICAL();
    return i;
}

#if 0
__STATIC_INLINE int uint32Touint8( uint32 data, uint8 *buff, int size )
{
    size = MIN( 4, size );
    switch( size )
    {
        case 4:
            buff[3] = data >> 24;
        case 3:
            buff[2] = data >> 16;
        case 2:
            buff[1] = data >> 8;
        case 1:
            buff[0] = data;
            break;
    }
    return size;
}


int fmc_Read( uint8 *FromFlash, uint8 *To, int size )
{
    uint32 Addr = (uint32) FromFlash;
    uint32 tmp;
    int ret = 0;
    int cnt;
    FVoid *pd = (FVoid *)To;
    /* Unlock protected registers */
    SYS_UnlockReg();
    fmc_open();
    while( size > 0 )
    {
        pd->dat = FMC_Read( Addr );
        pr_debug("FMC Write %08x: %08x\n", Addr, pd->dat  );
        pd++;
        Addr += 4;
        size -= 4;
        ret  += 4;
        /*
        tmp = FMC_Read( Addr );
        cnt = uint32Touint8( tmp, To, size );
        size -= cnt;
        ret  += cnt;
        To   += cnt;
        //*/
    }
    fmc_close();
    
    /* Lock protected registers */
    SYS_LockReg();

    return ret;
}
#else
int fmc_Read( uint8 *FromFlash, uint8 *To, int size )
{
    for( int i=0; i<size; i++)
    {
        To[i] = FromFlash[i];
    }
    return size;
}
#endif


//#define EE_DEBUG

#if defined( EE_DEBUG )
#include "pcommand.h"


uint8 fmcbuff[128];

static void fmc_read(int addr, int size)
{
    size = MIN( size, sizeof( fmcbuff ));
    if( fmc_Read( &__DataFlash[addr], fmcbuff, size) )
    {
        for( int i=0; i<size; i++)
        {
        	TRACE("%02x ", fmcbuff[i] );
        }
        TRACE("read 0x%04x\n", addr);
    }
}

static void fmc_write(int addr, int size )
{
    size = MIN( size, sizeof( fmcbuff ));
    for(int i=0; i<size; i++)
    {
        fmcbuff[i] = i;
    }
    if( fmc_Write( &__DataFlash[addr], fmcbuff, size) )
    {
    	TRACE("write 0x%04x\n", addr);
    }
}

static int fmc_cmd(int args, char **argv )
{
    int addr = atoi( argv[2] );
    if( !strcmp( argv[1], "i" ) )
    {
        fmc_Init();
    }else if( !strcmp( argv[1], "r"))
    {
        fmc_read( addr, sizeof( fmcbuff ));
    }else if( !strcmp( argv[1], "w" ))
    {
        fmc_write( addr, sizeof( fmcbuff));
    }
    return 0;
}
PSHELL_CMD( fmc, fmc_cmd, "fmc [i|r|w] address" );
#endif

