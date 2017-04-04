
#ifndef __AT24C256_H__
#define __AT24C256_H__


void AT24C256_Init(void);
int AT24C256_Write( int addr, const uint8 *buff, int size );
int AT24C256_Read( int addr, uint8 *buff, int size );


#define eeprom_init              AT24C256_Init
#define eeprom_write             AT24C256_Write
#define eeprom_read              AT24C256_Read

#endif

