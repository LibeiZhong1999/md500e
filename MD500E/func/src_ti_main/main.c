/****************************************************************
文件说明：基于TMS320F280X DSP的电机控制软件
          深圳市汇川技术股份有限公司
文件版本：V1.0dd
最新更新：2007.04.06
****************************************************************/
#include "MotorInclude.h"

extern void ADCOverInterrupt(void);
extern void PWMZeroInterrupt(void);
extern void HardWareErrorDeal(void);
extern void Main2msMotor(void);
extern void Main2msFunction(void);
extern void Main05msMotor(void);
extern void Main05msFunction(void);            
extern void Main0msMotor(void);
extern void Main0msFunction(void);
/***************************************************************
-------------------------主程序部分-----------------------------
****************************************************************/
void main(void)
{					
	Ulong m_BaseTime,m_DetaTime;
	Uint  m_LoopFlag;
   
   	InitSysCtrl();						// Step 1. Initialize System Control
   
   	InitInterrupt();					// Step 2. Initialize Interrupt service program:
   
   	InitPeripherals(); 					// Step 3. Initialize all the Device Peripherals:
   
   	InitForMotorApp();					// Step 4. User specific code
   	InitForFunctionApp();

	EnableDog();
	SetInterruptEnable();				// Step 5. enable interrupts:
   	EINT;   							    
   	ERTM;   							    

	m_LoopFlag = 0;
	m_BaseTime = GetTime();
  	while(1)							// Step 6. User Application function:
   	{
		m_DetaTime = m_BaseTime - GetTime();
		if(m_DetaTime >= C_TIME_05MS)	//判断0.5MS周期
        {
			m_LoopFlag ++;			
			m_BaseTime -= C_TIME_05MS;

 			KickDog();
            VCGetFeedBackSpeed(); //编码器测速，确保间隔均匀
            if(ASYNC_SVC == gCtrMotorType)
            {
                SVCCalRotorSpeed();
            }
            gCpuTime.Function05MsBase = GetTime();
            Main05msFunction();     			
            gCpuTime.Function05Ms = gCpuTime.Function05MsBase - GetTime();

            Main05msMotor();
                    
			if((m_LoopFlag & 0x03) == 0)  
			{
				gCpuTime.Motor2MsBase = GetTime();
				Main2msMotor();				//主程序的双2ms循环－用于电机控制程序
				gCpuTime.Motor2Ms = gCpuTime.Motor2MsBase - GetTime();
											//减速中所有功能打开的时候约145us
			}
			if((m_LoopFlag & 0x03) == 2)
			{
				gCpuTime.Function2MsBase = GetTime();
				Main2msFunction();			//主程序的单2ms循环－用于功能部分程序
				gCpuTime.Function2Ms = gCpuTime.Function2MsBase - GetTime();
			}								//不被中断打断情况下执行时间约150us
        }

		Main0msFunction();				//不等待循环－执行功能部分程序
		Main0msMotor();					//不等待循环－执行电机控制部分程序		
   	}
} 


/***************************************************************
-----------------------中断程序部分-----------------------------
****************************************************************/

/***************************************************************
	1ms的定时器0中断－暂时没有使用
****************************************************************/
#if 0
interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;	// Acknowledge this interrupt
}
#endif
/***************************************************************
	EPWM的周期中断，约30us
****************************************************************/
interrupt void ADC_Over_isr(void)
{
    EALLOW;             //28035改为EALLOW保护
    ADC_CLEAR_INT_FLAG;
    EDIS;
	EINT;
	gCpuTime.ADCIntBase = GetTime();
	ADCOverInterrupt();					
	gCpuTime.ADCInt = gCpuTime.ADCIntBase - GetTime();
	DINT;
    EALLOW;
    ADC_RESET_SEQUENCE;
    EDIS;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;	// Acknowledge this interrupt
}

/***************************************************************
	EPWM的下溢中断，约18us
****************************************************************/
interrupt void EPWM1_zero_isr(void)
{
   	EALLOW;
   	EPwm1Regs.ETCLR.bit.INT = 1;
   	EDIS;

	EINT;
	gCpuTime.PWMIntBase = GetTime();
	PWMZeroInterrupt();						//下溢中断－电机控制模块处理
	gCpuTime.PWMInt = gCpuTime.PWMIntBase - GetTime();
	DINT;

   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge this interrupt
}

/***************************************************************
	EPWM的过流中断，对硬件过流信号处理
****************************************************************/
interrupt void EPWM1_TZ_isr(void)
{
	DisableDrive();								//首先封锁输出
	HardWareErrorDeal();					//处理硬件故障－电机控制模块处理

   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	// Acknowledge this interrupt
	
}

/***************************************************************
----------------------------END---------------------------------
****************************************************************/
