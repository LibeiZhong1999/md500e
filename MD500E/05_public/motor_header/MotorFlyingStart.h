/***************************************************************************
�ļ�˵����
�ļ����ܣ�
�ļ��汾��
���¸��£�
������־��
***************************************************************************/
#ifndef MOTOR_FLYING_START_H
#define MOTOR_FLYING_START_H

#ifdef __cplusplus
extern "C" {
#endif


//***************************************************************************
#include "MotorInclude.h"
#include "SystemDefine.h"
#include "MotorDefine.h"


//***************************************************************************
#define	FLYING_START_TIME_BASE 1200
/***********************�ṹ�嶨��***************************/
typedef struct PMSM_FLYING_START_STRUCT_DEF{
	u16		Step;							// ת�ٸ���������
	u16		SubStep;						// ת�ٸ��ٸ�����
	s16     CurIU;							// ��·U�����
    s16     CurIV;							// ��·V�����
	s16     CurIW;							// ��·W�����
	s16		CurRemU[3];						// ���ڼ�¼���ζ�·������ֵ
	s16		CurRemV[3];						 
	s16		CurRemW[3];						 
	s16		CurRemAlpha[3];				// ���ζ�·������������任�Ľ��
	s16		CurRemBeta[3];
	s16		ThetaAB[3];						// ���ζ�·������Alpha - Beta ��ϵ�µĽǶ�
	s32		Tsc;							// ��·ʱ��
	s32		Ts[2];							// ������·ʱ����, ���71.58s * 2 * 48
	u16		CurLimit;						// ʵ�ʵ����޷�ֵ, Q12
	u16		CurLimitAdj;					// �������趨�ĵ����޷�ֵ, ��������ٷֱ�
	u16		PWMTs;							// ��ֵ��PWM���ڼĴ�����ֵ
	u16		Flag;							// ת�ٸ��ټ�����ɱ�־, 0-δ���, 1-�����
	s32		Freq[2];						// �������������Ƶ��, 0.01Hz
	s32		Freq01;							// ת�ٸ��ٱ�ʶ��Ƶ��, 0.01Hz
	s32		FreqPer;						// ת�ٸ��ٱ�ʶ��Ƶ��, Q15
	s16		Theta;							// ת�ٸ��ٱ�ʶ�ĽǶ�, Q16
	u32		DetaT;							// ��ʶ�����ʵ�ʷ������ʱ��, ���ڽ��нǶȲ���
	s16		ThetaEncoder;					// �����������ĽǶ�, ������֤�㷨
	s32		BmfEstValue;					// ת�ٸ��ٹ��Ʒ�����, Q12
	s16		FcChgFlag;						// ��ʶ��������Ƶ, ������, �ջ����������־, 1 - ��Ҫ����, 0 - ���账��
	s16		FcChgCnt;						// ��ʶ���������⴦���ʱ�����, ����Ĭ��Ϊ25, 50ms
	s16		StartStop;						// ת�ٸ��ٽ��������ͣ��/����״̬�ı�־, 0 - ͣ��, 1 - ����
	u16		PhaseLoseCnt;					// ���ȱ���жϴ�������
	s32		Delay1;							// ��һ���ζ�·ʱ������ֵ
	s32		Delay2;							// һ���Ρ������ζ�·ʱ�������ֵ
	u16		Record;							// �����з���ԭ���¼
	u16		KpRatio;						// ��ʱ������Kp����ϵ��
	u16		KiRatio;						// ��ʱ������Ki����ϵ��
}PMSM_FLYING_START_STRUCT;

/* Private function prototypes -----------------------------------------------*/
extern void RunCaseFlyingStart(void);
extern void SynFlyingStart(Ulong FullFreq01, Uint InvCurrent, Uint MotorCurrent);
extern void SynFlyingStartCalc(Ulong FullFreq01);
extern void SynFlyingStartSetTs(void);
extern void ADCEndIsrFlying_Start(void);
extern void FlyingStratPreparePar(void);
extern void FlyingStartParaAdjust(void);
extern void FlyingStartFcDeal(void);
extern void FlyingStartInitDeal(void);
//***************************************************************************
extern PMSM_FLYING_START_STRUCT gFlyingStart;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/



