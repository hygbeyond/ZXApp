

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include "bsp.h"
#include "MicroOS.h"

#define AD_IN          PBI_P( 3, 1 )
#define ERROR_IN       PBI_P( 3, 3 )

#define ADO_H          SBI_P( 1, 2 )
#define ADO_L          CBI_P( 1, 2 )
#define ADO_NOT        NBI_P( 1, 2 )

#define CLK_H          SBI_P( 3, 0 )
#define CLK_L          CBI_P( 3, 0 )


#define LEn_H          SBI_P(1, 0 )
#define LEn_L          CBI_P(1, 0 )




void IO_Init(void);
uint32 Key_Read( );
void KeyEnd(int time);

#endif
