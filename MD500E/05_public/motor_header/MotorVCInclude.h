/***************************************************************
�ļ����ܣ���ʸ��������ص����ݽṹ���壬��������
�ļ��汾��VERSION 1.0
���¸��£�2007.09.27

************************************************************/
#ifndef MOTOR_VC_INCLUDE_H
#define MOTOR_VC_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SystemDefine.h"
#include "MotorDefine.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"
/************************************************************/
/************************���ݽṹ����************************/
typedef struct VC_INFO_STRUCT_DEF {
	Uint 	ASRKpLow;				//��Ƶ�ٶȻ�KP
	Uint 	ASRTILow;				//��Ƶ�ٶȻ�TI
	Uint 	ASRKpHigh;				//��Ƶ�ٶȻ�KP
	Uint 	ASRTIHigh;				//��Ƶ�ٶȻ�TI
	Uint 	ASRKpZero;				//��Ƶ�ٶȻ�KP
	Uint 	ASRTIZero;				//��Ƶ�ٶȻ�TI
	Uint 	ASRSwitchLow;			//��Ƶ�л�Ƶ��
	Uint 	ASRSwitchHigh;			//��Ƶ�л�Ƶ��
	Uint    ASRSwitchZero;			//��Ƶ�л�Ƶ��
	Uint    ZeroServer;             //���ŷ�ʹ��
	Uint    AntiDirEnable;          //ͣ������ת����ʹ��

	//Uint 	ACRKpLow;				//������KP
	//Uint 	ACRKiLow;				//������KI

    Uint    AcrImKp;        // ͬ�������첽������������
    Uint    AcrImKi;        // M �����������
    Uint    AcrItKp;        // T�����������
    Uint    AcrItKi;        // T�����������
    Uint    SvcMode;
    
    Uint    VcOverExc;              //ʸ������������
	Uint 	PosVCTorqueLim;			//VCת���޶�
	Uint    NegVCTorqueLim;         //����ת���޶�
	Uint 	VCWsCoff;				//VCת���
	Uint    VCSpeedFilter;			//VC�ٶȻ��˲�ʱ��
	Uint 	VCTorqFilter;			//SVCת���˲�ϵ��
	Uint    FreqWsCalMode;          //ת������������ģʽ
	Uint    FreqWsVolSub;           //��T���ѹ����ת��������ʱ����ѹ������
	Uint    FreqWsVolModify;        //��T���ѹ����ת��������ʱ���������趨ֵ	
	s16     TorMasToFol;
	s32     AsrOut;                 //�ٶȻ���� 
}VC_INFO_STRUCT;	//��ʸ��������صĲ����������ݽṹ
typedef struct UDC_LIMIT_IT_STRUCT_DEF {
    int    UDCLimit;           //����ĸ�ߵ�ѹ���������Ƶ�ת������
    int    UDCBak;
    int    UDCBakCnt;
    int    UDCDeta;
    int    FirstOvUdcFlag;     //��һ��ĸ�ߵ�ѹ����������ֵ
    PID_STRUCT  UdcPid;
}UDC_LIMIT_IT_STRUCT;	//�ٶȻ��������ݽṹ
typedef struct ASR_STRUCT_DEF {
	PID_STRUCT	Asr;
    Uint        AsrQpi;             // ��λ����λ:Qp��ʮλ:Qi��
    PID_STRUCT  TorqueLimitPid;     // ��ת�ؿ��ƣ�����������pid
	int  	KPHigh;
	int  	KPLow;
	u16     KPLowCoff;              // ��ͬ�������µ���Kpϵ����ͬ
	int     KPZero;
	int  	KIHigh;
	int  	KILow;
	int     KIZero;
	int  	SwitchHigh;
	int  	SwitchLow;
	int  	TorqueLimit;
	s16     NegTorqueLimit;
	s16     PosTorqueLimit;
	long    DetaPos;
	s32     FreqFeed;               /*����Ƶ��*/
    s32     FreqFeedFilter;         /*����Ƶ��*/
    s32     FreqSet;                /*�趨Ƶ��*/
    s32     DetaFreq;               /*Ƶ��ƫ��*/

    s32     Total;                  /*�����ۼ���*/
    s32     Out;                    /*���*/

	s32 	FreqSetStep;            /*�趨Ƶ��ÿ�ز����ڱ仯����*/
	s32 	PosSet;                 /*�趨Ƶ�ʵĻ���(�趨��Ƕ�)*/
	s32     PosSetAdd;              /*���ӷ���ת�������нǶ�*/

    s16     Kp;                     /*��ǰʹ�õ�KP*/
    s16     Ki;                     /*��ǰʹ�õ�KI*/
    s16     KiPos;                  /*�ٶȻ����κ��KI*/
    s16     KiPosBak;
    s16     OutFlag;                /*�����־:0�����ͣ�1�����ͣ�2������*/
    
	s16  	KpZero;
	s16  	KiZero;
	u16     Mode;                   // �ٶȻ�ģʽ
	s16     FreqSetMaxLimitByUdc;
}ASR_STRUCT;	//�ٶȻ��������ݽṹ
typedef struct VC_CSR_PARA_DEF{
    long   ImModify;            //����ʸ����ѹ��������ʱ��  
    Uint   ExecSetMode;         // 0 ���ŵ��������ٶ��趨��1 ��������ѹ����
    Uint   ImAcrKp;             //���ŵ�������ֵ������KP
    Uint   ImAcrKi;             //���ŵ�������ֵ������KI
    Uint   ImModefyAdd;         //���ŵ�������ֵ����������
    Uint   ImModefySub;         //���ŵ�������ֵ����������
    Uint   LmGain;              //���������б仯����
}VC_CSR_PARA;

typedef struct MODIFYWS_STRUCT_DEF{
	int    Faiq;
    Uint   WsMax;   //�����������趨ֵ
    Uint   Kp;      
    int    Amp;
	int    Wsout; 
    int    Tmp;
	int    Delta;
	int    tc;
 	int    tmp1;
 	int 	tmp2;               // SVC �Ż�����������
 	Uint    Theta;  
 	int    Ea;
 	int    Eb; 
 	int    Utheta;  
	int    Xztotal;
}MODIFYWS_STRUCT;

typedef struct PMSM_FLUX_WEAK_DEF  //ͬ��������
{
    long  VoltCoef;           
    long  CurrCoef;
    int  IqLpf;
    int   VoltLpf;
    long  VoltMax;
    long  Omg;                // ʵ�ʽ��ٶȣ�Q10��ʽ  
    Ulong Ld;
    Ulong Lq;
    Ulong Flux;
	int   FreqMax;
	int   IdMax;
    long  AdId;
	s16  AdFreq;
	long  IdForTorq;
    long  AdIdIntg;
	long  AdFreqIntg;
	int   DecFluxCoff;
	Uint  FluxWeakDepth;
    long  AdIq;
    long  AdIqIntg;
    long  IdMixAdjFlag;
    Uint  SalientRate;
    s16   SalientRateCoff;           // ͬ����͹������   
    s16   CalImAdjGain;                  // ͬ������ͨ��С���� 
	u16   PmsmMaxTorqCtrlEnable;        // ���ת�ص�����ʹ�ܿ���
}PMSM_FLUX_WEAK_STRUCT;
/************************************************************/
/************************������������************************/
extern VC_INFO_STRUCT			gVCPar;			//VC����
extern MT_STRUCT_Q24            gIMTSet;		//MT��ϵ�µ��趨����
extern MT_STRUCT_Q24            gIMTSetApply;	//MT��ϵ�µĵ���ָ��ֵ
extern MT_STRUCT				gUMTSet;		//MT��ϵ�µ��趨��ѹ
extern AMPTHETA_STRUCT			gUAmpTheta;		//���������趨��ѹ

extern PID32_STRUCT     gImAcrQ24;
extern PID32_STRUCT     gItAcrQ24;
extern PID32_STRUCT        gIMAcr;
extern PID32_STRUCT        gITAcr;

extern ASR_STRUCT				gAsr;			//�ٶȻ�
extern VC_CSR_PARA              gVcCsrPara;
extern MODIFYWS_STRUCT          gModifyws;
extern MT_STRUCT               gPWMVAlphBeta;
extern UDC_LIMIT_IT_STRUCT      gUdcLimitIt;
/************************************************************/
/*******************���ⲿ���ú�������***********************/
extern void ResetParForVC(void);
extern void CalIMSet(void);
extern void VCSpeedControl(void);
extern void VcCalABVolt(void);
extern void PrepImCsrPara(void);
extern void PrepPmsmCsrPrar(void);
extern void PrepareCsrPara(void);
extern void VCCsrControl(void);


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================

