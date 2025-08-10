/***************************************************************
�ļ����ܣ�ģ��������
�ļ��汾��VERSION 1.0
���¸��£�2009.09.27
************************************************************/
#ifndef MOTORINFO_COLLECT_INCLUDE_H
#define  MOTORINFO_COLLECT_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MotorInclude.h"
#include "MotorDefine.h"
// // �ṹ�嶨�� 
typedef struct CUR_EXCURSION_STRUCT_DEF{
	long	TotalIu;
	long	TotalIv;
	int		Iu;                             // ȥ��Ưǰ
	int		Iv;
    long    IuValue;                        //���ڲ�����ʶ
    long    IvValue;                        //���ڲ�����ʶ
	int		ErrIu;				            //U����Ư��С
	int		ErrIv;				            //V����Ư��С
	int  	Count;
	int  	EnableCount;
	int  	ErrCnt;
#if (SOFTSERIES == MD500SOFT)
	int		Iw;
	int		ErrIw;				            //W����Ư��С
	long	TotalIw;
	Ulong	TotalIb;
	Uint    Ib;
	Uint	ErrIb;
	int  	IbCount;
	int		IbOkFlag ;
	Uint    IbExcursionOkFlag;
#endif

}CUR_EXCURSION_STRUCT;                  //�����Ưʹ�õĽṹ

//	MD500���¶ȼ�ⷶΧ�и�ֵ�������¶ȵı���Ҫ������з������͡�
#if (SOFTSERIES == MD500SOFT)
typedef struct TEMPLETURE_STRUCT_DEF{
	Uint	TempAD;				            //AD��ȡֵ�������¶Ȳ��
	int	    Temp;				            //�öȱ�ʾ��ʵ���¶�ֵ
	int	    TempBak;			            //�öȱ�ʾ��ʵ���¶�ֵ
	Uint	ErrCnt;
	Uint    OverTempPoint;                  //��Ƶ�����µ�
}TEMPLETURE_STRUCT;                     //�ͱ�Ƶ���¶���ص����ݽṹ
#else
typedef struct TEMPLETURE_STRUCT_DEF{
	Uint	TempAD;				            //AD��ȡֵ�������¶Ȳ��
	Uint	Temp;				            //�öȱ�ʾ��ʵ���¶�ֵ
	Uint	TempBak;			            //�öȱ�ʾ��ʵ���¶�ֵ
	Uint	ErrCnt;
	Uint    OverTempPoint;                  //��Ƶ�����µ�
}TEMPLETURE_STRUCT;                     //�ͱ�Ƶ���¶���ص����ݽṹ
#endif

typedef struct AI_STRUCT_DEF {
	Uint 	gAI1;
	Uint 	gAI2;
    Uint    gAI3;

    Ulong   ai1Total;
    Ulong   ai2Total;
    Ulong   ai3Total;
#if(AIRCOMPRESSOR == 1)
    Uint    gAI4;
	Uint    gAI5;
    Ulong   ai4Total;
	Ulong   ai5Total;
#endif
    int     aiCounter;
}AI_STRUCT;	//
typedef struct ADC_ADJ_STRUCT_DEF{
    Uint XL;
    Uint XH;
    Uint     YL;
    Uint     YH;
    Ulong YLTotal;
    Ulong YHTotal;
    Uint     Slope;
    int     Offset;
    Uint     SlopePU;//q14
    Uint     Ticker;
}ADC_ADJ_STRUCT; // ADCУ���ṹ��
// // ���ⲿ���ñ������� 
extern ADC_STRUCT				gADC;		//ADC���ݲɼ��ṹ
extern UDC_STRUCT				gUDC;		//ĸ�ߵ�ѹ����
extern IUVW_SAMPLING_STRUCT	    gCurSamp;
extern UVW_STRUCT				gIUVWQ12;	//�����������������
extern UVW_STRUCT_Q24           gIUVWQ24;   //Q24��ʽ�����ඨ�ӵ���
extern ALPHABETA_STRUCT		    gIAlphBeta;	//�����������������
extern ALPHABETA_STRUCT		    gIAlphBetaQ12;	//�����������������
extern MT_STRUCT				gIMTQ12;    //MT��ϵ�µĵ���
extern MT_STRUCT                gIMTSetQ12;
extern MT_STRUCT_Q24            gIMTQ24;
extern AMPTHETA_STRUCT			gIAmpTheta;	//�������ʾ�ĵ���
extern LINE_CURRENT_STRUCT		gLineCur;	
extern CUR_EXCURSION_STRUCT	    gExcursionInfo;//�����Ưʹ�õĽṹ
extern TEMPLETURE_STRUCT		gTemperature;
extern AI_STRUCT				gAI;
extern ADC_ADJ_STRUCT           gADCAdjust;
extern PID_STRUCT               gWspid;

// // ���ⲿ���ú������� 
void GetCurExcursion(void);
void GetUDCInfo(void);
void ADCProcess(void);
void GetCurrentInfo(void);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================


