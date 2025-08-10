/****************************************************************
�ļ�˵��������TMS320F280X DSP�ĵ���������
          �����л㴨�����ɷ����޹�˾
�ļ��汾��V1.0dd
���¸��£�2007.04.06
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
-------------------------�����򲿷�-----------------------------
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
		if(m_DetaTime >= C_TIME_05MS)	//�ж�0.5MS����
        {
			m_LoopFlag ++;			
			m_BaseTime -= C_TIME_05MS;

 			KickDog();
            VCGetFeedBackSpeed(); //���������٣�ȷ���������
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
				Main2msMotor();				//�������˫2msѭ�������ڵ�����Ƴ���
				gCpuTime.Motor2Ms = gCpuTime.Motor2MsBase - GetTime();
											//���������й��ܴ򿪵�ʱ��Լ145us
			}
			if((m_LoopFlag & 0x03) == 2)
			{
				gCpuTime.Function2MsBase = GetTime();
				Main2msFunction();			//������ĵ�2msѭ�������ڹ��ܲ��ֳ���
				gCpuTime.Function2Ms = gCpuTime.Function2MsBase - GetTime();
			}								//�����жϴ�������ִ��ʱ��Լ150us
        }

		Main0msFunction();				//���ȴ�ѭ����ִ�й��ܲ��ֳ���
		Main0msMotor();					//���ȴ�ѭ����ִ�е�����Ʋ��ֳ���		
   	}
} 


/***************************************************************
-----------------------�жϳ��򲿷�-----------------------------
****************************************************************/

/***************************************************************
	1ms�Ķ�ʱ��0�жϣ���ʱû��ʹ��
****************************************************************/
#if 0
interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;	// Acknowledge this interrupt
}
#endif
/***************************************************************
	EPWM�������жϣ�Լ30us
****************************************************************/
interrupt void ADC_Over_isr(void)
{
    EALLOW;             //28035��ΪEALLOW����
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
	EPWM�������жϣ�Լ18us
****************************************************************/
interrupt void EPWM1_zero_isr(void)
{
   	EALLOW;
   	EPwm1Regs.ETCLR.bit.INT = 1;
   	EDIS;

	EINT;
	gCpuTime.PWMIntBase = GetTime();
	PWMZeroInterrupt();						//�����жϣ��������ģ�鴦��
	gCpuTime.PWMInt = gCpuTime.PWMIntBase - GetTime();
	DINT;

   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge this interrupt
}

/***************************************************************
	EPWM�Ĺ����жϣ���Ӳ�������źŴ���
****************************************************************/
interrupt void EPWM1_TZ_isr(void)
{
	DisableDrive();								//���ȷ������
	HardWareErrorDeal();					//����Ӳ�����ϣ��������ģ�鴦��

   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	// Acknowledge this interrupt
	
}

/***************************************************************
----------------------------END---------------------------------
****************************************************************/
