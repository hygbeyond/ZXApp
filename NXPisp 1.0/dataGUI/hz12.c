/*@begin Info
校正温度漏电压报警流额定延时冷备系统过欠恢复出厂设置成功失败自动扫描探头类型传感器采集数据缺相常故障完闭门磁地址
@end Info*/
/*中文字库 宋体 font12.c*/
#include <gui.h>

struct FONT_CN_12
{
	char  name[2];
	uint8 buff[24];
};

static const PROGMEM struct FONT_CN_12 FONT_12Cn[] = {
	{"校",{
	__X___X_,________,
	__X____X,________,
	XXXX_XXX,XXX_____,
	__X___X_,X_______,
	__XX_X__,_X______,
	_XX_X___,X_X_____,
	X_X__X__,X_______,
	__X___X_,X_______,
	__X____X,________,
	__X___X_,X_______,
	__XXXX__,_XX_____,
	________,________,
	}},

	{"正",{
	________,_X______,
	XXXXXXXX,XXX_____,
	_____X__,________,
	_____X__,________,
	__X__X__,________,
	__X__XXX,XX______,
	__X__X__,________,
	__X__X__,________,
	__X__X__,________,
	__X__X__,________,
	XXXXXXXX,XXX_____,
	________,________,
	}},

	{"温",{
	X___XXXX,XX______,
	_X__X___,_X______,
	____XXXX,XX______,
	X___X___,_X______,
	_X__XXXX,XX______,
	_X______,________,
	_X_XXXXX,XX______,
	X__X_X_X,_X______,
	X__X_X_X,_X______,
	X__X_X_X,_X______,
	X_XXXXXX,XXX_____,
	________,________,
	}},

	{"度",{
	______X_,________,
	_XXXXXXX,XXX_____,
	_X__X___,X_______,
	_XXXXXXX,XXX_____,
	_X__X___,X_______,
	_X__XXXX,X_______,
	_X______,________,
	_X_XXXXX,X_______,
	_X___X_X,________,
	X____XXX,________,
	X_XXX___,XXX_____,
	________,________,
	}},

	{"漏",{
	X__XXXXX,XX______,
	_X_X____,_X______,
	___XXXXX,XX______,
	X__X____,________,
	X__XXXXX,XXX_____,
	_X_X___X,________,
	_X_XXXXX,XXX_____,
	XX_XX__X,__X_____,
	X__XXX_X,X_X_____,
	X_X_X__X,__X_____,
	XX__X__X,_XX_____,
	________,________,
	}},

	{"电",{
	_____X__,________,
	_____X__,_X______,
	_XXXXXXX,XXX_____,
	_X___X__,_X______,
	_XXXXXXX,XX______,
	_X___X__,_X______,
	_X___X__,_X______,
	_XXXXXXX,XX______,
	_X___X__,________,
	_____X__,__X_____,
	______XX,XXX_____,
	________,________,
	}},

	{"压",{
	_XXXXXXX,XXX_____,
	_X______,________,
	_X___X__,________,
	_X___X__,________,
	_X___X__,________,
	_X_XXXXX,XX______,
	_X___X_X,________,
	_X___X__,X_______,
	_X___X__,_X______,
	X____X__,________,
	X_XXXXXX,XXX_____,
	________,________,
	}},

	{"报",{
	__X__XXX,XX______,
	__X__X__,_X______,
	XXXXXX__,_X______,
	__X__X_X,X_______,
	__X__X__,________,
	__XX_XXX,XX______,
	XXX__X_X,_X______,
	__X__X_X,_X______,
	__X__X__,X_______,
	__X__X_X,_X______,
	XXX__XX_,__X_____,
	________,________,
	}},

	{"警",{
	XXXXXX_X,________,
	_X__X__X,XXX_____,
	_XXXXXXX,_X______,
	XX_X_X__,X_______,
	_XXX_XXX,_X______,
	____XX__,__X_____,
	XXXXXXXX,XXX_____,
	________,________,
	__XXXXXX,X_______,
	__X_____,X_______,
	__XXXXXX,X_______,
	________,________,
	}},

	{"流",{
	X_____X_,________,
	_X_XXXXX,XXX_____,
	_X____X_,________,
	_____X__,X_______,
	X_X_XXXX,XX______,
	_XX_____,_X______,
	__X_X_X_,X_______,
	_X__X_X_,X_______,
	XX__X_X_,X_X_____,
	_X_X__X_,X_X_____,
	_XX___X_,XXX_____,
	________,________,
	}},

	{"额",{
	__X__XXX,XXX_____,
	XXXXX___,X_______,
	X___X_XX,XXX_____,
	_XXXX_X_,__X_____,
	_X__X_X_,X_X_____,
	X_XX__X_,X_X_____,
	_X__X_X_,X_X_____,
	XXXXX_X_,X_X_____,
	_X__X___,X_______,
	_X__X__X,_X______,
	_XXXX_X_,__X_____,
	________,________,
	}},

	{"定",{
	_____X__,________,
	_XXXXXXX,XXX_____,
	XX______,__X_____,
	________,________,
	_XXXXXXX,XX______,
	_____X__,________,
	__X__X__,________,
	__X__XXX,XX______,
	__XX_X__,________,
	_X__XX__,________,
	X_____XX,XXX_____,
	________,________,
	}},

	{"延",{
	________,_XX_____,
	XXXX_XXX,X_______,
	__X____X,________,
	_X_____X,________,
	_XXX_X_X,XXX_____,
	___X_X_X,________,
	_X_X_X_X,________,
	_X_X_X_X,________,
	__X__XXX,XXX_____,
	_X_X____,________,
	X___XXXX,XXX_____,
	________,________,
	}},

	{"时",{
	________,X_______,
	XXXX____,X_______,
	X__XXXXX,XXX_____,
	X__X____,X_______,
	X__X_X__,X_______,
	XXXX__X_,X_______,
	X__X__X_,X_______,
	X__X____,X_______,
	XXXX____,X_______,
	X__X____,X_______,
	______XX,X_______,
	________,________,
	}},

	{"冷",{
	______X_,________,
	X_____X_,________,
	_X___X_X,________,
	__X_X___,X_______,
	__XX__X_,_XX_____,
	_X_____X,________,
	_X__XXXX,XX______,
	X_______,X_______,
	X____X_X,________,
	X_____X_,________,
	_______X,________,
	________,________,
	}},

	{"备",{
	___X____,________,
	___XXXXX,X_______,
	__X_X__X,________,
	_X___XX_,________,
	____X__X,________,
	__XX____,X_______,
	XXXXXXXX,XXX_____,
	__X__X__,X_______,
	__XXXXXX,X_______,
	__X__X__,X_______,
	__XXXXXX,X_______,
	________,________,
	}},

	{"系",{
	______XX,XX______,
	_XXXXX__,________,
	____X___,X_______,
	__XXXXXX,________,
	_____X__,________,
	____X___,X_______,
	__XXXXXX,XX______,
	___X_X_X,________,
	__X__X__,X_______,
	_X___X__,_X______,
	X__XXX__,__X_____,
	________,________,
	}},

	{"统",{
	__X____X,________,
	__X__XXX,XXX_____,
	_X_X___X,________,
	XXXX__X_,________,
	__X__X__,_X______,
	_X___XXX,XXX_____,
	XXXX__X_,X_______,
	______X_,X_______,
	__XX__X_,X_X_____,
	XX___X__,X_X_____,
	___XX___,XXX_____,
	________,________,
	}},

	{"过",{
	X_______,X_______,
	_X______,X_______,
	_X_XXXXX,XXX_____,
	________,X_______,
	XXX_X___,X_______,
	__X__X__,X_______,
	__X__X__,X_______,
	__X_____,X_______,
	__X___XX,X_______,
	_X_X____,________,
	X___XXXX,XXX_____,
	________,________,
	}},

	{"欠",{
	___X____,________,
	___X____,________,
	__XXXXXX,XX______,
	__X_____,_X______,
	_X___X__,X_______,
	X____X__,________,
	_____X__,________,
	____X_X_,________,
	___X___X,________,
	__X_____,X_______,
	XX______,_XX_____,
	________,________,
	}},

	{"恢",{
	_X___X__,________,
	_X_XXXXX,XXX_____,
	XX___X__,________,
	XXX__X_X,________,
	XX___X_X,__X_____,
	_X__X__X,_X______,
	_X__X_XX,X_______,
	_X_X___X,________,
	_X____X_,X_______,
	_X___X__,_X______,
	_X_XX___,__X_____,
	________,________,
	}},

	{"复",{
	___X____,_X______,
	__XXXXXX,XXX_____,
	_XX_____,X_______,
	X_XXXXXX,X_______,
	__X_____,X_______,
	__XXXXXX,X_______,
	___X____,________,
	___XXXXX,X_______,
	_XX_X__X,________,
	_____XX_,________,
	XXXXX__X,XXX_____,
	________,________,
	}},

	{"出",{
	_____X__,________,
	_X___X__,_X______,
	_X___X__,_X______,
	_X___X__,_X______,
	_XXXXXXX,XX______,
	_____X__,________,
	_X___X__,_X______,
	_X___X__,_X______,
	_X___X__,_X______,
	_X___X__,_X______,
	_XXXXXXX,XX______,
	________,________,
	}},

	{"厂",{
	_XXXXXXX,XXX_____,
	_X______,________,
	_X______,________,
	_X______,________,
	_X______,________,
	_X______,________,
	_X______,________,
	_X______,________,
	_X______,________,
	X_______,________,
	X_______,________,
	________,________,
	}},

	{"设",{
	_X___XXX,X_______,
	__X__X__,X_______,
	_____X__,X_______,
	_____X__,XXX_____,
	XXX_X___,________,
	__XX_XXX,XX______,
	__X__X__,_X______,
	__X___X_,X_______,
	__XX___X,________,
	__X___X_,X_______,
	___XXX__,_XX_____,
	________,________,
	}},

	{"置",{
	_XXXXXXX,XX______,
	_X__X_X_,_X______,
	_XXXXXXX,XX______,
	_____X__,________,
	XXXXXXXX,XXX_____,
	__X_____,X_______,
	__XXXXXX,X_______,
	__X_____,X_______,
	__XXXXXX,X_______,
	__X_____,X_______,
	XXXXXXXX,XXX_____,
	________,________,
	}},

	{"成",{
	______X_,X_______,
	______X_,_X______,
	_XXXXXXX,XXX_____,
	_X____X_,________,
	_X____X_,_X______,
	_XXXX_X_,_X______,
	_X__X_X_,X_______,
	_X__X__X,________,
	_XX_X__X,__X_____,
	_X_X__X_,X_X_____,
	X____X__,_XX_____,
	________,________,
	}},

	{"功",{
	_______X,________,
	XXXXX__X,________,
	__X____X,________,
	__X__XXX,XXX_____,
	__X____X,__X_____,
	__X____X,__X_____,
	__X____X,__X_____,
	__XXX_X_,__X_____,
	XX____X_,__X_____,
	_____X__,__X_____,
	___XX__X,XX______,
	________,________,
	}},

	{"失",{
	_____X__,________,
	__X__X__,________,
	__XXXXXX,XX______,
	__X__X__,________,
	_X___X__,________,
	XXXXXXXX,XXX_____,
	_____X__,________,
	____X_X_,________,
	___X___X,________,
	__X_____,X_______,
	XX______,_XX_____,
	________,________,
	}},

	{"败",{
	XXXXX__X,________,
	X___X__X,________,
	X_X_X_XX,XXX_____,
	X_X_X_X_,_X______,
	X_X_XXX_,_X______,
	X_X_X_X_,_X______,
	X_X_X_X_,_X______,
	X_X_X__X,X_______,
	_X_X___X,X_______,
	_X__X_X_,_X______,
	X____X__,__X_____,
	________,________,
	}},

	{"自",{
	_____X__,________,
	__XXXXXX,XX______,
	__X_____,_X______,
	__X_____,_X______,
	__XXXXXX,XX______,
	__X_____,_X______,
	__X_____,_X______,
	__XXXXXX,XX______,
	__X_____,_X______,
	__X_____,_X______,
	__XXXXXX,XX______,
	________,________,
	}},

	{"动",{
	_______X,________,
	XXXXX__X,________,
	_______X,________,
	______XX,XXX_____,
	XXXXXX_X,__X_____,
	__X____X,__X_____,
	_X_X___X,__X_____,
	X___X__X,__X_____,
	XXXXX_X_,__X_____,
	X____X__,_X______,
	____X__X,XX______,
	________,________,
	}},

	{"扫",{
	__X_____,________,
	__X__XXX,XXX_____,
	XXXX____,__X_____,
	__X_____,__X_____,
	__X_____,__X_____,
	__XX_XXX,XXX_____,
	XXX_____,__X_____,
	__X_____,__X_____,
	__X_____,__X_____,
	__X__XXX,XXX_____,
	XXX_____,__X_____,
	________,________,
	}},

	{"描",{
	__X__X__,X_______,
	__X__X__,X_______,
	XXXXXXXX,XXX_____,
	__X__X__,X_______,
	__X_____,________,
	__XXXXXX,XXX_____,
	XXX_X__X,__X_____,
	__X_XXXX,XXX_____,
	__X_X__X,__X_____,
	__X_X__X,__X_____,
	XXX_XXXX,XXX_____,
	________,________,
	}},

	{"探",{
	__X_XXXX,XXX_____,
	__X_X___,__X_____,
	XXXX__X_,X_______,
	__X__X__,_X______,
	__XXX__X,__X_____,
	_XX_XXXX,XXX_____,
	X_X___XX,________,
	__X__X_X,X_______,
	__X_X__X,_X______,
	__XX___X,__X_____,
	XXX____X,________,
	________,________,
	}},

	{"头",{
	______X_,________,
	__XX__X_,________,
	____X_X_,________,
	_XX___X_,________,
	___X__X_,________,
	______X_,_X______,
	XXXXXXXX,XXX_____,
	_____X__,________,
	____X_XX,________,
	___X____,XX______,
	XXX_____,__X_____,
	________,________,
	}},

	{"类",{
	__X__X__,X_______,
	___X_X_X,________,
	XXXXXXXX,XXX_____,
	___X_X_X,________,
	__X__X__,X_______,
	XX______,_XX_____,
	_____X__,________,
	XXXXXXXX,XXX_____,
	____X_X_,________,
	___X___X,________,
	XXX_____,XXX_____,
	________,________,
	}},

	{"型",{
	_XXXXX__,_X______,
	__X_X__X,_X______,
	__X_X__X,_X______,
	XXXXXXXX,_X______,
	__X_X__X,_X______,
	__X_X___,_X______,
	_X__X___,XX______,
	X____X__,________,
	_XXXXXXX,XX______,
	_____X__,________,
	XXXXXXXX,XXX_____,
	________,________,
	}},

	{"传",{
	___X___X,________,
	___X___X,________,
	__X_XXXX,XXX_____,
	__X___X_,________,
	_XX___X_,________,
	X_XXXXXX,XXX_____,
	__X__X__,________,
	__X__XXX,XX______,
	__X_____,X_______,
	__X__XXX,________,
	__X_____,XX______,
	________,________,
	}},

	{"感",{
	_______X,_X______,
	_XXXXXXX,XXX_____,
	_X_____X,________,
	_XXXXXXX,_X______,
	_X_XXX_X,_X______,
	_X_X_X__,X_X_____,
	X__XXX_X,_XX_____,
	X_____X_,__X_____,
	_X_X_X__,_X______,
	_X_X__X_,X_X_____,
	X__XXXXX,X_______,
	________,________,
	}},

	{"器",{
	_XXXX_XX,XX______,
	_X__X_X_,_X______,
	_X__X_X_,_X______,
	_XXXX_XX,XX______,
	_____X__,X_______,
	XXXXXXXX,XXX_____,
	___X____,X_______,
	XXXXX_XX,XXX_____,
	_X__X_X_,_X______,
	_X__X_X_,_X______,
	_XXXX_XX,XX______,
	________,________,
	}},

	{"采",{
	_____XXX,XX______,
	_XXXXX__,_X______,
	_X___X__,X_______,
	__X____X,________,
	_____X__,________,
	XXXXXXXX,XXX_____,
	____XXX_,________,
	___X_X_X,________,
	__X__X__,X_______,
	_X___X__,_X______,
	X____X__,__X_____,
	________,________,
	}},

	{"集",{
	___X_X__,________,
	__XXXXXX,XX______,
	_XX__X__,________,
	X_XXXXXX,XX______,
	__X__X__,________,
	__XXXXXX,XX______,
	__X__X__,________,
	XXXXXXXX,XXX_____,
	___X_X_X,________,
	__X__X__,X_______,
	XX___X__,_XX_____,
	________,________,
	}},

	{"数",{
	X__X_X_X,________,
	_X_XX__X,________,
	XXXXXX_X,XXX_____,
	__XX__X_,_X______,
	_X_XX_X_,_X______,
	X__X_X_X,_X______,
	__X____X,_X______,
	XXXXXX__,X_______,
	_X__X___,X_______,
	__XXX__X,_X______,
	XX___XX_,__X_____,
	________,________,
	}},

	{"据",{
	__X_XXXX,XX______,
	__X_X___,_X______,
	XXXXXXXX,XX______,
	__X_X__X,________,
	__XXX__X,________,
	_XX_XXXX,XXX_____,
	X_X_X__X,________,
	__X_XXXX,XX______,
	__X_XX__,_X______,
	__XX_X__,_X______,
	XXX__XXX,XX______,
	________,________,
	}},

	{"缺",{
	_X_____X,________,
	_X_____X,________,
	_XXX_XXX,XX______,
	X_X____X,_X______,
	XXXXX__X,_X______,
	__X__XXX,XXX_____,
	X_X_X__X,________,
	X_X_X__X,________,
	X_XXX_X_,X_______,
	XX___X__,_X______,
	___XX___,__X_____,
	________,________,
	}},

	{"相",{
	__X_____,________,
	__X___XX,XXX_____,
	__X___X_,__X_____,
	XXXXX_X_,__X_____,
	__X___XX,XXX_____,
	_XXX__X_,__X_____,
	_XX_X_XX,XXX_____,
	X_X___X_,__X_____,
	X_X___X_,__X_____,
	__X___XX,XXX_____,
	__X___X_,__X_____,
	________,________,
	}},

	{"常",{
	__X__X__,X_______,
	_XXXXXXX,XXX_____,
	XX______,__X_____,
	___XXXXX,X_______,
	___X____,X_______,
	___XXXXX,X_______,
	_____X__,________,
	__XXXXXX,XX______,
	__X__X__,_X______,
	__X__X_X,XX______,
	_____X__,________,
	________,________,
	}},

	{"故",{
	___X___X,________,
	___X___X,________,
	XXXXXX_X,XXX_____,
	___X__X_,_X______,
	___X__X_,_X______,
	_XXXXX_X,_X______,
	_X__X__X,_X______,
	_X__X___,X_______,
	_X__X___,X_______,
	_XXXX__X,_X______,
	_X__XXX_,__X_____,
	________,________,
	}},

	{"障",{
	XXXX___X,________,
	X__XXXXX,XXX_____,
	X_X___X_,X_______,
	X_X_XXXX,XXX_____,
	X_X__X__,_X______,
	X__X_XXX,XX______,
	X__X_X__,_X______,
	XXXX_XXX,XX______,
	X______X,________,
	X__XXXXX,XXX_____,
	X______X,________,
	________,________,
	}},

	{"完",{
	_____X__,________,
	XXXXXXXX,XXX_____,
	X_______,__X_____,
	__XXXXXX,X_______,
	________,________,
	________,________,
	XXXXXXXX,XXX_____,
	___X__X_,________,
	___X__X_,__X_____,
	__X___X_,__X_____,
	XX____XX,XXX_____,
	________,________,
	}},

	{"闭",{
	__X__XXX,XXX_____,
	___X____,__X_____,
	_X____X_,__X_____,
	_X____X_,__X_____,
	_XXXXXXX,X_X_____,
	_X___XX_,__X_____,
	_X__X_X_,__X_____,
	_X_X__X_,__X_____,
	_XX___X_,__X_____,
	_X__XXX_,__X_____,
	_X______,XXX_____,
	________,________,
	}},

	{"门",{
	__X_____,________,
	___X_XXX,XXX_____,
	_X_X____,__X_____,
	_X______,__X_____,
	_X______,__X_____,
	_X______,__X_____,
	_X______,__X_____,
	_X______,__X_____,
	_X______,__X_____,
	_X______,__X_____,
	_X______,XXX_____,
	________,________,
	}},

	{"磁",{
	_____X__,_X______,
	XXXX__X_,X_______,
	_X__XXXX,XXX_____,
	_X___X__,_X______,
	_XXX_X__,_X______,
	XX_XX_X_,X_X_____,
	_X_XXXX_,XXX_____,
	_X_X_X__,_X______,
	_X_X_X__,_X______,
	_XXXX_X_,X_X_____,
	_X__XXX_,XXX_____,
	________,________,
	}},

	{"地",{
	_X_____X,________,
	_X___X_X,________,
	_X___X_X,_X______,
	_X___X_X,XX______,
	XXXX_XXX,_X______,
	_X__XX_X,_X______,
	_X___X_X,_X______,
	_X___X_X,XX______,
	_XXX_X_X,__X_____,
	XX___X__,__X_____,
	_____XXX,XXX_____,
	________,________,
	}},

	{"址",{
	__X____X,________,
	__X____X,________,
	__X____X,________,
	XXXX_X_X,________,
	__X__X_X,XXX_____,
	__X__X_X,________,
	__X__X_X,________,
	__X__X_X,________,
	__XX_X_X,________,
	XX___X_X,________,
	___XXXXX,XXX_____,
	________,________,
	}},

    {{0xFF,0xFF},{
    ________,________,
    _XXXXXXX,XXX_____,
    _XX_____,_XX_____,
    _X_X____,X_X_____,
    _X__X__X,__X_____,
    _X___XX_,__X_____,
    _X___XX_,__X_____,
    _X__X__X,__X_____,
    _X_X____,X_X_____,
    _XX_____,_XX_____,
    _XXXXXXX,XXX_____,
    ________,________,
    }},
};


extern const uint8 *hz12_GetBitmap( uint16 ch );
__WEAK__
const uint8 *hz12_GetBitmap( uint16 ch )
{
    int tmp;
    for(int i=0; ARRAY_SIZE(FONT_12Cn); i++)
    {
        tmp = READHZ( pgm_read_byte(&FONT_12Cn[i].name[0]), pgm_read_byte(&FONT_12Cn[i].name[1]) );
        if( tmp == 0xFFFF || tmp == ch )
        {
            return FONT_12Cn[i].buff; 
        }
    }
    return NULL;
}
/*end font    汉字数量56*/
