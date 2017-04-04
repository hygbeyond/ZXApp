//eeprom.c

#define I2C_SOURCE

#include "mytype.h"
#include "nuc_i2c.h"
#include "bsp.h"
#include "app_cfg.h"

#define I2C_IDLE			0
#define I2C_STARTED			1
#define I2C_RESTARTED		2
#define I2C_REPEATED_START	3
#define DATA_ACK			4
#define DATA_NACK			5
#define I2C_BUSY              6
#define I2C_NO_DATA           7
#define I2C_NACK_ON_ADDRESS   8
#define I2C_NACK_ON_DATA      9
#define I2C_ARBITRATION_LOST  10
#define I2C_TIME_OUT          11
#define I2C_OK                12

#define I2CONSET_I2EN       (0x1<<6)  /* I2C Control Set Register */
#define I2CONSET_AA         (0x1<<2)
#define I2CONSET_SI         (0x1<<3)
#define I2CONSET_STO        (0x1<<4)
#define I2CONSET_STA        (0x1<<5)

#define I2CONCLR_AAC        (0x1<<2)  /* I2C Control clear Register */
#define I2CONCLR_SIC        (0x1<<3)
#define I2CONCLR_STAC       (0x1<<5)
#define I2CONCLR_I2ENC      (0x1<<6)

#define I2DAT_I2C			0x00000000  /* I2C Data Reg */
#define I2ADR_I2C			0x00000000  /* I2C Slave Address Reg */
#define I2SCLH_SCLH			0x00000180  /* I2C SCL Duty Cycle High Reg */
#define I2SCLL_SCLL			0x00000180  /* I2C SCL Duty Cycle Low Reg */
#define I2SCLH_HS_SCLH		0x00000015  /* Fast Plus I2C SCL Duty Cycle High Reg */
#define I2SCLL_HS_SCLL		0x00000015  /* Fast Plus I2C SCL Duty Cycle Low Reg */


#ifndef I2C0CONCLR
#define I2C0SCLH	I20SCLH
#define I2C0SCLL	I20SCLL
#define I2C0CONCLR	I20CONCLR
#define I2C0CONSET	I20CONSET
#define I2C0STAT	I20STAT
#define I2C0DAT		I20DAT
#endif

//EEPROM 相关,at24c04   i2c总线操作
void i2c_init(uint32 i2c_clk)
{
    SYS_UnlockReg();
    CLK->APBCLK |= ( CLK_APBCLK_I2C0_EN_Msk );

    /* Enable I2C0 Controller */
    I2C0->I2CON |= I2C_I2CON_ENS1_Msk;

    /* I2C0 bus clock 100K divider setting, I2CLK = PCLK/(100K*4)-1 */
    I2C0->I2CLK = (uint32_t)(((SystemCoreClock * 10) / (i2c_clk * 4) + 5) / 10 - 1); /* Compute proper divider for I2C clock */

    /* Get I2C0 Bus Clock */
    pr_debug("I2C clock %ld Hz\n", (SystemCoreClock / (((I2C0->I2CLK) + 1) << 2)));
    SYS_LockReg();
}

//向总线发开始条件
uint8 i2c_start(void)
{
    I2C_START( I2C0 );
    while((I2C0->I2CON &0x08)==0) ;
    I2C0->I2CON = 0x08;//清SI
    return I2C_GET_STATUS( I2C0 );
}

//向总线发一字节
uint8 i2c_writebyte(uint8 dat)
{
    I2C_SET_DATA( I2C0, dat );
    while((I2C0->I2CON &0x08)==0) ;
    I2C0->I2CON = 0x08;//清SI
    return I2C_GET_STATUS( I2C0 );
} 

uint8 i2c_readbyte(uint8 *c ,uint8 ack)
{		
    if(ack)
    {
        I2C0->I2CON |= 4;  //置AA
    }
    
    while((I2C0->I2CON &0x08)==0) ;
    I2C0->I2CON = 0x08;//清SI

	*c = I2C_GET_DATA( I2C0 );
    return I2C_GET_STATUS( I2C0 );
}

uint8 i2c_stop(void)
{
    I2C_STOP( I2C0 );
    while((I2C0->I2CON &0x08)==0) ;
    I2C0->I2CON = 0x08;//清SI
    return I2C_GET_STATUS( I2C0 );
}



uint8 i2c_Write(uint8 address, uint8 *str,int Nbyte)
{
	address = address & 0xfe;
	if(i2c_start() == TW_START)
	{
		if(i2c_writebyte(address) == TW_MT_SLA_ACK)
		{
			do{
				if(i2c_writebyte(*str++) != TW_MT_DATA_ACK)
					goto TWI_Write_Err;
			}while(--Nbyte!=0);

			i2c_stop();
			return(TRUE);
		}
	}
TWI_Write_Err:
	i2c_stop();
	return(FALSE);
}

uint8 i2c_Read(uint8 address, uint8 *str,int Nbyte)
{
	if(i2c_start() == TW_START)
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
TWI_Read_Err:
	i2c_stop();
	return(FALSE);
}


