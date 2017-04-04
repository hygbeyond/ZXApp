

#include "hardware.h"
#include "MicroOS.h"
#include "app_cfg.h"


void IO_Init(void)
{
    LPC_IOCON->R_PIO1_0 = 1;      /* gpio */
    LPC_IOCON->R_PIO1_1 = 1;      /* gpio */
    LPC_IOCON->R_PIO1_2 = 1;      /* gpio */

    LPC_IOCON->PIO1_10 = 0x10;     /* gpio */
    LPC_IOCON->PIO2_11 = 0x10;     /* gpio */
    LPC_IOCON->PIO0_9  = 0x10;     /* gpio */
    LPC_IOCON->SWCLK_PIO0_10 = 0x11; /* gpio */
    LPC_IOCON->SWDIO_PIO1_3 = 1;  /* IO */

    LPC_GPIO0->DIR = BIT(9)|BIT(10);
    LPC_GPIO1->DIR = BIT(0)|BIT(2)|BIT(10);

    LPC_GPIO2->DIR = BIT(11) | 0x1F;
    LPC_GPIO2->DATA = 0x0000;

    LPC_GPIO3->DIR = BIT(0);
}



#include "main_mac.h"
typedef struct{
    uint8 in;
    uint8 key;
}TKey;

static
const TKey keyTo[]={
    {8   ,KEY_1},
    {4   ,KEY_2},
    {2   ,KEY_3},
    {24  ,KEY_4},
    {20  ,KEY_5},
    {18  ,KEY_6},
    {40  ,KEY_7},
    {36  ,KEY_8},
    {34  ,KEY_9},
    {56  ,KEY_0},

    {65  ,KEY_OK},
    {49  ,KEY_CANCEL},
    {50  ,KEY_BACK},
    {52  ,KEY_UP},
    {68  ,KEY_DOWN},

    {72  ,KEY_F5},
    {66  ,KEY_F4},
    {1   ,KEY_F1},
    {17  ,KEY_F2},
    {33  ,KEY_F3},
};


#define PKEY_IN		(1<<5)
#define PKEY_0		(1<<0)
#define PKEY_1		(1<<1)
#define PKEY_2		(1<<2)
#define PKEY_3		(1<<3)
#define PKEY_4		(1<<4)
#define PKEY_OUT	(PKEY_0|PKEY_1|PKEY_2|PKEY_3|PKEY_4)
#define PKEY_PORT   2

#define INPORT         PIN
uint32 Key_Read( )
{
	uint32 ret;
	uint32 tmp;
	uint32 i;

	PORT(2) &= ~0x1F;
	OSTimeDly(1);
	ret  = ((~PORT(2))>>5) & 0xF;
	tmp = BIT(0);
	if (ret) {
		for (i = 0; i < 5; i++) {
			PORT(2) |= 0x1F;
			PORT(2) &= ~tmp;
			OSTimeDly(1);
			ret  = ((~PORT(2))>>5) & 0xF;
			if( ret ){
				ret += i << 4;

				//pr_debug("key %d\n", ret );
	            for(int j=0; j<ARRAY_SIZE(keyTo); j++)
	            {
	                if( keyTo[j].in == ret )
	                {
	                	return keyTo[j].key;
	                }
	            }
				return 0;
				//return ret;
			}
			tmp <<= 1;
		}
	}
	return 0;
}


