//eeprom.c

#define I2C_SOURCE

#include "mytype.h"
#include "I2C.h"
#include "MicroOS.h"
#include "bsp.h"
#include "wdt.h"
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
#define I2C0CONSET	I20CONSET```
#define I2C0STAT	I20STAT
#define I2C0DAT		I20DAT
#endif

//EEPROM 相关,at24c04   i2c总线操作
void i2c_init(uint32 i2c_clk)
{
    LPC_SYSCON->PRESETCTRL    |= (0x1<<1);
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);

    LPC_IOCON->PIO0_4 = 0x01;      /* I2C SCL */
    LPC_IOCON->PIO0_5 = 0x01;	   /* I2C SDA */

    /*--- Clear flags ---*/
    LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

#if 0
    LPC_I2C->SCLL   = I2SCLL_SCLL;
    LPC_I2C->SCLH   = I2SCLH_SCLH;
#else
    uint32 clk;
    uint32 tempH,tempL;
    clk = FPCLK;

    clk = clk/i2c_clk;

    tempH = clk/2;
    tempL = clk-tempH;
    LPC_I2C->SCLH = tempH;
    LPC_I2C->SCLL = tempL;
#endif

    LPC_I2C->CONSET = I2CONSET_I2EN;

}

//向总线发开始条件
uint8 i2c_start(void)
{
    LPC_I2C->CONCLR  = 0x2c;
    LPC_I2C->CONSET  = 0x60;
    while((LPC_I2C->CONSET &0x08)==0);      //i2c中断
    return LPC_I2C->STAT;
}

//向总线发一字节
uint8 i2c_writebyte(uint8 dat)
{
    LPC_I2C->CONCLR = 0x20;//清STA 
    LPC_I2C->DAT  = dat;

    LPC_I2C->CONCLR = 0x08;//清SI	

    while((LPC_I2C->CONSET &0x08)==0) ;
    return LPC_I2C->STAT;
} 

uint8 i2c_readbyte(uint8 *c ,uint8 ack)
{		
    if(ack)
        LPC_I2C->CONSET = 0x04; //置AA
    else
        LPC_I2C->CONCLR = 0x24; //清AA

    LPC_I2C->CONCLR  = 0x08;		//清SI	
    while((LPC_I2C->CONSET &0x08)==0)  ;
    LPC_I2C->CONCLR  = 0x24;

	*c = LPC_I2C->DAT;

	return(LPC_I2C->STAT);
}

uint8 i2c_stop(void)
{
    LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */ 
    LPC_I2C->CONCLR = I2CONCLR_SIC;      /* Clear SI flag */ 

    /*--- Wait for STOP detected ---*/
    while( LPC_I2C->CONSET & I2CONSET_STO );
    return TRUE;
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


