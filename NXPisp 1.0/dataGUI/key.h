
#ifndef __KEY_H__
#define __KEY_H__

#include "main_mac.h"

typedef int (*TKeyFunc)( uint8 );


extern uint32 Menu_buff[16];
extern uint8  Menu_page;
extern uint8  Menu_setEn;
extern uint8  Menu_setSize;



void CreateGUI( void );
void GUI_Goto( TKeyFunc func );
int  GUI_Key( uint8 key );

int CfgDis_Key( uint8 key );

extern int Main_Key( uint8 key );
extern int SetAddr_Key( uint8 key );
extern int MKDis_Key( uint8 key );
int AutoJZDis_Key( uint8 key );



#endif //__KEY_H__


