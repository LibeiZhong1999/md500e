/***************************************************************************
�ļ�˵���� 
�ļ����ܣ�
�ļ��汾��V1.0
���¸��£�2009.08.26
������־��
zhuozhe : linxib
***************************************************************************/
#ifndef MOTOR_PM_INCLUDE_H
#define MOTOR_PM_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif


//ͬ������ʼ�ż�λ���ƶϷ�ʽѡ��
#define INIT_POS_NULL			0		//Ӳ����ⷽʽ
#define INIT_POS_VOLT_PULSE		1		//��ѹ���巨


/*******************�ṹ�嶨��******************************************/
typedef struct IPM_ZERO_POS_STRUCT_DEF {
	s16	FirstPos;				//
	Uint	Cnt;
	//Uint	ABAngleBak;
	long	QepBak;
	int		DetectCnt;
    //int     UvwCnt;
	long	TotalErr;				//�ϴ��µ�ʱ��ĽǶ�

    Uint    CurLimit;
    Uint    time;
	Uint    zFilterCnt; 
	Uint    Flag;
}IPM_ZERO_POS_STRUCT;          //����ͬ����������������λ�ýǵ����ݽṹ

typedef struct IPM_INITPOS_PULSE_STRUCT_DEF {
	Uint	Waite;
	Uint	Step;					// ��־�� ����StepΪ1�𶯾�̬��ʶ��������Ϊ0
	Uint	Flag;					// 1�Ѿ������ż���ʼλ�ü��ı�־
	Uint    OkFlag;                 // ��ʼλ�ýǱ�ʶ�����������ظ���ʶ
	Uint	PeriodCnt;
	Uint	Section;
	Uint	PWMTs;
	Uint	PWMTs4L;
	Uint	PWMTsBak[12];
	Uint	InitPWMTs;
//	Uint	LPhase[3];				// ˳������������
	long	LPhase[3];				// ��ֹ���
	Uint    Ld;						// ���� >22����λ0.001mH������ <22����λ0.01mH
	Uint    Lq;						// ���� >22����λ0.001mH������ <22����λ0.01mH
//	Uint	CurLimit;
	Uint	CurLimitSmall;
	Uint	CurLimitBig;
	u16     CurLimitAdjSmall;            // ��ʼλ�ýǼ�����ϵ��
	u16     CurLimitAdjBig;            // ��ʼλ�ýǼ�����ϵ��
	int		Cur[12];
    int     PhsChkStep;             // ͬ����ȱ���ⲽ��
    Uint    InitTestFlag;           // ����ǰ�Ƿ���Ҫ�ż�λ�ü�� 0-��⣬1-�����,2-�ϵ��һ�μ��
    u16     ErrorIintPosEnable;
    s16     CurBase;
	//s16     CurBaseShow;
	s16     CurIU;
    s16     CurIV;
	s16     CurIW;
	Uint	WidthFlag;
	int		InitPosSrc;
	int		CurRem[24];
}IPM_INITPOS_PULSE_STR;        //����ͬ������ϵ��ʼλ�ü������ݽṹ(��ѹ���巨)

typedef struct PMSM_EST_PARAM_DEF
{
	Uint    IdKp;               // ������pi����
	Uint    IdKi;
	Uint    IqKp;
	Uint    IqKi;
	u16     IsKpK;              // PI����������
	u16     IsKiK;

    Uint    CoderPos_deg;       // ���������λ�ýǣ�
    Uint    EstZero;            // ���ر�ʶʱʹ�õ����λ�ý�
    
    //Uint    UvwZeroPhase_deg;   // UVW�ź����λ�ýǣ�
    Uint    UvwDir;             // ����������λ�÷��������� UVW��������ʾUVW�ź�����
    Uint    UvwZeroAng;         // uvw ���Ƕ�
    Uint    UvwZeroAng_deg;

    Uint    UvwZPos;
	Uint    SynTunePGLoad;  //ͬ�������ر�ʶ״̬
}PMSM_EST_PARAM_DATA;

typedef struct PMSM_EST_BEMF_DEF
{
    Uint    TuneFreq;           // ��¼���綯�Ʊ�ʶ����Ƶ��
    Ulong   TotalId1;           // ���ֵ�һ�����ֵ
    Ulong   TotalId2;           // ���ֵڶ������ֵ
    Ulong   TotalVq1;           // ���ֵ�һ���ѹֵ
    Ulong   TotalVq2;           // ���ֵڶ����ѹֵ

    int     IdSet;              // M ������趨
    int     IdSetFilt;
    int     IqSet;
    int     TuneFreqSet;        // ʵʱ���ٶȸ���
    int     TuneFreqAim;
    Ulong   AccDecTick;         // �Ӽ���ʱ��, 2ms Ticker����
    int     FreqStep;
    long    FreqRem;
    
    Uint    BemfVolt;           // ���綯�ƣ� ��ô����Ϊת�Ӵ���
    Uint    BemfVoltDisPlay;
    Uint    BemfVoltDisPlayLast;
    Uint    BemfVoltOnline;
	Uint    BemfVoltFilter;
	Ulong   BemfVoltDisPlayTotal;
    Uint    Cnt;
}PMSM_EST_BEMF;


//***************************************************************************
extern IPM_ZERO_POS_STRUCT		gIPMZero;
extern IPM_INITPOS_PULSE_STR	gIPMInitPos;
extern PMSM_EST_PARAM_DATA		gPmParEst;
extern PMSM_EST_BEMF            gEstBemf;


//***************************************************************************
extern void SynInitPosDetSetPwm(Uint Section);
extern void SynTuneInitPos(void);
extern void SynInitPosDetect(void);
extern void SynTunePGZero_No_Load(void);
extern void SynTunePGZero_Load(void);
extern void SetIPMPos(Uint Pos);
extern void SynTuneBemf(void);


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/

