/***************************************************************
�ļ����ܣ�����ת��ʹ�õĽṹ����
�ļ��汾��VERSION 1.0
���¸��£�2008.05.29
************************************************************/
#ifndef MOTOR_CURRENT_INCLUDE_H
#define  MOTOR_CURRENT_INCLUDE_H
#ifdef  __cplusplus
extern "C" {
#endif

//********************************************************
#include "SystemDefine.h"
#include "MotorDefine.h"

typedef struct MT_STRUCT_DEF{
	int  	M;      // Q12
	int  	T;
}MT_STRUCT;	//MT��ϵ�µĵ�������ѹ�ṹ
typedef struct DQ_STRUCT_DEF{
	int  	D;      // Q12
	int  	Q;
}DQ_STRUCT;
typedef struct MT_STRUCT_Q24_DEF{
    long    M;
    long    T;
}MT_STRUCT_Q24;
typedef struct ALPHABETA_STRUCT_DEF{
	long  	Alph;
	long  	Beta;
}ALPHABETA_STRUCT;//���������������������ѹ�ṹ
typedef struct ADC_STRUCT_DEF{					
	Uint	DelaySet;			//ADC������ʱʱ��(0.1us��λ)
	Uint	DelayApply;			//ADC������ʱʱ��(PWM��ʱ�����ڵ�λ)
	//Uint  	ResetTime;			//ADC�Ѿ������Ĵ���
	long	ZeroTotal;
	int		ZeroCnt;
	int		Comp;
}ADC_STRUCT;	//�����������������
typedef struct UDC_STRUCT_DEF {
	int		DetaUdc;
	Uint	uDCBak;
	Uint 	uDC;				//ĸ�ߵ�ѹ			��λ0.1V
	Uint 	uDCFilter;			//���˲�ĸ�ߵ�ѹ	��λ0.1V
	u16     uDCShow;            //������ʾ��   ��λ0.1V
	Uint 	uDCBigFilter;		//��ѹ��Ƿѹ�ж���ĸ�ߵ�ѹ	��λ0.1V
	Uint 	Coff;				//ĸ�ߵ�ѹ����ϵ��
	Uint    uDCADCoff;          //AD����ֵ��ʵ��ֵ֮���ϵ��
	Uint    uDcCalMax;          //��������ѹʹ�õ�ĸ�ߵ�ѹ
}UDC_STRUCT;	//ĸ�ߵ�ѹ����
typedef struct IUVW_SAMPLING_STRUCT_DEF{					
	long  	U;					//Q24��ʽ���Ե�������Ϊ��ôֵ��ֵ
	long  	V;
	long  	W;
	long	UErr;				//U��ë���˲�
	long 	VErr;				//V��ë���˲�
	long    Coff;				//��������ת��Ϊ��ôֵ������ϵ��
}IUVW_SAMPLING_STRUCT;	//���������ṹ
typedef struct UVW_STRUCT_DEF_Q24{					
	long  	U;					//Q24��ʽ���Ե���ֵΪ��ôֵ��ֵ
	long  	V;
	long  	W;
}UVW_STRUCT_Q24;	//�����������������
typedef struct UVW_STRUCT_DEF{					
	int  	U;					//Q12��ʽ���Ե���ֵΪ��ôֵ��ֵ
	int  	V;
	int  	W;
}UVW_STRUCT;	//�����������������

typedef struct LINE_CURRENT_STRUCT_DEF{
	Uint  	CurPer;				//Q12 �Ե������Ϊ��ֵ��ʾ���ߵ�����Чֵ
	Uint  	CurBaseInv;			//Q12 �Ա�Ƶ������Ϊ��ֵ��ʾ���ߵ�����Чֵ
	Ulong  	CurPerFilter;		//Q12 �Ե������Ϊ��ֵ��ʾ���ߵ�����Чֵ
	Uint  	CurPerShow;			//Q12 �Ե������Ϊ��ֵ��ʾ���ߵ�����Чֵ
	int    CurTorque;          //Q12 �Ե������Ϊ��ֵ��ʾ��ת�ص���
	int    Temp;
	Uint  	ErrorShow;			//����ʱ�̼�¼���ߵ�����Чֵ
	Uint    Cur_Ft4;
	Uint    MaxCurLimit;        //��������ߵ���
	Uint    OverLoadMargin;
	Uint    NoOverLoadEN;         //���������ߵ�������

#if (SOFTSERIES == MD500SOFT)
    Uint    Cur_Ft16;
	Uint    CurBaseInvForG;
	Uint    CurBaseInvOverload;
#endif
    long    ImTotal;
	long    ItTotal;
	int     ImAvr;
	int     ItAvr;
	Uint    CurCnt;
}LINE_CURRENT_STRUCT;//���������ʹ�õ��ߵ�����ʾ
typedef struct AMPTHETA_STRUCT_DEF{
	Uint  	Amp;				//Q12
	int  	Theta;				//Q15
	int     ThetaFilter;        //Q15 ----DBcomp
	int     PowerAngle;         // �������ؽ�
	int     PowerAngle1;         // �������ؽ�
	int     PowerAngle2;         // �������ؽ�

    int     CurTmpM;
    int     CurTmpT;	
	int  	ThetaOld;				// wg
}AMPTHETA_STRUCT;//�������ʾ�ĵ�������ѹ�ṹ

/************************************************************/
/*******************���ⲿ���ú�������***********************/
void AlphBetaToDQ(ALPHABETA_STRUCT* , int , MT_STRUCT_Q24* );


#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif
/*===========================================================================*/
// End of file.
/*===========================================================================*/






