/***************************************************************************
 该文件提供对所有需要使用的外设初始化操作，主要包括：
1、系统时钟初始化
2、搬移部分程序到RAM中运行
3、FLASH初始化
4、为需要使用的外设提供时钟
5、所有GPIO口应用情况初始化
6、初始化定时器
****************************************************************************
注意:

  此处只包含性能部分的初始化,功能模块的初始化在性能初始化完成后执行，调试时
  首先要判断此处的初始化结果，有没有被功能的初始化意外修改
***************************************************************************/
#include "MotorDefine.h"
#include "MotorInclude.h"


/**修改FLASH寄存器的程序需要搬移到FLASH中执行**/
#pragma CODE_SECTION(InitFlash, "ramfuncs");
extern COPY_TABLE prginRAM;

void 	copy_prg(COPY_TABLE *tp);
void   	DisableDog(void);   
void 	InitPll(Uint val);
void	InitFlash(void);
void	InitPeripheralClocks(void);

void 	InitPieCtrl(void);
void 	InitPieVectTable(void);

void 	InitSetGpio(void);
extern interrupt void SCI_RXD_isr(void);
extern interrupt void SCI_TXD_isr(void);

/************************************************************
初始化DSP系统，包括初始化时钟和配置寄存器
************************************************************/
void InitSysCtrl()
{   
   DisableDog();			// Disable the watchdog

   InitPll(DSP_CLOCK/10);	// Change 20MHz clock to 100MHz or 60MHz
   
   copy_prg(&prginRAM);		// Move the program from FLASH to RAM
   InitFlash();				// Initializes the Flash Control registers
   InitPeripheralClocks();	// Initialize the peripheral clocks
}

/************************************************************
初始化中断服务程序
************************************************************/
void InitInterrupt()
{
   DINT;							//Clear all interrupts and initialize PIE vector table:
   InitPieCtrl();  					//Disable PIE
   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();				//Enable PIE

   EALLOW;  						//设置用户服务程序
   //XIntruptRegs.XNMICR.bit.POLARITY = 1;
   //XIntruptRegs.XNMICR.bit.ENABLE = 1;
   XIntruptRegs.XNMICR.all = 0x0;

   //PieVectTable.TINT0 		= &cpu_timer0_isr;		//定时中断	
   PieVectTable.ADCINT		= &ADC_Over_isr;		//ADC结束中断
   PieVectTable.EPWM1_TZINT = &EPWM1_TZ_isr;		//过流中断
   PieVectTable.EPWM1_INT 	= &EPWM1_zero_isr;		//下溢中断   
   PieVectTable.SCIRXINTA 	= &SCI_RXD_isr; 		//通讯接收中断
   PieVectTable.SCITXINTA 	= &SCI_TXD_isr; 		//通讯发送中断

   EDIS;    	
}

/************************************************************
开启已经使用的中断
************************************************************/
void SetInterruptEnable()
{
   IER |= (M_INT1 | M_INT2 | M_INT3 | M_INT5);	//  Enable interrupts:
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;           //  ADCINT
   PieCtrlRegs.PIEIER2.bit.INTx1 = 1;           // 	EPWM1_TZ
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1;           // 	EPWM1_INT
}
/************************************************************
初始化DSP的所有外设，为电机控制和接口做准备
************************************************************/
void InitPeripherals(void)
{
   	InitSetGpio();  
   	InitSetPWM();
   	InitSetAdc();


   	InitCpuTimers();
   	//ConfigCpuTimer(&CpuTimer0, DSP_CLOCK, 1000000L);//100MHz CPU, 1 millisecond Period
   	
   	//StartCpuTimer0();					 //暂时没用的定时器中断
   	StartCpuTimer1();				     //作为主程序的时间基准
}

//---------------------------------------------------------------------------
// Move program from FLASH to RAM size < 65535 words
//---------------------------------------------------------------------------
void copy_prg(COPY_TABLE *tp)
{
	Uint size;
	COPY_RECORD *crp = &tp->recs[0];

	size = (Uint)crp->size - 1;

	MovePrgFrFlashToRam(crp->src_addr, crp->dst_addr, size);
}

//---------------------------------------------------------------------------
// This function initializes the Flash Control registers
//                   CAUTION 
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results
//---------------------------------------------------------------------------
void InitFlash(void)
{
   EALLOW;
   //Enable Flash Pipeline mode to improve performance of code executed from Flash.
   //FlashRegs.FOPT.bit.ENPIPE = 1;
   FlashRegs.FOPT.all = 0x01;
   //Set the Random Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = READ_RAND_FLASH_WAITE;
   //Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = READ_PAGE_FLASH_WAITE;
   //Set the Waitstate for the OTP
   FlashRegs.FOTPWAIT.bit.OTPWAIT = READ_OTP_WAITE;
   
   //Set number of cycles to transition from sleep to standby
   //FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;       
   //Set number of cycles to transition from standby to active
   //FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;   
   EDIS;
   asm(" RPT #7 || NOP");	
}	

//---------------------------------------------------------------------------
// This function resets the watchdog timer.
//---------------------------------------------------------------------------
void KickDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}
//---------------------------------------------------------------------------
// This function disables the watchdog timer.
//---------------------------------------------------------------------------
void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}
//---------------------------------------------------------------------------
// This function Enables the watchdog timer.
//---------------------------------------------------------------------------
void EnableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x002A;
    EDIS;
}

//---------------------------------------------------------------------------
// This function initializes the PLLCR register.
//---------------------------------------------------------------------------
void InitPll(Uint16 val)
{
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)	// Make sure the PLL is not running in limp mode
   {
       if (SysCtrlRegs.PLLCR.bit.DIV != val)
       {
          EALLOW;          
          SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;	// Before setting PLLCR turn off missing clock detect
          SysCtrlRegs.PLLCR.bit.DIV = val;
          EDIS;
   
          DisableDog();   
          while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1){}
          
          EALLOW;
          SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
          EDIS;
       }
   }   
}

//--------------------------------------------------------------------------
// This function initializes the clocks to the peripheral modules.
//---------------------------------------------------------------------------
void InitPeripheralClocks(void)
{
   EALLOW;

   SysCtrlRegs.HISPCP.all = 0x0001;	//50MHz for ADC / 30MHz
   SysCtrlRegs.LOSPCP.all = 0x0002;	//25MHz for SCI and SPI / 15MHz

   //SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;	//Set ClockOut Pin = 100MHz/60MHZ
   SysCtrlRegs.XCLK.all = 0x02;

   //SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    // ADC
   //SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   // I2C

   //SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;     // SPI-A
   //SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 0;     // SPI-B
   //SysCtrlRegs.PCLKCR0.bit.SPICENCLK = 0;     // SPI-C
   //SysCtrlRegs.PCLKCR0.bit.SPIDENCLK = 0;     // SPI-D

   //SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;     // SCI-A
   //SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;     // SCI-B

   //SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;    // eCAN-A
   //SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;    // eCAN-B   

   //SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Enable TBCLK within the ePWM

   SysCtrlRegs.PCLKCR0.all = 0x051C;

   //SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;  // eCAP1
   //SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;  // eCAP2
   //SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  // eCAP3
   //SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  // eCAP4
   
   //SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;  // ePWM1
   //SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;  // ePWM2
   //SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;  // ePWM3
   //SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;  // ePWM4
   //SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;  // ePWM5
   //SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;  // ePWM6
  
   //SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;  // eQEP1
   //SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;  // eQEP2

   SysCtrlRegs.PCLKCR1.all = 0x4307;

   EDIS;
}

//--------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
void InitPieCtrl(void)
{
	Uint * m_Point1,* m_Point2;
	Uint   m_Index;

    DINT;    
    
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	// Disable the PIE
	// Clear all PIEIER registers:
	m_Point1 = (Uint *)&PieCtrlRegs.PIEIER1.all;
	m_Point2 = (Uint *)&PieCtrlRegs.PIEIFR1.all;
	for(m_Index = 0;m_Index<12;m_Index++)
	{
		*(m_Point1++) = 0;
		*(m_Point2++) = 0;
	}
}	

/************************************************************
所有误操作的中断处理
************************************************************/
int gErrorIntCnt;
interrupt void rsvd_ISR(void)      
{
	gErrorIntCnt++;				//监控是否有错误进入中断的情况
}
//--------------------------------------------------------------------------
//初始化的时候首先把所有的中断服务程序 都初始化为默认服务程序
//---------------------------------------------------------------------------
void InitPieVectTable(void)
{
	int16	i;
	PINT *pPieTable = (void *) &PieVectTable;
		
	EALLOW;	
	for(i=0; i < 128; i++)
	{
		*pPieTable++ = rsvd_ISR;	
	}
	EDIS;

	// Enable the PIE Vector Table
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;	
			
}

//---------------------------------------------------------------------------
// Set GPIO 口的复用、上拉下拉、同步
//---------------------------------------------------------------------------
void InitSetGpio(void)
{
   EALLOW;
   //GPIO口上拉选择
   GpioCtrlRegs.GPAPUD.all = 0x00000000L;	//与功能相关的8个管脚需要去掉上拉，否则会引起低电平错误。由功能自己完成
   GpioCtrlRegs.GPAPUD.all = 0x00000080;
   GpioCtrlRegs.GPBPUD.all = 0x0;			//其它管脚都上拉

   //GPIO口功能选择：设置使用的外设
   GpioCtrlRegs.GPAMUX1.all = 0x55000555L;
   //GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
   //GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B
   //GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
   //GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
   //GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
   //GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B
   //GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // Configure GPIO12 as TZ1(OC)
   //GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // Configure GPIO13 as TZ2
   //GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // Configure GPIO14 as TZ3
   //GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // Configure GPIO15 as TZ4
   GpioCtrlRegs.GPAMUX2.all = 0x052A4515L;
   //GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;    //eQEP1A
   //GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;    //eQEP1B
   //GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;    //eQEP1I

    //GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;    //eQEP2A
    //GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;    //eQEP2B
    //GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;    //eQEP2I


   //GPIO数据先初始化为需要的值
   GpioDataRegs.GPADAT.all = 0x00000540L;
   //GpioDataRegs.GPADAT.bit.GPIO6  = 0;        
   //GpioDataRegs.GPADAT.bit.GPIO8  = 0;        
   //GpioDataRegs.GPADAT.bit.GPIO10 = 1;        
   GpioDataRegs.GPBDAT.all = 0x00000000L;
   
   //GPIO口方向选择1：复位为输入
   GpioCtrlRegs.GPADIR.all = 0x00000540L;
   //GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;        //DRIVE(PWM输出允许信号)
   //GpioCtrlRegs.GPADIR.bit.GPIO8  = 1;        //制动电阻控制
   //GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;        //继电器控制	
     GpioCtrlRegs.GPBDIR.all = 0x00;
 
   //GPIO口同步选择
   GpioCtrlRegs.GPAQSEL1.all = 0x57000000L;    	// GPIO0-GPIO15 Synch to SYSCLKOUT 
   //GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 0;     	// Synch to SYSCLKOUT GPIO7 (CAP2)
   //GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 1;     	// Synch to SYSCLKOUT GPIO7 (CAP2)
   //GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 1;     	// Synch to SYSCLKOUT GPIO7 (CAP2)
   //GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 1;     	// Synch to SYSCLKOUT GPIO7 (CAP2)
   //GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 1;     	// Synch to SYSCLKOUT GPIO7 (CAP2)
   GpioCtrlRegs.GPAQSEL2.all = 0x0300803FL;    	// GPIO16-GPIO31 Synch to SYSCLKOUT
   //GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; 		// Asynch input GPIO16 (SPISIMOA)
   //GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; 		// Asynch input GPIO17 (SPISOMIA)
   //GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; 		// Asynch input GPIO18 (SPICLKA)
   //GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;    	// Synch to SYSCLKOUT GPIO24 (CAP1)
   //GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  	// Asynch input GPIO28 (SCIRXDA)
   //GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 2;
   GpioCtrlRegs.GPBQSEL1.all = 0x0000000FL;    	// GPIO32-GPIO34 Synch to SYSCLKOUT 
   //GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  	// Asynch input GPIO32 (SDAA)
   //GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  	// Asynch input GPIO33 (SCLA)

    EDIS;
}	
/************************************************************
函数输入:无
函数输出:无
调用位置:主循环之前
调用条件:无
函数功能:初始化DSP的AD采样模块
************************************************************/
void InitSetAdc(void)
{
	int waite;

    AdcRegs.ADCTRL3.all = 0x00E0;  // Power up bandgap/reference/ADC circuits

    for(waite = 0;waite<5000;waite++){}
	
	//以下开始设置ADC的控制寄存器、转换通道选择寄存器等
   	AdcRegs.ADCST.all = 0x30;				//首先清除中断标志位
	AdcRegs.ADCOFFTRIM.all = 0;
	AdcRegs.ADCTRL3.all = 0x0E2;
	//AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;
	//AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;	//设置为顺序采样模式

	AdcRegs.ADCTRL1.all = 0x0190;
	//AdcRegs.ADCTRL1.bit.CPS = 1;			//设置ADC时钟为12.5MHz = 50MHz/2/2 100
											//设置ADC时钟为7.5MHz = 30MHz/2/2  60 
	//AdcRegs.ADCTRL1.bit.ACQ_PS = 1;			//采样时间 10*ADCCLK
	//AdcRegs.ADCTRL1.bit.CONT_RUN = 0;		//设置为启停模式
	//AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;		//级联模式

	AdcRegs.ADCTRL2.all = 0x0900;
	//AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;	//允许SOCA起动ADC A
	//AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
	//AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;

	AdcRegs.ADCMAXCONV.all = 0xD;			//最大转换14通道
	AdcRegs.ADCCHSELSEQ1.all = 0x3210;
	//AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;	//IU
	//AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1;	//IV
	//AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2;	//IW
	//AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3;	//UDC

	AdcRegs.ADCCHSELSEQ2.all = 0x9874;
	//AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4;	//TEMPERATURE
	//AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 7;	//GND
	//AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 8;	//AI1
	//AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 9;	//AI2
    AdcRegs.ADCCHSELSEQ3.all = 0xDCBA;
    //AdcRegs.ADCCHSELSEQ3.bit.CONV08 = A;  //AI3
    //AdcRegs.ADCCHSELSEQ3.bit.CONV09 = B;  //PG-SinCos1
    //AdcRegs.ADCCHSELSEQ3.bit.CONV10 = C;  //PG-SinCos2
    //AdcRegs.ADCCHSELSEQ3.bit.CONV11 = D;  //PG-SinCos3
    AdcRegs.ADCCHSELSEQ4.all = 0x00FE;
    //AdcRegs.ADCCHSELSEQ4.bit.CONV12 = E;  //PG-SinCos
    //AdcRegs.ADCCHSELSEQ4.bit.CONV13 = F;  //PL_VOE_Protect
}	



//===========================================================================
// End of file.
//===========================================================================
