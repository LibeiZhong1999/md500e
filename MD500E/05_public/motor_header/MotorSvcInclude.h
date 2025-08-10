/***************************************************************
�ļ����ܣ�����ʸ������
�ļ��汾��VERSION 1.0
���¸��£�2009.09.27
************************************************************/
#ifndef MOTOR_SVC_INCLUDE_H
#define MOTOR_SVC_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MotorInclude.h"
/************************************************************/
/***********************�ṹ�嶨��***************************/
typedef struct SVC_FLUX_CAL_STRUCT_DEF {
	long 	FilterTime;
	Uint 	SampleTime;
}SVC_FLUX_CAL_STRUCT;	//�����ͨ�õı�������

typedef struct FLUX_STRUCT_DEF {
	Uint	Amp;
	Uint	Theta;
}FLUX_STRUCT;	//��ͨ�۲����õı�������

/************************************************************/
//	ͬƵ�����˲�
typedef struct  {
	long  PutIn_Q24;
	long  Delta_Q24;
	long  PutOut_Q24;
	long  PutOut_II_Q24;
	long  PutOutLast_Q24;
	long  PutOut_I_Q24;
	long  Ts_Q24;
	long  K1_Q0;
	long  K2_Q4;
	long  FreqSyn_Mpy100;
	void  (*calc)(void *);   
}P_FILTER;

#define P_FILTER_DEF {0,0,0,0,0,0,0,0,0,0,\
	(void (*)(void *))PFilter}
extern P_FILTER gPfilter_ObsIT;
void PFilter(P_FILTER *p);

/************************************************************/


#ifdef __cplusplus
}
#endif /* extern "C" */
#endif  // end of definition

