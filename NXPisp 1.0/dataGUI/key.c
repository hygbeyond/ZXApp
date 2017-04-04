

#include "mytype.h"
#include "key.h"

uint32 Menu_buff[16];
uint8  Menu_page;
uint8  Menu_setEn;
uint8  Menu_setSize;

TKeyFunc key_Func;

#define MAINKEY            MKDis_Key

void CreateGUI( void )
{
    key_Func = MAINKEY;
}

void GUI_Goto( TKeyFunc func )
{
    key_Func = func!=NULL? func: MAINKEY;
    key_Func( KEY_INIT );
}

int GUI_Key( uint8 key )
{
    if( key_Func == NULL )
    {
        key_Func = MAINKEY;
        key_Func( KEY_INIT );
    }
    return key_Func( key );
}




