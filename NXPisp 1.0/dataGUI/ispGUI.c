/*
 * ispGUI.c
 *
 *  Created on: 2016Äê11ÔÂ17ÈÕ
 *      Author: Administrator
 */


#include "mytype.h"
#include "key.h"
#include "GUI.h"
#include "uudata.h"

int MKDis_Key( uint8 key )
{
	GUI_SetPos(0,32);
	GUI_Print("name:%s\n", uuFile.name );
	GUI_Print("size:%4d\n", uuFile.filesize );
	return 0;
}
