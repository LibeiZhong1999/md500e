/****************************************************************
�ļ����ܣ��͹���ģ������ݽ������򣬵�CPU��ʹ�����鿽����˫CPU��ͨѶ
�ļ��汾�� 
���¸��£� 
	
****************************************************************/
#include "MotorInclude.h"
#include "MotorIPMSVC.h"

//------------------���ܰ汾����ʾ--------//
// ����ͨ������汾��Ϣ
/************************************************************************
F7-10:���ܰ汾��,F7-11:���ܰ汾��,F7-15:������ʱ�汾��,F7-16:������ʱ�汾��
ע��:�汾�����Ų��ܳ�����Ӧ��λ��,����"ͨ������汾������"���ܳ���8λ������
*************************************************************************/
#if (SOFTSERIES == MD500SOFT) 
	#define CORE_SOFTWARE_VERSION           00        // ͨ������汾
	#define CORE_SOFTWARE_VERSION_TEMP      57        // ͨ������汾��������  

	// ����Ǳ�����汾��Ϣ
	#define CORE_NON_STANDARD_VERSION       0        // �Ǳ��
	#define CORE_NON_STANDARD_VERSION_TEMP  0        // �Ǳ����������

	// ������ʱ�汾�����Ϣ
	#define CORE_TEMP_SOFTWARE_NO           0        // ��ʱ�����
	#define CORE_TEMP_SOFTWARE_VERSION      0        // ��ʱ�����������

	#define CORE_SOFT_TYPE                  0         // �������       0:ͨ��    1:�Ǳ�
	#define CORE_IS_TEMP_SOFTWARE           0         // �Ƿ�Ϊ��ʱ��� 0:����ʱ  1:��ʱ  
#else
	#define CORE_SOFTWARE_VERSION           00        // ͨ������汾
	#define CORE_SOFTWARE_VERSION_TEMP      61        // ͨ������汾�������� 

	// ����Ǳ�����汾��Ϣ
	#define CORE_NON_STANDARD_VERSION       0        // �Ǳ��
	#define CORE_NON_STANDARD_VERSION_TEMP  0        // �Ǳ����������

	// ������ʱ�汾�����Ϣ
	#define CORE_TEMP_SOFTWARE_NO           0        // ��ʱ�����
	#define CORE_TEMP_SOFTWARE_VERSION      0        // ��ʱ�����������

	#define CORE_SOFT_TYPE                  0         // �������       0:ͨ��    1:�Ǳ�
	#define CORE_IS_TEMP_SOFTWARE           0         // �Ƿ�Ϊ��ʱ��� 0:����ʱ  1:��ʱ
#endif
	// // ȫ�ֱ�������
	//#define     NUM_2MS_F2M     151     // ����2ms���ݸ�����
	#define     NUM_2ms_FUNC_TO_MOTOR           160   //
	#define     NUM_2ms_FUNC_TO_MOTOR_debug     30
	//#define     NUM_2MS_M2F     60      // ����2ms���ݸ�����
#if(AIRCOMPRESSOR == 1)
    #define     NUM_2ms_MOTOR_TO_FUNC           38  
#else
	#define     NUM_2ms_MOTOR_TO_FUNC           36  
#endif
	#define     NUM_2ms_MOTOR_TO_FUNC_debug     30
	#define     NUM_05MS_F2M    14      // ����05ms���ݸ�����
	#define     NUM_05MS_M2F    5       // ����05ms���ݸ�����
	#define     NUM_TUNE_M2F    20      // ������гʱ���ݸ�����(2ms)


Uint gCoreSoftWareVesion = (CORE_SOFTWARE_VERSION << 8) + CORE_SOFTWARE_VERSION_TEMP;             // ��8λ:ͨ���������8λ:��������
Uint gCoreNonStandardVesion = (CORE_NON_STANDARD_VERSION << 6) + CORE_NON_STANDARD_VERSION_TEMP;  // ��10λ:�Ǳ�ţ���6λ:�Ǳ���������
Uint gCoreTmpsoftNoAndVersion = (CORE_TEMP_SOFTWARE_NO << 6) + CORE_TEMP_SOFTWARE_VERSION;        // ��10λ:��ʱ����ţ���6λ:��ʱ��������
Uint gCoreSoftType = (CORE_SOFT_TYPE << 8) + CORE_IS_TEMP_SOFTWARE;                               // ��8λ:ͨ�û�Ǳ꣬��8λ:�Ƿ���ʱ�汾

int  gSendToMotor05MsDataBuff[NUM_05MS_F2M];
int  gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + NUM_2ms_FUNC_TO_MOTOR_debug+3];
int  gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC + NUM_2ms_MOTOR_TO_FUNC_debug];
int  gRealTimeToFunctionDataBuff[NUM_05MS_M2F];
int  gParaIdToFunctionDataBuff[NUM_TUNE_M2F];

Uint uReservedData;              // ��������
Uint gSoftVersion = SOFT_VERSION;

extern PMSM_FLUX_WEAK_STRUCT   gPmFluxWeak;
extern MT_STRUCT_Q24           gUMTQ24; 
extern u16 InvCurForCutDown;
/**************************************************************
	0.5Msѭ���У��ӹ���ģ���ȡ��Ҫ�����в���(��ʱ�����������)
*************************************************************/
void ParSend05Ms(void)
{
    //gRealTimeToFunctionDataBuff[0]	=	gUDC.uDC	;
    //gRealTimeToFunctionDataBuff[1]	=	gLineCur.CurBaseInv	;
    //gRealTimeToFunctionDataBuff[2]	=	gIAmpTheta.anglePF	;
    //gRealTimeToFunctionDataBuff[3]	=	gVfCsr.active	;
    //gRealTimeToFunctionDataBuff[4]	=	gOutVolt.VoltApply	;
}

void ParGet05Ms(void)
{
    gMainCmd.Command.all            =	gSendToMotor05MsDataBuff[0]	;
    gExtendCmd.all                  =	gSendToMotor05MsDataBuff[1]	;
    gGetParVarable.TuneType         =	(TUNE_FLOW_ENUM)gSendToMotor05MsDataBuff[2]	;
    gMotorInfo.MotorType            =	gSendToMotor05MsDataBuff[3]	;
    gMainCmd.FreqSet                =	gSendToMotor05MsDataBuff[4]	;
    gOutVolt.vfSplit                =	gSendToMotor05MsDataBuff[5]	;
    gMotorInfo.Votage               =	gSendToMotor05MsDataBuff[6]	;
    gMotorInfo.CurrentGet           =	gSendToMotor05MsDataBuff[7]	;
    gMotorInfo.Frequency            =	gSendToMotor05MsDataBuff[8]	;
    gVFPar.VFLineType               =	gSendToMotor05MsDataBuff[9]	;

    gVFPar.ovGain                   =    gSendToMotor05MsDataBuff[10]	;
    gVFPar.ovPoint                  =    gSendToMotor05MsDataBuff[11]	;
    gVFPar.ocGain                   =    gSendToMotor05MsDataBuff[12]	;
    gVFPar.ocPoint                  =    gSendToMotor05MsDataBuff[13]	;
}

void ParSend2Ms(void)
{
    gSendToFunctionDataBuff[0]	=	gMainStatus.StatusWord.all	;
    gSendToFunctionDataBuff[1]	=	gGetParVarable.StatusWord	;
    gSendToFunctionDataBuff[2]	=	gError.ErrorCode.ErrorCodeStruct.ErrorCode1	;
    gSendToFunctionDataBuff[3]	=	gError.ErrorCode.ErrorCodeStruct.ErrorCode2	;
    gSendToFunctionDataBuff[4]	=	gError.ErrorInfo[0].all	;
    gSendToFunctionDataBuff[5]	=	gError.ErrorInfo[1].all	;
    gSendToFunctionDataBuff[6]	=	gError.ErrorInfo[2].all	;
    gSendToFunctionDataBuff[7]	=	gError.ErrorInfo[3].all	;
    gSendToFunctionDataBuff[8]	=	gError.ErrorInfo[4].all	;
    gSendToFunctionDataBuff[9]	=	gLineCur.ErrorShow	;
    gSendToFunctionDataBuff[10]	=	gMainCmd.FreqToFunc	;
    gSendToFunctionDataBuff[11]	=	gOutVolt.VoltDisplay	;
    gSendToFunctionDataBuff[12]	=	gTemperature.Temp	;
    gSendToFunctionDataBuff[13]	=	gMotorInfo.Current	;
    gSendToFunctionDataBuff[14]	=	gSoftVersion	;
    gSendToFunctionDataBuff[15]	=	gAI.gAI1	;
    gSendToFunctionDataBuff[16]	=	gAI.gAI2	;
    gSendToFunctionDataBuff[17]	=	gAI.gAI3	;
    gSendToFunctionDataBuff[18]	=	gUDC.uDCShow	;
    gSendToFunctionDataBuff[19]	=	gLineCur.CurTorque	;
    gSendToFunctionDataBuff[20]	=	gLineCur.CurPerShow	;
    gSendToFunctionDataBuff[21]	=	gIPMPos.RealPos	;
    gSendToFunctionDataBuff[22]	=	gPowerTrq.TrqOutHoAndFo_pu	;	//���ӿ�����ʹ�õ�ת�ص���
    gSendToFunctionDataBuff[23]	=	gRotorSpeed.SpeedEncoder;
    gSendToFunctionDataBuff[24] =   gPowerTrq.InvPower_si;
    gSendToFunctionDataBuff[25] =   gPowerTrq.TrqOut_pu;
    gSendToFunctionDataBuff[26] =   gRotorTrans.AbsRotPos;              // ABZ������ʱ������ʾ�ο�λ�ú�ʵ��ת��λ��ƫ��
    gSendToFunctionDataBuff[27] =   gPowerTrq.anglePF;
    gSendToFunctionDataBuff[28] =   gIPMPos.ZSigNumSet;
    gSendToFunctionDataBuff[29] =   gInvInfo.InvTypeApply;      // u0- 59

// �汾����Ϣ����
    gSendToFunctionDataBuff[30] =   gCoreSoftWareVesion;
    gSendToFunctionDataBuff[31] =   gCoreNonStandardVesion;
    gSendToFunctionDataBuff[32] =   gCoreTmpsoftNoAndVersion;
    gSendToFunctionDataBuff[33] =   gCoreSoftType;
    gSendToFunctionDataBuff[34] =   gEstBemf.BemfVoltDisPlay;   //���߷��綯�Ʊ�ʶֵ
    gSendToFunctionDataBuff[35] =   gOverLoad.TotalPercent;
#if(AIRCOMPRESSOR == 1)
	gSendToFunctionDataBuff[36] =   gAI.gAI4; 
	gSendToFunctionDataBuff[37] =   gAI.gAI5; 
#endif


// UF ������
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC + 0 ] =   gBasePar.FcSetApply;												// uf-00 ʵ����Ƶ, 0.1Hz
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC + 1 ] =   gMainCmd.FreqSet;													// uf-01 ���ܴ��ݵ��趨Ƶ��, Q15
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	2 ] =   gMainCmd.FreqSetApply;												// uf-02 ����ʵ���趨Ƶ��, Q15
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	3 ] =   gRotorSpeed.SpeedEncoder;											// uf-03 ����������Ƶ��, Q15
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	4 ] =   gOverLoad.TotalPercent;											// uf-04 ���س̶�ϵ��
    
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	5 ] =   gIMTSetApply.M >> 12;												// uf-05 �������ŵ���, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	6 ] =   gIMTSetApply.T >> 12;												// uf-06 �������ص���, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	7 ]	=	gIMTQ24.M >> 12;													// uf-07 �������ŵ���, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	8 ]	=	gIMTQ24.T >> 12;													// uf-08 �������ص���, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	9 ]	=	gRotorSpeed.SpeedApply;											// uf-09 ����Ƶ��, Q15
    
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	10]	=	gOutVolt.LimitOutVoltPer;											// uf-10 ���������ѹ, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	11] =   gPmFluxWeak.VoltLpf;												// uf-11 �����ѹ�˲�ֵ, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	12]	=	gOutVolt.MaxOutVoltPer;											// uf-12 ��������ѹ, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	13]	=	gIAmpTheta.Amp;													// uf-13 �ߵ�����Чֵ, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	14]	=	gIAmpTheta.Theta;													// uf-14 �ߵ����Ƕ�, Q15
    
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	15]	=	gUMTSet.M	;														// uf-15 d���ѹ����, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	16]	=	gUMTSet.T 	;														// uf-16 q���ѹ����, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	17]	=	((Ulong)gPmsmRotorPosEst.SvcRotorPos >> 16) * 3600 >> 16;		// uf-17 SVC����Ƕ�, 0.1��
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	18]	=	(Ulong)gIPMPos.RotorPos * 3600 >> 16;							// uf-18 ת��λ�ýǶ�, 0.1��
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	19]	=	gUDC.uDC;															// uf-19 ĸ�ߵ�ѹ, Q12
    
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	20]	=   gPhase.IMPhase;													// uf-20 d����λ, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	21]	=   (Ulong)gIPMPos.InitPos * 3600 >> 16;								// uf-21 �ż���ʼλ�ý�, 0.1��
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	22]	=   gMainStatus.RunStep;												// uf-22 ������
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	23]	=   gUAmpTheta.Theta;													// uf-23 ��ѹ�Ƕ�
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	24]	=	gPmsmRotorPosEst.SvcRotorPos >> 16;								// uf-24
    
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	25]	=	gIUVWQ12.U;														// uf-25 U�����, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	26]	=	gIUVWQ12.V;														// uf-26 V�����, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	27]	=	gIUVWQ12.W;														// uf-27 W�����, Q12
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	28]	=	gOverLoad.InvTotal.half.MSW;										// uf-28
    gSendToFunctionDataBuff[NUM_2ms_MOTOR_TO_FUNC +	29]	=	EPwm1Regs.TBPRD;													// uf-29
}

void ParGet2Ms(void)
{    
    gSubCommand.all              =	gSendToMotor2MsDataBuff[0]	;  
    gMainCmd.FreqDesired         =	gSendToMotor2MsDataBuff[1]	;  
    gBasePar.MaxFreq             =	gSendToMotor2MsDataBuff[2]	;  
#if (SOFTSERIES == MD500SOFT)
    gBasePar.FcSet               =	gSendToMotor2MsDataBuff[3]	;  
#else
    gBasePar.FcSetToMotor        =	gSendToMotor2MsDataBuff[3]	;  
#endif
    gMotorInfo.Power             =	gSendToMotor2MsDataBuff[4]	;  
    gMotorExtInfo.Rpm            =	gSendToMotor2MsDataBuff[5]	;  
    gMotorExtInfo.R1             =	gSendToMotor2MsDataBuff[6]	;  
    gMotorExtInfo.R2             =	gSendToMotor2MsDataBuff[7]	;  
    gMotorExtInfo.L0             =	gSendToMotor2MsDataBuff[8]	;  
    gMotorExtInfo.LM             =	gSendToMotor2MsDataBuff[9]	;  
    gMotorExtInfo.I0             =	gSendToMotor2MsDataBuff[10]	;  
    gMotorExtInfo.RsPm           =	gSendToMotor2MsDataBuff[11]	;  
    gMotorExtInfo.LD             =	gSendToMotor2MsDataBuff[12]	;  
    gMotorExtInfo.LQ             =	gSendToMotor2MsDataBuff[13]	;  
    gVCPar.ASRKpLow              =	gSendToMotor2MsDataBuff[14]	;  
    gVCPar.ASRTILow              =	gSendToMotor2MsDataBuff[15]	;  
    gVCPar.ASRKpHigh             =	gSendToMotor2MsDataBuff[16]	;  
    gVCPar.ASRTIHigh             =	gSendToMotor2MsDataBuff[17]	;  
    gVCPar.ASRSwitchLow          =	gSendToMotor2MsDataBuff[18]	;  
    gVCPar.ASRSwitchHigh         =	gSendToMotor2MsDataBuff[19]	;  
    gVCPar.VCWsCoff              =	gSendToMotor2MsDataBuff[20]	;  
    gVCPar.VCTorqFilter          =	gSendToMotor2MsDataBuff[21]	;  //SVC���ת���˲�ϵ��
    gPGData.PulseNum             =	gSendToMotor2MsDataBuff[22]	;  
    gVCPar.VcOverExc             =	gSendToMotor2MsDataBuff[23]	;  
    gLoadLose.ChkLevel           =  gSendToMotor2MsDataBuff[24]	; 
    gLoadLose.ChkTime            =  gSendToMotor2MsDataBuff[25]	; 
    gVCPar.PosVCTorqueLim        =	gSendToMotor2MsDataBuff[26]	;  
    gVFPar.VFTorqueUp            =	gSendToMotor2MsDataBuff[27]	;  
    gVFPar.VFTorqueUpLim         =	gSendToMotor2MsDataBuff[28]	;  
    gVFPar.VFLineFreq1           =	gSendToMotor2MsDataBuff[29]	;  
    gVFPar.VFLineVolt1           =	gSendToMotor2MsDataBuff[30]	;  
    gVFPar.VFLineFreq2           =	gSendToMotor2MsDataBuff[31]	;  
    gVFPar.VFLineVolt2           =	gSendToMotor2MsDataBuff[32]	;  
    gVFPar.VFLineFreq3           =	gSendToMotor2MsDataBuff[33]	;  
    gVFPar.VFLineVolt3           =	gSendToMotor2MsDataBuff[34]	;  
    gVFPar.VFWsComp              =	gSendToMotor2MsDataBuff[35]	;  
    gVFPar.VFOverExc             =	gSendToMotor2MsDataBuff[36]	;  
    gVFPar.VFOvShock             =	gSendToMotor2MsDataBuff[37]	;  
    gComPar.StartDCBrakeCur      =	gSendToMotor2MsDataBuff[38]	;  
    gComPar.StopDCBrakeCur       =	gSendToMotor2MsDataBuff[39]	;  
    gComPar.BrakeCoff            =	gSendToMotor2MsDataBuff[40]	;  
    gComPar.MotorOvLoad          =	gSendToMotor2MsDataBuff[41]	;  
    gComPar.PerMotorOvLoad       =	gSendToMotor2MsDataBuff[42]	;  
    gPWM.SoftPWMTune             =	gSendToMotor2MsDataBuff[43]	;  
    gADC.DelaySet                =	gSendToMotor2MsDataBuff[44]	;  
    gInvInfo.LowUdcCoff          =	gSendToMotor2MsDataBuff[45]	;  
    gPGData.PGTypeGetFromFun     =	gSendToMotor2MsDataBuff[46]	;  
    gRotorSpeed.TransRatio       =	gSendToMotor2MsDataBuff[47]	;  
    gInvInfo.InvTypeSet          =	gSendToMotor2MsDataBuff[48]	;  
    gInvInfo.GpTypeSet           =	gSendToMotor2MsDataBuff[49]	;  
    gInvInfo.TempType            =	gSendToMotor2MsDataBuff[50]	;   
    gInvInfo.UDCCoff             =	gSendToMotor2MsDataBuff[51]	;  
    gInvInfo.CurrentCoff         =	gSendToMotor2MsDataBuff[52]	;  
    gUVCoff.UDivVGet             =	gSendToMotor2MsDataBuff[53]	;  
    gVCPar.SvcMode               =	gSendToMotor2MsDataBuff[54]	;  
    gDeadBand.DeadTimeSet        =	gSendToMotor2MsDataBuff[55]	; 
//#if (SOFTSERIES == MD380SOFT) 
    //gVCPar.NegVCTorqueLim        =	gSendToMotor2MsDataBuff[56]	;  // �޸�Ϊ����ת�����ޣ�wyk
//#endif   
    gComPar.SpdSearchTimeSet     =	gSendToMotor2MsDataBuff[57]	;  
    gIPMPos.PowerOffPos          =	gSendToMotor2MsDataBuff[58]	;       // �ϴε���ͬ�����Ƕ�
    gRotorTrans.Poles            =	gSendToMotor2MsDataBuff[59]	;   
    gMotorExtInfo.BemfVolt       =	gSendToMotor2MsDataBuff[60]	;  
    gVCPar.AcrImKp               =	gSendToMotor2MsDataBuff[61]	;  
    gVCPar.AcrImKi               =	gSendToMotor2MsDataBuff[62]	;  
    gVCPar.AcrItKp               =	gSendToMotor2MsDataBuff[63]	;  
    gVCPar.AcrItKi               =	gSendToMotor2MsDataBuff[64]	;   
    gPGData.SpeedDir             =	gSendToMotor2MsDataBuff[65]	;  
    gIPMPos.RotorZeroGet         =	gSendToMotor2MsDataBuff[66]	;    
    gPWM.PwmModeSwitchLF         =    gSendToMotor2MsDataBuff[67] ;
    gUVWPG.UvwDir                           =	gSendToMotor2MsDataBuff[68]	;  
    gUVWPG.UvwZeroPos_deg                   =	gSendToMotor2MsDataBuff[69]	;  
    gFluxWeak.Mode                          =	gSendToMotor2MsDataBuff[70]	;  
    gFluxWeak.CoefFlux                      =	gSendToMotor2MsDataBuff[71]	;  
    gFluxWeak.IdMax                         =	gSendToMotor2MsDataBuff[72]	;  
    gFluxWeak.CoefAdj                       =	gSendToMotor2MsDataBuff[73]	;  
    gFluxWeak.CoefKI                        =	gSendToMotor2MsDataBuff[74]	; 

	gFlyingStart.KpRatio                    = gSendToMotor2MsDataBuff[108];
	gFlyingStart.KiRatio                    = gSendToMotor2MsDataBuff[109];
	gFlyingStart.CurLimitAdj                = gSendToMotor2MsDataBuff[110];
		
    gVCPar.NegVCTorqueLim                   =	gSendToMotor2MsDataBuff[119]	;    
    gPmFluxWeak.FluxWeakDepth               =	gSendToMotor2MsDataBuff[120]	;
    gIPMInitPos.CurLimitAdjBig			  =	gSendToMotor2MsDataBuff[121]	;
    gIPMInitPos.InitTestFlag                =	gSendToMotor2MsDataBuff[122]	;
    gAsr.Mode                               =	gSendToMotor2MsDataBuff[123]	;
    gPmFluxWeak.SalientRateCoff             =   gSendToMotor2MsDataBuff[124]	;
    gPmFluxWeak.PmsmMaxTorqCtrlEnable       =   gSendToMotor2MsDataBuff[125]	;
	gPmDecoup.EnableDcp                     =	gSendToMotor2MsDataBuff[126]	;
	gPmParEst.IsKpK                         =	gSendToMotor2MsDataBuff[127]	;
	gPmParEst.IsKiK                         =	gSendToMotor2MsDataBuff[128]	;
    gIPMPos.ZSigEnable                      =	gSendToMotor2MsDataBuff[129]	;
    gPmsmRotorPosEst.SvcSpeedLpfTs          =	gSendToMotor2MsDataBuff[130]	;
    gPmsmRotorPosEst.Kb                     =	gSendToMotor2MsDataBuff[131]	;
    gPmsmRotorPosEst.Ka                     =	gSendToMotor2MsDataBuff[132]	;
    gPmsmRotorPosEst.SvcIdMaxForLowSpeed    =	gSendToMotor2MsDataBuff[133]	;
    gPmsmRotorPosEst.FcLow                  =	gSendToMotor2MsDataBuff[134]	;
    gPmsmRotorPosEst.SvcLowSpeedMode        =	gSendToMotor2MsDataBuff[135]	;
	gPmsmRotorPosEst.BrakeFreq              =	gSendToMotor2MsDataBuff[136]	;
	gPmsmRotorPosEst.Step                   =	gSendToMotor2MsDataBuff[137]	;
	gIPMInitPos.CurLimitAdjSmall			  =	gSendToMotor2MsDataBuff[138]	;
    gPmsmRotorPosEst.SpeedCheckEnable       =	gSendToMotor2MsDataBuff[139]	;
	gVCPar.ZeroServer                       =	gSendToMotor2MsDataBuff[140]	;
	gVCPar.ASRSwitchZero                    =	gSendToMotor2MsDataBuff[141]	;
	gVCPar.ASRKpZero                        =	gSendToMotor2MsDataBuff[142]	;
	gVCPar.ASRTIZero                        =	gSendToMotor2MsDataBuff[143]	;
	gVCPar.AntiDirEnable                    =	gSendToMotor2MsDataBuff[144]	;
	gAsr.PosSetAdd                          =	gSendToMotor2MsDataBuff[145]	;
    //gVCPar.NegVCTorqueLim                   =	gSendToMotor2MsDataBuff[146]	;
    gBrake.Ontimeset                        =	gSendToMotor2MsDataBuff[147]	;
    gUVWPG.ErrorEnable                      =	gSendToMotor2MsDataBuff[148]	;
    gIPMInitPos.ErrorIintPosEnable          =	gSendToMotor2MsDataBuff[149]	;
    gIPMPos.LoadTuneErrorEnable             =	gSendToMotor2MsDataBuff[150]	;

	gUVCoff.BeforeRunTuneEnable             =	gSendToMotor2MsDataBuff[151]	;
	gUVCoff.OnlineTuneBemfVoltEnable        =	gSendToMotor2MsDataBuff[152]	;
	gOverLoad.OverLoadPreventEnable         =	gSendToMotor2MsDataBuff[153]	;
#if (SOFT_INPUT_DETECT == STLEN)
    gSoftInLose.HalfInvPowerPU              =	gSendToMotor2MsDataBuff[154]	;
#endif
	gMainCmd.XiuMianFreq                    =	gSendToMotor2MsDataBuff[155]	;
	gPGData.DiscDetectDelayTime             =   gSendToMotor2MsDataBuff[156]	;
//	gOverLoad.GPOverLoadLine                =   gSendToMotor2MsDataBuff[157]	;
    gIPMPos.CompPosFun			             =   gSendToMotor2MsDataBuff[158]	;    
    gPWM.OverModuleCoff                     =   gSendToMotor2MsDataBuff[159]	;

    gRotorSpeed.SvcSpeed             =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 0 ]	;  //svc�²���ʹ��
    gTestDataReceive.TestData1       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 1 ]	;  //���������ʾ��ʽ
    gTestDataReceive.TestData2       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 2 ]	;  //5�η���ʹ��
    gTestDataReceive.TestData3       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 3 ]	;  
    gTestDataReceive.TestData4       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 4 ]	;  
    gTestDataReceive.TestData5       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 5 ]	;  
    gTestDataReceive.TestData6       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 6 ]	; // SVC���ŵ�������
    gTestDataReceive.TestData7       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 7 ]	; // SVC���ŵ���������ֹƵ��
    gVCPar.VCSpeedFilter             =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 8 ]	; 
    gIPMPos.TuneDetaPosDisable       =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 9 ]	; // SVCת���޷���ʽѡ��
    gTestDataReceive.TestData10      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 10]	; 
    gTestDataReceive.TestData11      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 11]	; 
    gTestDataReceive.TestData12      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 12]	; 
    gTestDataReceive.TestData13      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 13]	; 
    gTestDataReceive.TestData14      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 14]	; 
    gTestDataReceive.TestData15      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 15]	; 
    gTestDataReceive.TestData16      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 16]	; 
    gTestDataReceive.TestData17      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 17]	; 
    gTestDataReceive.TestData18      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 18]	; 
    gTestDataReceive.TestData19      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 19]	; 
    gTestDataReceive.TestData20      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 20]	; 
    gTestDataReceive.TestData21      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 21]	; 
    gTestDataReceive.TestData22      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 22]	; 
    gTestDataReceive.TestData23      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 23]	; 
    gTestDataReceive.TestData24      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 24]	; 
    gTestDataReceive.TestData25      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 25]	; 
    gTestDataReceive.TestData26      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 26]	; 
    gTestDataReceive.TestData27      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 27]	; 
    gTestDataReceive.TestData28      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 28]	; 
    gTestDataReceive.TestData29      =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + 29]	;  // ����תʱʹ�ô˲���   
 
    gRotorSpeed.FreWindow            =	gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + NUM_2ms_FUNC_TO_MOTOR_debug]; 
    gVCPar.TorMasToFol               =  gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + NUM_2ms_FUNC_TO_MOTOR_debug +1];  
	gBrake.VoltageLim                =  gSendToMotor2MsDataBuff[NUM_2ms_FUNC_TO_MOTOR + NUM_2ms_FUNC_TO_MOTOR_debug +2];
}

void ParSendTune(void)
{
    gParaIdToFunctionDataBuff[0]	=	gMotorExtReg.R1	;
    gParaIdToFunctionDataBuff[1]	=	gMotorExtReg.R2	;
    gParaIdToFunctionDataBuff[2]	=	gMotorExtReg.L0	;
    gParaIdToFunctionDataBuff[3]	=	gMotorExtReg.LM	;
    gParaIdToFunctionDataBuff[4]	=	gMotorExtReg.I0	;
    
    gParaIdToFunctionDataBuff[5]	=	gMotorExtReg.RsPm ;
    gParaIdToFunctionDataBuff[6]	=	gMotorExtReg.LD	;
    gParaIdToFunctionDataBuff[7]	=	gMotorExtReg.LQ	;
    gParaIdToFunctionDataBuff[8]	=	uReservedData	;
    gParaIdToFunctionDataBuff[9]	=	gEstBemf.BemfVolt	;
    
    gParaIdToFunctionDataBuff[10]	=	gPmParEst.IdKp	;
    gParaIdToFunctionDataBuff[11]	=	gPmParEst.IdKi	;
    gParaIdToFunctionDataBuff[12]	=	gPmParEst.IqKp	;
    gParaIdToFunctionDataBuff[13]	=	gPmParEst.IqKi	;
    gParaIdToFunctionDataBuff[14]	=	uReservedData ;
    
    gParaIdToFunctionDataBuff[15]	=	gPGData.PGDir	;
    gParaIdToFunctionDataBuff[16]	=	gPmParEst.CoderPos_deg	;
//    gParaIdToFunctionDataBuff[17]	=	gUVCoff.UDivVSave	;
    gParaIdToFunctionDataBuff[18]	=	gPmParEst.UvwDir	;
    gParaIdToFunctionDataBuff[19]	=	gPmParEst.UvwZeroAng_deg	;
}

void ParSendTuneBeforeRun(void)
{
    gParaIdToFunctionDataBuff[0]	=	gMotorExtReg.R1	;
    gParaIdToFunctionDataBuff[1]	=	gMotorExtReg.R2	;
    gParaIdToFunctionDataBuff[2]	=	gMotorExtReg.L0	;
    gParaIdToFunctionDataBuff[3]	=	gMotorExtReg.LM	;
    gParaIdToFunctionDataBuff[4]	=	gMotorExtReg.I0	;
    
    gParaIdToFunctionDataBuff[5]	=	gMotorExtReg.RsPm ;
    gParaIdToFunctionDataBuff[6]	=	gMotorExtReg.LD	;
    gParaIdToFunctionDataBuff[7]	=	gMotorExtReg.LQ	;
    gParaIdToFunctionDataBuff[8]	=	uReservedData	;
	gParaIdToFunctionDataBuff[9]    =   gMotorExtInfo.BemfVolt;
   
    gParaIdToFunctionDataBuff[10]	=	gPmParEst.IdKp	;
    gParaIdToFunctionDataBuff[11]	=	gPmParEst.IdKi	;
    gParaIdToFunctionDataBuff[12]	=	gPmParEst.IqKp	;
    gParaIdToFunctionDataBuff[13]	=	gPmParEst.IqKi	;
    gParaIdToFunctionDataBuff[14]	=	uReservedData ;
    
    gParaIdToFunctionDataBuff[15]	=	gPGData.PGDir	;
    gParaIdToFunctionDataBuff[16]	=	gPmParEst.CoderPos_deg	;
//    gParaIdToFunctionDataBuff[17]	=	gUVCoff.UDivVSave	;
    gParaIdToFunctionDataBuff[18]	=	gPmParEst.UvwDir	;
    gParaIdToFunctionDataBuff[19]	=	gPmParEst.UvwZeroAng_deg	;
}
