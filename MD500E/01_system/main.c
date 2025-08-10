/****************************************************************
�ļ�˵��������TMS320F280X DSP�ĵ���������
          �����л㴨�����ɷ����޹�˾
�ļ��汾�� 
���¸��£� 
****************************************************************/
#include "MotorInclude.h"

extern void ADCOverInterrupt(void);
extern void PWMZeroInterrupt(void);
extern void HardWareErrorDeal(void);

extern void Main2msMotorA(void);
extern void Main2msMotorB(void);
extern void Main2msMotorC(void);
extern void Main2msMotorD(void);

extern void Main05msMotor(void);     
//extern void Main0msMotor(void);

//extern void Main2msFunction(void);
//extern void Main05msFunction(void);     
extern void Main05msFunctionA(void);
extern void Main05msFunctionB(void);
extern void Main05msFunctionC(void);
extern void Main05msFunctionD(void);
extern void Main0msFunction(void);

Ulong m_BaseTime,m_DetaTime;
Uint  m_LoopFlag;
/***************************************************************
-------------------------�����򲿷�-----------------------------
****************************************************************/
void main(void)
{					
//	Ulong m_BaseTime,m_DetaTime;
//	Uint  m_LoopFlag;
   
   	InitSysCtrl();						// Step 1. Initialize System Control
   
   	InitInterrupt();					// Step 2. Initialize Interrupt service program:
   
   	InitPeripherals(); 					// Step 3. Initialize all the Device Peripherals:
   
   	InitForMotorApp();					// Step 4. User specific code
   	InitForFunctionApp();

	EnableDog();                  // d
	SetInterruptEnable();				// Step 5. enable interrupts:
   	EINT;   							    
   	ERTM;   							    

	m_LoopFlag = 2;
	m_BaseTime = GetTime();
    
  	while(1)							// Step 6. User Application function:
   	{
		m_DetaTime = m_BaseTime - GetTime();		
		if(m_DetaTime >= C_TIME_05MS)	//�ж�0.5MS����
        {            
			m_LoopFlag ++;			
			m_BaseTime -= C_TIME_05MS;
 			KickDog();

            Main05msMotor();                        // ����0.5ms����          
                    
			if((m_LoopFlag & 0x03) == 0)            // prA
			{         
                Main05msFunctionA();
                Main05msFunctionB();					
			}
            else if((m_LoopFlag & 0x03) == 1)       // prB
            {                                
                //Main05msFunctionB();
                Main05msFunctionC();
                //Main05msFunctionD();	
            }
			else if((m_LoopFlag & 0x03) == 2)       // prC
			{    

                Main05msFunctionD();
                Main2msMotorA(); 

			}
            else if((m_LoopFlag & 0x03) == 3)       // prD
            {             
                Main2msMotorB(); 
                Main2msMotorC(); 
				Main2msMotorD(); 	
            }

            // ����cpuæµϵ��1
           // gCpuTime.Det05msClk = __IQsat(m_DetaTime, 65535, C_TIME_05MS);
        }

		Main0msFunction();				//���ȴ�ѭ����ִ�й��ܲ��ֳ���
//		Main0msMotor();					//���ȴ�ѭ����ִ�е�����Ʋ��ֳ���
   	}
} 


/***************************************************************
-----------------------�жϳ��򲿷�-----------------------------
****************************************************************/
/***************************************************************
	EPWM�������жϣ�Լ30us
****************************************************************/
interrupt void ADC_Over_isr(void)
{
    EALLOW;             //28035��ΪEALLOW����
    ADC_CLEAR_INT_FLAG;
    EDIS;
	EINT;
	//gCpuTime.ADCIntBase = GetTime();
	//ADCOverInterrupt();
	gMainCmd.pADCIsr();                         /*�޸�Ϊ����ָ�뷽ʽִ��ADC�����жϳ���Ĭ��ADCEndIsr()*/					
//	gCpuTime.ADCInt = gCpuTime.ADCIntBase - GetTime();
	DINT;
    EALLOW;
    ADC_RESET_SEQUENCE;
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;	// Acknowledge this interrupt
    EDIS;
}
/***************************************************************
	EPWM�Ĺ����жϣ���Ӳ�������źŴ���
****************************************************************/
interrupt void EPWM1_TZ_isr(void)
{
	DisableDrive();								//���ȷ������
	HardWareErrorDeal();					    //����Ӳ�����ϣ��������ģ�鴦��
                // 
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	    // Acknowledge this interrupt
}

/***************************************************************
	CBC������TZ�жϣ��ж���ǿ�ƹر�����ʹ���ź�
****************************************************************/
interrupt void EPWM2_TZ_isr(void)
{               
	DisableDrive();								//���ȷ������,�����ж��п���
    gCBCProtect.CBCIntFlag = 1;  
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	    // Acknowledge this interrupt
}

/*----------------------------END---------------------------------
****************************************************************/
