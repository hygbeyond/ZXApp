


#ifndef __FMC_RW_H__
#define __FMC_RW_H__

#include "bsp.h"

extern uint8 __DataFlash[0x1000];
extern uint8 __DataFlash_end[];


void fmc_Init(void);
int fmc_Write( uint8 *ToFlash, uint8 *From, int size );
int fmc_Read( uint8 *FromFlash, uint8 *To, int size );

__STATIC_INLINE
int fmc_WriteBuff( int addr, uint8 *buff, int size )
{
    return fmc_Read( &__DataFlash[addr], buff, size );
}

__STATIC_INLINE
int fmc_ReadBuff( int addr, uint8 *buff, int size )
{
    return fmc_Write( &__DataFlash[addr], buff, size );
}



#endif




