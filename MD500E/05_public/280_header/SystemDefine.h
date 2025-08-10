/************************************************************
    TMS2808�����28035ϵͳ�꣬�Լ���Ӳ���Ľӿ�
    
************************************************************/
#ifndef MAIN_DEFINE_2808_H
#define MAIN_DEFINE_2808_H

#ifdef __cplusplus
extern "C" {
#endif
#include "DSP280x_Device.h"


/************************************************************
*************************************************************
	2808�汾����������汾�ŵ�ά��
************************************************************/
#define SOFT_VERSION		69		// 2808version
// 0.02 update on 
// 0.04 update on 2010��4��26 (�Բ�ڶ��汾)
// 0.06 update on 2010��5��17 (380���������ύ���԰汾)
// 0.08 update on 2010, 6, 21 (380������һ�ֲ������޸ĺ�İ汾)
// 0.10 update on 2010, 6, 30 (��һ����֤����2)
// 0.16 update on 2010��8��25 (����֮��)
// 0.18 sizeD��֤

// 0.20 update 2010��11��15 ����SVC �Ż�����
// 0.24 update 2010��12��22 �޸�VF������
                         //�޸��˲�������

// �Ǳ��¼
// 60001�Ǳ꣬��55kw/220V ���ƻ��Ͷ�Ӧ��132kw/380V (ͨ����110kw/380V)
// 60002�Ǳ꣬��55kw/220V ���ƻ��Ͷ�Ӧ��132kw/380V (ͨ����110kw/380V)
// 60003�Ǳ꣬��3.7kw/220V���ƻ��Ͷ�Ӧ��7.5kw/380V (ͨ����5.5kw/380V) 2010/11/11


/************************************************************
************************************************************/

// // ����DSPоƬ��ʱ��
//#define    TMS320F2801		1		                        //2801оƬ
//#define    TMS320F2802		2		                        //2802оƬ
#define      TMS320F2808		8		                 //2808оƬ
//#define      TMS320F28035       3                             //28035оƬ

#define    	 DSP_CLOCK100			100		            //100MHzʱ�� 
//#define    DSP_CLOCK60			60		                  //60MHzʱ�� 

#ifdef		DSP_CLOCK100
	#define    	DSP_CLOCK			100		            //100MHzʱ��
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           100                 //PWMģ��ʱ������ 
    #define     DBTIME_1140V         50                      //1140V����ʱ��
    #define     DCTIME_1140V         20                      //1140V��������
#endif    
#ifdef      DSP_CLOCK60
	#define    	DSP_CLOCK			60		            //60MHzʱ�� 
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           60                  //PWMģ��ʱ������   
    #define     DBTIME_1140V         30                     //1140V����ʱ��
    #define     DCTIME_1140V         12                     //1140V��������
#endif

#define  PWM_CLOCK_HALF      (PWM_CLOCK/2)              //PWMʱ�ӳ���2
#define  C_TIME_05MS         (DSP_CLOCK*500L)	        //0.5ms��Ӧ�Ķ�ʱ��1����ֵ
#define     C_TIME_045MS        (DSP_CLOCK*450L)            //0.45ms��Ӧ�Ķ�ʱ��1����ֵ

#define  READ_RAND_FLASH_WAITE		3		            //Flash�����еĵȴ�ʱ��
#define  READ_PAGE_FLASH_WAITE		3		            //
#define  READ_OTP_WAITE				8		            //OTP�ж����ݵĵȴ�ʱ��

// // ��оƬ��صļĴ�������
#define	ADC_IV				AdcRegs.ADCRESULT0          // ����ط�������Ӳ�������嵽DSP��ʱ�ӿڷ�����
#define	ADC_IU				AdcRegs.ADCRESULT1
#define ADC_IW              AdcRegs.ADCRESULT2
#define	ADC_UDC				AdcRegs.ADCRESULT3

#define	ADC_TEMP			AdcRegs.ADCRESULT4
#define ADC_GND             AdcRegs.ADCRESULT5
#define ADC_VREFLO          AdcRegs.ADCRESULT5
#define ADC_AI1             AdcRegs.ADCRESULT6
#define	ADC_AI2				AdcRegs.ADCRESULT7

#define ADC_AI3             AdcRegs.ADCRESULT8
#define	ADC_UU4             AdcRegs.ADCRESULT9          //PG-SinCos/UVW���ö���
#define ADC_UU5             AdcRegs.ADCRESULT10         //PG-SinCos/UVW���ö���
#define ADC_UU6             AdcRegs.ADCRESULT11         //PG-SinCos/UVW���ö���
#define ADC_UU7             AdcRegs.ADCRESULT12         //PG-SinCos�ö���
#define	PL_VOE_PROTECT		AdcRegs.ADCRESULT13

#define UVW_PG_U            AdcRegs.ADCRESULT9          //PG-SinCos/UVW���ö���
#define UVW_PG_V            AdcRegs.ADCRESULT10         //PG-SinCos/UVW���ö���
#define UVW_PG_W            AdcRegs.ADCRESULT11         //PG-SinCos/UVW���ö���

#define  ADC_VOLTAGE_10         (10L*65535/30)          // AD����1.0V��Ӧ�Ĳ���ֵ 2808AD��Χ��0-3V
#define  ADC_VOLTAGE_20         (20L*65535/30)          // AD����2.0V��Ӧ�Ĳ���ֵ
#define  ADC_VOLTAGE_25         (25L*65535/30)          // AD����2.5V��Ӧ�Ĳ���ֵ

#define  PL_INPUT_HIGH          ((PL_VOE_PROTECT > ADC_VOLTAGE_10) \
                                 && (PL_VOE_PROTECT < ADC_VOLTAGE_20))  //����ȱ���źŵ�ƽ�ж�

// ��оƬ��صļĴ�������
#define PIE_VECTTABLE_ADCINT    PieVectTable.ADCINT                         //ADC�ж�����

#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1          //���ADCģ����жϱ�־
#define ADC_RESET_SEQUENCE   	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1            //Reset the ADC Sequebce   
#define ADC_START_CONVERSION   	AdcRegs.ADCTRL2.all = 0x2000;		        //�������AD
#define ADC_END_CONVERSIN       AdcRegs.ADCST.bit.INT_SEQ1                  //ADת����ɱ�־λ

#define DisConnectRelay()   	GpioDataRegs.GPASET.bit.GPIO10   = 1        //�ϵ绺��̵�������
#define ConnectRelay()    	    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1	

#define EnableDrive()    	    GpioDataRegs.GPACLEAR.bit.GPIO6  = 1						  
#define DisableDrive()     	GpioDataRegs.GPASET.bit.GPIO6  = 1	        //PWM����������
#define GetOverUdcFlag()	    (PL_VOE_PROTECT < ADC_VOLTAGE_10)           //AD����Ϊ��ʱ��Ϊ1����ʾӲ����ѹ��������

#define RT_SAMPLE_START (GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1)              // �����źſ�ʼ����
#define RT_SAMPLE_END   (GpioDataRegs.GPBSET.bit.GPIO34 = 1)                // �����źŲ������

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/
