
#ifndef __UUDATA_H__
#define __UUDATA_H__

#include "mytype.h"


typedef struct TUUfile
{
    uint16 crc;         /*0x55A5*/
    uint8  ID[3];
    uint8  fulse[3];
    uint32 filesize;
    uint8  name[16];
} __PACKED__ TUUFile;

extern TUUFile  uuFile;
extern uint32 UU_crc;

int uuInit(void);
uint8 DataToUU( const char *pDat, char *pUU, int size );
int uuGetFlashBuff( uint32 pos, uint8 *buff, int size );
int uuPutFlashBuff( uint32 pos, uint8 *buff, int size );
int uuGetFlashLen( void );

#endif //__UUDATA_H__


