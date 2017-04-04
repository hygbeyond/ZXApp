
/*
   文件名：user.c
   功能：用户配置信息，保存在eeprom中的数据
   */
#define USER_GLOBAL
#include "mytype.h"
#include "OSstring.h"
#include "crc16.h"
#include "user.h"
#include "app_cfg.h"

#include "fmc_rw.h"
#include "bsp.h"
#include "data.h"
#include "protocol.h"

#define   USER_JZV      1024
#define   USER_JZI      500

typedef struct
{
    TUser   usr;
    uint16  crc;
}__PACKED__ TUserSave;

TUserSave userSave;
TUser *pUsrCfg = &userSave.usr;


extern void reset_cmd(void);


//static uint8 UserRead(uint16 addr)
//{
//    uint16 crc;
//    fmc_Read( __DataFlash, (uint8 *)&User,sizeof(User));
//    crc = modbus_crc(0x55AA,(uint8*)&User,sizeof(User.Usr));
//    return(crc == User.crc);
//}
//
//static void UserWrite(void)
//{
//    User.crc = modbus_crc(0x55AA,(uint8*)&User,sizeof(User.Usr));
//    fmc_Write( __DataFlash,  (uint8 *)&User, sizeof(User));
//}

int UsrRead( uint16 addr )
{
    uint16 crc;
    fmc_Read( __DataFlash, (uint8*) &userSave, sizeof( userSave )  );
    crc = modbus_crc( 0, (uint8*) pUsrCfg, sizeof( *pUsrCfg) );
    return crc == userSave.crc;
}

void UsrWrite( void )
{
    uint16 crc;
    crc = modbus_crc( 0, (uint8*)pUsrCfg, sizeof( *pUsrCfg ));
    userSave.crc = crc;
    fmc_Write( __DataFlash, (uint8*) &userSave, sizeof( userSave));
}


void UsrInit( void )
{
    fmc_Init();
    if( !UsrRead(0))
    {
        UsrDefault();
        UsrWrite();
    }
    pUsrCfg->count = MIN( MAX_POINT_COUNT, pUsrCfg->count );
}


void UserSetDef( void )
{
    pUsrCfg->address = 1;
    pUsrCfg->delay   = 4;
#if OEM == 1
//#warning "count 6"
    pUsrCfg->count   = 6;
#else
//#warning "count 7"
    pUsrCfg->count   = 7;
#endif
    UsrSetType(pUsrCfg->type );
}

void UsrDefault( void )
{
    pUsrCfg->address = 1;
#if OEM == 1
    pUsrCfg->count   = 6;
#else
    pUsrCfg->count   = 7;
#endif
    pUsrCfg->type    = eCGQvas2; //CGQ_TYPE_VI_3B;
    pUsrCfg->delay   = 20;
    pUsrCfg->delayI  = 50;
    pUsrCfg->enable  = 0;
    pUsrCfg->Aed     = 100;
    pUsrCfg->Abj     = 100;
    pUsrCfg->Vqy     = 187;
    pUsrCfg->Vbj     = 242;

    pUsrCfg->WDJZen = 1;

    pUsrCfg->JZ[0] = USER_JZV;
    pUsrCfg->JZ[1] = USER_JZV;
    pUsrCfg->JZ[2] = USER_JZV;

    pUsrCfg->JZ[3] = USER_JZV;
    pUsrCfg->JZ[4] = USER_JZV;
    pUsrCfg->JZ[5] = USER_JZV;

    pUsrCfg->JZ[6] = USER_JZV;
    pUsrCfg->JZ[7] = USER_JZI;


    //剩余电流默认配置
    pUsrCfg->LDCfg.ed  = 1600;
    pUsrCfg->LDCfg.bj  = 800;
    pUsrCfg->LDCfg.jz  = 440;

    //剩余电流默认配置
    pUsrCfg->DLCfg.ed  = 100;
    pUsrCfg->DLCfg.bj  = 100;
    pUsrCfg->DLCfg.jz  = 245;

    //温度配置文件
    pUsrCfg->WDCfg.bj  = 80;
    pUsrCfg->WDCfg.jzL = 511;
    pUsrCfg->WDCfg.jzH = 1024;
    pr_debug("user default\n");
}

int UsrSetAddr( int addr )
{
    if( addr>0 && addr <= 2000 )
    {
        pUsrCfg->address = addr;
        UsrWrite();
    }
    return pUsrCfg->address;
}

int UsrSetDelay( int delay )
{
    if(delay > 0 )
    {
        pUsrCfg->delay = delay;
        UsrWrite();
    }
    return pUsrCfg->delay;
}

int UsrSetDelayI( int delay )
{
    if(delay > 0 )
    {
        pUsrCfg->delayI = delay;
        UsrWrite();
    }
    return pUsrCfg->delayI;
}

int UsrSetIed( int ed )
{
    if( ed >= 5 )
    {
        pUsrCfg->Aed = ed;
        pUsrCfg->Abj = ed;
        UsrWrite();
    }
    return pUsrCfg->Aed;
}

int UsrSetIbj( int bj )
{
    if( bj > 0 && bj <= pUsrCfg->Aed)
    {
        pUsrCfg->Abj = bj;
        UsrWrite();
    }
    return pUsrCfg->Abj;
}

int UsrSetVbj( int bj )
{
    if( bj > 5)
    {
        pUsrCfg->Vbj = bj;
        UsrWrite();
    }
    return pUsrCfg->Vbj;
}

int UsrSetVqy( int bj )
{
    if( bj > 5)
    {
        pUsrCfg->Vqy = bj;
        UsrWrite();
    }
    return pUsrCfg->Vqy;
}

int UsrSetWbj( int bj )
{
    if( bj > 40 && bj <= 160 )
    {
        pUsrCfg->WDCfg.bj = bj;
        UsrWrite();
    }
    return pUsrCfg->WDCfg.bj;
}

int UsrSetSbj( int bj )
{
    if( bj > 200 && bj <= 1600 )
    {
        pUsrCfg->LDCfg.bj = bj;
        UsrWrite();
    }
    return pUsrCfg->LDCfg.bj;
}

int UsrSetCnt( int cnt )
{
    if( cnt > 0 && cnt <= MAX_POINT_COUNT )
    {
        pUsrCfg->count = cnt;
        UsrWrite();
    }
    return pUsrCfg->count;
}

int UsrSetBD( int cnt )
{
	if (cnt >= 0) {
		pUsrCfg->BDenable = cnt ? 1 : 0;
		UsrWrite();
	}
	return pUsrCfg->BDenable;
}


int UsrSetWDJZ(int cnt)
{
	if (cnt >= 0) {
		pUsrCfg->WDJZen = cnt ? 1 : 0;
		UsrWrite();
	}
	return pUsrCfg->WDJZen;
}

int UsrSetType( int type )
{
	if( type <= 0 || type >= eCGQend )
	{
		return pUsrCfg->type;
	}
    switch( type )
    {
    case eCGQFJ:
    case eCGQld:        /*15 8路漏电、温度、电流*/
    case eCGQTX:        /*16 255路二总线模块 */
            return pUsrCfg->type;
    }

    pUsrCfg->type = type;
    pUsrCfg->Aed     = 100;
    pUsrCfg->Abj     = 100;
    pUsrCfg->Vqy     = 187;
    pUsrCfg->Vbj     = 242;

    switch( type )
    {
        case eCGQvaw2:
        case CGQ_TYPE_VI_W:
        case CGQ_TYPE_VA_W:
            pUsrCfg->Vqy     = 323;
            pUsrCfg->Vbj     = 418;
            break;
        case eCGQvd:
        case eCGQvdi:
            pUsrCfg->Vqy     = 204;
            pUsrCfg->Vbj     = 262;
            break;
    }

    UsrWrite();
    reset_cmd();
    return pUsrCfg->type;
}



typedef struct TUserCMD
{
	char name[8];
	int (*func)(int );
}TUserCMD;

const TUserCMD userCMD[]=
{
	{ "addr",   UsrSetAddr  },
	{ "cnt",    UsrSetCnt   },
	{ "type",   UsrSetType  },
	{ "delay",  UsrSetDelay },
	{ "delayI", UsrSetDelayI },
	{ "edI",    UsrSetIed   },
	{ "bjI",    UsrSetIbj   },
	{ "bjV",    UsrSetVbj   },
	{ "qyV",    UsrSetVqy   },
	{ "BD",     UsrSetBD    },
	{ "WDJZ",   UsrSetWDJZ  },
};


#include "pcommand.h"
static void user_cmd(int args, char **argv )
{
	int cnt;
	cnt = atoi(argv[2]);
	int i;
	for (i = 0; i < ARRAY_SIZE(userCMD); i++) {
		if (!strcmp(argv[1], userCMD[i].name)) {
			TRACE("%s %d\n", userCMD[i].name, userCMD[i].func(cnt));
			return;
		}
	}
	if (!strcmp(argv[1], "get")) {
		for (i = 0; i < ARRAY_SIZE(userCMD); i++) {
			TRACE("%s\t%d\n", userCMD[i].name, userCMD[i].func(-1));
		}
	} else if (!strcmp(argv[1], "def")) {
		UsrDefault();
		UsrWrite();
	}
}
PSHELL_CMD( user, user_cmd, "user");



