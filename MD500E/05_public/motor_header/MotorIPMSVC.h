/***********************************Inovance***********************************
����������Function Description��:
����޸����ڣ�Date����
�޸���־��History��:�����¼�¼Ϊ��һ��ת���Ժ󣬿�ʼ��¼��
	���� 		ʱ�� 		����˵��
1 	xx 		xxxxx 		xxxxxxx
2 	yy 		yyyyy 		yyyyyyy
************************************Inovance***********************************/
#ifndef MOTOR_IPM_SVC_INCLUDE_H
#define MOTOR_IPM_SVC_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DataTypeDef.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct PMSM_ROTOR_POS_EST_STRUCT_DEF{
	u16		Ka;                 //Q7   
	u16     Kb;		            //Q4   	
	u16		Kc;                 //Q10  �̶�ֵ768	 
    s32     TDivLdCoef;         //Q10
    s32     TDivLqCoef;
    s32   	KiForEmf;			//Q17
	s32		KiForSpeed;			//Q15
	s32		InvOfKf;            //Q10    
    s32     FcSetPer;           //�ز�Ƶ�ʱ�ôֵ Q10��ʽ
    s32 	TsPer;              //ʱ���ôֵ Q20    
	//s16     Id;
	s32  	IdLast;             //D������ϴβ���ֵ��Q12
	//s16     Iq;
	s32 	IqLast; 
	//s16     Ud;
	s32  	UdLast;             //D���ѹ�ϴμ���ֵ��Q12
	//s16     Uq;
	s32 	UqLast;
    s32 	IdEstValue;         //Id����ֵ Q12		
	s32 	IqEstValue;		
    s32 	DetaId;             //����ֵ����ʵֵ��ƫ�Q12	
	s32 	DetaIq;	    
	s32   	BmfEstValue;        //���綯�ƹ���ֵ��Q12
	s32   	BmfEstValueLast;    
	s32 	SpeedEstValue;	    //�ٶȹ���ֵ��Q15
    s32 	SpeedEstValueLast;    
	s32 	SpeedEstValueLpf;      
	s32 	SvcRotorSpeed;      //�����ٶȻ�������Ƶ�ʹ���ֵ��Q15       
	s32 	SvcRotorPos;        // ת��λ�ù���ֵ��Q32
    u16     FcLow;
	u16     FcLowPoint;
	u16 	SvcSpeedLpfTs;	     //�ٶ��˲�ʱ�䳣��
	u16 	SvcSpeedLpfCoef;	 //�ٶ��˲�ϵ��	
	u16     FcSetlpf;            //�˲�����ز�Ƶ��
    s16     SvcIdMaxForLowSpeed; //����ʱD��������ֵ �ٷֱ� ��������Ϊ��ֵ ��Χ 0��80
    s16     SvcIdSetForLowSpeed; //����ʱD���������ֵ Q12 ��������Ϊ��ֵ 
    u32     TimeBak;
    u32     SampleTime;
    u16     FcCoff;              //һ���ز����ڵ������κ͵���һ�ε�ϵ��

    s16     PosError;
    s16     SpeedError;

    s16     SvcLowSpeedMode;     //��Ƶ���з�ʽ��0- ������SVC, 1-ֻ�ڼ���ʱ��Ч,2-����ĳһƵ��ֵ��Ϊ��Ч��Ҳ���ǶԼ��٣����پ����
    s16     ImBrake;
    s16     BrakeFreq;
    s16     BrakeFlagD;         // 1-��Ƶ���д���ʽ��Ч
    s32     CurFreqSetReal;         // 0.0001Hz
    s16     Step;               // // 0.0001Hz  ÿ0.5ms�仯����
	u16     SpeedCheckEnable;  // ����SVC�ٶȸ���ģʽ
	s32     Ud;
    s32     Uq;
	u16     FCMode;
}PMSM_ROTOR_POS_EST_STRUCT;

extern void PmsmSvcCalFc(void);
//extern void ResetParForPmsmSvc(void);
extern void PmsmSvcCtrl(void);
extern void PmsmSvcCalImForLowSpeed();

extern PMSM_ROTOR_POS_EST_STRUCT   gPmsmRotorPosEst;
#define SvcSetRotorPos(x)   gPmsmRotorPosEst.SvcRotorPos = (((s32)(x))<<16)    //����gPmsmRotorPosEst.SvcRotorPos

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of definition

/******************************* END OF FILE***********************************/

