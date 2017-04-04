


#include "hardware.h"
#include "nuc_adc.h"
#include "OSstring.h"




uint32_t  ulADCBuf;
int     ADCsize;
int     ADCFlag;
int     ADCchannel;


uint32_t lADCBuff[8];

#define  ADC_TIMER32               12000000
#define  ACHZ_TIMER                ( ADC_TIMER32 / 50 )       /* 20mS */
#define  ACHZ_CNT                  64
#define  ACHZ_GETCNT               ( ACHZ_CNT * 1 )

#define  ACHZ_MR0                  ((ACHZ_TIMER ) / (ACHZ_CNT))

/*********************************************************************************************************
 ** Function name:       ADCInit
 ** Descriptions:        ADC初始化
 ** input parameters:    无
 ** output parameters:   无
 ** Returned value:      无
 *********************************************************************************************************/
void ADCInit( void )
{
    ADC->ADCR = (ADC_ADCR_ADMD_SINGLE_CYCLE 
            | ADC_ADCR_DIFFEN_SINGLE_END 
            | ADC_ADCR_ADEN_CONVERTER_ENABLE  /*使能AD*/
            );

    ADC->ADCHER =  ADC_ADCHER_PRESEL_INT_BANDGAP;

    //NVIC_EnableIRQ(ADC_IRQn); 
    //NVIC_SetPriority(ADC_IRQn, 2);
}



/*********************************************************************************************************
 * Function Name:        ADC_IRQHandler
 * Description:          ADC中断处理函数
 * Input:                无
 * Output:               无
 * Return:               无
 *********************************************************************************************************/
__AFTER__ void ADC_IRQHandler(void);
void ADC_IRQHandler(void)
{
    for(int i=0; i<8; i++)
    {
        uint32 tmp = (ADC->ADDR[i] & ADC_ADDR_RSLT_Msk) >> ADC_ADDR_RSLT_Pos;        
//        tmp >>= 2;
        lADCBuff[i] += tmp * tmp;
    }
    ADCsize--;
    if( ADCsize == 0 )
    {
        ADC_Disable();
    }

    ADC->ADSR = ADC_ADSR_ADF_Msk;      /* clear the A/D conversion flag */
    //NBI_P( B, 5 );
}


__AFTER__ void TMR0_IRQHandler(void);
void TMR0_IRQHandler(void)
{
    if(TIMER_GetCaptureIntFlag(TIMER0) == 1)
    {
        TIMER_ClearCaptureIntFlag(TIMER0);
        //NBI_P( B, 5 );
    }

    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);

        //NBI_P( B, 4 );
        ADC->ADCR |= ADC_ADCR_ADST_Msk;
    }
}

/*
 * 采用定时器32B0, 200uS触发一次。50HZ交流信号，每个周期采集100次
 */
void ADC_Enable( uint8 channel )
{
    ADCsize  = ACHZ_GETCNT;
    ADCFlag  = 0;
    ulADCBuf = 0;

    ADCchannel = channel & 7;
 
    memset( lADCBuff, 0, sizeof( lADCBuff ));

    NVIC_EnableIRQ(ADC_IRQn );
    NVIC_EnableIRQ(TMR0_IRQn );

    ADC->ADCHER =
    		ADC_ADCHER_PRESEL_EXT_INPUT_SIGNAL //通道7 模拟输入
    		| 0xFF;                            //打开所有通道
    ADC->ADCR   =  ADC_ADCR_ADEN_CONVERTER_ENABLE //AD使能
        | ADC_ADCR_ADIE_Msk  //转换中断打开
        | ADC_ADCR_ADMD_SINGLE_CYCLE;//单周期转换
    /* Enable analog input channel 2 for differential input channel pair 1 */
    //ADC->ADCHER |= ((ADC->ADCHER & ~ADC_ADCHER_CHEN_Msk) | (1 << ADCchannel ));

    /* Clear the A/D interrupt flag for safe */
    ADC->ADSR = ADC_ADSR_ADF_Msk;

    /* Enable Timer0 event counter input and external capture function */
    TIMER0->TCMPR   = ACHZ_MR0;
    TIMER0->TCSR    = TIMER_TCSR_CEN_Msk | TIMER_TCSR_IE_Msk  |  TIMER_PERIODIC_MODE;
    while(!(TIMER0->TCSR & TIMER_TCSR_CACT_Msk));
   
}

void ADC_Disable( void )
{
    /* Disable the ADC interrupt */
    ADC->ADCR = (ADC_ADCR_ADMD_SINGLE_CYCLE 
            | ADC_ADCR_DIFFEN_SINGLE_END 
            | ADC_ADCR_ADEN_CONVERTER_ENABLE  /*使能AD*/
            );
    TIMER0->TCSR  = 0;
    NVIC_DisableIRQ( ADC_IRQn );
    NVIC_DisableIRQ( TMR0_IRQn );
    ADCFlag  = 1;                             /* 置标志位                     */
}

static uint16 sqrt_fixed(uint32 a)
{
    uint32 i,c;
    uint32 b = 0;
    if(a == 0) return 0;
    for(i=0x40000000; i!=0; i>>=2)
    {
        c = i+b;
        b >>= 1;
        if(c <= a)
        {
            a -= c;
            b += i;
        }
    }
    return ((uint16)b);
}



int ADC_Flag( void )
{
    int ret = ADCFlag;
    ADCFlag = 0;
    return ret ? 0 : -1;
}


int ADC_GetExt( int channel, int ac )
{
    int ret ;
    ret = ac? ACHZ_GETCNT/2: ACHZ_GETCNT;
    ret = sqrt_fixed(   lADCBuff[channel & 0x07] / ret );
    return ret;
}


int ADC_Get( int ac )
{
    int ret ;
    if( !ADCFlag )
        return -1;
    ADCFlag = 0;
    ret = ac? ACHZ_GETCNT/2: ACHZ_GETCNT;

    ret = sqrt_fixed( lADCBuff[ADCchannel & 0x07] / ret );
    return ret;
}






