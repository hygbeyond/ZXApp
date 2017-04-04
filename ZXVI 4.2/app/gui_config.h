

#ifndef __GUI_CONFIG_H__
#define __GUI_CONFIG_H__

#include "mytype.h"	 
#include "portlcd.h"  
#include "main_mac.h"
#include "hardware.h"


#define GUI_OS          0
#define GUI_MAXTASK     (4)

#define ASCII	2

#define FONT5_7_EN      0  
#define FONT6_8_EN      1
#define FONT8_8_EN 		0  
#define FONT6_12_EN		0
#define FONT8_16_EN		1

#define HZJ_EN          1
#define HZ_EN           0
#define HZJ16x16_EN     1      /*编辑12汉字库*/

//#define HZ_FONT         FONT_TYPE_HZJ

#define FONT_CN_WIDTH	12
#define FONG_CN_HEIGHT	12

#define GUI_LCD_XMAX    128
#define GUI_LCD_YMAX    32

#define GUI_Y		0
#define GUI_X		0
#define GUI_YMAX	(GUI_LCD_YMAX)
#define GUI_XMAX	(GUI_LCD_XMAX)
#define DEFAULT_ASCIIFONT           ASCII8x16
#define DEFAULT_HZFONT              HZJ16x16

#define GUI_DrawStr_EN     0
#define GUI_LINE_EN        0
#define GUI_FONTHEIGHT_EN  0
#define GUI_RectDrawString_EN  0


#define PROGRESSBAR_EN     0
#define SCROLL_EN          1
#define MENU_EN            1


//Menu配置参数
#define MMENU_CH_SIZE		4
#define MENU_RECT_EN		1
#define MMENU_SIZE			5
#define SMENU_SIZE			20 
#define MENU_FONT           HZJ16x16
#define MENU_CHAR_height	MENU_FONT.height/*菜单字体高度*/



#ifndef KEY_OK
#define KEY_OK              13
#endif

#ifndef KEY_CANCEL
#define KEY_CANCEL          3
#endif

#ifndef KEY_INC          
#define KEY_INC             1
#endif

#ifndef KEY_DEC
#define KEY_DEC             2
#endif

#if GUI_OS > 0
#define GUI_LOCK()    GUI_X_Lock()
#define GUI_UNLOCK()  GUI_X_UnLock()
#define GUI_OSINIT()  GUI_X_InitOS()
#else	 
#define GUI_LOCK() 
#define GUI_UNLOCK()
#define GUI_OSINIT()
#endif



#include "OSgui.h" 

#endif //__GUI_CONFIG_H__



