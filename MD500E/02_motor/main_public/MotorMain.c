/****************************************************************
�ļ����ܣ��͵��������صĳ����ļ����������ģ������岿��
�ļ��汾�� 
�������ڣ� 

****************************************************************/
#include "MotorInclude.h"
#include "MotorDataExchange.h"
#include "SystemDefine.h"
#include "MotorDefine.h"
#include "MotorIPMSVC.h"

// // �ڲ���������
void RunCaseDeal05Ms(void);
void RunCaseDeal2Ms(void);
void RunCaseRun05Ms(void);
void RunCaseRun2Ms(void);
void RunCaseStop(void);
void SendDataPrepare(void);	
void PWMPeriodIsr(void);
void PWMZeroIsr(void);
#if (SOFTSERIES == MD500SOFT)
void GetADCAdjustInfo(void);
#endif
extern s32 PmsmFwcAdjMethod(void);
//extern s32 PmsmFwcMixMethod(void);
Uint gAI1,gAI2,gAI3;
#if(AIRCOMPRESSOR == 1)
Uint gAI4,gAI5;
#endif
//VFFILTER1ST_STRUCT gVoltFilt;
extern BEFORE_RUN_PHASE_LOSE_STRUCT gBforeRunPhaseLose;
/************************************************************
    ����ģ���ʼ��������ѭ��ǰ��ʼ�����ܲ��ֵı���
(���е���0�ı��������ʼ��)
************************************************************/
void InitForMotorApp(void)
{
	DisableDrive();
	TurnOffFan();
	ResetADCEndIsr();                                   //����ADC�����жϵ�Ĭ�ϴ������
	//ResetPGType();
    
// ����������ʼ��
	gMainStatus.RunStep = STATUS_LOW_POWER;	//������
	gMainStatus.SubStep = 1;				//������
	gMainStatus.ParaCalTimes = 0;
	gError.LastErrorCode = gError.ErrorCode.all;
	//gMainStatus.StatusWord.all = 0;
	gCBCProtect.EnableFlag = 1;				//Ĭ����������������
	gADC.ZeroCnt = 0;
	//gADC.DelaySet = 100;
	gADC.DelayApply = 600;
	gFcCal.FcBak = 50;
	gBasePar.FcSetApply = 50;
//	gVoltFilt.InputLast = 0;
//	gVoltFilt.OutLast = 0;
//	gVoltFilt.Total = 0;
//	gVoltFilt.FiltimeVsSamptime = 512;
#if (SOFTSERIES == MD500SOFT)
	gOverLoad.BreakTotal = 0;        //�ƶ���������ۻ���
	gBrake.FilterTicker = 0; 
	gExcursionInfo.IbExcursionOkFlag = 0;   //�ϵ��ʼ���ƶ�������Ư����־
#endif
    gBrake.DelayClose = 0; 
	        
//	gUVCoff.UDivV = 4096;
    gPWM.gPWMPrd = C_INIT_PRD;
	gDeadBand.DeadBand = C_MAX_DB;	//��ʼ������ʱ�䣬3.2us
    gMainStatus.StatusWord.all = 0;
    
// ʸ����ر�����ʼ��
    gRotorSpeed.TransRatio = 1000;                  // ���ٴ����ȹ̶�ֵ
    gPGData.QEPIndex = QEP_SELECT_NONE; 
    gPGData.PGType = PG_TYPE_NULL;                  // ��ʼ��Ϊ null��
    gPWM.gZeroLengthPhase = ZERO_VECTOR_NONE; 
	gAsr.KPLowCoff = 10;                            // ��ʼ��Kpϵ��
	gTemperature.OverTempPoint = 95;                // ��ʼ�����µ�   
	gEstBemf.BemfVoltDisPlayTotal = 0;        

// ���������صĳ�ʼ����Ĭ�����첽�����
    gMotorInfo.MotorType = MOTOR_TYPE_IM;
    //gMotorInfo.LastMotorType = gMotorInfo.MotorType;
    gMotorInfo.LastMotorType = MOTOR_NONE;                // ��֤������������ܽ�����س�ʼ��
    
    //(*EQepRegs).QEINT.all = 0x0;  //ȡ��QEP��I�ź��ж�
    
    gPGData.PGMode = 0;
    gIPMInitPos.Flag = 0;
	gUVCoff.Flag = 0;
	gUVCoff.RsTune = 0;
    gPmParEst.SynTunePGLoad = 0;
    gIPMInitPos.InitPWMTs = (20 * DSP_CLOCK);	  //20us
    gPWM.PWMModle = MODLE_CPWM;
	gFanCtrl.RunCnt = 5000;
    gFanCtrl.ErrTimes = 0;
#if(AIRCOMPRESSOR == 1)
	gMainCmd.RestarCnt = 0;
#endif
	gMainStatus.StatusWord.bit.OverLoadPreventState = 0;
	FlyingStartInitDeal();

    ParSend2Ms();
    ParSend05Ms();
}

/************************************************************
�����򲻵ȴ�ѭ��������ִ����Ҫ����ˢ�µĳ���Ҫ�����ǳ����
************************************************************/
/*void Main0msMotor(void)
{

}*/

/************************************************************
����:
1. ���������١�SVC�ٶȼ��㣻
2. �ٶȱջ����ƣ�
3. ���¼���gMainCmd.FreqSetApply�� ���ϴ���gMainCmd.FreqToFunc��

4. SVC �����б��������٣� FVC��VF�����������ٶȣ�

************************************************************/
void Main05msMotor(void)
{
	//gCpuTime.ADCInt = GetTime() - gCpuTime.ADCIntBase;
	//gCpuTime.ADCIntBase = GetTime();

#if (SOFTSERIES == MD500SOFT)
    GetADCAdjustInfo();
#endif

    if(gFluxWeak.Mode == 0)
	{
	    if(gMainCmd.FreqSet > 0)
		{
		    gMainCmd.FreqSetApply = gMainCmd.FreqSet + PmsmFreqAdjMethod();
		}
		else if(gMainCmd.FreqSet < 0)
		{
		    gMainCmd.FreqSetApply = gMainCmd.FreqSet - PmsmFreqAdjMethod();
		}
	}
	else
	{
	    gMainCmd.FreqSetApply = gMainCmd.FreqSet;
	}

    if(SYNC_SVC == gCtrMotorType)
    {   
		if(gRotorSpeed.SvcSpeed != 0)
		{
		    GetFeedRotorSpeed();               //���������٣�ȷ��������� 
		}
		else
		{
		    gRotorSpeed.SpeedEncoder = 0;     // �������������ֵ�쳣ʱ��svc�����ٶȲ�Ϊ0
		}
		gAsr.KPLowCoff = 10;

		gRotorSpeed.SpeedApply = (s16)gPmsmRotorPosEst.SvcRotorSpeed;
      
        if(STATUS_STOP != gMainStatus.RunStep)
		{
            VcAsrControl();
		}
		gMainCmd.FreqSyn = gRotorSpeed.SpeedApply;
        gMainCmd.FreqToFunc = gRotorSpeed.SpeedApply;
        
        gPmsmRotorPosEst.CurFreqSetReal = (((s32)gMainCmd.FreqSyn * (s32)gBasePar.FullFreq01)>>15) * 100;  //��¼�����Ƶ����֮ǰ��Ƶ��
    }  
    else if((SYNC_FVC == gCtrMotorType)||(gPmParEst.SynTunePGLoad == 1))
    {       
        GetFeedRotorSpeed();               //���������٣�ȷ��������� 
        gAsr.FreqFeedFilter   = gRotorSpeed.SpeedFeedFilter; 					
		gRotorSpeed.SpeedApply = gRotorSpeed.SpeedEncoder;
        gMainCmd.FreqFeed = gRotorSpeed.SpeedEncoder;
		if(1 == gAsr.Mode)
		{
            VcAsrControl1();
		}
		else
		{
			VcAsrControl();
		}
		gMainCmd.FreqSyn = gRotorSpeed.SpeedApply;
        gMainCmd.FreqToFunc = gRotorSpeed.SpeedApply;	
    }
	else
	{
	    GetFeedRotorSpeed();               //���������٣�ȷ��������� 
	}

	OutputLoseAdd();             // ���ȱ����
#if (SOFTSERIES == MD500SOFT)
		InputPhaseLoseDetect();        //����ȱ����
	#if (SOFT_INPUT_DETECT == STLEN)    
	    SoftInputPhaseLoseUdcCal();
	    SoftInputPhaseLoseDetect();
	#endif
#endif
}
/************************************************************
������ĵ�2msѭ��������ִ�е�����Ƴ���
˼·����������->����ת��->�����㷨->������������->���ұ���->�������
ִ��ʱ�䣺�����жϴ�ϵ������Լ120us
************************************************************/
void Main2msMotorA(void)
{

//�ӿ��ư��ȡ����	
	ParGet2Ms();
    ParGet05Ms();
//ParameterChange2Ms();
    if(gMainCmd.Command.bit.Start == 0)
    {
        SystemParChg2Ms();
        SystemParChg05Ms();                 // ����ʱ��ת���Ĳ���

        ChangeMotorPar();       //�������ת��?���в�ת��
    }
    RunStateParChg2Ms();
    //RunStateParChg05Ms();

}

void Main2msMotorB(void)
{     
    gIPMZero.zFilterCnt++;  
	if(gIPMZero.zFilterCnt >= 4)      
	{
	    gIPMZero.zFilterCnt = 4;
	} 
    switch(gCtrMotorType)
    {
        case SYNC_SVC:	 
            gPhase.OutVoltPhaseCom = 0;
            break;
        case SYNC_VF:
        case ASYNC_VF:
		    gPhase.OutVoltPhaseCom = 0;
            break;

        case SYNC_FVC:
            IPMPosAdjustZIndex();		    
            break;           

        case DC_CONTROL:
            gMainCmd.FreqSyn = 0;
            RunCaseDcBrake();
            gOutVolt.VoltPhaseApply = 0;        // ���ǵ�ͬ������������׼ת�Ӵż�                                      // ���Ӵ����ͻ�ڸ������?
            gMainCmd.FreqToFunc = 0;		    
            break;

        case RUN_SYNC_TUNE:  //����������?
		    IPMPosAdjustZIndex();
            // ������ʶ�󣬵�����������Ҫ���ñ�ʶ�õ���pi����, �õ�pi����ǰ�ı�ʶ���̲���ʹ�õ�����
            if(gBasePar.FcSetApply > C_DOUBLE_ACR_MAX_FC)
			{
	            gImAcrQ24.KP = (long)gPmParEst.IdKp * gBasePar.FcSetApply * gPmParEst.IsKpK / 1000;         // ��ʶʱ��СKp,һ�η�����СKp��Ki       
	            gItAcrQ24.KP = (long)gPmParEst.IqKp * gBasePar.FcSetApply * gPmParEst.IsKpK / 1000;
	            gImAcrQ24.KI = (s32)gPmParEst.IdKi * gPmParEst.IsKiK / 10;
	            gItAcrQ24.KI = (s32)gPmParEst.IqKi * gPmParEst.IsKiK / 10;
			}
			else
			{
				gImAcrQ24.KP = (long)gPmParEst.IdKp * gBasePar.FcSetApply * gPmParEst.IsKpK / 500;         // ������ʶʱ��СKp��Ki       
	            gItAcrQ24.KP = (long)gPmParEst.IqKp * gBasePar.FcSetApply * gPmParEst.IsKpK / 500;
	            gImAcrQ24.KI = (s32)gPmParEst.IdKi * gPmParEst.IsKiK / 10;
	            gItAcrQ24.KI = (s32)gPmParEst.IqKi * gPmParEst.IsKiK / 10;
			}
			gImAcrQ24.KP = Min(gImAcrQ24.KP,8000);  //���ֵ����Ϊ8000��wyk
            gItAcrQ24.KP = Min(gItAcrQ24.KP,8000);
            break;

        default:
            gMainCmd.FreqSyn = 0;
            gMainCmd.FreqToFunc = 0;
            break;
    }

    //�����ز�Ƶ��
	CalCarrierWaveFreq();
	FlyingStartFcDeal();
	
	switch(gMainStatus.RunStep)
	{
		case STATUS_RUN:		                    //����״̬������VF/FVC/SVC����
			RunCaseRun2Ms();
			break;

        case STATUS_STOP:
            RunCaseStop();
            break;

        case STATUS_IPM_INIT_POS:                   //ͬ������ʼλ�ýǼ��׶�
			RunCaseIpmInitPos();            
            break;
            
		case STATUS_SPEED_CHECK:                    //ת�ٸ���״̬
		    
			break;

		case STATUS_GET_PAR:	                    //������ʶ״̬���Ƶ�0.5msʱҪͬʱ�޸Ĳ�������
			RunCaseGetPar();            
			break;

		case STATUS_LOW_POWER:	                    //�ϵ绺��״̬/Ƿѹ״̬
			RunCaseLowPower();
			break;
            
		case STATUS_SHORT_GND:	                    //�ϵ�Եض�·�ж�״̬
			RunCaseShortGnd();
			break;

		case STATUS_BEFORE_RUN_DETECT:
		    OutputPhaseLoseAndShortGndDetect();
		    break;

		case STATUS_RS_TUNE:
		    BeforeRunRsIdentify();
			break;

		case STATUS_FLYING_START:
			RunCaseFlyingStart();
            break;
			
		default:
            gMainStatus.RunStep = STATUS_STOP;      // �ϵ��һ�Ż����
			break;
	}	
}

void Main2msMotorC(void)
{
    InvCalcPower();     // ���ʡ�ת�ؼ���
//    VfOscIndexCalc();
    
//��Ƶ������ͱ���	
	InvDeviceControl();			
}

void Main2msMotorD(void)
{
//������Ư��⣬AD��Ư�����Զȼ��	
	GetCurExcursion();				    

//׼����Ҫ���͸����ư������
    SendDataPrepare(); 
    
//��ʵʱ���ݴ��͸����ư�	
	ParSend2Ms();
    ParSend05Ms();
// End
}

/*************************************************************
	Ϊ����ģ��׼����Ҫ�����в���
*************************************************************/
void SendDataPrepare(void)		
{
    Uint tempU;
    int   mAiCounter;
	long  m_Long;
    Ulong mTotal1;
    Ulong mTotal2;
    Ulong mTotal3;
#if(AIRCOMPRESSOR == 1)
    Ulong mTotal4;
	Ulong mTotal5;
#endif
    Uint   mRatio;
	///////////////////////////////////////////////ͣ��ʱ����ʾ����Ϊ0����
	if((gMainStatus.RunStep == STATUS_LOW_POWER) ||
	   (gMainStatus.RunStep == STATUS_STOP) ||
	   (gLineCur.CurBaseInv < (4096/50)&& 
	   (gMotorInfo.MotorType != MOTOR_TYPE_PM))  ||          //������С�ڱ�Ƶ�������2%����ʾ0
          //������С�ڱ�Ƶ�������2%����ʾ0
	   (1 == gMainStatus.StatusWord.bit.OutOff ))	
	{
		gLineCur.CurPerShow = 0;
        gLineCur.CurTorque  = 0;
        gLineCur.Temp       = 0;
	}
	else
	{
		gLineCur.CurPerShow = gLineCur.CurPerFilter >> 7;
		if(gCtrMotorType == ASYNC_VF)
		{
        	gLineCur.Temp = Filter32(gIMTQ12.T, gLineCur.Temp);				
		}
		else
		{
        	gLineCur.Temp = Filter32(gIMTSetQ12.T, gLineCur.Temp);
		}
        //if(((long)gRotorSpeed.SpeedApply * gIMTSetQ12.T) < 0)
		if(((long)gRotorSpeed.SpeedApply * gLineCur.Temp) < 0)       // ֮ǰ�������ڹ���ʱ�����ͻ�䣬wyk
        {
        	gLineCur.CurTorque = -(int)abs(gLineCur.Temp);
        }
        else
        {
        	gLineCur.CurTorque = (int)abs(gLineCur.Temp);
        }
	}
    
	//ͬ�����Ƕ�ת��
	tempU = (Uint)((int)gRotorTrans.RTPos + gRotorTrans.PosComp);
    gRotorTrans.RtRealPos = ((Ulong)tempU * 3600L + 10) >> 16;
	if(gMotorInfo.MotorType == MOTOR_TYPE_PM)
    {   
	    if(SYNC_SVC == gCtrMotorType)
		{
		    gIPMPos.RealPos = ((Ulong)((Uint)gPhase.IMPhase) * 3600L + 10) >> 16;
		}
		else
		{
	        gIPMPos.RealPos = ((Ulong)gIPMPos.RotorPos * 3600L + 10) >> 16;
		}
    }
    
    // ai ��������
    DINT;
    mTotal1 = gAI.ai1Total;
    mTotal2 = gAI.ai2Total;
    mTotal3 = gAI.ai3Total;
#if(AIRCOMPRESSOR == 1)
    mTotal4 = gAI.ai4Total;
	mTotal5 = gAI.ai5Total;
#endif
    mAiCounter = gAI.aiCounter;
    
    gAI.ai1Total = 0;
    gAI.ai2Total = 0;
    gAI.ai3Total = 0;
#if(AIRCOMPRESSOR == 1)
    gAI.ai4Total = 0;
	gAI.ai5Total = 0;
#endif
    gAI.aiCounter = 0;
    EINT;
    
    gAI.gAI1 = mTotal1 / mAiCounter;
    gAI.gAI2 = mTotal2 / mAiCounter;
    gAI.gAI3 = mTotal3 / mAiCounter;
#if(AIRCOMPRESSOR == 1)
    gAI.gAI4 = mTotal4 / mAiCounter;
	gAI.gAI5 = mTotal5 / mAiCounter;
#endif
	gAI1 = gAI.gAI1;
    gAI2 = gAI.gAI2;
	gAI3 = gAI.gAI3;
#if(AIRCOMPRESSOR == 1)
    gAI4 = gAI.gAI4;
	gAI5 = gAI.gAI5;
#endif
    // ����ʵ�������ѹ
    if(gMainCmd.Command.bit.ControlMode == IDC_SVC_CTL)		// SVCʹ�õ�ѹ�ع��ĵ�ѹ����
    {
		m_Long = (((long)gPmsmRotorPosEst.Ud * (long)gPmsmRotorPosEst.Ud) + 				// ��ѹ�ع�����ֵ�����ý����޷�
          ((long)gPmsmRotorPosEst.Uq * (long)gPmsmRotorPosEst.Uq));
		gOutVolt.VoltApplySVC = (Uint)qsqrt(m_Long);
		mRatio = gOutVolt.VoltApplySVC;
    }
	else
	{
	    mRatio = gOutVolt.VoltApply;                              // FVC��VFʹ�ü���������ѹ����
	}
    mRatio = __IQsat(mRatio, 8192, 0);                              // �й�����
	//gOutVolt.VoltDisplay = VFFilter1st(mRatio, &gVoltFilt);
	//gOutVolt.VoltDisplay = Min(gOutVolt.VoltDisplay,4915);       //�����ѹ���������1.2��
	gOutVolt.VoltDisplay = Filter(gOutVolt.VoltDisplay,mRatio,32);		// 1s�˲�
    gOutVolt.VoltDisplay = Min(gOutVolt.VoltDisplay,4915);      //�����ѹ���������1.2��
}

/************************************************************
    ����������ʽ����Ҫ���Ϊ05ms�ٶȻ�����׼���ò�����
    
************************************************************/
void RunCaseRun2Ms(void)
{
    long  Imtset_M;

    //EnableDrive();
    if((gMainCmd.Command.bit.Start == 0)||( gBrake.ErrCode != 0))            // ��������
    {
        gMainStatus.RunStep = STATUS_STOP;
		ResetADCEndIsr();
        RunCaseStop();
#if(AIRCOMPRESSOR == 1)
		if(gMainStatus.StatusWord.bit.OverLoadPreventState != 2)  //����ͣ��������������
		{
		    if(gMainCmd.RestarCnt <= 3)
			{
		        gMainCmd.RestarCnt = 0;
			}
		}
#endif
        return;
    }

    gMainStatus.StatusWord.bit.StartStop = 1;         
    
    switch(gCtrMotorType)
    {            
        case ASYNC_VF:  //�첽����ͬ����VF����,�ݱ�����?
        case SYNC_VF:  
            //gIPMInitPos.Flag = 0;               // VF���к��´�������ʸ����Ҫ��ʶ��ʼλ��          
            VFWsTorqueBoostComm();				//ת����������������������?
            VFWSCompControl();					//ת�������(����F)
            VFAutoTorqueBoost();        
            #if 1                           // �������ƹ���Ƶ�����Ӹ������������´���
            if(speed_DEC &&(abs(gMainCmd.FreqSet) > abs(gVFPar.tpLst)))
            {
              gMainCmd.FreqSet = gVFPar.tpLst;
            }
            gVFPar.tpLst = gMainCmd.FreqSet;
            #endif
    		VfOverCurDeal();				        //�������ƴ���(����F)
    		//VfOverUdcDeal();					    //��ѹ���ƴ���(����F)
    		VfFreqDeal();                           // gVFPar.FreqApply

            gMainCmd.FreqToFunc = gVFPar.FreqApply;
            gMainCmd.FreqSetApply = gVFPar.FreqApply;

            VFSpeedControl();
            CalTorqueUp(); 
            
            HVfOscDampDeal();             // HVf �����ƣ� ������ѹ��λ
            gOutVolt.VoltPhaseApply = gHVfOscDamp.VoltPhase;            
            gOutVolt.Volt = gHVfOscDamp.VoltAmp;
            
            VFOverMagneticControl();               
            break;
            
        case SYNC_SVC:	
            CalTorqueLimitPar();
            //CalUdcLimitIT();                   //ʸ�����ƵĹ�ѹ���ƹ���// ����ת�����޺�ת�ؿ���
            PrepareAsrPar();
            PrepPmsmCsrPrar();
            if(gFluxWeak.Mode == 1)
        	{
        		Imtset_M = PmsmFwcAdjMethod();				
        	}
        	/*else if(gFluxWeak.Mode == 2)
        	{
        		PMFluxWeak05Ms();//Imtset_M = PmsmFwcAdjMethod();     //PmsmFwcMixMethod();
        	}*/
            else
            {
                Imtset_M = PmsmMaxTorqCtrl();

            }

			if(gFluxWeak.Mode == 2)
		    {
		        PMFluxWeak05Ms();
		    }

            PmsmSvcCalImForLowSpeed(); 
            Imtset_M += (long)gPmsmRotorPosEst.SvcIdSetForLowSpeed<<12;
            gIMTSet.M = Imtset_M;
				//get coff 
			FlyingStartParaAdjust();
            break;    
            
        case SYNC_FVC:        			   
            CalTorqueLimitPar();
            //CalUdcLimitIT();                   //ʸ�����ƵĹ�ѹ���ƹ���// ����ת�����޺�ת�ؿ���
            if(1 == gAsr.Mode)           
            {
	            PrepareAsrPar1();
			}
			else
			{
				PrepareAsrPar();
			}
	        PrepPmsmCsrPrar();		
            if(gFluxWeak.Mode == 1)
        	{
        		gIMTSet.M = PmsmFwcAdjMethod();				
        	}
        /*	else if(gFluxWeak.Mode == 2)
        	{
        		gIMTSet.M = PmsmFwcAdjMethod();	//PmsmFwcMixMethod();
        	}*/
			else
			{
			    gIMTSet.M = PmsmMaxTorqCtrl();
			}

			if(gFluxWeak.Mode == 2)
		    {
		        PMFluxWeak05Ms();
		    }
            break;

        case DC_CONTROL:
            ;
            break;
                        
        default:            
            break;         
    }
}

/************************************************************
    ����������ǰ�����ݳ�ʼ������Ϊ�������׼����ʼ����

************************************************************/
void PrepareParForRun(void)
{
    // ���ÿ���ģʽ�͵�����͵���ϣ����ڿ��߼����ࡣ�����в������޸ģ������ǲ����������ʶ״̬��ͻ
    gCtrMotorType = (CONTROL_MOTOR_TYPE_ENUM)(gMainCmd.Command.bit.ControlMode + 10);
	gPmParEst.SynTunePGLoad =0;

// ����������ʼ��
    gMainStatus.StatusWord.bit.StartStop = 0;
    gMainStatus.StatusWord.bit.SpeedSearchOver = 0;

	gMainStatus.PrgStatus.all = 0;
	gMainStatus.PrgStatus.bit.ACRDisable = 1;    
    gGetParVarable.StatusWord = TUNE_INITIAL;
    gVarAvr.UDCFilter = gInvInfo.BaseUdc;
	gMainCmd.FreqSyn = 0;
	gMainCmd.FreqReal = 0;
	gOutVolt.Volt = 0;
	gOutVolt.VoltApply = 0;
	gRatio = 0;
	gCurSamp.U = 0;
	gCurSamp.V = 0;
	gCurSamp.W = 0;
	gCurSamp.UErr = 600L<<12;
	gCurSamp.VErr = 600L<<12;
    gIUVWQ24.U = 0;
    gIUVWQ24.V = 0;
    gIUVWQ24.W = 0;
	gIUVWQ12.U = 0;
	gIUVWQ12.V = 0;
	gIUVWQ12.W = 0;
	gLineCur.CurPerShow = 0;
    gLineCur.CurTorque  = 0;
	gLineCur.CurBaseInv = 0;
	gLineCur.CurPer = 0;
	gLineCur.CurPerFilter = 0;
	gLineCur.ImTotal = 0;
    gLineCur.ItTotal = 0;
    gLineCur.CurCnt = 0;
	gIMTQ12.M = 0;
	gIMTQ12.T = 0;
    gIMTQ24.M = 0;
    gIMTQ24.T = 0;
	gDCBrake.Time = 0;
    gPWM.gZeroLengthPhase = ZERO_VECTOR_NONE;
    gIAmpTheta.ThetaFilter = gIAmpTheta.Theta;
	gMainStatus.StatusWord.bit.RunStart = 0;
#if (SOFTSERIES == MD500SOFT)
	gBrake.IBreakTotal = 0;
	gBrake.OnCounter = 0;
	gBrake.TotalCounter	= 0;
#endif
// Vf ��ض���ʼ��
	if(gMainCmd.Command.bit.ControlMode == IDC_VF_CTL)
	{
	    VfVarInitiate();
	}
    
    if(gMainCmd.Command.bit.ControlMode != IDC_VF_CTL)
    {
  	    ResetParForVC();  //VF���øú��������⣬��Ϊ���ı��������ѹ��Ƶ��
  	    ResetParForPmsmSvc();
//		gMainStatus.StatusWord.bit.OverLoadPreventState = 0;
    }
    if(gOverLoad.OverLoadPreventEnable == 1)    //10s���־λ���㣬�ȴ�����
	{
	    static Uint m_WaitTime = 0;
		if(gMainStatus.StatusWord.bit.OverLoadPreventState == 2)
		{
			m_WaitTime ++;
			if(m_WaitTime > 5000)    //10s
			{
			    gMainStatus.StatusWord.bit.OverLoadPreventState = 0;
			    m_WaitTime = 0;
#if(AIRCOMPRESSOR == 1)
				gMainCmd.RestarCnt ++;
#endif
			}
		}
		else
		{
		    m_WaitTime = 0;
		}
	}
}

/************************************************************
	�л���ͣ��״̬(�����Ӻ���)
************************************************************/
void TurnToStopStatus(void)
{
	DisableDrive();
	gMainStatus.RunStep = STATUS_STOP;
	gMainStatus.SubStep = 1;
}

/*******************************************************************
    ͣ��״̬�Ĵ���
********************************************************************/
void RunCaseStop(void)
{
//ͣ���������
    ResetADCEndIsr();
	DisableDrive();	    
	PrepareParForRun();
  
#if(SOFTSERIES == MD380SOFT)    
    if( gBrake.ErrCode == 0) 
#endif
	{
	    gMainCmd.FreqToFunc = 0; 
	}
    if(gMainCmd.Command.bit.Start == 0)
	{
	    gBforeRunPhaseLose.CheckOverFlag = 0;
		gBforeRunPhaseLose.CheckFirstFlag = 0;
	}
//�ȴ���Ư������
#if (SOFTSERIES == MD500SOFT)
	if(gMainStatus.StatusWord.bit.RunEnable != 1)
#else
	if((gMainStatus.StatusWord.bit.RunEnable != 1)||( gBrake.ErrCode != 0))
#endif
	{
		return;
	}

//�ж��Ƿ���Ҫ�Եض�·���
	if((1 == gExtendCmd.bit.ShortGnd) && (gMainStatus.StatusWord.bit.ShortGndOver == 0))
	{
		gMainStatus.RunStep = STATUS_SHORT_GND;
		gMainStatus.SubStep = 1;        // ���½��жԵض�·���
		if(gBforeRunPhaseLose.CheckFirstFlag == 0)
		{
			EALLOW;
			EPwm1Regs.AQCSFRC.all 	= 0x0A;
			EPwm2Regs.AQCSFRC.all 	= 0x0A;			
			EPwm3Regs.AQCSFRC.all 	= 0x0A;
			EDIS;
			gBforeRunPhaseLose.CheckFirstFlag = 1;
		}
		return;
	}
	else
	{
		gMainStatus.StatusWord.bit.ShortGndOver = 1;
	}

//�ж��Ƿ���Ҫ�𶯵��
	if(gMainCmd.Command.bit.Start == 1)	
	{
	    // ������ʶ
	    if(TUNE_NULL != gGetParVarable.TuneType)
	    {
		    gMainStatus.RunStep = STATUS_GET_PAR;
            PrepareParForTune();
		    return;			
	    }

        if(((gSubCommand.bit.OutputLostBeforeRun == 1)||
		(gExtendCmd.bit.ShortGndBeforeRun == 1))&&
		(gBforeRunPhaseLose.CheckOverFlag == 0))
		{
			gMainStatus.RunStep = STATUS_BEFORE_RUN_DETECT;
			gMainStatus.SubStep = 1;
			if(gBforeRunPhaseLose.CheckFirstFlag == 0)
			{
				EALLOW;
				EPwm1Regs.AQCSFRC.all 	= 0x0A;
				EPwm2Regs.AQCSFRC.all 	= 0x0A;			
				EPwm3Regs.AQCSFRC.all 	= 0x0A;
				EDIS;
				gBforeRunPhaseLose.CheckFirstFlag = 1;
			}
			return;	
		}
    	if(gError.ErrorCode.all != 0 )
    	{
    		return;
    	}
		if(((gExtendCmd.bit.SpeedSearch == 1) && (SYNC_SVC == gCtrMotorType)) && (gFlyingStart.Flag == 0))
		{
		    gMainStatus.RunStep = STATUS_FLYING_START;   // ת�ٸ�����״̬
		    gFlyingStart.Step = 0;
		    gMainStatus.SubStep = 1;
			gFlyingStart.PhaseLoseCnt = 0;
		    return;
		}

		if((gUVCoff.BeforeRunTuneEnable != 0)&&(gUVCoff.RsTune==0)&&(SYNC_SVC == gCtrMotorType))
		{
		    gMainStatus.RunStep = STATUS_RS_TUNE;   // ���붨�ӵ����ʶ
			gGetParVarable.IdSubStep = 1;
			return;
		}
        //ͬ����ʶ��ż���ʼλ�ýǽ׶�
#define FVC_INIT_TEST_CONDITION  ((0 == gRotorSpeed.SpeedEncoder) && (gPGData.PGType == PG_TYPE_ABZ))

    	if(((gIPMInitPos.Flag == 0) &&(1 != gIPMInitPos.InitTestFlag) &&
            (FVC_INIT_TEST_CONDITION || (SYNC_SVC == gCtrMotorType))&&(SYNC_VF != gCtrMotorType))||
			((gIPMInitPos.Flag == 2)&&(SYNC_SVC == gCtrMotorType)))
		{
			gMainStatus.RunStep = STATUS_IPM_INIT_POS;
			gMainStatus.SubStep = 1;
            gIPMInitPos.Step = 0;
            gIPMInitPos.OkFlag = 1;
            return;
		}

        
		if((gUVCoff.RsTune == 2)&&(SYNC_SVC == gCtrMotorType))   // ����ʶ�Ķ��ӵ磬DQ���к͵�����PI������������
		{		    
			PrepareParForTuneBeforeRun();
			ParSendTuneBeforeRun();
			gGetParVarable.StatusWord = TUNE_SUCCESS;
            gUVCoff.RsTune = 1;
		}

		if(SYNC_SVC == gCtrMotorType)
		{
		    gMainStatus.StatusWord.bit.RunStart = 1;   //�������ܵ��ٶ�ָ��
		}

        if((SYNC_SVC == gCtrMotorType)
            &&(0 == gIPMInitPos.InitTestFlag))
        {
            gIPMInitPos.Flag = 0;  
        }

		if((SYNC_SVC == gCtrMotorType)&&(gUVCoff.BeforeRunTuneEnable == 2))
		{
            gUVCoff.RsTune   = 0;
		}

		FlyingStartInitDeal();
		
        // else ...STATUS_RUN
		gMainStatus.RunStep = STATUS_RUN;
		gMainStatus.PrgStatus.all = 0;            
		gMainStatus.SubStep = 1; 
		             
        gBrake.DelayClose = 0;    //LJH1917
		EnableDrive();

        RunCaseRun2Ms();        // �Ż�����ʱ�䣬�ڸ��ľ��ܷ���
	}
}

/*************************************************************
	�������ϵ�����������ѹ�������ϵ��
*************************************************************/
void CalRatioFromVot(void)
{
	Uint	m_Ratio;
    
    // �������ϵ��
    if( 1 == gMainStatus.StatusWord.bit.OutOff )   //�������
    {
	    gOutVolt.VoltApply = 287;                       
    } 
	
	m_Ratio = ((Ulong)gOutVolt.VoltApply * (Ulong)gMotorInfo.Votage)/gInvInfo.InvVolt;	
    //gOutVolt.VoltDisplay = (m_Ratio > 4096) ? 4096 : m_Ratio;
	m_Ratio = ((Ulong)m_Ratio<<2) * gInvInfo.BaseUdc / gUDC.uDC >> 2;  // ��ѹ��ʱ���С1
	gRatio = (m_Ratio > 8192) ? 8192 : m_Ratio;
}

/*************************************************************
    �����жϣ����ģ�����������������㡢VC���������ƵȲ���

ע��:�ú����ڲ�����ʶ��Ҳ��ʹ�ì�������޸ģ���Ҫ����Ƿ�Ӱ����ر��?
*************************************************************/
void ADCEndIsr()
{  
    if((gCBCProtect.CBCIntFlag == 1)&&(gMainCmd.Command.bit.Start == 1)
        &&(0 == EPwm1Regs.TBSTS.bit.CTRDIR)&&(gError.ErrorCode.all == 0))        //�����ж�  
    {
	    EnableDrive();								//����״̬��������������ٴο���
	    gCBCProtect.CBCIntFlag = 0;
        //clear the Flag of CBC and Int
        EALLOW;      
        EPwm2Regs.TZCLR.bit.CBC = 1;  //���ڴ˴���0��ȷ��CBC�ж�����֮ǰֻ��һ��
        EPwm2Regs.TZCLR.bit.INT = 1; 
        EDIS;
    }
	if(gBasePar.FcSetApply > C_DOUBLE_ACR_MAX_FC)
	{
	    /*���¸�ֵ����Ƶ��ص�ϵ��*/        
	    if(EPwm1Regs.TBSTS.bit.CTRDIR == 1)
		{
		    PWMZeroIsr();		 	
			OutPutVolt();
		}
		else
		{
		    if(gPmsmRotorPosEst.FcCoff == 200)
			{
			    PWMZeroIsr();
		        PWMPeriodIsr();
				return;
			}
		    PWMPeriodIsr();
#if (SOFTSERIES == MD500SOFT)
			BrakeIGBTProtect();
#endif
            BrakeResControl();                      //�ƶ��������
		}
		gPmsmRotorPosEst.FcCoff = 400;
		gMainCmd.StepCoeff      = 2;
		
	}
	else if((gMainCmd.StepCoeff == 2)&&(EPwm1Regs.TBSTS.bit.CTRDIR == 0)  //�ӵ��η�����Ϊ���η����Ĺ��Ƚ׶�
	     &&(gBasePar.FcSetApply <= C_DOUBLE_ACR_MAX_FC))
	{        
        PWMPeriodIsr();
		gPmsmRotorPosEst.FcCoff = 200;
		gMainCmd.StepCoeff      = 1;
		
	}
    else
    {
        gPmsmRotorPosEst.FcCoff = 200;
	    gMainCmd.StepCoeff      = 1; 
		PWMZeroIsr();
		PWMPeriodIsr();
		if(EPwm1Regs.TBSTS.bit.CTRDIR == 0)
		{
#if (SOFTSERIES == MD500SOFT)
			BrakeIGBTProtect();
#endif
            BrakeResControl();                      //�ƶ��������
		}			
    }  
           
}
/******************************************************************************
* Function Name  : �����жϺ���ΪPWMPeriodIsr
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void PWMPeriodIsr(void)
{
    switch(gCtrMotorType)
    {            
        case ASYNC_VF:                                      //�첽����ͬ����VF����,��ʱ������?
        case SYNC_VF:
             
            break;
            
        case SYNC_SVC:
            
           	if(STATUS_RUN != gMainStatus.RunStep) // ͣ������ִ�дż�λ�ñ�ʶ���򣬻ᵼ���´�����ʱ�Ĵż�λ�øı�
                break;

            PmsmSvcCtrl();                                  /*ͬ��������ʸ�������ٶȺ�λ�ù���*/

            gPhase.IMPhaseApply = gPmsmRotorPosEst.SvcRotorPos;           
            gPhase.IMPhase = gPmsmRotorPosEst.SvcRotorPos>>16;

            VCCsrControl();
#ifdef  SYNC_SVC_TEST_CODE
            gPmsmRotorPosEst.PosError = (gPmsmRotorPosEst.SvcRotorPos>>16) - (s16)gIPMPos.RotorPos;
            gPmsmRotorPosEst.SpeedError = gPmsmRotorPosEst.SvcRotorSpeed - gRotorSpeed.SpeedEncoder;
#endif
            
            break;
            
        case SYNC_FVC:
             PmDecoupleDeal();          
        	 VCCsrControl();
            break;
            
        case RUN_SYNC_TUNE:                          
            PmDecoupleDeal();
            VCCsrControl();
            break;
            
        default:            
            break;         
    }  	     
    gPhase.OutPhase = gPhase.IMPhase + gOutVolt.VoltPhaseApply + gPhase.OutVoltPhaseCom; //���ӷ�����������Ҫ���������ʱ�����壬wyk      
    CalRatioFromVot(); // �������ϵ��   
    SoftPWMProcess();//���PWM������ȡ�ز����ںͽǶȲ���  						      
    OutPutPWM1();   //���ӹ�����
	OutPutVolt();

	if(gPGData.PGType == PG_TYPE_RESOLVER)  // �����źŲ�����ɣ��ȴ��´��ж϶�ȡ
	{
		RT_SAMPLE_END;        
	    #ifdef TMS320F28035
	    ROTOR_TRANS_RD = 1;
	    ROTOR_TRANS_SCLK   = 1;
	    #endif
	} 
}

/******************************************************************************
* Function Name  : �����жϺ���ΪPWMZeroIsr
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void PWMZeroIsr(void)
{		
#if (SOFTSERIES == MD380SOFT) 
    if(GetOverUdcFlag())                    //��ѹ����
	{
	   HardWareOverUDCDeal();				
	}
#endif
		
    GetUDCInfo();							//��ȡĸ�ߵ�ѹ��������    
	GetCurrentInfo();						//��ȡ��������, �Լ��¶ȡ�ĸ�ߵ�ѹ�Ĳ���
	CalOutputPhase();                       //���������λ	�����ǵ����ת�ӵ�Ƕ�
	ChangeCurrent();						//������ֳ����µĵ�����	
}
/*******************************************************************************
* Function Name  : ��λADC���������жϵ�ִ�г���ΪADCEndIsr
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ResetADCEndIsr(void)
{
    gMainCmd.pADCIsr = ADCEndIsr;                    //����ADC�����жϵ�Ĭ�ϴ������
}

/*******************************************************************************
* Function Name  : ����ADC�����ж�ִ�г���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetADCEndIsr(void (*p_mADCIsr)())
{
    gMainCmd.pADCIsr = p_mADCIsr;                    //����ADC�����жϵ�Ĭ�ϴ������
}




