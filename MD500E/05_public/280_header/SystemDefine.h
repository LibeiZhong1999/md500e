/************************************************************
    TMS2808相对于28035系统宏，以及与硬件的接口
    
************************************************************/
#ifndef MAIN_DEFINE_2808_H
#define MAIN_DEFINE_2808_H

#ifdef __cplusplus
extern "C" {
#endif
#include "DSP280x_Device.h"


/************************************************************
*************************************************************
	2808版本，性能软件版本号的维护
************************************************************/
#define SOFT_VERSION		69		// 2808version
// 0.02 update on 
// 0.04 update on 2010、4、26 (自测第二版本)
// 0.06 update on 2010、5、17 (380驱动部分提交测试版本)
// 0.08 update on 2010, 6, 21 (380驱动第一轮测试完修改后的版本)
// 0.10 update on 2010, 6, 30 (第一轮验证程序2)
// 0.16 update on 2010、8、25 (中试之后)
// 0.18 sizeD验证

// 0.20 update 2010、11、15 加入SVC 优化程序
// 0.24 update 2010、12、22 修改VF振荡抑制
                         //修改了测试问题

// 非标记录
// 60001非标，将55kw/220V 改制机型对应到132kw/380V (通用是110kw/380V)
// 60002非标，将55kw/220V 改制机型对应到132kw/380V (通用是110kw/380V)
// 60003非标，将3.7kw/220V改制机型对应到7.5kw/380V (通用是5.5kw/380V) 2010/11/11


/************************************************************
************************************************************/

// // 定义DSP芯片和时钟
//#define    TMS320F2801		1		                        //2801芯片
//#define    TMS320F2802		2		                        //2802芯片
#define      TMS320F2808		8		                 //2808芯片
//#define      TMS320F28035       3                             //28035芯片

#define    	 DSP_CLOCK100			100		            //100MHz时钟 
//#define    DSP_CLOCK60			60		                  //60MHz时钟 

#ifdef		DSP_CLOCK100
	#define    	DSP_CLOCK			100		            //100MHz时钟
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           100                 //PWM模块时钟周期 
    #define     DBTIME_1140V         50                      //1140V死区时间
    #define     DCTIME_1140V         20                      //1140V死区补偿
#endif    
#ifdef      DSP_CLOCK60
	#define    	DSP_CLOCK			60		            //60MHz时钟 
	#define		PWM_CLK_DIV			0
	#define     PWM_CLOCK           60                  //PWM模块时钟周期   
    #define     DBTIME_1140V         30                     //1140V死区时间
    #define     DCTIME_1140V         12                     //1140V死区补偿
#endif

#define  PWM_CLOCK_HALF      (PWM_CLOCK/2)              //PWM时钟除以2
#define  C_TIME_05MS         (DSP_CLOCK*500L)	        //0.5ms对应的定时器1计数值
#define     C_TIME_045MS        (DSP_CLOCK*450L)            //0.45ms对应的定时器1计数值

#define  READ_RAND_FLASH_WAITE		3		            //Flash中运行的等待时间
#define  READ_PAGE_FLASH_WAITE		3		            //
#define  READ_OTP_WAITE				8		            //OTP中读数据的等待时间

// // 与芯片相关的寄存器定义
#define	ADC_IV				AdcRegs.ADCRESULT0          // 这个地方是由于硬件驱动板到DSP版时接口反掉了
#define	ADC_IU				AdcRegs.ADCRESULT1
#define ADC_IW              AdcRegs.ADCRESULT2
#define	ADC_UDC				AdcRegs.ADCRESULT3

#define	ADC_TEMP			AdcRegs.ADCRESULT4
#define ADC_GND             AdcRegs.ADCRESULT5
#define ADC_VREFLO          AdcRegs.ADCRESULT5
#define ADC_AI1             AdcRegs.ADCRESULT6
#define	ADC_AI2				AdcRegs.ADCRESULT7

#define ADC_AI3             AdcRegs.ADCRESULT8
#define	ADC_UU4             AdcRegs.ADCRESULT9          //PG-SinCos/UVW共用端子
#define ADC_UU5             AdcRegs.ADCRESULT10         //PG-SinCos/UVW共用端子
#define ADC_UU6             AdcRegs.ADCRESULT11         //PG-SinCos/UVW共用端子
#define ADC_UU7             AdcRegs.ADCRESULT12         //PG-SinCos用端子
#define	PL_VOE_PROTECT		AdcRegs.ADCRESULT13

#define UVW_PG_U            AdcRegs.ADCRESULT9          //PG-SinCos/UVW共用端子
#define UVW_PG_V            AdcRegs.ADCRESULT10         //PG-SinCos/UVW共用端子
#define UVW_PG_W            AdcRegs.ADCRESULT11         //PG-SinCos/UVW共用端子

#define  ADC_VOLTAGE_10         (10L*65535/30)          // AD输入1.0V对应的采样值 2808AD范围是0-3V
#define  ADC_VOLTAGE_20         (20L*65535/30)          // AD输入2.0V对应的采样值
#define  ADC_VOLTAGE_25         (25L*65535/30)          // AD输入2.5V对应的采样值

#define  PL_INPUT_HIGH          ((PL_VOE_PROTECT > ADC_VOLTAGE_10) \
                                 && (PL_VOE_PROTECT < ADC_VOLTAGE_20))  //输入缺相信号电平判断

// 与芯片相关的寄存器定义
#define PIE_VECTTABLE_ADCINT    PieVectTable.ADCINT                         //ADC中断向量

#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1          //清除ADC模块的中断标志
#define ADC_RESET_SEQUENCE   	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1            //Reset the ADC Sequebce   
#define ADC_START_CONVERSION   	AdcRegs.ADCTRL2.all = 0x2000;		        //软件启动AD
#define ADC_END_CONVERSIN       AdcRegs.ADCST.bit.INT_SEQ1                  //AD转换完成标志位

#define DisConnectRelay()   	GpioDataRegs.GPASET.bit.GPIO10   = 1        //上电缓冲继电器控制
#define ConnectRelay()    	    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1	

#define EnableDrive()    	    GpioDataRegs.GPACLEAR.bit.GPIO6  = 1						  
#define DisableDrive()     	GpioDataRegs.GPASET.bit.GPIO6  = 1	        //PWM输出允许控制
#define GetOverUdcFlag()	    (PL_VOE_PROTECT < ADC_VOLTAGE_10)           //AD采样为低时，为1，表示硬件过压保护动作

#define RT_SAMPLE_START (GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1)              // 旋变信号开始采样
#define RT_SAMPLE_END   (GpioDataRegs.GPBSET.bit.GPIO34 = 1)                // 旋变信号采样完成

#ifdef __cplusplus
}
#endif /* extern "C" */
#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/
