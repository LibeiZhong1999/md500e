//=====================================================================
//
// �����빦�ܵĽӿ�
//
//=====================================================================


#include "f_funcCode.h"
#include "f_runSrc.h"
#include "f_main.h"
#include "f_frqSrc.h"
#include "f_io.h"
#include "f_menu.h"
#include "f_posCtrl.h"

#if F_DEBUG_RAM
#define DEBUG_F_INTERFACE                   0   // ����, ���ܽ���
#define DEBUG_F_POWER_ADD                   0   // �ۼƺĵ���
#elif 1
#define DEBUG_F_INTERFACE                   1   // ����, ���ܽ���
#define DEBUG_F_POWER_ADD                   1   // �ۼƺĵ���
#endif

#define DROOP_VOLTAGE_LEVEL 6500  // �´�����ĸ�ߵ�ѹ��׼
//========================================
// ���ܴ��ݸ����ܵ�����
Uint16 frq2Core;
Uint16 frqCurAim2Core;
Uint16 rsvdData;            // ��������
Uint16 driveCoeffFrqFdb;
Uint16 vcSpdLoopKp1;
Uint16 vcSpdLoopKp2;
Uint16 jerkFf;

extern Uint16 vfSeparateVol;

Uint16 speedMotor;      // 
Uint16 motorRun;        // �����������ٶ�

int32 frqRun;           // ����ʵ�ʷ�����Ƶ��(��VF���ԣ���ͬ��ת��), 0.01Hz
int32 frqVFRun;           // ����ʵ�ʷ�����Ƶ��(��VF���ԣ���ͬ��ת��), 0.01Hz
int32 frqVFRunRemainder;  // ������ֵ
Uint16 outVoltage;
Uint16 outCurrent;      // Q12
Uint16 generatrixVoltage;   // ��λ: 0.1V
int16 outPower;         // ��λΪ0.1KW
int16 outPowerDisp;     // ���������ʾֵ 0.1kw
Uint16 errorCodeFromMotor;

Uint16 errorsCodeFromMotor[2];
Uint16 motorErrInfor[5];

//#define OUT_CURRENT_FILTER_TIME 30  // ��������˲�ʱ��ϵ��
//LOCALF LowPassFilter currentInLpf = LPF_DEFALUTS;

Uint16 outCurrentDispOld;
//Uint16 PGErrorFlag;     // ������ʶ��PG�����ϱ�־  2-�����������趨����  1-δ�ӱ�����

// ���ܴ��ݸ����ܼ�ʱ
int16 gPhiRt;         // ���������Ƕ�
int16 gPhiRtDisp;     // ��ʾֵ
Uint16 pmsmRotorPos;  // ͬ����ת��λ��(����ʵʱ����)
Uint16 frqFdb;        // �������ת��
Uint16 frqFdbDisp;    // ��������ٶ���ʾֵ
int32  frqFdbTmp;
Uint16 frqFdbFlag;
Uint16 torqueCurrent;
Uint16 enCoderPosition;
Uint16 ABZPos;
Uint16 ZCount;        // Z�źż���
Uint16 antiVibrateCoeff;
Uint16 vfTransitoryNoStopFlag;
Uint16 motorRs;   // �����תȦ��
Uint16 motorRsreal;
Uint16 ABZPosPre; //
int32 curABZPulseNum;
Uint32 curABZPulseNumshow;
Uint16 onlineCoeff;     // ���߱�ʶ�ķ��綯��
Uint16 overLoaderPercent;

#if DEBUG_F_INTERFACE
void copyDataFunc2Motor2ms(void);
// ���ܴ��ݸ����ܵ�����
Uint16 currentOc;
Uint16 currentPu;
Uint16 spdLoopOut;                  // ���ܲ��ֵ��ٶȻ����


LOCALF Uint32 softOCTicker;         // ���������ʱ
extern Uint16 softOcDoFlag;

// ����==>���ܵĲ�����0.5ms
void copyDataFunc2Motor05ms(void)
{
    gSendToMotor05MsDataBuff[0] = dspMainCmd.all;                            // 0   ��������
    gSendToMotor05MsDataBuff[1] = dspMainCmd1.all;                           // 1   ��������1
    gSendToMotor05MsDataBuff[2] = tuneCmd;                                   // 2   ��гѡ��
    gSendToMotor05MsDataBuff[3] = motorFc.motorPara.elem.motorType;          // 3   �������
    gSendToMotor05MsDataBuff[4] = frq2Core;                                  // 4   ʵʱ�ٶȸ���
    gSendToMotor05MsDataBuff[5] = vfSeparateVol;                             // 5   VF����ʱ�������ѹ
    gSendToMotor05MsDataBuff[6] = motorFc.motorPara.elem.ratingVoltage;      // 6   ������ѹ
    gSendToMotor05MsDataBuff[7] = motorFc.motorPara.elem.ratingCurrent;      // 7   ��������
    gSendToMotor05MsDataBuff[8] = motorFc.motorPara.elem.ratingFrq;          // 8   ����Ƶ��
    gSendToMotor05MsDataBuff[9] = funcCode.code.vfCurve;                     // 9   VF����ѡ��
    gSendToMotor05MsDataBuff[10] = funcCode.code.ovGain;                      // 10  F9-03 ��ѹʧ������
    gSendToMotor05MsDataBuff[11] = funcCode.code.ovPoint;                     // 11  F9-04 ��ѹʧ�ٱ�����ѹ    
    gSendToMotor05MsDataBuff[12] = funcCode.code.ocGain;                      // 12  F9-05 ����ʧ������
    gSendToMotor05MsDataBuff[13] = funcCode.code.ocPoint;                     // 13  F9-06 ����ʧ�ٱ�������
}

// ����==>���ܵĲ�����2ms
// ���Բ��������������ֱ�Ӵ��ݸ�gSendToMotor2MsDataBuff[]�Ķ�Ӧλ��
void copyDataFunc2Motor2ms(void)
{
#if  DEBUG_F_INTERFACE
    gSendToMotor2MsDataBuff[0] = dspSubCmd.all;                             // 0    ����������
    gSendToMotor2MsDataBuff[1] = frqCurAim2Core;                            // 1    Ŀ��Ƶ��
    gSendToMotor2MsDataBuff[2] = funcCode.code.maxFrq;                      // 2    ���Ƶ��
    gSendToMotor2MsDataBuff[3] = funcCode.code.carrierFrq;                  // 3    �ز�Ƶ��
    gSendToMotor2MsDataBuff[4] = motorFc.motorPara.elem.ratingPower;        // 4    ��������

#if 1    
    memcpy(&gSendToMotor2MsDataBuff[5], &motorFc.motorPara.elem.ratingSpeed, 6);
#else 
    gSendToMotor2MsDataBuff[5] = motorFc.motorPara.elem.ratingSpeed;        // 5    ����ת��
    gSendToMotor2MsDataBuff[6] = motorFc.motorPara.elem.statorResistance;   // 6    ���ӵ���
    gSendToMotor2MsDataBuff[7] = motorFc.motorPara.elem.rotorResistance;    // 7    ת�ӵ���
    gSendToMotor2MsDataBuff[8] = motorFc.motorPara.elem.leakInductance;     // 8    ©��
    gSendToMotor2MsDataBuff[9] = motorFc.motorPara.elem.mutualInductance;   // 9   ����
    gSendToMotor2MsDataBuff[10] = motorFc.motorPara.elem.zeroLoadCurrent ;  // 10   ���ص���
#endif

#if 1    
    memcpy(&gSendToMotor2MsDataBuff[11], &motorFc.motorPara.elem.pmsmRs, 3);
#else 
    gSendToMotor2MsDataBuff[11] = motorFc.motorPara.elem.pmsmRs;             // 11   ͬ�������ӵ���
    gSendToMotor2MsDataBuff[12] = motorFc.motorPara.elem.pmsmLd;             // 12   ͬ����D����
    gSendToMotor2MsDataBuff[13] = motorFc.motorPara.elem.pmsmLq;             // 13   ͬ����Q����
#endif

    gSendToMotor2MsDataBuff[14] = motorFc.vcPara.vcSpdLoopKp1;               // 14   �ٶȻ�Kp1
    gSendToMotor2MsDataBuff[15] = motorFc.vcPara.vcSpdLoopTi1;               // 15   �ٶȻ�Ti1
    gSendToMotor2MsDataBuff[16] = motorFc.vcPara.vcSpdLoopKp2;               // 16   �ٶȻ�Kp2
    gSendToMotor2MsDataBuff[17] = motorFc.vcPara.vcSpdLoopTi2;               // 17   �ٶȻ�Ti2
    gSendToMotor2MsDataBuff[18] = motorFc.vcPara.vcSpdLoopChgFrq1;           // 18   �ٶȻ��л�Ƶ��1
    gSendToMotor2MsDataBuff[19] = motorFc.vcPara.vcSpdLoopChgFrq2;           // 19   �ٶȻ��л�Ƶ��2
    gSendToMotor2MsDataBuff[20] = motorFc.vcPara.vcSlipCompCoef;             // 20   VCת�������    
    gSendToMotor2MsDataBuff[21] = motorFc.vcPara.vcSpdLoopFilterTime;        // 21   �ٶȻ�����������˲�ʱ��
    gSendToMotor2MsDataBuff[22] = motorFc.pgPara.elem.encoderPulse;          // 22   ������������
    gSendToMotor2MsDataBuff[23] = motorFc.vcPara.vcOverMagGain;              // 23   ʸ�����ƹ���������
    gSendToMotor2MsDataBuff[24] = funcCode.code.loseLoadLevel;               // 24   ���ؼ��ˮƽ
    gSendToMotor2MsDataBuff[25] = funcCode.code.loseLoadTime;                // 25   ���ؼ��ʱ��
    gSendToMotor2MsDataBuff[26] = ABS_INT16(upperTorque);                    // 26   ת���޶�
    gSendToMotor2MsDataBuff[27] = motorFc.torqueBoost;                       // 27   ת������

#if 1    
    memcpy(&gSendToMotor2MsDataBuff[28], &funcCode.code.boostCloseFrq, 9);
#else 
    gSendToMotor2MsDataBuff[28] = funcCode.code.boostCloseFrq;               // 28   VF������ֹƵ��
    gSendToMotor2MsDataBuff[29] = funcCode.code.vfFrq1;                      // 29   VFƵ�ʵ�1
    gSendToMotor2MsDataBuff[30] = funcCode.code.vfVol1;                      // 30   VF��ѹ��1
    gSendToMotor2MsDataBuff[31] = funcCode.code.vfFrq2;                      // 31   VFƵ�ʵ�2
    gSendToMotor2MsDataBuff[32] = funcCode.code.vfVol2;                      // 32   VF��ѹ��2
    gSendToMotor2MsDataBuff[33] = funcCode.code.vfFrq3;                      // 33   VFƵ�ʵ�3
    gSendToMotor2MsDataBuff[34] = funcCode.code.vfVol3;                      // 34   VF��ѹ��3
    gSendToMotor2MsDataBuff[35] = funcCode.code.slipCompCoef;                // 35   VFת���
    gSendToMotor2MsDataBuff[36] = funcCode.code.vfOverMagGain;               // 36   VF����������
#endif
    
    gSendToMotor2MsDataBuff[37] = motorFc.antiVibrateGain;                   // 37   VF����������
    gSendToMotor2MsDataBuff[38] = funcCode.code.startBrakeCurrent;           // 38   ����ֱ���ƶ�����
    gSendToMotor2MsDataBuff[39] = funcCode.code.stopBrakeCurrent;            // 39   ͣ��ֱ���ƶ�����
    gSendToMotor2MsDataBuff[40] = funcCode.code.brakeDutyRatio;              // 40   �ƶ�ʹ����
    gSendToMotor2MsDataBuff[41] = funcCode.code.overloadGain;                // 41   ������ر�������
    gSendToMotor2MsDataBuff[42] = funcCode.code.foreOverloadCoef;            // 42   �������Ԥ����ϵ��
    gSendToMotor2MsDataBuff[43] = funcCode.code.softPwm;                     // 43   ���PWMѡ��
    //gSendToMotor2MsDataBuff[44] = funcCode.code.curSampleDelayComp;          // 44   ���������ʱ����
    gSendToMotor2MsDataBuff[45] = funcCode.code.uvPoint;                     // 45   Ƿѹ�� 
    gSendToMotor2MsDataBuff[46] = motorFc.pgPara.elem.pgType;                // 46   ����������
    gSendToMotor2MsDataBuff[47] = driveCoeffFrqFdb;                          // 47   ���ٴ�����
    gSendToMotor2MsDataBuff[48] = funcCode.code.inverterType;                // 48   ��Ƶ������
    gSendToMotor2MsDataBuff[49] = funcCode.code.inverterGpType;              // 49   GP����
    gSendToMotor2MsDataBuff[50] = funcCode.code.tempCurve;                   // 50   �¶�����ѡ��
    gSendToMotor2MsDataBuff[51] = funcCode.code.volJudgeCoeff;               // 51   ��ѹУ��ϵ��
    gSendToMotor2MsDataBuff[52] = funcCode.code.curJudgeCoeff;               // 52   ����У��ϵ��
    gSendToMotor2MsDataBuff[53] = funcCode.code.uvGainWarp;                  // 53   UV��������ƫ��
    gSendToMotor2MsDataBuff[54] = funcCode.code.svcMode;                     // 54   SVC�Ż�ѡ�� 0-���Ż�  1-�Ż�ģʽ1  2-�Ż�ģʽ2
    gSendToMotor2MsDataBuff[55] = funcCode.code.deadTimeSet;                 // 55   ����ʱ�����-1140Vר��
    gSendToMotor2MsDataBuff[56] = funcCode.code.speedTrackMode;              // 56   ת�ٸ���
    gSendToMotor2MsDataBuff[57] = funcCode.code.speedTrackVelocity;          // 57   ת�ٸ���
    gSendToMotor2MsDataBuff[58] = funcCode.code.pmsmRotorPos;                // 58   ͬ����ת��λ��
    gSendToMotor2MsDataBuff[59] = motorFc.pgPara.elem.enCoderPole;           // 59   ���伫����
    gSendToMotor2MsDataBuff[60] = motorFc.motorPara.elem.pmsmCoeff;          // 60   ͬ�������綯��ϵ��  

#if 1    
    memcpy(&gSendToMotor2MsDataBuff[61], &motorFc.vcPara.mAcrKp, 4);
#else     
    gSendToMotor2MsDataBuff[61] = motorFc.vcPara.mAcrKp;                     // 61  M�������Kp
    gSendToMotor2MsDataBuff[62] = motorFc.vcPara.mAcrKi;                     // 62  M�������Ki
    gSendToMotor2MsDataBuff[63] = motorFc.vcPara.tAcrKp;                     // 63  T�������Kp
    gSendToMotor2MsDataBuff[64] = motorFc.vcPara.tAcrKi;                     // 64  T�������Ki
#endif

    gSendToMotor2MsDataBuff[65] = motorFc.pgPara.elem.enCoderDir;            // 65  ���䷽��
    gSendToMotor2MsDataBuff[66] = motorFc.pgPara.elem.enCoderAngle;          // 66  ��������װ��
    //gSendToMotor2MsDataBuff[66] = motorFc.pgPara.elem.fvcPgLoseTime;          // 66  �ٶȷ���PG���߼��ʱ��
	gSendToMotor2MsDataBuff[67] = funcCode.code.pwmMode;                     // 67 DPWM�л�����Ƶ��
    gSendToMotor2MsDataBuff[68] = motorFc.pgPara.elem.uvwSignDir;            // 68 UVW�źŷ���
    gSendToMotor2MsDataBuff[69] = motorFc.pgPara.elem.uvwSignAngle;          // 69 UVW�ź����λ�ý�

#if 1    
    memcpy(&gSendToMotor2MsDataBuff[70], &motorFc.vcPara.weakFlusMode, 5);
#else  
    gSendToMotor2MsDataBuff[70] = motorFc.vcPara.weakFlusMode;               // 70 F2-18 ͬ��������ģʽ
    gSendToMotor2MsDataBuff[71] = motorFc.vcPara.weakFlusCoef;               // 71 F2-19 ͬ��������ϵ��
    gSendToMotor2MsDataBuff[72] = motorFc.vcPara.weakFlusCurMax;             // 72 F2-20 ������ŵ���
    gSendToMotor2MsDataBuff[73] = motorFc.vcPara.weakFlusAutoCoef;           // 73 F2-21 �����Զ���гϵ��
    gSendToMotor2MsDataBuff[74] = motorFc.vcPara.weakFlusIntegrMul;          // 74 F2-22 ���Ż��ֱ���
	//gSendToMotor2MsDataBuff[74] = motorFc.vcPara.spdCtrlBrakeTorqueEnable;          // 74 F2-22 �ٶ�ģʽ�·���ת��ʹ��ѡ��
#endif 

#if 1
    memcpy(&gSendToMotor2MsDataBuff[75], &funcCode.code.VFOverCurLimitPoint, 4);
#else
    gSendToMotor2MsDataBuff[75] = funcCode.code.VFOverCurLimitPoint;               // 75 F3-18   VF����ʧ�ٶ�������
    gSendToMotor2MsDataBuff[76] = funcCode.code.VFOverCurEnableSwitch;             // 76 F3-19   VF����ʧ��ʹ��
    gSendToMotor2MsDataBuff[77] = funcCode.code.VFOverCurKp;                       // 77 F3-20   VF����ʧ����������
    gSendToMotor2MsDataBuff[78] = funcCode.code.VFOverCurHighFreqComCoff;          // 78 F3-21   VF���ٹ���ʧ�ٶ�����������ϵ��    
#endif

#if 1
	memcpy(&gSendToMotor2MsDataBuff[79], &funcCode.code.VFOverVdcLimitPoint, 5);
#else
    gSendToMotor2MsDataBuff[79] = funcCode.code.VFOverVdcLimitPoint;               // 79 F3-22   VF��ѹʧ�ٶ�����ѹ
    gSendToMotor2MsDataBuff[80] = funcCode.code.VFOverVdcEnableSwitch;             // 80 F3-23   VF��ѹʧ��ʹ��
    gSendToMotor2MsDataBuff[81] = funcCode.code.VFOverVdcFreqKp;                   // 81 F3-24   VF��ѹʧ������Ƶ������
    gSendToMotor2MsDataBuff[82] = funcCode.code.VFOverVdcVoltKp;                   // 82 F3-25   VF��ѹʧ�����Ƶ�ѹ����
    gSendToMotor2MsDataBuff[83] = funcCode.code.VFOverVdcLimitFreq;                // 83 F3-26  ��ѹʧ�������������Ƶ��   
#endif

/*#if 1
	memcpy(&gSendToMotor2MsDataBuff[84], &funcCode.code.TransitoryNoStopEnable, 6);
#else
    gSendToMotor2MsDataBuff[84] = funcCode.code.TransitoryNoStopEnable;           // 84 F3-27  ˲ͣ��ͣʹ��
    gSendToMotor2MsDataBuff[85] = funcCode.code.TransitoryNoStopKp;               // 85 F3-28  ˲ͣ��ͣ����
    gSendToMotor2MsDataBuff[86] = funcCode.code.TransitoryNoStopKiCoff;           // 86 F3-29  ˲ͣ��ͣ����ϵ��
    gSendToMotor2MsDataBuff[87] = funcCode.code.TransitoryNoStopRecVoltage;       // 87 F3-30  ˲ͣ��ͣ�ָ���ѹ
    gSendToMotor2MsDataBuff[88] = funcCode.code.TransitoryNoStopVoltageJudgeTime; // 88 F3-31  ˲ͣ��ͣ��ѹ�ж�ʱ��  
    gSendToMotor2MsDataBuff[89] = funcCode.code.TransitoryNoStopDcVoltage;        // 89 F3-32  ˲ͣ��ͣ����ĸ�ߵ�ѹ   
#endif*/

    gSendToMotor2MsDataBuff[84] = funcCode.code.pOffTransitoryNoStop;             // 84 F9-59  ˲ͣ��ͣʹ��
    gSendToMotor2MsDataBuff[85] = funcCode.code.pOffTransitoryNoStopKp;           // 85 F9-71  ˲ͣ��ͣ����
    gSendToMotor2MsDataBuff[86] = funcCode.code.pOffTransitoryNoStopKiCoff;       // 86 F9-72  ˲ͣ��ͣ����ϵ��
    gSendToMotor2MsDataBuff[87] = funcCode.code.pOffTransitoryFrqDecSlope;        // 87 F3-60  ˲ͣ��ͣ�ָ���ѹ
    gSendToMotor2MsDataBuff[88] = funcCode.code.pOffVolBackTime;                  // 88 F3-61  ˲ͣ��ͣ��ѹ�ж�ʱ��  
    gSendToMotor2MsDataBuff[89] = funcCode.code.pOffThresholdVol;                 // 89 F3-62  ˲ͣ��ͣ����ĸ�ߵ�ѹ   


	gSendToMotor2MsDataBuff[90] = funcCode.code.VFOvShockMode;                    // 90  F3-12  ����������ģʽ
	gSendToMotor2MsDataBuff[91] = funcCode.code.VFWsComTimeCoff;                  // 91  F3-27  ת���ʱ�䳣��

#if 1
	memcpy(&gSendToMotor2MsDataBuff[92], &funcCode.code.VFAutoUpFreqEnableSwitch, 5);
#else
    gSendToMotor2MsDataBuff[92] = funcCode.code.VFAutoUpFreqEnableSwitch;         // 92 F3-28  �Զ���Ƶʹ��
    gSendToMotor2MsDataBuff[93] = funcCode.code.VFMinDriveCur;                    // 93 F3-29   ��С�綯���ص���
    gSendToMotor2MsDataBuff[94] = funcCode.code.VFMaxFdbCur;                      // 94 F3-30   ��󷢵����ص���
    gSendToMotor2MsDataBuff[95] = funcCode.code.VFAutoUpFreqKp;                   // 95 F3-31   �Զ���ƵKP
    gSendToMotor2MsDataBuff[96] = funcCode.code.VFAutoUpFreqKi;                   // 96 F3-32   �Զ���ƵKI
#endif
	gSendToMotor2MsDataBuff[97] = funcCode.code.torquepCompGain;                  // 97 F3-33    ����ת�ز�������
#if 1
	memcpy(&gSendToMotor2MsDataBuff[98], &funcCode.code.rsvdF3[0], 10);
#else
    gSendToMotor2MsDataBuff[98]  = funcCode.code.rsvdF3[0];                       // 98  F3-35  Ԥ��
    gSendToMotor2MsDataBuff[99]  = funcCode.code.rsvdF3[1];                       // 99  F3-36   
    gSendToMotor2MsDataBuff[100] = funcCode.code.rsvdF3[2];                       // 100 F3-37   
    gSendToMotor2MsDataBuff[101] = funcCode.code.rsvdF3[3];                       // 101 F3-38   
    gSendToMotor2MsDataBuff[102] = funcCode.code.rsvdF3[4];                       // 102 F3-39   

    gSendToMotor2MsDataBuff[103] = funcCode.code.rsvdF3[5];                       // 103 F3-35  
    gSendToMotor2MsDataBuff[104] = funcCode.code.rsvdF3[6];                       // 104 F3-36   
    gSendToMotor2MsDataBuff[105] = funcCode.code.rsvdF3[7];                       // 105 F3-37   
    gSendToMotor2MsDataBuff[106] = funcCode.code.rsvdF3[8];                       // 106 F3-38   
    gSendToMotor2MsDataBuff[107] = funcCode.code.rsvdF3[9];                       // 107 F3-39   

#endif
#if 1
	memcpy(&gSendToMotor2MsDataBuff[108], &funcCode.code.pdSearchCurKp, 6);
#else
    gSendToMotor2MsDataBuff[108] = funcCode.code.pdSearchCurKp;                   // 108 F6-16  ת�ٸ��ٱջ�����KP
    gSendToMotor2MsDataBuff[109] = funcCode.code.SpdSearchCurKi;                  // 109 F6-17  ת�ٸ��ٱջ�����KI 
    gSendToMotor2MsDataBuff[110] = funcCode.code.SpdSearchCur;                    // 110 F6-18  ת�ٸ��ٱջ�������С  
    gSendToMotor2MsDataBuff[111] = funcCode.code.SpdSearchScanCurDownLimit;       // 111 F6-19  ת�ٸ��ٱջ��������޶�ֵ  
    gSendToMotor2MsDataBuff[112] = funcCode.code.SpdSearchVoltUpTime;             // 112 F6-20  ת�ٸ��ٵ�ѹ����ʱ�� 
    gSendToMotor2MsDataBuff[113] = funcCode.code.SpdSearchOffMagTime;             // 113 F6-21  ȥ��ʱ��
#endif
#if 1
    memcpy(&gSendToMotor2MsDataBuff[114], &funcCode.code.rsvdF6[0], 5);
#else
    gSendToMotor2MsDataBuff[114] = funcCode.code.rsvdF6[0];                       // 108 F6-22  Ԥ��
    gSendToMotor2MsDataBuff[115] = funcCode.code.rsvdF6[1];                       // 109 F6-23   
    gSendToMotor2MsDataBuff[116] = funcCode.code.rsvdF6[2];                       // 110 F6-24   
    gSendToMotor2MsDataBuff[117] = funcCode.code.rsvdF6[3];                       // 111 F6-25    
    gSendToMotor2MsDataBuff[118] = funcCode.code.rsvdF6[4];                       // 112 F6-26   
#endif

#endif
    gSendToMotor2MsDataBuff[119] = ABS_INT16(brakeUpperTorque);                  // 119   �ٶȿ���ʱ�ƶ�ת������            

#if 1
    // ����ͬ��������Ʋ���
    if (MOTOR_SN_1 == motorSn)  // ��1���(F2-23~F2-51)
   	{
        memcpy(&gSendToMotor2MsDataBuff[120], &funcCode.code.vcparaM1.pmsmWeakCoef, sizeof(struct SYNCHRO_MOtOR_VC_PARA));
	}
	else if(MOTOR_SN_2 == motorSn)// ��1���(A2-66~F2-94)
	{
	    memcpy(&motorFc, &funcCode.all[GetCodeIndex(funcCode.code.motorFcM2.FluxWeakDepth)], sizeof(struct SYNCHRO_MOtOR_VC_PARA));
	}
	
#else
    gSendToMotor2MsDataBuff[120] = funcCode.code.pmsmPrecedentAble;          // F2-23ͬ���������ѹ����ԣ��
    gSendToMotor2MsDataBuff[121] = funcCode.code.dAcrPrecedentAble;          // F2-24ͬ������ʼλ�ýǼ�����
    gSendToMotor2MsDataBuff[122] = funcCode.code.startPositionCheckAble;     // F2-25��ʼλ���Ƿ���
    gSendToMotor2MsDataBuff[123] = funcCode.code.speedHoopMode;              // F2-26�ٶȻ�ģʽѡ��
    gSendToMotor2MsDataBuff[124] = funcCode.code.mostOutpowerCoef;           // F2-27����������ϵ��
    
    gSendToMotor2MsDataBuff[125] = funcCode.code.actinoPorcelainCoef;        // F2-28��������ϵ��
    gSendToMotor2MsDataBuff[126] = funcCode.code.PrecedentCompMode;          // F2-29ǰ������ģʽ
    gSendToMotor2MsDataBuff[127] = funcCode.code.PmParEstIsKpK;              // f2-30 ��гʱ������KP
    gSendToMotor2MsDataBuff[128] = funcCode.code.PmParEstIsKiK;              // F2-31��гʱ������KI
    gSendToMotor2MsDataBuff[129] = funcCode.code.IPMPosZSigable;             // F2-32 Z�ź�У��ʹ��

    gSendToMotor2MsDataBuff[130] = funcCode.code.PmsmSvcSpeedLpfTs;          // F2-33 ͬ����SVC�ٶ��˲�����
    gSendToMotor2MsDataBuff[131] = funcCode.code.PmsmSvcKb;                  // F2-34 ͬ����SVC�ٶȹ����������
    gSendToMotor2MsDataBuff[132] = funcCode.code.PmsmSvcKa;                  // f2-35 ͬ����SVC�ٶȹ����������
    gSendToMotor2MsDataBuff[133] = funcCode.code.PmsmSvcIdMaxLowSpeed;       // F2-36 ͬ����SVC��ʼ���ŵ����޷�
    gSendToMotor2MsDataBuff[134] = funcCode.code.PmsmSvcFcLow;               // F2-37 ͬ����SVC����ز�Ƶ��
    gSendToMotor2MsDataBuff[135] = funcCode.code.lowFrqRunMode;              // f2-38 ��Ƶ���з�ʽ
    gSendToMotor2MsDataBuff[136] = funcCode.code.lowFrqDealAble;             // F2-39 ��Ƶ��Ч
    gSendToMotor2MsDataBuff[137] = funcCode.code.lowChangeUpDown;            // f2-40 ��ƵƵ�ʲ���
    gSendToMotor2MsDataBuff[138] = funcCode.code.lowBreakCurrut;             // f2-41 ��Ƶ�ƶ�����  

	gSendToMotor2MsDataBuff[139] = funcCode.code.PmsmSvcspeedTrack;          // F2-42 ͬ����SVC�ٶȸ���
	gSendToMotor2MsDataBuff[140] = funcCode.code.zeroServoEnable;            // F2-43 ���ŷ�ʹ��
	gSendToMotor2MsDataBuff[141] = funcCode.code.switchFrq;                  // F2-44 �л�Ƶ��
	gSendToMotor2MsDataBuff[142] = funcCode.code.zeroServoKp;                // F2-45 ���ŷ��ٶȻ���������
    gSendToMotor2MsDataBuff[143] = funcCode.code.zeroServoKi;                // F2-46 ���ŷ��ٶȻ� ����ʱ��
	gSendToMotor2MsDataBuff[144] = funcCode.code.stopForbidReverse;          // F2-47 ͣ����ֹ��ת
	gSendToMotor2MsDataBuff[145] = funcCode.code.stopAngle;                  // F2-48 ͣ���Ƕ�
	gSendToMotor2MsDataBuff[146] = funcCode.code.F2_49;                      // F2-49
#endif
    gSendToMotor2MsDataBuff[147] = funcCode.code.breakDutyOutTime;           // F6-16 �ƶ��ܿ���ʱ��
    gSendToMotor2MsDataBuff[148] = funcCode.code.uVWPGErrorEnable;           // F9-71 UVW����������ʹ�� 
    gSendToMotor2MsDataBuff[149] = funcCode.code.initPosErrorEnable%10;      // F9-72 ��ʼλ�ù���ʹ��
    gSendToMotor2MsDataBuff[150] = funcCode.code.initPosErrorEnable/10;      // F9-73 �������λ�ýǵ�г����ʹ��
    // ���ӷ������� modify by libo-2103 2014-12-24 10:13:00
    gSendToMotor2MsDataBuff[151] = funcCode.code.vcparaM1.onlineTuneEnable;	 // F2-49 ���߲�����ʶ
    gSendToMotor2MsDataBuff[152] = funcCode.code.vcparaM1.onlineCoeffEnable; // F2-50 ���߷��綯��
    //gSendToMotor2MsDataBuff[153] = funcCode.code.overLoadEnable;		     // A9-00 ��������ʹ��
    //gSendToMotor2MsDataBuff[154] = funcCode.code.stemFrq;		             // A9-01 ��תƵ��
    //gSendToMotor2MsDataBuff[155] = funcCode.code.dormanFrq_Air;	         // A9-02 ��ѹ������Ƶ��
    gSendToMotor2MsDataBuff[153] = funcCode.code.errorAction[1]/100%10;      // F9-48 ��λ:��Ƶ������ѡ�� 0:����ͣ�� 1:��������
	//gSendToMotor2MsDataBuff[154] = funcCode.code.errorAction[0]%10;        // F9-47 ��λ:�������ѡ�� 0:����ͣ�� 1:��������
    //gSendToMotor2MsDataBuff[154] = funcCode.code.stemFrq;		             // F9-78 ��תƵ��
    gSendToMotor2MsDataBuff[155] = funcCode.code.lowerFrq;                   // F0-14 ����Ƶ��
    gSendToMotor2MsDataBuff[156] = motorFc.pgPara.elem.fvcPgLoseTime;
    gSendToMotor2MsDataBuff[157] = funcCode.code.overLoderCurve;
    gSendToMotor2MsDataBuff[158] = funcCode.code.vcparaM1.rateFrqPercent;
    gSendToMotor2MsDataBuff[159] = funcCode.code.overModulationFactor;
}

// ����==>���ܵĵ��Բ�����2ms
// ���Բ��������������ֱ�Ӵ��ݸ�gSendToMotor2MsDataBuff[]�Ķ�Ӧλ��
void copyDataFunc2CF2ms(void)
{
    
#if DEBUG_F_MSC_CTRL
    int16 masterTorqueUp;
    masterTorqueUp = (int16)(((int32)(int16)MasterSlaveCtrl.SlaveRcvTorque * 2000) / 10000);
    if(MasterSlaveCtrl.p2pRunDir^funcCode.code.runDir) // ����F0-09�෴
    {
        masterTorqueUp = -masterTorqueUp;
    }
#endif
    memcpy(&gSendToMotor2MsDataBuff[FUNC_TO_MOTOR_2MS_DATA_NUM], &funcCode.group.cf[0], CFNUM);
#if DEBUG_F_MSC_CTRL
    gSendToMotor2MsDataBuff[FUNC_TO_MOTOR_2MS_DATA_NUM + CFNUM] = funcCode.code.slaveWacthWindowFrq;
    gSendToMotor2MsDataBuff[FUNC_TO_MOTOR_2MS_DATA_NUM + CFNUM + 1] = masterTorqueUp;///����ת��,�ӻ�ֱ�ӵ���
#endif
#if DEBUG_MD500_SEARIS
   gSendToMotor2MsDataBuff[FUNC_TO_MOTOR_2MS_DATA_NUM + CFNUM + 2] = funcCode.code.brakeStartVoltage; // 107 F9-08�ƶ���ʼ��ѹ
#elif DEBUG_MD290_SEARIS
   gSendToMotor2MsDataBuff[FUNC_TO_MOTOR_2MS_DATA_NUM + CFNUM + 2] = funcCode.code.brakeStartVoltage; // 107 F9-08�ƶ���ʼ��ѹ
#else
   gSendToMotor2MsDataBuff[FUNC_TO_MOTOR_2MS_DATA_NUM + CFNUM + 2] = funcCode.code.brakeStartVoltage; // 107 F9-08�ƶ���ʼ��ѹ
#endif   


}                                                  
                                                   
// ���Բ��������������ֱ�Ӷ�gSendToFunctionDataBuff[]�Ķ�Ӧλ��
extern Uint16 errorInfoFromMotor;     
extern Uint16 motorCtrlTuneStatus;

void copyDataCore2Func2ms(void)
{
    dspStatus.all                  = gSendToFunctionDataBuff[0];             // 0    ״̬��
    motorCtrlTuneStatus            = gSendToFunctionDataBuff[1];             // 1    ������ʶ״̬��
    errorsCodeFromMotor[0]         = gSendToFunctionDataBuff[2];             // 2    ���ܹ��ϴ���1
    errorsCodeFromMotor[1]         = gSendToFunctionDataBuff[3];             // 3    ���ܹ��ϴ���2
    motorErrInfor[0]               = gSendToFunctionDataBuff[4];             // 4    ������ʾ��Ϣ1
    motorErrInfor[1]               = gSendToFunctionDataBuff[5];             // 5    ������ʾ��Ϣ2
    motorErrInfor[2]               = gSendToFunctionDataBuff[6];             // 6    ������ʾ��Ϣ3
    motorErrInfor[3]               = gSendToFunctionDataBuff[7];             // 7    ������ʾ��Ϣ4
    motorErrInfor[4]               = gSendToFunctionDataBuff[8];             // 8    ������ʾ��Ϣ5
    currentOc                      = gSendToFunctionDataBuff[9];             // 9    ����ʱ�ĵ���
    speedMotor                     = gSendToFunctionDataBuff[10];            // 10   ���Ƶ��
    outVoltage                     = gSendToFunctionDataBuff[11];            // 11   �����ѹ
    funcCode.code.radiatorTemp     = gSendToFunctionDataBuff[12];            // 12   ɢ�����¶�
    currentPu                      = gSendToFunctionDataBuff[13];            // 13   ��������ֵ���������������Ƶ���Ķ�������ϴ�ʱ�����������������ͬ.
    funcCode.code.motorSoftVersion = gSendToFunctionDataBuff[14];            // 14   DSP����汾�ţ�����FF-07
    aiDeal[0].sample               = gSendToFunctionDataBuff[15];            // 15   AI1�Ĳ���ֵ���Ѿ��˲�
    aiDeal[1].sample               = gSendToFunctionDataBuff[16];            // 16   AI2�Ĳ���ֵ���Ѿ��˲�
    aiDeal[2].sample               = gSendToFunctionDataBuff[17];            // 17   AI3�Ĳ���ֵ���Ѿ��˲�
    generatrixVoltage              = gSendToFunctionDataBuff[18];            // 18   ֱ��ĸ�ߵ�ѹ
    torqueCurrent                  = gSendToFunctionDataBuff[19];            // 19   ת�ص�������ֵ�Ǵ��ݹ����ġ�ʵ��ʹ�õĵ�����ֵ��
    outCurrent                     = gSendToFunctionDataBuff[20];            // 20   �������
    pmsmRotorPos                   = gSendToFunctionDataBuff[21];            // 21   ͬ����ת��λ��
    itDispP2pSlave                 = gSendToFunctionDataBuff[22];            // 22  �Ա�Ƶ���������۵����ת��
    motorRun                       = gSendToFunctionDataBuff[23];            // 23   �������Ƶ��  
    outPower                       = gSendToFunctionDataBuff[24];            // 24   �������
    itDisp                         = gSendToFunctionDataBuff[25];            // 25   ���ת��
    enCoderPosition                = gSendToFunctionDataBuff[26];            // 26   �����еλ��
    gPhiRt                         = gSendToFunctionDataBuff[27];                                    // 27   ����������
    ZCount                         = gSendToFunctionDataBuff[28];                                    // 28   Z�źż�����
    //antiVibrateCoeff             = gSendToFunctionDataBuff[29];            // 29   VF��ϵ��
    vfTransitoryNoStopFlag         = gSendToFunctionDataBuff[29];            // 29   VF˲ͣ��ͣƵ�ʽ�����־
	coreSoftWareVesion             = gSendToFunctionDataBuff[30];            // 30   ��8λ:ͨ���������8λ:��������
	coreNonStandardVesion          = gSendToFunctionDataBuff[31];            // 31   ��10λ:�Ǳ�ţ���6λ:�Ǳ���������
	coreTmpsoftNoAndVersion        = gSendToFunctionDataBuff[32];            // 32   ��10λ:��ʱ����ţ���6λ:��ʱ�������� 
	coreSoftType                   = gSendToFunctionDataBuff[33];  	         // 33   ��8λ:ͨ�û�Ǳ꣬��8λ:�Ƿ���ʱ�汾 
	onlineCoeff                    = gSendToFunctionDataBuff[34];  	         // 34   ��8λ:ͨ�û�Ǳ꣬��8λ:�Ƿ���ʱ�汾 
	overLoaderPercent              = gSendToFunctionDataBuff[35];            // 35   �����ۼ���
}

// ���ܵ����ܼ��Ӳ���
void copyDataCore2Uf2ms(void)
{
#if 1
    memcpy(&funcCode.group.uf[0], &gSendToFunctionDataBuff[MOTOR_TO_Func_2MS_DATA_NUM], 30);
#elif 1
    funcCode.group.uf[0]  = gSendToFunctionDataBuff[26];
...
    funcCode.group.uf[29] = gSendToFunctionDataBuff[55];
    
#endif    
}

void copyDataCore2Func05ms(void)
{
}


#elif 1

Uint16 currentPu;

#endif



void UpdateDataCore2Func(void);
void UpdateDataFunc2Core0p5ms(void);
void UpdateDataFunc2Core2ms(void);


#define CHECK_SPEED_DIR_NO     0   // δ�������������ٶȷ���
#define CHECK_SPEED_DIR_START  1   // ��ʼ�����������ٶȽ��?
#define CHECK_SPEED_DIR_END    2   // ��ɱ����������ٶȷ�����
#define CHECK_SPEED_DIR_QUITE  3   // �˳������������ٶȷ�����
//=====================================================================
//
// �������������ٶ���ʵ��ת�ٶ�Ӧ
// ͬ��:��������������Ϊ�������
// ����:��������������ȡ����Ϊ�������
//
//=====================================================================
void checkSpeedFdbDir(int32 frqFdb)
{   
    
    static Uint16 checkFlag = CHECK_SPEED_DIR_NO;  // �������ٶȷ�����״̬
    static Uint16 speedDir;
    static Uint16 speedDirBak = 0;
    static Uint16 speedFdbDirTcnt = 0;
    
    if (checkFlag == CHECK_SPEED_DIR_QUITE)
    {
        return;
    }
    
    // ͣ���ҷ����ٶ�Ϊ0ʱ�����������־
    if ((!runFlag.bit.run)
        && (!frqFdb)
        && (checkFlag == CHECK_SPEED_DIR_NO)
        )
    {
        checkFlag = CHECK_SPEED_DIR_START;         // ��ʼ���
    }

    if (runFlag.bit.run                                // ����������
        &&(checkFlag == CHECK_SPEED_DIR_START)         // ֮ǰ���жϷ���
        )
    {
        speedDirBak = speedDir;  // ����ǰһ������
        
        // ʵ������Ƶ���뷴��Ƶ�ʷ���һ��
        if ((int64)frqTmp*((int32)frqFdb) > 0) 
        {
            speedDir = 0;  // ����������������Ƶ��ͬ��
        }
        // ʵ������Ƶ���뷴��Ƶ�ʷ����෴
        else if ((int64)frqTmp*((int32)frqFdb) < 0) 
        {
            speedDir = 1;  // ����������������Ƶ�ʷ���
        }

        if (speedDir == speedDirBak)
        {
            speedFdbDirTcnt++;
        }
        else
        {
            speedFdbDirTcnt = 0;
        }
        
        // ��ɱ����������ٶȷ�����
        if (speedFdbDirTcnt > 2500)      // 5s�ڼ���Ϊһ�������־ʱ�жϸ�ֵΪ��ʵ����
        {
            checkFlag = CHECK_SPEED_DIR_END;
        }
    }

    if ((!runFlag.bit.run)
        &&(checkFlag == CHECK_SPEED_DIR_END)
        )
    {
        funcCode.code.speedFdbDir = speedDir;
        checkFlag = CHECK_SPEED_DIR_QUITE;    // �˳����
    }
}

//=====================================================================
//
// �������ܴ��ݸ����ܵĽ�������
//
//=====================================================================
Uint16 powerAddDec;

#define POWER_ADD_COUNT_DEC    5000
#define POWER_ADD_COUNT_INT    3600 // 100*3600*500/5000
void UpdateDataCore2Func(void)
{
    int32 tmp;
    int32 frqFdbDispTmp;
    static int32 outCurrentDispOldTmp;
    static LowPassFilter frqFdbLpf = LPF_DEFALUTS;
	static LowPassFilter outValLpf = LPF_DEFALUTS;


#if DEBUG_F_INTERFACE
    copyDataCore2Func2ms();     // ���ܴ��ݸ�����2ms
    copyDataCore2Func05ms();    // ���ܴ��ݸ�����0.5ms
   
#endif


#if F_DEBUG_RAM    //+= û�����ܳ���ʱ��ĵ���
    if (ERROR_DEALING)        // �����Ѿ��������ܽ�����������
    {
        errorCodeFromMotor = ERROR_NONE;     // errorCodeFromMotor = 0
    }

    if (dspMainCmd.bit.run && (!errorCodeFromMotor)) // �������������������û�д���
    {
        dspStatus.bit.run = 1;
    }
    else
    {
        dspStatus.bit.run = 1;
    }

    speedMotor = frq2Core;

    {
        static int16 k;
        
        if (++k >= (Uint16)(200/RUN_CTRL_PERIOD))  // _ms֮�󣬲Ž���Щ��־��Ϊ1
        {
            dspStatus.bit.runEnable = 1;
        }
    }

    currentPu = funcCode.code.motorParaM1.elem.ratingCurrent;  // currentPu

    speedMotor = frq2Core;  // ����ʱ�������ٶ����� �����ٶ�

    generatrixVoltage = 12345;      // ĸ�ߵ�ѹ
    outCurrent = 2048;              // �������, Q12
    outVoltage = 1<<12;             // �����ѹ, Q12
//    radiatorTemp = -9;              // ɢ�����¶�
#endif


#if DEBUG_F_MOTOR_FUNCCODE
#if !F_DEBUG_RAM
// ��ʾ������ʹ�ã�motor2func
    copyDataCore2Uf2ms();  // ���ܴ��ݸ����ܣ���ʾ
#endif
#endif

    // ��Ƶ��ʵ������Ƶ��
    frqRun = ((int32)(int16)speedMotor * (int32)frqPuQ15 + (1 << 14)) >> 15;

    if (motorFc.motorCtrlMode == FUNCCODE_motorCtrlMode_VF)
    {
        // �����ѹ��������ʱʹ�õ�Ƶ��
    	frqVFRun = ((int32)(int16)speedMotor * (int32)frqPuQ15) >> 15;


    	if (frqVFRun < frqRun)
    	{
    		frqVFRunRemainder = ((int32)(int16)speedMotor * (int32)frqPuQ15) - (frqRun<<15);
    	}
    	else
    	{
    	    // ������ֵ
    	    frqVFRunRemainder = ((int32)(int16)speedMotor * (int32)frqPuQ15) - (frqVFRun<<15);
    	}
    }
    else

    {
        frqVFRunRemainder = 0;
    }

    // ���ʵ��ת��,����������
    frqFdbTmp = ((int32)(int16)motorRun * (int32)frqPuQ15 + (1 << 14)) >> 15;

#define    OUT_FDB_FRQ_DISP_FILTER_TIME   30 
    frqFdbLpf.t = OUT_FDB_FRQ_DISP_FILTER_TIME;
    frqFdbLpf.in = (frqFdbTmp);
    frqFdbLpf.calc(&frqFdbLpf);
    // ��������ٶ�
    frqFdbDispTmp = frqFdbLpf.out;

#if 0

    // ��һ������ʱ�����������ٶȷ�����
    checkSpeedFdbDir(frqFdbDispTmp);    

    if (funcCode.code.speedFdbDir != funcCode.code.runDir)
#endif

#if 1
    if (funcCode.code.runDir)
    {
        frqFdbDispTmp = -frqFdbDispTmp;   // �����ٶ�ȡ����Ϊ�����ٶ�
    }    
    if((funcCode.code.speedDispPointPos/10) == 1)
    {
        int32 frqdisptmp;
        frqdisptmp = frqFdbDispTmp/10;
        if(ABS_INT32(frqdisptmp) > (Uint32)5000)
        {
            frqFdb = (frqFdbDispTmp > 0) ? (int16)5000 : (int16)-5000;
        }
        else
        {
            frqFdb = frqdisptmp;
        }
    }
    else        //��λС��ʱҲ��ʾ�����Ƶ����������Ƶ��500.00hz,������320.00hz by modfied yz1990 2014-07-11
    {
        if(ABS_INT32(frqFdbDispTmp) > (Uint32)9999)
        {
            frqFdb = frqFdb / 10;
			//frqFdb = (frqFdbDispTmp > 0) ? (int16)32000 : (int16)-32000;
        }
        else
        {
            frqFdb = frqFdbDispTmp;
        }  
    }

#else
    if (funcCode.code.runDir)
    {
        frqFdbDispTmp = -frqFdbDispTmp;   // �����ٶ�ȡ����Ϊ�����ٶ�
    }
    
    if(ABS_INT32(frqFdbDispTmp) > (Uint32)32000)
    {
        frqFdb = (frqFdbDispTmp > 0) ? (int16)32000 : (int16)-32000;
    }
    else
    {
        frqFdb = frqFdbDispTmp;
    }    
#endif
    // ���������Ƕ�
    gPhiRtDisp = (int32)gPhiRt*1800 >> 15;

#if DSP_2803X
    ABZPos = EQep1Regs.QPOSCNT & 0x0000FFFF;

    curABZPulseNum += (int16)(ABZPos - ABZPosPre);//*(1L-2L*(funcCode.code.runDir^motorFc.pgPara.elem.enCoderDir));
    ABZPosPre = ABZPos;
    //if (diFunc.f2.bit.clearMotorRs)
    //{
    //    curABZPulseNum = 0; // ����
    //}  

    curABZPulseNumshow = ABS_INT32(curABZPulseNum);  
    motorRs = curABZPulseNumshow/(4*motorFc.pgPara.elem.encoderPulse); 


#else
    if (motorFc.pgPara.elem.fvcPgSrc == FUNCCODE_fvcPgSrc_QEP2)
    {
        ABZPos = EQep2Regs.QPOSCNT & 0x0000FFFF;   
    }
    else
    {
        ABZPos = EQep1Regs.QPOSCNT & 0x0000FFFF;   
    }
#endif


    
#if DEBUG_F_INTERFACE
    if (speedMotor == frq2Core)  // ���������������
    {
        frqRun = frqDroop;
    }
#if 1
    if((funcCode.code.speedDispPointPos/10) == 1)
    {
        int32 frqdisptmp;
        frqdisptmp = frqRun/10;
        if(ABS_INT32(frqdisptmp) > (Uint32)5000)
        {
            frqRunDisp = (frqRun > 0) ? (int16)5000 : (int16)-5000;
        }
        else
        {
            frqRunDisp = frqdisptmp;
        }
    }
    else     //��λС��ʱҲ��ʾ�����Ƶ����������Ƶ��500.00hz,������320.00hz by modfied yz1990 2014-07-11
    {
        if(ABS_INT32(frqRun) > (Uint32)9999)
        {
            frqRunDisp = frqRun/10;
            //frqRunDisp = (frqRun > 0) ? (int16)32000 : (int16)-32000;
        }
        else
        {
            //funcCode.code.speedDispPointPos = 20;
            frqRunDisp = frqRun;
			
        }  
    }
    if (FUNCCODE_runDir_REVERSE == funcCode.code.runDir)
    {
        frqRunDisp = -(int32)frqRunDisp;
    }
#else
    if(ABS_INT32(frqRun) > (Uint32)32000)
    {
        frqRunDisp = (frqRun > 0) ? (int16)32000 : (int16)-32000;
    }
    else
    {
        frqRunDisp = frqRun;
    }  

    if (FUNCCODE_runDir_REVERSE == funcCode.code.runDir)
    {
        frqRunDisp = -(int32)frqRunDisp;
    }

#endif
    
#if DEBUG_F_POSITION_CTRL
    pcOriginDisp = pcOrigin / 4;    // ��ʾ4��Ƶ֮ǰ
#endif

// ��ʾ�����ٶȣ�0.1rpm
//    funcCode.group.f2[18] = ((int32)(int16)frqFdb * ((int32)maxFrq + 2000) * 3 + (1 << 14)) >> 15;
// ��ʾ�����ٶȣ�0.01Hz
//    funcCode.group.f2[18] = ((int32)(int16)frqFdb * ((int32)maxFrq + 2000) + (1 << 14)) >> 15;
//    funcCode.group.f2[18] = frqRun;

    outCurrentDispOld = outCurrentDisp;    // ������һ�ĵ�������������ϼ�¼ʹ��
//  currentInLpf.t = OUT_CURRENT_FILTER_TIME;   // �˲�ʱ��ϵ��
    tmp = ((int32)outCurrent * currentPu) >> 8;

#if 1
    if (ABS_INT32(outCurrentDispOldTmp - tmp) > 15) // ��С��ʾ����Ĳ���. 15/16
    {
        outCurrentDisp = (tmp + (1 << 3)) >> 4;
		outCurrentDisp = (Uint32)outCurrentDisp * funcCode.code.outCurrentGain/1000; // У��ϵ��

        outCurrentDispOldTmp = tmp;
    }
#else    
    // ���������ʾ�˲�
    currentInLpf.in = (tmp + (1 << 3)) >> 4;
    currentInLpf.calc(&currentInLpf);
    outCurrentDisp = currentInLpf.out;
#endif
    currentOcDisp = ((int32)currentOc * currentPu + (1 << 11)) >> 12;

    if(!funcCode.code.outVolMode)
    {
		//��ʵ�������ѹ
		//outVoltageDisp = ((int32)outVoltage * invPara.ratingVoltage + (1 << 11)) >> 12;
        outVoltageDisp = ((int32)outVoltage * motorFc.motorPara.elem.ratingVoltage + (1 << 11)) >> 12;
    }
    else
    {
		// ����VF�㷨��ʾ��ѹ
        outVoltageDisp = (Uint32)ABS_INT16((int16)frqRunDisp) * funcCode.code.motorParaM1.elem.ratingVoltage / funcCode.code.motorParaM1.elem.ratingFrq; 
#define    OUT_VOLTAGE_DISP_FILTER_TIME   40 
        outValLpf.t = OUT_VOLTAGE_DISP_FILTER_TIME;
        outValLpf.in = (outVoltageDisp);
        outValLpf.calc(&outValLpf);
        // �����ѹ
        outVoltageDisp = outValLpf.out;

        if(outVoltageDisp >= funcCode.code.motorParaM1.elem.ratingVoltage) // �޷�
        {
            outVoltageDisp = funcCode.code.motorParaM1.elem.ratingVoltage;
        }
    }

    // �������
    softOcDoFlag = 0;
    if(runFlag.bit.run &&(funcCode.code.softOCValue) && (outCurrentDisp >= ((Uint32)funcCode.code.softOCValue*motorFc.motorPara.elem.ratingCurrent/1000)))
    {
        // ����������ʱ��(������Ϊ�㲻���)
        if(softOCTicker < ((Uint32)funcCode.code.softOCDelay*TIME_UNIT_CURRENT_CHK / CORE_FUNC_PERIOD))
        {
            softOCTicker++;
        }
        else
        {
            softOcDoFlag = 1;
            // errorOther = ERRROR_SOFT_OC;  // �������
        }
    }
    else
    {
        softOCTicker = 0;
    }
    
#if DEBUG_F_POWER_ADD
    // �ۼƺĵ�������
    if (outPower > 0)   // Ϊ�����粻����
    {
        powerAddDec += outPower;                 // С������

        if (powerAddDec > POWER_ADD_COUNT_DEC)   // ��������+1
        {
            // С��������0
            powerAddDec = 0;
            // ����������1
            if (++funcCode.code.powerAddupInt >= POWER_ADD_COUNT_INT)
            {
                // �ﵽ1�ȵ�
                funcCode.code.powerAddupInt = 0;
                // �ۼƺĵ�����1
                funcCode.code.powerAddup++;
            }
        }
    }
#endif
    outPowerDisp = ((int32)(int16)outPower) * funcCode.code.outPowerGain / 1000;

    vcSpdLoopKp1 = funcCode.code.vcParaM1.vcSpdLoopKp1;
    vcSpdLoopKp2 = funcCode.code.vcParaM1.vcSpdLoopKp2;
#if DEBUG_F_POSITION_CTRL
    bPcErrorOk = 0;
    bPcErrorNear = 0;
#endif

    // bit0: 0-����  1-ͣ��
    // bit1��2:  0-���� 1-����  2-���� 
    // bit3: 0-���� 1-����
    invtStatus.bit.run = runFlag.bit.run;
    invtStatus.bit.accDecStatus = runFlag.bit.accDecStatus;
    invtStatus.bit.uv = bUv;
#endif

}
//---------------------------------380v  220  220  480  690  1140 ----------//
Uint16 droopCtrlVoltageTable[6] = {6500,3760,3760,8210,11800,19500};
//=====================================================================
//
// ���¹��ܴ��ݸ����ܵĽ������ݣ�0.5ms
//
//=====================================================================    
void UpdateDataFunc2Core0p5ms(void)
{
//    Uint16 voltageTmp;
    int32 droopValue; // �´���Χ
	static Uint16 frq2CoreTmp;
    
// �´�����
        if (funcCode.code.droopCtrl 
            && frq 
            && (generatrixVoltage < droopCtrlVoltageTable[invPara.volLevel]) 
            && (runStatus != RUN_STATUS_TUNE))             // ��Ϊ��г����
        {
            droopValue = ((Uint64)torqueCurrentAct * funcCode.code.droopCtrl) / motorFc.motorPara.elem.ratingCurrent;
            frqDroop = frq - (int64)frq * droopValue / 1000;//������ļ��㹫ʽ��MD380E˵�����157ҳ���´��ٶȼ���
            // ��ֹ�´����·���
            if(((int64)frqDroop*frq) <= 0)
            {
                frqDroop = 0; // ���ƹ�"0"
            }
                
        }
        else
        {
            frqDroop = frq;
        }

    
    frq2CoreTmp = (frqVFRunRemainder + (frqDroop << 15)) / ((int32)frqPuQ15);
#if 1
    if (((funcCode.code.ovGain) || (funcCode.code.ocGain))
            && (motorFc.motorCtrlMode == FUNCCODE_motorCtrlMode_VF)   // VF����
#if F_MOTOR_TYPE_MMSM
            && (motorFc.motorPara.elem.motorType != MOTOR_TYPE_PMSM)  // ��ͬ����
#endif
            && (!funcCode.code.droopCtrl)                             // �´�������Ч
            && (runStatus != RUN_STATUS_TUNE)                         // �����ڵ�г״̬
            && (runFlag.bit.accDecStatus == DEC_SPEED)
			&& (ABS_INT16((int16)frq2CoreTmp) >= ABS_INT16((int16)speedMotor))
        ) // ��ѹʧ�٣�����ʧ�����涼Ϊ0
    {
        //frq2Core = (frqVFRunRemainder + (frqDroop << 15)) / ((int32)frqPuQ15); 
    }
    else
    {
        if(funcCode.code.motorCtrlMode == FUNCCODE_motorCtrlMode_SVC)
        {
            if(dspStatus.bit.onlineTuneStatus == 1)
            {
                frq2Core = frq2CoreTmp;
            }
            else
            {
                frq2Core = 0;
            }
        }
        else
        {
            frq2Core = frq2CoreTmp;
        }
    }
#endif

#if DEBUG_F_INTERFACE  
#if !F_DEBUG_RAM
    copyDataFunc2Motor05ms();  // ���ܴ��ݸ����ܣ�0.5ms  
#endif
#endif
}


//=====================================================================
//
// ���¹��ܴ��ݸ����ܵĽ������ݣ�2ms
//
//=====================================================================
extern Uint32 aptpSetOrigin;
void UpdateDataFunc2Core2ms(void)
{
#if DEBUG_F_INTERFACE      
    frqCurAim2Core = ((frqCurAim << 15) + frqCurAimFrac) / ((int32)frqPuQ15); // ��ǰ��Ŀ��Ƶ��  <<15����������۱�ֵ��Q15��ʽ
//����������frqCurAim2Core=13653    frqCurAim=5000��          frqCurAimFrac=0��frqPuQ15=12000     13653=��5000*32768+0��/12000
//                                                                   �������Ŀ��Ƶ��*100            С����                 ���Ƶ��*100+2000
    // ������
    // �����ܴ������� 0.5ms
    dspMainCmd.bit.motorCtrlMode = motorFc.motorCtrlMode;
    dspMainCmd.bit.accDecStatus = runFlag.bit.accDecStatus;     // ����runFlag�ļӼ��ٱ�־������dspMainCmd�ļӼ��ٱ�־
    dspMainCmd.bit.torqueCtrl = runFlag.bit.torque;             // ת�ؿ���
    dspMainCmd.bit.spdLoopI1 = motorFc.vcPara.spdLoopI;         // ���ַ���
    // ����������(2ms)    
    dspSubCmd.bit.outPhaseLossProtect = funcCode.code.outPhaseLossProtect%10;  // ���ȱ�ౣ��(��λ)
    dspSubCmd.bit.outPhaseLossProtect_beforeRun = funcCode.code.outPhaseLossProtect/10;  // ����ǰ���ȱ�ౣ��(ʮλ)
	dspSubCmd.bit.inPhaseLossProtect = funcCode.code.inPhaseLossProtect%10; // ����ȱ�ౣ��
    dspSubCmd.bit.contactorMode = funcCode.code.inPhaseLossProtect/10;      // �Ӵ������ϱ���
    dspSubCmd.bit.overloadMode = funcCode.code.overloadMode;                // ������ر���
    dspSubCmd.bit.loseLoadProtectMode = funcCode.code.loseLoadProtectMode;  // ������ر���ʹ�ܱ�־
    dspSubCmd.bit.poffTransitoryNoStop = funcCode.code.pOffTransitoryNoStop;// ˲ͣ��ͣ
    //dspSubCmd.bit.overModulation = funcCode.code.overModulation;            // ������ʹ��
    //dspSubCmd.bit.fieldWeak = funcCode.code.fieldWeak;                      // ���ſ���
    dspSubCmd.bit.cbc = funcCode.code.cbcEnable;                            // ������ʹ��
    //dspSubCmd.bit.narrowPulseMode = funcCode.code.narrowPulseMode;          // խ�������ѡ��
    //dspSubCmd.bit.currentSampleMode = funcCode.code.currentSampleMode;      // ��������˲����޳�ë�̣�ѡ��
    dspSubCmd.bit.varFcByTem = funcCode.code.varFcByTem;                    // �ز�Ƶ�����¶ȵ�����MD280һֱ��Ч
    //dspSubCmd.bit.pmsmInitPosNoSame = funcCode.code.pmsmInitPosNoSame;
    //dspSubCmd.bit.pmsmZeroPosBig = funcCode.code.pmsmZeroPosBig;
    // ��������1(0.5ms)
    dspMainCmd1.bit.pgLocation = motorFc.pgPara.elem.fvcPgSrc;            // FVC��PG��ѡ��, 0-QEP1,1-QEP2(��չ)
    // �������A0�鹦����
    //dspMainCmd1.bit.pwmMode = funcCode.code.pwmMode;                // PWMģʽѡ��. 
    dspMainCmd1.bit.modulationMode = funcCode.code.modulationMode;  // ���Ʒ�ʽ
    dspMainCmd1.bit.deadCompMode = funcCode.code.deadCompMode;      // ��������ģʽѡ��
    dspMainCmd1.bit.frqPoint = funcCode.code.frqPoint;              // Ƶ��ָ�λ
    dspMainCmd1.bit.shortGnd_beforeRun = funcCode.code.shortCheckMode/10; //����ǰ�Եض�·����־(ʮλ)

    //jerkFf = (((int64)jerk * 32 * (int64)funcCode.code.servoKp2) << 15) / ((int32)maxFrq + 2000) / 100;

    driveCoeffFrqFdb = 1000;
#if !F_DEBUG_RAM

    // ��ѹ���Ƶ����
    //GetOverUdcPoint();
    copyDataFunc2Motor2ms();  // ���ܴ��ݸ����ܣ�2ms  
#else    
#endif

#if DEBUG_F_MOTOR_FUNCCODE
#if !F_DEBUG_RAM
// ����ʹ�ù����룬func2motor 
    copyDataFunc2CF2ms();  // ���ܴ��ݸ�����
#endif
#endif
#endif
}







