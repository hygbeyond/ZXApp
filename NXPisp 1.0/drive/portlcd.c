/*************************************
  文件名：lcd.c
  功能：LCD内存读写（T6963 及兼容芯片)

 *************************************/

#define LCD_SOURCE

#include "mytype.h"
#include "portlcd.h"
#include "FONT_MACRO.H"

/* 显示点与内存地址关系：
 * 功能：图形液晶COG12832驱动。
 *      在前。
 * 说明：图形液晶采用NT7502为LCD控制芯片，串行接口(使用M32驱动)。
 *	串口脚位排列
 *	1 = VCC			3.3V	Power supply for lcm
 *	2 = GND			0V		Ground
 *	3 = A0			H/L		register select input pin(Data/Instruction)
 *	4 = SCK			H/L		serial input clock
 *	5 = SID			H/L		serial input data
 *	6 = CS1B		L		chip select
 *	7 = RES1B		L		reset input pin
 *	8 = LED+		3.3V
 * 硬件连接：
 *           CS1     <===>  PB1
 *           RES     <===>  PB4
 *           A0      <===>  PD7
 *           SCK     <===>  PD6
 *           SDI     <===>  PD5
 */


/*
   12832 COG 驱动芯片SN7502/SN7532
   */

#define LCD_INIT


#define LCD_RES     4
#define LCD_A0		5
#define LCD_DA		9
#define LCD_SCK		10
#define LCD_PORT    0

#define LCD_CS_H	//SBI_P(LCD_PORT,LCD_CS)
#define LCD_CS_L	//CBI_P(LCD_PORT,LCD_CS)
//#define LCD_RES_H	SBI_P(LCD_PORT,LCD_RES)
//#define LCD_RES_L	CBI_P(LCD_PORT,LCD_RES)


#define LCD_A0_H	SBI_P(1,10)
#define LCD_A0_L	CBI_P(1,10)
#define LCD_SCK_H	SBI_P(LCD_PORT,LCD_SCK)
#define LCD_SCK_L	CBI_P(LCD_PORT,LCD_SCK)
#define LCD_DA_H	SBI_P(LCD_PORT,LCD_DA)
#define LCD_DA_L	CBI_P(LCD_PORT,LCD_DA)

#define LCD_DELAY				NOP()
#define LCD_DELAY_COUNT			100

#define LCD_A0SPLAY_ON			0xAF
#define LCD_A0SPLAY_OFF			0xAE

#define LCD_SET_START			0x40

#define LCD_SET_PAGE_ADDR		  0xB0 
#define LCD_SET_COLUMN_ADDRH	0x10
#define LCD_SET_COLUMN_ADDRL	0x00


#define LCD_END					0xEE
#define LCD_RESET				0xE2

static uint8 lcdBuff[LCD_XMAX * ( (LCD_YMAX+7)/8 )];
static uint8 lcd_x;	
//static inline void Delay(uint32 time)
//{
//    while(time--)
//    {
//        ;
//    }
//}

static void WriteByte(uint8 dat)
{
    uint8 i;

    LCD_SCK_H;
    for(i=0; i<8; i++)
    {
        if(dat & 0x80)
        {
            LCD_DA_H;
        }else
        {
            LCD_DA_L;
        }
        LCD_SCK_L;
        LCD_DELAY;
        dat <<= 1;
        LCD_SCK_H;
        LCD_DELAY;
    }
}

static void WriteCMD(uint8 cmd)
{
    LCD_CS_L;
    LCD_A0_L;	
    WriteByte(cmd);	
    LCD_CS_H;
}

static void WriteData(uint8 dat)
{
    LCD_CS_L;
    LCD_A0_H;
    WriteByte(dat);	
    LCD_CS_H;
}

#define X_MOVE     2

int mLCD_RefreshLine( int x, int y, int width)
{
    if( x < 0 ) x=0;
    int x2 = MIN( x+width, LCD_XMAX);
    y /= 8;

    int tmp = x+X_MOVE;
    WriteCMD( LCD_SET_COLUMN_ADDRH + ((tmp>>4)&0x0F) );
    WriteCMD( LCD_SET_COLUMN_ADDRL + (tmp & 0x0F) );
    WriteCMD( LCD_SET_PAGE_ADDR + y );

    uint8 *pd = &lcdBuff[ y*LCD_XMAX + x ];
    while( x<x2 )
    {
        WriteData(*pd++);
        x++;
    }
    return y * 8;
}

void mLCD_RefreshRect( int x1, int y1, int width, int height )
{
    int y2 = MIN( y1+height, LCD_YMAX-1);
    for( int y=y1; y<=y2; )
    {
        y  = mLCD_RefreshLine(x1, y, width );
        y += 8;
    }
}

const uint8 DCB2HEX[]=
{
	X_______,
	_X______,
	__X_____,
	___X____,
	____X___,
	_____X__,
	______X_,
	_______X,
};

const uint8 LCD_HEX[]={
    1,2,4,8,0x10,0x20,0x40,0x80
};

static __inline int _bufPos( int x, int y )
{
	int addr;
    if(x<0 || x>=LCD_XMAX || y<0 || y>=LCD_YMAX) return -1;

    addr = x;
    y   /= 8;
    addr += LCD_XMAX * y;
	return addr;
}

int mLCD_GetPixel( int x, int y )
{
    int address = _bufPos(x,y);
	if( address < 0 ) return 0;
    return lcdBuff[address] &  LCD_HEX[y & 0x07]?  1: 0;
}

int mLCD_SetPixel( int x, int y, int color )
{
    int address = _bufPos(x,y);
	if( address < 0 ) return 0;
    if(color) {
        lcdBuff[address] |=  LCD_HEX[y & 0x07];
    }else {
        lcdBuff[address] &= ~LCD_HEX[y & 0x07];
    }
    return TRUE;
}

int mLCD_Pixel( int x, int y, int color )
{
	if( !mLCD_SetPixel( x, y, color ) )
		return FALSE;
    return mLCD_RefreshLine( x, y, 1 );
}

//清屏
void LCD_Clear(uint8 dat)
{
    for(int i=0; i<sizeof(lcdBuff); i++)
        lcdBuff[i] = dat;

    mLCD_RefreshRect(0, 0, LCD_XMAX, LCD_YMAX);
}

void mLCD_Refresh(void)
{
    int i,j;
    uint8 page = LCD_SET_PAGE_ADDR;
    uint8 *pd = lcdBuff;
    for(i=0; i<LCD_YMAX/8; i++)
    {
        WriteCMD(page++);
        WriteCMD(LCD_SET_COLUMN_ADDRH);
        WriteCMD(LCD_SET_COLUMN_ADDRL + X_MOVE);
        for(j=0; j<LCD_XMAX; j++)
        {
            WriteData(*pd++);
        }
    }
    WriteCMD(0xAF);//DIAPLAY ON
}

#define SSD1306A

#ifdef SSD1306A
static CONST_UINT8 init_lcd[]={
    0xae,		//--turn off oled panel
    0x00,		//---set low column address
    0x10,		//---set high column address
    0x40,		//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    0x81,		//--set contrast control register
    0xcf,		 // Set SEG Output Current Brightness
    0xa1,		//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    0xc8,		//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    0xa6,		//--set normal display
    0xa8,		//--set multiplex ratio(1 to 64)
    0x3f,		//--1/64 duty
    0xd3,		//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    0x00,		//-not offset
    0xd5,		//--set display clock divide ratio/oscillator frequency
    0x80,		//--set divide ratio, Set Clock as 100 Frames/Sec
    0xd9,		//--set pre-charge period
    0xf1,		//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    0xda,		//--set com pins hardware configuration
    0x12,		
    0xdb,		//--set vcomh
    0x40,		//Set VCOM Deselect Level
    0x20,		//-Set Page Addressing Mode (0x00/0x01/0x02)
    0x02,		//
    0x8d,		//--set Charge Pump enable/disable
    0x14,		//--set(0x10) disable
    0xa4,		// Disable Entire Display On (0xa4/0xa5)
    0xa6,		// Disable Inverse Display On (0xa6/a7) 
    0xaf,		//--turn on oled panel
};
#endif
/******************接口函数***********************/

//初始化LCD
void mLCD_Init(void)
{
    int i;
    lcd_x = 0;
    LCD_RES_L;
    OSTimeDly( HZ/50 );
    LCD_RES_H;       //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
    for(i=0; i<ARRAY_SIZE( init_lcd); i++)
        WriteCMD( pgm_read_byte( &init_lcd[i] ) );
    LCD_Clear(0);  //初始清屏
}

void mLCD_ON(void)
{
	WriteCMD(0x8D);  //SET DCDC命令
	WriteCMD(0x14);  //DCDC ON
	WriteCMD(0xAF);  //DISPLAY ON
}

void mLCD_OFF(void)
{
	WriteCMD(0x8D);  //SET DCDC命令
	WriteCMD(0x10);  //DCDC OFF
	WriteCMD(0xAE);  //DISPLAY OFF
}

/*******************************************************************/




