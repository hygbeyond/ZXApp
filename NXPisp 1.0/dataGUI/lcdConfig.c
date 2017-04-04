
#include "mytype.h"
#include "portlcd.h"
#include "gui.h"

#define LCD_BITSPERPIXEL 1

#ifndef LCD_NUM_COLORS
#define LCD_NUM_COLORS (1L<<LCD_BITSPERPIXEL)
#endif

static void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex) {
	if (gui_context.pDrawRect) {
		if (x < gui_context.pDrawRect->x0 || x > gui_context.pDrawRect->x1
				|| y < gui_context.pDrawRect->y0
				|| y > gui_context.pDrawRect->y1)
			return;
	}
	mLCD_Pixel(x, y, PixelIndex);
}

static unsigned LCD_L0_GetPixelIndex(int x, int y) {
	return mLCD_GetPixel(x, y);
}

static void LCD_L0_XorPixel(int x, int y) {
	int c = mLCD_GetPixel(x, y);
	c |= gui_context.Color;

	mLCD_Pixel(x, y, c);
}

static void LCD_L0_DrawHLine(int x0, int y, int x1) {
	int x;
	int tmp;
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (gui_context.pDrawRect) {
		if (y < gui_context.pDrawRect->y0 || y > gui_context.pDrawRect->y1)
			return;
		if (x0 < gui_context.pDrawRect->x0)
			x0 = gui_context.pDrawRect->x0;
		if (x1 > gui_context.pDrawRect->x1)
			x1 = gui_context.pDrawRect->x1;
	}

	for (x = x0; x <= x1; x++) {
		mLCD_SetPixel(x, y, gui_context.Color);
	}
	mLCD_RefreshLine(x0, y, x1 - x0 + 1);
}

static void LCD_L0_DrawVLine(int x, int y0, int y1) {
	int y;
	int tmp;
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	if (gui_context.pDrawRect) {
		if (y0 < gui_context.pDrawRect->y0)
			y0 = gui_context.pDrawRect->y0;
		if (y1 > gui_context.pDrawRect->y1)
			y1 = gui_context.pDrawRect->y1;
	}

	for (y = y0; y <= y1; y++) {
		mLCD_SetPixel(x, y, gui_context.Color);
	}

    mLCD_RefreshRect( x,y0, 1, y1-y0+1 );
}

static void LCD_L0_FillRect(int x0, int y0, int x1, int y1) {
	int x, y;
	if (y0 > y1) {
		y = y0;
		y0 = y1;
		y1 = y;
	}

	if (gui_context.pDrawRect) {
		if (y0 < gui_context.pDrawRect->y0)
			y0 = gui_context.pDrawRect->y0;
		if (y1 > gui_context.pDrawRect->y1)
			y1 = gui_context.pDrawRect->y1;
	}

    for(x=x0; x<=x1; x++)
    {
        for( y=y0; y<=y1; y++)
        {
            mLCD_SetPixel( x, y, gui_context.Color );
        }
    }
    mLCD_RefreshRect( x0,y0, x1-x0+1, y1-y0+1 );
}

extern const uint8 DCB2HEX[];

static void LCD_L0_Pixel(int x, int y, int color) {
	if (gui_context.pDrawRect) {
		if (x < gui_context.pDrawRect->x0 || x > gui_context.pDrawRect->x1
				|| y < gui_context.pDrawRect->y0
				|| y > gui_context.pDrawRect->y1)
			return;
	}
	mLCD_SetPixel(x, y, color);
}

static void LCD_L0_DrawLine(int x, int y, int xsize, const uint8 *pData) {
	int i;
	for (i = 0; i < xsize; i++) {
		LCD_L0_Pixel(x + i, y,
				pData[i / 8] & DCB2HEX[i & 7] ?
						gui_context.Color : gui_context.BkColor);
	}
}

static void LCD_L0_DrawBitmap(int x, int y, int xsize, int ysize, const U8 * pData) {
	int j;
	const uint8 *pd = pData;
	for (j = 0; j < ysize; j++) {
		LCD_L0_DrawLine(x, y + j, xsize, pd);
		pd += (xsize + 7) / 8;
	}
    mLCD_RefreshRect( x,y, xsize, ysize );
}

const GUI_DEVICE mLCD_Device = { LCD_L0_DrawBitmap, LCD_L0_DrawHLine,
		LCD_L0_DrawVLine, LCD_L0_FillRect, LCD_L0_GetPixelIndex,
		LCD_L0_SetPixelIndex, LCD_L0_XorPixel };

extern void LCD_Config(void) ;
void LCD_Config(void) 
{
	mLCD_Init();
	GUI_SetSize( LCD_XMAX, LCD_YMAX);
	gui_context.pAFont  = &GUIFont12;
	gui_context.pDriver = &mLCD_Device;
}

