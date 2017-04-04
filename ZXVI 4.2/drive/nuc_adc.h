


#ifndef __NUC_ADC_H___
#define __NUC_ADC_H___



void ADCInit( void );
void ADC_Enable( uint8 channel );
void ADC_Disable( void );
int  ADC_Get( int ac );
void ADC_En(void );
int ADC_GetChannel( int ch, int ac );


int ADC_Flag( void );
int ADC_GetExt( int channel, int ac );


#endif





