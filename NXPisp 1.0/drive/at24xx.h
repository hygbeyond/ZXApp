
#ifndef __AT24XX_h__
#define __AT24XX_h__


void   AT24_Init(void);
int    AT24_Read( int addr, uint8 *buff, int size );
int    AT24_Write( int addr, uint8 *buff, int size );

#define eeprom_init              AT24_Init
#define eeprom_write             AT24_Write
#define eeprom_read              AT24_Read

#endif //__AT24XX_h__

