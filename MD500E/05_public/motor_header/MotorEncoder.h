/***************************************************************************
�ļ�˵����
�ļ����ܣ�
�ļ��汾��
���¸��£�
������־��
***************************************************************************/
#ifndef MOTOR_ENCODER_INCLUDE_H
#define MOTOR_ENCODER_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif


//***************************************************************************
#include "MotorInclude.h"
#include "SystemDefine.h"
#include "MotorDefine.h"


//***************************************************************************
#define C_MAX_C_ZERO            200             //�����ұ�����Z�źŵ���ʱ��C�ź�����ƽ
#define C_MIN_D_ZERO            19000           //�����ұ�����Z�źŵ���ʱ��D�ź���С��ƽ
#define C_UVW_FOR_ZERO          5
#define C_UVW_BACK_ZERO         6

#define DIR_FORWARD   1
#define DIR_BACKWARD  -1
#define DIR_ERROR     0

//#define	RCK	(GpioDataRegs.GPBDAT.bit.GPIO34) // ��74HC594/165�����
#define GetQepCnt()			(*EQepRegs).QPOSCNT
#define SetQepCnt(X)		    (*EQepRegs).QPOSCNT = X;


// UVW��������UVW�źŶ���,ͨ��ģ����ABC�ĵ�ƽ�ж�UVW�źŵĸߵ�(UVW�ź�Ҳ��̧����1.5V)
#define Get_UVW_PG_U()	            (((u16)UVW_PG_U > 39719)?4:0)   //bit2 //65536��Ӧ3.3V ,39719��Ӧ2V
#define Get_UVW_PG_V()	            (((u16)UVW_PG_V > 39719)?2:0)   //bit1
#define Get_UVW_PG_W()	    	    (((u16)UVW_PG_W > 39719)?1:0)   //bit0
// ʡ�߷�ʽUVW��������UVW���� = ABZ�˿�
#define EXT_UVW_PG_U()	         GpioDataRegs.GPADAT.bit.GPIO20
#define EXT_UVW_PG_V()	         GpioDataRegs.GPADAT.bit.GPIO21
#define EXT_UVW_PG_W()	    	 GpioDataRegs.GPADAT.bit.GPIO23

#define C_Z_RESET_POS_LIMIT         0       // PG_DATA_STRUCT�ṹ��ZResetFlag��ֵ����
#define C_Z_RESET_POS_NOLIMIT       1   
#define C_Z_DONT_RESET_POS          2
#define C_MAX_DETA_POS_Z            (65536*45/360)      // ��׼Z�źŵ��������λ�ǶȺ͵�ǰ�Ƕ�ƫ��ķ�ֵ
#define C_MAX_DETA_POS_ABS          (65536*40/360)      // ͨ��UVW�źż��ľ���λ�ú͵�ǰλ��ƫ��ķ�ֵ
/***********************�ṹ�嶨��***************************/
typedef enum PG_TYPE_ENUM_STRUCT_DEF{  
    PG_TYPE_ABZ,                //��ͨABZ������
    PG_TYPE_UVW,                //��UVW�źŵ�ABZ��ֱ�����
    PG_TYPE_RESOLVER,           //��ת��ѹ��
    PG_TYPE_SC,                 //���������źŵ�ABZ��ֱ����� 
    PG_TYPE_SPECIAL_UVW,        //ʡ�߷�ʽUVW������
    PG_TYPE_NULL=100            //û�нӱ�����
}PG_TYPE_ENUM_STRUCT;       //����������

typedef enum QEP_INDEX_ENUM_STRUCT_DEF{
    QEP_SELECT_1,               //ѡ������QEP1���ڲ���
    QEP_SELECT_2,               //ѡ������QEP2���ڲ���
    QEP_SELECT_PULSEIN,         //ʹ��PULSE�������
    QEP_SELECT_NONE=100         //δѡ���������ٵ�QEPģ��
}QEP_INDEX_ENUM_STRUCT;

typedef struct PG_DATA_STRUCT_DEF {
	Uint 	PGType;
    QEP_INDEX_ENUM_STRUCT   QEPIndex;   //��ǰ����ʹ�õ�QEPģ�飬Ҫ����280xDSP������QEPģ��֮���л�

    Uint    PGMode;                     //��������ʽ�������ͷ�����ʽ��0Ϊ����ʽ
    Uint    PGTypeGetFromFun;           //���ܴ��ݵ�PG������
	Uint 	PulseNum;				    //����������
    Uint    SpeedDir;                   //ABZ,UVW����������AB�źţ�������������ź�
    Uint    SpeedDirLast;               //
    Uint    PGDir;                      //��ʶ�õ��ı������ٶ�������
    Uint    PGErrorFlag;                //��ʶ�õ��ı�����������Ϣ 0-����;1-δ��⵽������;2-�����������趨���� 

    Uint    imPgEstTick;                // im ��������ʶ������
    int     imDirAdder;
    long    imFreqErr;                  // ���ֱ������������
    long    imFrqEncoder;               // 
    u16 	PGTypeBak;              /*����������*/
    //s16     ZeroPos;                /*���������λ��(��������װ�Ƕ�,ͬ������)*/
    s32     RefPos;                 /*���������λ��,���㵽��Ƕȣ���׼λ�õ���󲻸�λ�Ƕȣ������ٶȻ�*/
    s32     RefPosLast;             /*���ڼ�¼��һ�ĵĵ�Ƕ�*/
    s16     RotorPos;               /*����������λ��,���㵽��Ƕȣ�ͬ����������*/
	s16     ZRotorPos;              /*z�źŵ���ʱ��������λ��*/
	u32     ZQepBak;                /*ΪZ�źŵ����¼������*/
	s16     ZRemainAbs;             /*ΪZ�źŵ����¼��������*/
    u16     Poles;                  /*������������(ֻ����������)*/
    u16     PGErrDetTime;           /*���������߼��ʱ��*/
    s16     MoveDircetion;              //��¼�����趨�����з���
    u16     DiscDetectDelayTime;    /*���������߼��ʱ��*/
	u16     Cnt;
}PG_DATA_STRUCT;	            //��������ز���

typedef struct ROTOR_SPEED_SMOOTH_DEF {
    int     LastSpeed;
    int     SpeedMaxErr;
}ROTOR_SPEED_SMOOTH;

typedef struct ROTOR_SPEED_STRUCT_DEF {
	int 	SpeedApply;			//ʵ��ʹ��ת���ٶ�
	s16     SpeedBigFilter;     // ���ڼ�����ʾ������ٶ�
	int 	SpeedEncoder;		// ͨ����������⵽��ת��Ƶ��, �����������
	Uint    OverSpeedEnable;    //������ʹ��
	int     SpeedTemp;          // ����ǰ����������ֵ
	int		SpeedApplyFilter;
	int		Flag;
    int     DetaTimer;
    int     DetaPhase;
    Uint    MTZeroCnt;		//
	//SVC���ٱ���
	int		SvcLastFluxPos;
	int		SvcSynSpeed;
	int		SvcWs;
	int		SvcRotorSpeed;
	Uint	SvcSignal;
	Ulong	SvcLastTime;
	Ulong	Timer;

    Uint    IntCnt;
    Uint    IntNum;
    
    Uint    TransRatio;         //������ٴ�����
	Uint	FirstCnt;
	s16     SpeedFeedQ12;           //�ٶȼ��ֵ  Q15��ʽ
	s32     SpeedFeedFilter;        //�˲�����ٶȼ��ֵ  Q24��ʽ
    s32     SpeedFeed;              //�ٶȼ��ֵ  Q24��ʽ
	s32     SpeedFeedLast;          //��¼�ϴε��ٶȼ��ֵ
    u32     PulseNum;               //�����õı���������
    u32     Pos;                    //��ǰλ��
    u32     Time;                   //��ǰʱ��
    s16     DetaPos;                //λ��ƫ��
	s16     DetaPosC;               //λ��ƫ���¼ֵ
	s16     DetaPosLast;            //�ϴε�λ��ƫ��
    u32     DetaTime;               //ʱ��ƫ��
	u32     DetaTimeLast;           //�ϴε�ʱ��ƫ��
    u32     DetaTimeAdd;            //��һ�β�׽��������Ҫ���ӵ�ʱ��
    u16     CapTimeLast;            //���ڼ�¼�ϴζ�ȡλ��ʱ�Ĳ���ֵ
    u16     CapTimeLast1;           //
    u16     PrescaFlag;
    u16     ChangeFlag;
    s32     Speed;
	s16     FreWindow;
	u16     SvcSpeed;              // svcģʽ���Ƿ�ͨ����������
}ROTOR_SPEED_STRUCT;	//�ٶȷ����������ݽṹ

typedef struct IPM_UVW_PG_STRUCT_DEF {
	Uint	UVWData;
	Uint    UVWExtData;     // ʡ��ʽ�������ϵ��ʼ�Ƕ�
	Uint	UVWAngle;       // UVW�źž���λ�ýǶ�, ���ڱ������͵����������ȣ���˱�ʾ��Ƕ�
	
	Uint    U_Value;
	Uint    V_Value;
	Uint    W_Value;
	Uint	ZeroCnt;

    Uint    LogicU;         // U�ź��߼�
    Uint    LogicV;         // V�ź��߼�
    Uint    LogicW;         // W�ź��߼�

    Uint	UvwDir;                 // UVW�ź�������

    Uint    debugdeta1;
    Uint    debugdeta2;
    
    Uint    lastAgl;
    Uint    NewAgl;
    Uint    ErrAgl;
    Uint    TuneFlag;
    
    llong   TotalErr;
    Ulong   UvwCnt;

    int     UvwZIntErr;      // Z�ж�uvw���
    int     UvwZIntErr_deg;
    
    Uint    UvwZeroPos;     // uvw�����λ�ýǶ�
    Uint    UvwZeroPos_deg;

    Uint    UvwEstStep;
    int     UvwCnt2;        // ��е���ڼ�����
	u16     ErrorEnable;    // UVW����ʹ��
}IPM_UVW_PG_STRUCT;//����ͬ�������UVW�����������ݽṹ

typedef struct IPM_PG_DIR_STRUCT_DEF {
	int	    ABAngleBak;
    int     ABDirCnt;
    int     ABDir;
	int	    UVWAngleBak;
    int     UVWDirCnt;
    int     UVWDir;
	int	    CDAngleBak;
    int     CDDirCnt;
    int     CDDir;
    int     CDErr;
    int     RtPhaseBak;
    int     RtDirCnt;
}IPM_PG_DIR_STRUCT; //����ͬ�����������ʶ����������߷�������ݽṹ

typedef struct ROTOR_TRANS_STRUCT_DEF{
    int     RealTimeSpeed;
	int     ConFlag;
    Uint    SimuZBack;
    Uint    SimuZBack2;

    Uint    IntNum;
    Uint    IntCnt;

	u32 	TimeBak;				//��һ�β��ٵĻ�׼ʱ��
	u32     SampleTime;
	u16     SampleRTPos;
	u32	    DetaTime;
    u16     ReadPos;
	u16		RTPos;                  // ��е�Ƕ�
	s32     AddPos;                // ����ۼӽ�(�첽����)
	u16		PosBak;
    u16		PosRTBak;
	s16		DetaPos;
    s16     Remain;
	s32		FreqFeed;
    s16     PosComp;
    
    u16    EstZero;
    u16    RtorBuffer; 
    u16    Poles;  
    u16    PolesRatio;  // ���伫��������
    u16    RtRealPos;          // �����ʱ��Ƕȣ����˲����Ƕ�֮��

    s16    AbsRotPos;          // �������λ�ã�0-4096
	int     PgPos;
    Ulong   QepBak; 
    u16    RtError;        // ������ϣ���Ҫ��δ�ӱ�������   
}ROTOR_TRANS_STRUCT; 	// �������ݽṹ


/* Private typedef -----------------------------------------------------------*/


#define QEPSetRefPos(x)     gPGData.RefPosLast = ((s32)(x)<<8)          //����gPGData.RefPos
#define QEPSetRotorPos(x)   gPGData.RotorPos = x                    //����gPGData.RotorPos
#define QEPGetRefPos()      (gPGData.RefPos)                        //����gPGData.RefPos
#define QEPGetRotorPos()    (gPGData.RotorPos)                      //����gPGData.RotorPos


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
#define MIN_DETA_QEP            4                       /*�������ٲ�����������*/

#define C_SPEED_M_METHOD        0                       /*���ٷ���ѡ����ΪVCGetFeedBackSpeed()����������ֵ*/
#define C_SPEED_MT_METHOD       1
#define OLDSPEED
/* Private function prototypes -----------------------------------------------*/
extern void    ResetPGType(void);
extern void    ResetSpeedGetReg(void);
extern void    GetFeedRotorSpeed(void);
extern void    GetFeedSpeedQEP(void);
extern s32     CalFeedSpeed(s16 DetaPos, u32 DetaTime);
extern void    QEPGetRealPos(void);              
extern void    QEPIndexIsr(void);

//***************************************************************************
extern volatile struct EQEP_REGS        *EQepRegs;
extern IPM_UVW_PG_STRUCT        gUVWPG;
extern PG_DATA_STRUCT			gPGData;
extern IPM_PG_DIR_STRUCT        gPGDir;
extern BURR_FILTER_STRUCT		gSpeedFilter;
extern CUR_LINE_STRUCT_DEF		gSpeedLine;
extern ROTOR_TRANS_STRUCT		gRotorTrans;
extern ROTOR_SPEED_STRUCT		gRotorSpeed;

extern void GetUvwPhase(void);
extern s16  JudgeABDir(void);
extern s16  JudgeUVWDir(void);
extern void GetMTTimeNum(void);
extern void GetMDetaPos(void);
extern void RotorTransCalVel(void);
extern u16  GetRotorTransPos(void);
extern void RotorTransSamplePos(void);
extern void VCGetFeedBackSpeed(void);
extern void ReInitForPG(void);
extern void InitSetQEP(void);

extern ROTOR_SPEED_STRUCT		gRotorSpeed;
extern PG_DATA_STRUCT			gPGData;
#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of definition

/*===========================================================================*/
// End of file.
/*===========================================================================*/

