/***********************************Inovance***********************************
����������Function Description��: SVPWMģ��ͷ�ļ�������SVPWMģ�������õ��ṹ��
�������ͺ���غ궨�壬����Svpwm.c�ļ��еĽṹ������ͺ�����������
����޸����ڣ�Date����2011-04-28
�޸���־��History��:�����¼�¼Ϊ��һ��ת���Ժ󣬿�ʼ��¼��
	���� 		ʱ�� 		����˵��
1 	xx 		xxxxx 		xxxxxxx
2 	yy 		yyyyy 		yyyyyyy
************************************Inovance***********************************/
#ifndef SVPWM_INCLUDE_H
#define  SVPWM_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DataTypeDef.h"
//#include "SubPrgInclude.h"

/* Private typedef -----------------------------------------------------------*/
/*typedef struct PWM_OUT_STRUCT_DEF {
	u16		Phase;			//�����λ
	s16		Ratio;			//����ϵ��
	u16		PWMPrd;			//��ǰ��Ч��PWM����
	u16     SoftPWMFlag;    //���PWM�Ƿ���Ч��ѡ��
	u16		PWMModle;		//CPWM/DPWMѡ��
	u16		OverModule;		//������ѡ��
	u16		MinPwmCtl;		//�Ƿ����խ�������ѡ��
	s16		DeadBand;		//����ʱ��
	u16     ZeroLengthPhase;//��¼DPWM��������һ��Ϊȫ����
	s32		U;				//U��PWM����Ƚ�ֵ
	s32		V;				//V��PWM����Ƚ�ֵ
	s32		W;				//W��PWM����Ƚ�ֵ
	
	s32		USet;			//U��PWM����Ƚ�ֵ
	s32		VSet;			//V��PWM����Ƚ�ֵ
	s32		WSet;			//W��PWM����Ƚ�ֵ
}PWM_OUT_STRUCT;	        //��ΪPWM����Ľṹ

*/
/* Private define ------------------------------------------------------------*/
//extern PWM_OUT_STRUCT   gPWM;

/* Private macro -------------------------------------------------------------*/
//#define MODLE_CPWM		    0		//��������
//#define MODLE_DPWM		    1		//��ɢ����
#define OVER_MODULE_CANCEL	0		//�����Ʋ���Ч
#define OVER_MODULE			1		//��������Ч

#define NARROW_PWM_CANCEL	0		//ȡ��խ�������
#define NARROW_PWM			1		//խ�������

#define	ZERO_VECTOR_U		0		//DPWM����ʱ��U�෢ȫ�����������
#define	ZERO_VECTOR_V		1		//DPWM����ʱ��V�෢ȫ�����������
#define	ZERO_VECTOR_W		2		//DPWM����ʱ��W�෢ȫ�����������
//#define	ZERO_VECTOR_NONE	100		//û�з�ȫ��������������

/* Private function prototypes -----------------------------------------------*/
extern void OutPutPWM(void);
extern void DeadBandComp(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of definition

/******************************* END OF FILE***********************************/

