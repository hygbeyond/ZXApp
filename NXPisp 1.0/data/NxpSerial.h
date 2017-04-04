
#ifndef __NXPSERIAL_H__
#define __NXPSERIAL_H__



void SendComPortBlock(const void *s, size_t n);
void SendComPort(const char *s);
int ReceiveComPortBlockComplete( void *block, size_t size, unsigned timeout);

void RectComPort(
	char *Ans, unsigned long MaxSize,
	unsigned long *RealSize, unsigned long WantedNr0x0A,
	unsigned timeout );

int TransAndVerify(const char *Command, char *AnswerBuffer, int AnswerLength, int cnt, const char *verify);


int SendAndVerifyExt( const char *Command, char *AnswerBuffer, int AnswerLength, int cnt);
int SendAndVerify( const char *Command, char *AnswerBuffer, int AnswerLength);

void FormatCommand(const char *In, char *Out);
int GetCommand( const char *In, char *out, int size );

#endif

