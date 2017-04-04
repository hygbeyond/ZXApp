//eeprom.h

#ifndef __I2C_H__
#define __I2C_H__

/* TWSR values (not bits) */
/* Master */
#define TW_START                0x08
#define TW_REP_START            0x10
/* Master Transmitter */
#define TW_MT_SLA_ACK           0x18
#define TW_MT_SLA_NACK          0x20
#define TW_MT_DATA_ACK          0x28
#define TW_MT_DATA_NACK         0x30
#define TW_MT_ARB_LOST          0x38
/* Master Receiver */
#define TW_MR_ARB_LOST          0x38
#define TW_MR_SLA_ACK           0x40
#define TW_MR_SLA_NACK          0x48
#define TW_MR_DATA_ACK          0x50
#define TW_MR_DATA_NACK         0x58
/* Slave Transmitter */
#define TW_ST_SLA_ACK           0xA8
#define TW_ST_ARB_LOST_SLA_ACK  0xB0
#define TW_ST_DATA_ACK          0xB8
#define TW_ST_DATA_NACK         0xC0
#define TW_ST_LAST_DATA         0xC8
/* Slave Receiver */
#define TW_SR_SLA_ACK           0x60
#define TW_SR_ARB_LOST_SLA_ACK  0x68
#define TW_SR_GCALL_ACK         0x70
#define TW_SR_ARB_LOST_GCALL_ACK 0x78
#define TW_SR_DATA_ACK          0x80
#define TW_SR_DATA_NACK         0x88
#define TW_SR_GCALL_DATA_ACK    0x90
#define TW_SR_GCALL_DATA_NACK   0x98
#define TW_SR_STOP              0xA0
/* Misc */
#define TW_NO_INFO              0xF8
#define TW_BUS_ERROR            0x00

#define TWI_ACK                 0X01
#define TWI_NACK                0X00


#define I2C_20K			 20000UL
#define I2C_50K			 50000UL
#define I2C_100K		100000UL
#define I2C_200K		200000UL
#define I2C_400K		400000UL

#ifdef I2C_SOURCE
	#define EEPROM_EXT
#else
	#define EEPROM_EXT extern
#endif

void  i2c_init     (uint32 i2c_clk);
uint8 i2c_Write    (uint8 address, uint8 *str, int Nbyte);
uint8 i2c_Read     (uint8 address, uint8 *str, int Nbyte);

uint8 i2c_start    (void);
uint8 i2c_writebyte(uint8 dat);
uint8 i2c_readbyte (uint8 *c ,uint8 ack);
uint8 i2c_stop     (void);




static inline uint8 i2c_WriteExt(uint8 address, uint8 ad_rst, uint8 *str,uint8 Nbyte)
{
	address = address & 0xfe;
	if(i2c_start() == TW_START)
	{
		if(i2c_writebyte(address) == TW_MT_SLA_ACK)
        {
            if(i2c_writebyte(ad_rst) != TW_MT_DATA_ACK)             
                goto TWI_Write_Err;

            do{
                if(i2c_writebyte(*str++) != TW_MT_DATA_ACK)
                    goto TWI_Write_Err;
            }while(--Nbyte!=0);

            i2c_stop();
            return(TRUE);
        }
		goto TWI_Write_Err;
	}
	return(FALSE);
TWI_Write_Err:

	i2c_stop();
	return(FALSE);
}

static inline uint8 i2c_ReadExt(uint8 address, uint8 ad_rst, uint8 *str,uint8 Nbyte)
{
	if(i2c_start() == TW_START)
	{
		if(i2c_writebyte(address&0xfe) != TW_MT_SLA_ACK)        
			goto TWI_Read_Err;
		if(i2c_writebyte(ad_rst) != TW_MT_DATA_ACK)             
			goto TWI_Read_Err;
		if(i2c_start() == TW_REP_START)
		{
			if(i2c_writebyte(address|0x01) != TW_MR_SLA_ACK)
				goto TWI_Read_Err;
			Nbyte--;
			if(Nbyte != 0)
			{
				do{
					if(i2c_readbyte(str++,TWI_ACK) != TW_MR_DATA_ACK)
						goto TWI_Read_Err;
				}while(--Nbyte != 0);
			}
			if(i2c_readbyte(str,TWI_NACK) != TW_MR_DATA_NACK)
				goto TWI_Read_Err;
			i2c_stop();
			return(TRUE);
		}
	}
	return(FALSE);
TWI_Read_Err:

	i2c_stop();
	return(FALSE);
}

static inline uint8 i2c_WriteExtWord(uint8 address, uint16 ad_rst, uint8 *str,uint8 Nbyte)
{
	address = address & 0xfe;
	if(i2c_start() == TW_START)
	{
		if(i2c_writebyte(address) == TW_MT_SLA_ACK)
        {
            if(i2c_writebyte(ad_rst/256) != TW_MT_DATA_ACK)             
                goto TWI_Write_Err;

            if(i2c_writebyte(ad_rst%256) != TW_MT_DATA_ACK)             
                goto TWI_Write_Err;

            do{
                if(i2c_writebyte(*str++) != TW_MT_DATA_ACK)
                    goto TWI_Write_Err;
            }while(--Nbyte!=0);

            i2c_stop();
            return(TRUE);
        }
		goto TWI_Write_Err;
	}
	return(FALSE);
TWI_Write_Err:

	i2c_stop();
	return(FALSE);
}

static inline uint8 i2c_ReadExtWord(uint8 address, uint16 ad_rst, uint8 *str,uint8 Nbyte)
{
	if(i2c_start() == TW_START)
	{
		if(i2c_writebyte(address&0xfe) != TW_MT_SLA_ACK)        
			goto TWI_Read_Err;

        if(i2c_writebyte(ad_rst/256) != TW_MT_DATA_ACK)             
            goto TWI_Read_Err;

        if(i2c_writebyte(ad_rst%256) != TW_MT_DATA_ACK)             
            goto TWI_Read_Err;

        if(i2c_start() == TW_REP_START)
		{
			if(i2c_writebyte(address|0x01) != TW_MR_SLA_ACK)
				goto TWI_Read_Err;
			Nbyte--;
			if(Nbyte != 0)
			{
				do{
					if(i2c_readbyte(str++,TWI_ACK) != TW_MR_DATA_ACK)
						goto TWI_Read_Err;
				}while(--Nbyte != 0);
			}
			if(i2c_readbyte(str,TWI_NACK) != TW_MR_DATA_NACK)
				goto TWI_Read_Err;
			i2c_stop();
			return(TRUE);
		}
	}
	return(FALSE);
TWI_Read_Err:

	i2c_stop();
	return(FALSE);
}



#undef EEPROM_EXT
#endif
