#ifndef __MCG12864_H__
#define __MCG12864_H__

#ifdef   MCG12864_GLOBALS
#define  MCG12864_EXT
#else
#define  MCG12864_EXT extern
#endif /*MCG12864_GLOBALS*/

#include "hardware.h"

#ifndef LCDUINT 
typedef uint8 LCDUINT;
#endif

#ifndef TLCDCOLOR 
typedef uint8 TLCDCOLOR;
#endif


#define LCD_RES_H   SBI_P(2, 11)
#define LCD_RES_L   CBI_P(2, 11)


#define  LCD_YMAX       64
#define  LCD_XMAX       128

#define  GUI_LCM_XMAX		128							/* 定义液晶x轴的点数 */
#define  GUI_LCM_YMAX		64							/* 定义液晶y轴的点数 */

void   mLCD_Init    (void);
int    mLCD_RefreshLine( int x, int y, int x2);
void   mLCD_RefreshRect( int x1, int y1, int x2, int y2 );

int mLCD_GetPixel( int x, int y );
int mLCD_SetPixel( int x, int y, int color );
int mLCD_Pixel( int x, int y, int color );

void   LCD_Clear   (uint8 color);

void mLCD_Refresh(void);
void mLCD_ON(void);
void mLCD_OFF(void);

#endif /*__MCG12864_H__*/
