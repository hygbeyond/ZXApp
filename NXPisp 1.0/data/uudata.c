
/*
 * 数据流采用UU 编码格式
 */

#include "mytype.h"
#include <string.h>
#include "uudata.h"


uint32 UU_crc = 0;


static inline
uint8 To_uuData( uint8 dat )
{
    dat &= 63;
    return dat == 0? 0x60: dat+0x20;
}

static inline
uint8 uuData_Get( uint8 dat )
{
    if( dat >= 0x60 )
        return 0;
    return dat - 0x20;
}

uint8 DataToUU( const char *pDat, char *pUU, int size )
{
    int i=0;
    uint32 k;
    char *pUUDat = pUU;
    size = MIN( size, 45 );
    *pUUDat++ = ' ' + size;
    while( i<size  )
    {
        UU_crc += pDat[i+0] + pDat[i+1] + pDat[i+2];
        k       = pDat[i+0]*0x10000 + pDat[i+1]*0x100 + pDat[i+2];
		*pUUDat++ = To_uuData(k >> 18);
		*pUUDat++ = To_uuData(k >> 12);
		*pUUDat++ = To_uuData(k >>  6);
		*pUUDat++ = To_uuData(k      );
        i+=3;
    }
    *pUUDat = 0;
    return size;
}

uint32 uuData_To( char *pDat )
{
    uint32 ret = 0;
    ret += uuData_Get( *pDat++ );
    ret <<= 6;
    ret += uuData_Get( *pDat++ );
    ret <<= 6;
    ret += uuData_Get( *pDat++ );
    ret <<= 6;
    ret += uuData_Get( *pDat   );
    return ret;
}


uint8 uuData_GetTo( char *pUU, uint8 *pDat )
{
	int len = strlen( pUU );
    int ret = 0;
    int cnt;
    uint32 tmp;
    if( len > 5 && len <= 61 )
    {
        cnt = uuData_Get( *pUU++ );
        if( cnt > 45 )
            return 0;
        while( ret < cnt )
        {
            tmp = uuData_To( pUU );
            pDat[ret+2] = tmp;
            pDat[ret+1] = tmp>>8;
            pDat[ret  ] = tmp>>16;
            ret += 3;
            pUU += 4;
        }
        return cnt;
    }
    return 0;
}


#include "pcommand.h"
#include "app_cfg.h"
#include "at24c256.h"

TUUFile  uuFile;
int uuInit(void)
{
	eeprom_init();
    eeprom_read( 0, (uint8*)&uuFile, sizeof(uuFile )) ;
    if( uuFile.crc == 0x55A5 )
    {
        return uuFile.filesize;
    }
    memset(&uuFile, 0, sizeof(uuFile));
    return 0;
}

int uuGetFlashBuff( uint32 pos, uint8 *buff, int size )
{
    pos += sizeof( TUUFile);
    return eeprom_read( pos, buff, size );
}

int uuPutFlashBuff( uint32 pos, uint8 *buff, int size )
{
    pos += sizeof( TUUFile);
    return eeprom_write( pos, buff, size );
}

int uuGetFlashLen( void )
{
	return uuInit();
}



#define Pstrcmp         strcmp
uint8  uu_buff[128];
uint16 uu_addr;
uint32 uu_crc;
int reset_cmd(void);
void uu_cmd(uint8 args, char **argv)
{
//    TUUFile file;
    int   cnt;
    int   i;
    uint32 crc;
    if( args > 2 )    
    {
        if( !Pstrcmp( argv[1], "b" ) )
        {
            uu_addr = 0;
            uu_crc = 0;
            cnt = uuData_GetTo( argv[2], uu_buff );
            if( cnt )
            {
                TUUFile *pFile = (TUUFile *)uu_buff;
                if( pFile->crc == 0x55A5 )
                {
                    uu_addr = eeprom_write( 0, uu_buff, sizeof( TUUFile ));
                }
                SH_PRINT("OK\n%d\n", uu_addr );
//                SH_PRINT("uuFile %04x %02x %02x %02x\n", pFile->crc, pFile->fulse[0], pFile->fulse[1], pFile->fulse[2] );
            }
        }else if( !Pstrcmp( argv[1], "c" ) )
        {
            crc = atol( argv[2] );
            if( crc == uu_crc )
            {
            	SH_PRINT("OK\n");
            	OSTimeDly(200);
            	reset_cmd();
            }else{
            	SH_PRINT("error\n");
            }
            uu_crc = 0;
        }
    }else if( args == 2 )
    {
        if( !Pstrcmp( argv[1], "get" ))
        {
            eeprom_read( 0, (uint8*)&uuFile, sizeof(uuFile )) ;
            SH_PRINT("file crc  %04x\n", uuFile.crc );
            SH_PRINT("file id   %02x %02x %02x\n", uuFile.ID[0], uuFile.ID[1], uuFile.ID[2]);
            SH_PRINT("file fuls %02x %02x %02x\n", uuFile.fulse[0], uuFile.fulse[1], uuFile.fulse[2]);
            SH_PRINT("file size %d\n", uuFile.filesize );
            if( uuFile.crc == 0x55A5 )
            {
            	SH_PRINT("file name %s\n", uuFile.name );
            }
            return;
        }else
        {
            cnt = uuData_GetTo( argv[1], uu_buff );
            if( cnt )
            {
//            	SH_PRINT("write ");
                uu_addr += eeprom_write( uu_addr, uu_buff, cnt );
            }
            SH_PRINT("OK\n%d\n", cnt );
            for(i=0; i<cnt; i++)
            {
                uu_crc += uu_buff[i];
                //printk("%02x ", uu_buff[i] );
            }
            //printk("\n");
        }
    }
}
PSHELL_CMD( uu, uu_cmd, "uu [b|c|get|data]" );


