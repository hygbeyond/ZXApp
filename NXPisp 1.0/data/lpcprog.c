
#include "mytype.h"
#include "NxpMcu.h"
#include "NxpSerial.h"
#include "uudata.h"
#include "OSstring.h"
#include "OSstdio.h"
#include "OSstdlib.h"
#include "GUI.h"
#include "MicroOS.h"

static char Answer[128];
static char temp[128];
static char tmpString[128];




static int GUIPut(int ic, FILE *stream)
{
	int GUI_x = gui_context.DispPosX;
	int GUI_y = gui_context.DispPosY;

	GUI_RECT *pRect = gui_context.pDrawRect==NULL? &gui_context.ClipRect: gui_context.pDrawRect;

	int x1 = pRect->x0;
	int y1 = pRect->y0;
	int x2 = pRect->x1;
	int y2 = pRect->y1;
	int h = GUI_GetFontHeight(0xAAAA);

	switch(ic)
	{
	case '\r':
		GUI_SetPos( 0, 0);
		GUI_ClearRect(0, 0, x2, h);
		break;
	case '\n':{
		GUI_x = x1;
		GUI_y += h;
		if (GUI_y > y2-h)
			GUI_y = y1;

		GUI_SetPos( GUI_x, GUI_y);
		GUI_ClearRect(GUI_x, GUI_y, x2, GUI_y+h-1);
	}
	break;
	default:
		GUI_PutZF(ic);
		GUI_x = gui_context.DispPosX;

		if (GUI_x > x2) {
			GUI_x = x1;
			GUI_y += h;
			if (GUI_y > y2 - h)
				GUI_y = y1;
			GUI_SetPos(GUI_x, GUI_y);
			GUI_ClearRect(GUI_x, GUI_y, x2, GUI_y + h-1);
		}
		return GUI_x;
	}
    return 0;
}

void DebugPrintf(int p,const char *fmt, ... )
{
	if(p>2) return;
	static __FILECONST( guifile, GUIPut, NULL, __SWR | __SSTRHZ, NULL );
    va_list ap;
	va_start(ap, fmt);
	vfprintf((FILE*)&guifile, fmt, ap);
	va_end(ap);
}


//#define DebugPrintf(p, fmt, args...)        GUI_Print(fmt, ## args)

//void DebugPrintf( int p, const char *fmt, ... )
//{
//	va_list ap;
//	int i;
//	static char buff[128];
//	uint8 *str = (uint8*)buff;
//	va_start(ap, fmt);
//	i = vsnprintf(buff, sizeof(buff), fmt, ap);
//	va_end(ap);
//
//	for(int a=0; a<HZ/15; a++)
//	{
//		int ret = Uart_SendBuff(str, i);
//		str += ret;
//		i   -= ret;
//		if( i==0 ) return 1;
//		OSTimeDly(1);
//	}
////    Uart_Init( UART_BPS );
//	return 0;
//}


/***************************** GetAndReportErrorNumber ***************************/
/**  Find error number in string.  This will normally be the string
returned from the microcontroller.
\param [in] Answer the buffer to search for the error number.
\return the error number found, if no linefeed found before the end of the
string an error value of 255 is returned. If a non-numeric value is found
then it is printed to stdout and an error value of 255 is returned.
*/
static unsigned char GetAndReportErrorNumber(const char *pAnswer)
{
	unsigned char Result = 0xFF;                            // Error !!!
	unsigned int i = 0;

	while (1)
	{
		if (pAnswer[i] == 0x00)
		{
			break;
		}

		if (pAnswer[i] == 0x0a)
		{
			i++;

			if (pAnswer[i] < '0' || pAnswer[i] > '9')
			{
				DebugPrintf(1, "ErrorString: %s", &pAnswer[i]);
				break;
			}

			Result = (unsigned char) (atoi(&pAnswer[i]));
			break;
		}

		i++;
	}
	return Result;
}


int NxpSynchronizing( )
{
	int ret  = 0;
	unsigned long realsize = 0;
	int i =  15;

	DebugPrintf(2, "\rSynchronizing ");
	while( i )
	{
		DebugPrintf(2, ".");
		SendComPort( "?");

		memset(Answer,0,sizeof(Answer));
		RectComPort( Answer, sizeof(Answer)-1, &realsize, 1, 500);
		FormatCommand(Answer, Answer);
		if (strcmp(Answer, "Synchronized\n") == 0)
		{
			ret++;
			break;
		}
		i--;
	}
	if( i == 0 )
	{
		DebugPrintf(2, "\rerror\n");
		return 0;
	}

	DebugPrintf(2, " OK\n");

	if( !TransAndVerify( "Synchronized\r\n", Answer, sizeof(Answer), 2, "OK\n") )
	{
		DebugPrintf(1, "E 'Synchronized'\n");
		return (NO_ANSWER_SYNC);
	}

	DebugPrintf(3, "Synchronized 1\n");

	DebugPrintf(3, "Setting oscillator\n");

	if( !TransAndVerify( "12000\r\n", Answer, sizeof(Answer), 2, "OK\n"))
	{
		DebugPrintf(1, "E Oscillator-Command\n");
		return (NO_ANSWER_OSC);
	}

	DebugPrintf(3, "Unlock\n");
	if (!SendAndVerify( "U 23130\r\n", Answer, sizeof Answer))
	{
		DebugPrintf(1, "Unlock-Command:\n");
		return (UNLOCK_ERROR );
	}

	DebugPrintf(2, "Version: ");

	if (!SendAndVerifyExt(  "K\r\n", Answer, sizeof Answer, 4))
	{
		DebugPrintf(1, "E Boot Code Version\n" );
		return (NO_ANSWER_RBV);
	}
	int v1,v2;
	GetCommand( Answer, temp, 3 );
	v1 = atoi(temp);
	GetCommand( Answer, temp, 4 );
	v2 = atoi(temp);
	DebugPrintf(2, "%d.%d\n", v1, v2);
	return ret;
}

const LPC_DEVICE_TYPE *NxpGetLPC( )
{
	const LPC_DEVICE_TYPE *pLPC = NULL;
	unsigned long realsize = 0;

	DebugPrintf(2, "ID: ");

	if( SendAndVerifyExt( "J\r\n", Answer, sizeof(Answer), 3 ) )
	{
		if( GetCommand( Answer, temp, 3 )  )
		{
			DWORD id = atol( temp );
			DebugPrintf(2, " %08X\n", id);
			pLPC = FindID( id );
			if( pLPC )
			{
				if( pLPC->EvalId2 != 0 )
				{
					RectComPort( Answer, sizeof(Answer)-1, &realsize, 1, 100);
					if( realsize )
					{
						int id2 = atol( Answer );
						return FindIDExt( id, id2 );
					}
					return NULL;
				}
				return pLPC;
			}
		}
	}
	return NULL;
}


/*
	通过ISP下载程序到内存中
	采用UU编码格式下载程序
*/
int NxpCopyToRam( const LPC_DEVICE_TYPE *pLPC, unsigned long Pos, unsigned long RamAddr, int size )
{
	int cnt;
	int len;//  = UU_SIZE;
	int i=0;

    len = (size+44)/45;
    len *= 45;

	sprintf(tmpString, "W %ld %ld\r\n", RamAddr, len);
	if (!SendAndVerify( tmpString, Answer, sizeof Answer))
	{
		DebugPrintf(1, "\rWrong answer on Write-Command\n");
		return (WRONG_ANSWER_WRIT + GetAndReportErrorNumber(Answer));
	}

	DebugPrintf(3, "\nWrite %08x %08x:\n", Pos, RamAddr );
    UU_crc = 0;
    while( i < len )
    {
//		DebugPrintf(2, ".");
        cnt = MIN( 512-i, 45 );
        memset( temp, 0xFF, sizeof( temp ));
        if( uuGetFlashBuff( Pos, (uint8*)temp, cnt ) != cnt )
        {
            DebugPrintf(1, "\rError on read flash\n");
			return -1;
        }

        if( Pos == 0 )
        {
            NxpSetBinCRC( pLPC,(uint8 *) temp );
        }

		DataToUU( temp, tmpString, 45 );
		strcat( tmpString, "\r\n" );

		if( !TransAndVerify( tmpString, Answer, sizeof( Answer), 1, NULL ) )
		{
			DebugPrintf(1, "\rError on writing data (1)\n");
			return -1;
		}
        Pos += 45;
        i   += 45;
    }

//    OSTimeDly(HZ/10);
//	DebugPrintf(2, "\nblock_CRC = %ld\n", UU_crc);
	sprintf(tmpString, "%ld\r\n", UU_crc);
	if( !TransAndVerify( tmpString, Answer, sizeof(Answer), 2, "OK\n") == 0)
	{
		return 0;
	}
	DebugPrintf(1, "\rError on writing block_CRC (2)\n");
	return (ERROR_WRITE_CRC);
}



int NxpCopyToRam512( const LPC_DEVICE_TYPE *pLPC, unsigned long Pos, unsigned long RamAddr )
{
    return NxpCopyToRam( pLPC, Pos, RamAddr, 512 );
}


int NxpCopyRamToFlash( const LPC_DEVICE_TYPE *pLPC, unsigned long FlashaAddr, unsigned long RamAddr, int size )
{
	if ( pLPC->ChipVariant == CHIP_VARIANT_LPC43XX ||
		pLPC->ChipVariant == CHIP_VARIANT_LPC18XX)
	{
		sprintf(tmpString, "P %d %d 0\r\n", 0, pLPC->FlashSectors-1);
	}
	else {
		sprintf(tmpString, "P %d %d\r\n", 0, pLPC->FlashSectors-1);
	}

	if (!SendAndVerify( tmpString, Answer, sizeof Answer))
	{
		DebugPrintf(1, "E P %d %d\n", 0, pLPC->FlashSectors-1);
		return (WRONG_ANSWER_PREP);
	}
	sprintf(tmpString, "C %ld %ld %d\r\n", FlashaAddr, RamAddr, size );
	if (!SendAndVerify( tmpString, Answer, sizeof Answer))
	{
		DebugPrintf(1, "E C %x %x %d\n", FlashaAddr, RamAddr, size);
		return (WRONG_ANSWER_PREP);
	}



//	OSTimeDly(5);
	if( FlashaAddr )
	{
		sprintf(tmpString, "M %ld %ld %d\r\n", FlashaAddr, RamAddr, size );
		if (!SendAndVerify(tmpString, Answer, sizeof Answer))
		{
			DebugPrintf(1, "Wrong answer on Compare-Command\n");
			return (WRONG_ANSWER_COPY + GetAndReportErrorNumber(Answer));
		}
	}else
	{
		int cnt = MIN( size, 512 );
		if( cnt < size )
		{
			sprintf(tmpString, "M %ld %ld %d\r\n", cnt, RamAddr+cnt, size-cnt );
			if (!SendAndVerify(tmpString, Answer, sizeof Answer))
			{
				DebugPrintf(1, "Wrong answer on Compare-Command\n");
				return (WRONG_ANSWER_COPY + GetAndReportErrorNumber(Answer));
			}
		}
	}
	return 0;
}

int NxpWriteFlash1024( const LPC_DEVICE_TYPE *pLPC, unsigned long FlashAddr )
{
	unsigned long RamAddr = ReturnValueLpcRamBaseExt(pLPC);

	DebugPrintf(2, "\rW %08x", FlashAddr );
	if( NxpCopyToRam512( pLPC, FlashAddr, RamAddr ) )
	{
		return 1;
	}
	if( NxpCopyToRam512( pLPC, FlashAddr+512, RamAddr+512 ) )
	{
		return 1;
	}
//	DebugPrintf(2, "\n rom to flash\n");
	if( NxpCopyRamToFlash( pLPC, FlashAddr, RamAddr, 1024 )) return 1;
	return 0;
}


int NxpEras( const LPC_DEVICE_TYPE *pLPC, int len )
{
	int Eras_Sect = 0;
	if( len == 0 )
	{
		Eras_Sect = pLPC->FlashSectors-1;;
	}else
		Eras_Sect = NxpGetSector( pLPC, len );


	DebugPrintf(2, "eras Device %d - %d. ", 0, Eras_Sect);

	if ( pLPC->ChipVariant == CHIP_VARIANT_LPC43XX ||
		pLPC->ChipVariant == CHIP_VARIANT_LPC18XX)
	{
		// TODO: Quick and dirty hack to address bank 0
		sprintf(tmpString, "P %d %d 0\r\n", 0, Eras_Sect);
	}
	else
	{
		sprintf(tmpString, "P %d %d\r\n", 0, Eras_Sect);
	}

	if (!SendAndVerify( tmpString, Answer, sizeof Answer))
	{
		DebugPrintf(1, "Wrong answer on Prepare-Command\n");
		return (WRONG_ANSWER_PREP);
	}

	if ( pLPC->ChipVariant == CHIP_VARIANT_LPC43XX ||
		pLPC->ChipVariant == CHIP_VARIANT_LPC18XX)
	{
		// TODO: Quick and dirty hack to address bank 0
		sprintf(tmpString, "E %d %d 0\r\n", 0, Eras_Sect);
	}
	else
	{
		sprintf(tmpString, "E %d %d\r\n", 0, Eras_Sect);
	}
	if (!SendAndVerify( tmpString, Answer, sizeof Answer))
	{
		DebugPrintf(1, "Wrong answer on Erase-Command\n");
		return (WRONG_ANSWER_ERAS + GetAndReportErrorNumber(Answer));
	}
	DebugPrintf(2, "OK \n");

	if (pLPC->ChipVariant == CHIP_VARIANT_LPC43XX ||
		pLPC->ChipVariant == CHIP_VARIANT_LPC18XX)
	{
		DebugPrintf(2, "ATTENTION: Only bank A was wiped!!!\n");
	}

	if (!SendAndVerify( "U 23130\r\n", Answer, sizeof Answer))
	{
		DebugPrintf(1, "Unlock-Command:\n");
		return (UNLOCK_ERROR );
	}

	return 0;
}

int NxpErrorPro( const LPC_DEVICE_TYPE *pLPC )
{
	int cnt = 10;

	while( cnt )
	{
		if( SendAndVerifyExt(  "J\r\n", Answer, sizeof(Answer), 3 ) )
		{
			break;
		}
		cnt--;
	}
	if( cnt == 0 )
		return 0;
	if (!SendAndVerify( "U 23130\r\n", Answer, sizeof Answer))
	{
		DebugPrintf(1, "Unlock-Command:\n");
		return (UNLOCK_ERROR );
	}
	return 0;
}


int NxpDownloadFlash( const LPC_DEVICE_TYPE *pLPC  )
{
	unsigned long len = uuGetFlashLen();
	unsigned long addr = (len+1023)/1024;
	while( addr-- )
	{
		if( NxpWriteFlash1024( pLPC, addr * 1024 ))
		{
			NxpErrorPro(pLPC);
			if( NxpWriteFlash1024( pLPC, addr * 1024 ))
			{
				NxpErrorPro(pLPC);
				if( NxpWriteFlash1024( pLPC, addr * 1024 ))
				{
					DebugPrintf(2, "write flash %d err\n", addr);
					return 1;
				}
			}
		}

//		DebugPrintf(2, "write flash %d OK\n", addr);
//		OSTimeDly(HZ);
	}
	DebugPrintf(2, "\rwrite flash OK\n");
	return 0;
}



int  NxpGoStart( const LPC_DEVICE_TYPE *pLPC, uint32 StartAddress  )
{
	char ExpectedAnswer[32];
	unsigned long realsize;

	//if(IspEnvironment->DoNotStart == 0)
	{
		if (!SendAndVerify( "U 23130\r\n", Answer, sizeof Answer))
		{
			DebugPrintf(1, "Unlock-Command:\n");
			return (UNLOCK_ERROR );
		}

		DebugPrintf(2, "Now launching the brand new code\n");
		//fflush(stdout);

		if(pLPC->ChipVariant == CHIP_VARIANT_LPC2XXX)
		{
			sprintf(tmpString, "G %ld A\r\n", StartAddress);
			sprintf(ExpectedAnswer, "G %ld A\n0", StartAddress);
		}
		else if(pLPC->ChipVariant == CHIP_VARIANT_LPC43XX ||
			pLPC->ChipVariant == CHIP_VARIANT_LPC18XX ||
			pLPC->ChipVariant == CHIP_VARIANT_LPC17XX ||
			pLPC->ChipVariant == CHIP_VARIANT_LPC13XX ||
			pLPC->ChipVariant == CHIP_VARIANT_LPC11XX)
		{
			sprintf(tmpString, "G %ld T\r\n", StartAddress & ~1);
			sprintf(ExpectedAnswer, "G %ld T\n0", StartAddress & ~1);
		}
		else if(pLPC->ChipVariant == CHIP_VARIANT_LPC8XX)
		{
			sprintf(tmpString, "G 0 T\r\n");
			sprintf(ExpectedAnswer, "G 0 T\n0");
		}
		else
		{
			DebugPrintf(1, "Internal Error %s %d\n", __FILE__, __LINE__);
			return(1);
		}

		SendComPort(tmpString); //goto 0 : run this fresh new downloaded code code

		RectComPort( Answer, sizeof(Answer)-1, &realsize, 2, 500);
		FormatCommand(Answer, Answer);
		if (realsize == 0 || strncmp((const char *)Answer, /*cmdstr*/ExpectedAnswer, strlen(/*cmdstr*/ExpectedAnswer)) != 0)
		{
			DebugPrintf(2, "Failed to run the new downloaded code: ");
			return (FAILED_RUN + GetAndReportErrorNumber(Answer));
		}
		return 0;
	}
	return 1;
}


//int NxpBinCRC( const LPC_DEVICE_TYPE *pLPC )
//{
//	DWORD ivt_CRC = 0;
//	uint8 buff[32];
//	int i;
//	int pos;
//
//	if (pLPC->ChipVariant == CHIP_VARIANT_LPC2XXX) {
//		pos = 0x14;
//	} else if (pLPC->ChipVariant == CHIP_VARIANT_LPC43XX
//			|| pLPC->ChipVariant == CHIP_VARIANT_LPC18XX
//			|| pLPC->ChipVariant == CHIP_VARIANT_LPC17XX
//			|| pLPC->ChipVariant == CHIP_VARIANT_LPC13XX
//			|| pLPC->ChipVariant == CHIP_VARIANT_LPC11XX
//			|| pLPC->ChipVariant == CHIP_VARIANT_LPC8XX) {
//		pos = 0x1C;
//	} else {
//		//DebugPrintf(1, "Internal error: wrong chip variant %d\n", pLPC->ChipVariant);
//		return 1;
//	}
//
//	uuGetFlashBuff(0, buff, sizeof(buff));
//
//	// Calculate a native checksum of the little endian vector table:
//	for (i = 0; i < (4 * 8);) {
//		ivt_CRC += buff[i++];
//		ivt_CRC += buff[i++] << 8;
//		ivt_CRC += buff[i++] << 16;
//		ivt_CRC += buff[i++] << 24;
//	}
//
//	if (ivt_CRC == 0)
//		return 0;
//
//	ivt_CRC = 0;
//
//	for (i = 0; i < 4; i++) {
//		buff[i + pos] = 0;
//	}
//
//	for (i = 0; i < (4 * 8);) {
//		ivt_CRC += buff[i++];
//		ivt_CRC += buff[i++] << 8;
//		ivt_CRC += buff[i++] << 16;
//		ivt_CRC += buff[i++] << 24;
//	}
//	/* Negate the result and place in the vector at 0x14 as little endian
//	 * again. The resulting vector table should checksum to 0. */
//	ivt_CRC = (unsigned long) (0 - ivt_CRC);
//	//DebugPrintf(3, "Position 0x%x patched: ivt_CRC = 0x%08lX\n", pos, ivt_CRC);
//	for (i = 0; i < 4; i++) {
//		buff[i + pos] = (unsigned char) ivt_CRC;
//		ivt_CRC >>= 8;
//	}
//
//	uuPutFlashBuff(0, buff, sizeof(buff));
//    return 0;
//}


int NxpDownloadExt( void )
{
	const LPC_DEVICE_TYPE *pLPC;
	if( NxpSynchronizing( ) != 1 ) {
		return 0;
	}

	pLPC = NxpGetLPC( );
	if( pLPC == NULL ) {
		return 0;
	}

	DebugPrintf(2, "LPC%s, %d kiB FLASH / %d kiB SRAM\n",
		pLPC->Product, pLPC->FlashSize, pLPC->RAMSize);


    if( uuGetFlashLen() == 0 )
    {
        DebugPrintf(1, "Error on flash data\n" );
        return 0;
    }

//	if( NxpBinCRC( pLPC ) ){
//		return 0;
//	}

	if( NxpEras( pLPC, 0 ) ){
		return 0;
	}

//	OSTimeDly(HZ);
	if( NxpDownloadFlash( pLPC ) ){
		return 0;
	}

//	OSTimeDly(HZ);
	if( NxpGoStart( pLPC, 0 ) )
		return 0;


    DebugPrintf(1, "\rIs OK\n" );
	return TRUE;
}


