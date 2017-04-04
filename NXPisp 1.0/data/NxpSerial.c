


#include "mytype.h"
#include "uart.h"
#include "MicroOS.h"
#include "OSstring.h"

static inline void Sleep( int i ) { OSTimeDly(i);};

/***************************** SendComPortBlock *************************/
/**  Sends a block of bytes out the opened com port.
\param [in] s block to send.
\param [in] n size of the block.
*/
void SendComPortBlock(const void *s, size_t n)
{
    uint8 *str = s;
    int i = n;
	for(int a=0; a<HZ/15; a++)
	{
		int ret = Uart_SendBuff(str, i);
		str += ret;
		i   -= ret;
		if( i==0 ) return;
		OSTimeDly(1);
	}
}

void SendComPort(const char *s)
{
	Uart_clean();
//	SendComPortBlock(s, strlen(s));
	while(*s){
		Uart_SendByte( *s++ );
	}
}



/***************************** ReceiveComPortBlockComplete **************/
/**  Receives a fixed block from the open com port. Returns when the
block is completely filled or the timeout period has passed
\param [out] block buffer to hold the bytes read from the serial port.
\param [in] size the size of the buffer pointed to by block.
\param [in] timeOut the maximum amount of time to wait before guvung up on
completing the read.
\return 0 if successful, non-zero otherwise.
*/
int ReceiveComPortBlockComplete( void *block, size_t size, unsigned timeout)
{
	int ret;
	int cnt=0;
	BYTE *pd = (BYTE*)block;
	while( timeout-- && size )
	{
		while( (ret=Uart_GetChar_nb()) >= 0 )
		{
			*pd++ = ret;
			size--;
			cnt++;
			if(size == 0 )
				return cnt;
		}
		Sleep(1);
	}
	return 0;
}


void RectComPort(
	char *Ans, unsigned long MaxSize,
	unsigned long *RealSize, unsigned long WantedNr0x0A,
	unsigned timeout )
{
	unsigned long nr_of_0x0A = 0;
	unsigned long nr_of_0x0D = 0;
	unsigned char *Answer;
	int size = 0;
	int lf = 0;
	int ret;
	Answer  = (unsigned char*) Ans;

	*RealSize = 0;
	do
	{
		while( (ret = Uart_GetChar_nb() ) >= 0 )
		{
			if( ret == 0x0a )
			{
				if (lf != 0)
				{
					nr_of_0x0A++;
					lf = 0;
					Answer[size++] = ret;
					if( nr_of_0x0A >= WantedNr0x0A )
						break;
				}
			}else	if( ret == 0x0d )
			{
				nr_of_0x0D++;
				lf = 1;
			}else if (lf != 0)
			{
				nr_of_0x0D++;
				nr_of_0x0A++;
				lf = 0;
			}
			Answer[ size++ ] = ret;
		}
		Sleep(1);
		timeout--;
	} while ((size < MaxSize) && timeout && (nr_of_0x0A < WantedNr0x0A) );

	/* Torsten Lang 2013-05-06 Store residual data and cut answer after expected nr. of 0x0a */
	Answer[ size ] = '\0';
	*RealSize = size;
}


/***************************** FormatCommand ********************************/
/**  2013-06-28 Torsten Lang, Uwe Schneider GmbH
According to the various NXP user manuals the ISP bootloader commands should
be terminated with <CR><LF>, the echo and/or answer should have the same line
termination. So far for the theory...
In fact, the bootloader also accepts <LF> as line termination, but it may or
may not echo the linebreak character. Some bootloaders convert the character
into <CR><LF>, some leave the <LF> and append another one (<LF><LF> in the
answer). Furthermore, during uuencoded data transfer the bootloader may or
may not append an additional <LF> character at the end of the answer
(leading to a <CR><LF><LF> sequence).
A reliable way to handle these deviations from the UM is to strictly send the
commands according to the description in the UM and to re-format commands
and answers after a transfer.
FormatCommand works in a way that it drops any leading linefeeds which only
can be surplus <LF> characters from a previous answer. It then converts any
sequence of <CR> and <LF> into a single <LF> character.
FormatCommand can work in place, meaning that In==Out is allowed!
\param [in]  In  Pointer to input buffer.
\param [out] Out Pointer to output buffer.
*/

void FormatCommand(const char *In, char *Out)
{
	size_t i, j;
	for (i = 0, j = 0; In[j] != '\0'; i++, j++)
	{
		if ((In[j] == '\r') || (In[j] == '\n'))
		{
			if (i > 0) // Ignore leading line breaks (they must be leftovers from a previous answer)
			{
				Out[i] = '\n';
			}
			else
			{
				i--;
			}
			while ((In[j+1] == '\r') || (In[j+1] == '\n'))
			{
				j++;
			}
		}
		else
		{
			Out[i] = In[j];
		}
	}
	Out[i] = '\0';
}

int GetCommand( const char *In, char *out, int size )
{
	int i,j;
	const char *pd = In;
	for(i=0; i<size-1; i++)
	{
		for(j=0; j<128; j++ )
		{
			if( *pd == 0 )
				return 0;

			if( *pd == '\n' )
			{
				pd++;
				break;
			}
			pd++;
		}
	}
	for(i=0; i<128; i++)
	{
		*out++ = *pd;
		if( *pd == '\n' )
		{
			*out = 0;
			return 1;
		}
		if( *pd == '\0' )
			break;
		pd++;
	}
	return 0;
}


int TransAndVerify(const char *Command, char *AnswerBuffer, int AnswerLength, int cnt, const char *verify)
{
	unsigned long realsize = 0;
	static char temp[128];
	static char cmd[128];

	SendComPort( Command);
	RectComPort( AnswerBuffer, AnswerLength - 1, &realsize, cnt, 500);

	if( realsize )
	{
		FormatCommand(Command, cmd);
		FormatCommand(AnswerBuffer, AnswerBuffer);
		if( GetCommand( AnswerBuffer, temp, 1) && !strcmp( temp, cmd ) )
		{
			if( verify == NULL || cnt == 1 ) return 1;
			if( GetCommand( AnswerBuffer, temp, 2) && !strcmp( temp, verify ) )
			{
				return 1;
			}
		}
	}
	return 0;
}

int SendAndVerifyExt( const char *Command, char *AnswerBuffer, int AnswerLength, int cnt)
{
	return TransAndVerify( Command, AnswerBuffer, AnswerLength, cnt, "0\n");
}


int SendAndVerify( const char *Command, char *AnswerBuffer, int AnswerLength)
{
	return TransAndVerify( Command, AnswerBuffer, AnswerLength, 2, "0\n");
}


