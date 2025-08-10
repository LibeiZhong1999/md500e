/*****************************************************************
 *
 * �����붨���ͷ�ļ�
 * 
 * Time-stamp: <2008-08-20 08:57:13  Shisheng.Zhi, 0354>
 * 
 *
 *
 *
 *****************************************************************/

#ifndef __F_FUNCCODE_H__
#define __F_FUNCCODE_H__


#if defined(DSP2803X)         // 2803x����2808ƽ̨
//#include "DSP28x_Project.h"
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File 
#else
#include "DSP280x_Device.h"
#include "DSP280x_Examples.h"
#endif

#include "f_debug.h"
#include "f_interface.h"
#include "f_p2p.h"

 

// �벻Ҫ�����޸�������ֵ������EEPROM�е�ֵ��ȫ��(����P0�顢P1�顢���ּ�¼)�ָ�����ֵ��
#define EEPROM_CHECK1       (0x0022)                        // EEPROMУ����1
#define EEPROM_CHECK2       (0xFFFF - EEPROM_CHECK1)        // EEPROMУ����2




#define USER_MENU_GROUP     FUNCCODE_GROUP_E4

// ��ù�����code��ȫ���������е�index����������������funcCode.all[]�е��±ꡣ
// ���� FUNCCODE_ALL �Ķ��壬һ��������ı�ʾ��4�ְ취����Ӧ�Ļ��index�İ취��
// 1. funcCode.all[i]     ---- i
// 2. funcCode.f5[7]      ---- GetCodeIndex(funcCode.group.f5[7])
// 3. group, grade        ---- GetGradeIndex(group, grade)
// 4. funcCode.code.maxFrq---- GetCodeIndex(funcCode.code.maxFrq)
#define GetCodeIndex(code)    ((Uint16)((&(code)) - (&(funcCode.all[0]))))
#define GetGradeIndex(group, grade)  (funcCodeGradeSum[group] + (grade))


//=====================================================================
// ����������λ����
//
// ��λ
#define ATTRIBUTE_UNIT_HZ_BIT           0
#define ATTRIBUTE_UNIT_A_BIT            1
#define ATTRIBUTE_UNIT_V_BIT            2

// ��д����
#define ATTRIBUTE_READ_AND_WRITE        0   // (�κ�ʱ��)��д
#define ATTRIBUTE_READ_ONLY_WHEN_RUN    1   // ����ʱֻ��
#define ATTRIBUTE_READ_ONLY_ANYTIME     2   // ֻ��

// �๦�����������
#define ATTRIBUTE_MULTI_LIMIT_SINGLE    0   // �����Ĺ�����
#define ATTRIBUTE_MULTI_LIMIT_DEC       1   // ��������룬ʮ����
#define ATTRIBUTE_MULTI_LIMIT_HEX       2   // ��������룬ʮ������

struct  FUNC_ATTRIBUTE_BITS
{                           // bits   description
    Uint16 point:3;         // 2:0    radix point,С���㡣
                            //        0-��С���㣬1-1λС����...��4-4λС��
                            //        (0.0000-100,00.000-011,000.00-010,0000.0-001,00000-000)
    Uint16 unit:3;          // 5:3    unit,��λ
                            //        1-hz, 2-A, 3-RPM, 4-V, 6-%; 001-Hz, 010-A, 100-V
    Uint16 displayBits:3;   // 8:6    5�������Ҫ��ʾ��λ����0-��ʾ0λ��1-��ʾ1λ��...��5-��ʾ5λ
    Uint16 upperLimit:1;    // 9      1-������������ع��������ƣ�0-ֱ������������
    Uint16 lowerLimit:1;    // 10     1-������������ع��������ƣ�0-ֱ������������
    Uint16 writable:2;      // 12:11  ������д���ԣ�00-���Զ�д, 01-������ֻ����10-����ֻ��
    Uint16 signal:1;        // 13     ���ţ�unsignal-0; signal-1
    Uint16 multiLimit:2;    // 15:14  �ù�����Ϊ�������������. 
                            //        00-����������(�����); 
                            //        01-ʮ����,  �������������; 
                            //        10-ʮ������,�������������; 
};

union FUNC_ATTRIBUTE
{
   Uint16                      all;
   struct FUNC_ATTRIBUTE_BITS  bit;
};
//=====================================================================



//=====================================================================
// ���������Ա����ޡ����ޡ�����
// ������ĳ���ֵ������EEPROM_CHECK��������䣬����������ʾ��
typedef struct FUNCCODE_ATTRIBUTE_STRUCT
{
    Uint16                  lower;          // ����
    Uint16                  upper;          // ����
    Uint16                  init;           // ����ֵ
    union FUNC_ATTRIBUTE    attribute;      // ����

//    Uint16                  eepromIndex;    // ��ӦEEPROM�洢��index
} FUNCCODE_ATTRIBUTE;

extern const FUNCCODE_ATTRIBUTE funcCodeAttribute[];
//=====================================================================



//=====================================================================
// �������code��һЩ���ݽṹ����
struct PLC_STRUCT
{
    Uint16 runTime;         // PLC��_������ʱ��
    Uint16 accDecTimeSet;   // PLC��_�μӼ���ʱ��ѡ��
};
//=================================


//=================================
struct AI_SET_CURVE  // AI�趨����
{
    Uint16 minIn;       // ������С����(��ѹֵ)
    Uint16 minInSet;    // ������С�����Ӧ�趨(�ٷֱ�)
    Uint16 maxIn;       // �����������(��ѹֵ)
    Uint16 maxInSet;    // ������������Ӧ�趨(�ٷֱ�)
};
//=================================


//=================================
struct AI_JUMP
{
    Uint16 point;   // �趨��Ծ��
    Uint16 arrange; // �趨��Ծ����
};
//=================================


//=================================
struct ANALOG_CALIBRATE_CURVE  // ģ����У�����ߣ�AIAO
{
    Uint16 before1;     // У��ǰ��ѹ1
    Uint16 after1;      // У�����ѹ1
    
    Uint16 before2;     // У��ǰ��ѹ2
    Uint16 after2;      // У�����ѹ2
};
//=================================


//=================================
typedef struct AO_PARA_STRUCT
{
    Uint16 offset;          // AO��ƫϵ��
    Uint16 gain;            // AO����
} AO_PARA;
//=================================


//=================================
struct FC_GROUP_HIDE_STRUCT //LLH 20130129
{
    Uint16 password;    // ��������������������
    
    Uint16 f;           // F���������ԣ�F0-FF��
    Uint16 e;           // E���������ԣ�E0-EF��
    Uint16 b;           // B���������ԣ�B0-BF��
    Uint16 y;           // Y���������ԣ�Y0-Y7��
    Uint16 l;           // L���������ԣ�L0-L9��
};
union FC_GROUP_HIDE
{
    Uint16 all[6];  
    struct FC_GROUP_HIDE_STRUCT elem;
};
//=================================

//=================================

//=================================
struct ERROR_SCENE_STRUCT
{
    Uint16 errorFrq;                    // ������(���һ��)����ʱƵ��
    Uint16 errorCurrent;                // ������(���һ��)����ʱ����
    Uint16 errorGeneratrixVoltage;      // ������(���һ��)����ʱĸ�ߵ�ѹ
    Uint16 errorDiStatus;               // ������(���һ��)����ʱ�������״̬
    Uint16 errorDoStatus;               // ������(���һ��)����ʱ�������״̬
    
    Uint16 errorInverterStatus;         // ������(���һ��)����ʱ��Ƶ��״̬
    Uint16 errorTimeFromPowerUp;        // ������(���һ��)����ʱʱ�䣨�ӱ����ϵ翪ʼ��ʱ��
    Uint16 errorTimeFromRun;            // ������(���һ��)����ʱʱ�䣨������ʱ��ʼ��ʱ��
};

union ERROR_SCENE
{
    Uint16 all[sizeof(struct ERROR_SCENE_STRUCT)];

    struct ERROR_SCENE_STRUCT elem;
};
//=================================


//=================================
#define APTP_NUM    32  // aptp����
typedef struct
{
    Uint16 low;     // aptp��λ��0-9999
    Uint16 high;    // aptp��λ��0-65535
} LENGTH_SET;
// ��Χ: 0-655359999
//=================================


//=================================
#define MOTOR_TYPE_ACI_GENERAL  0   // ��ͨ�첽���
#define MOTOR_TYPE_ACI_INV      1   // ��Ƶ�첽���
#define MOTOR_TYPE_PMSM         2   // ����ͬ�����
struct MOTOR_PARA_STRUCT
{
    // �����������
    Uint16 motorType;               // b0-00  �������ѡ��
    Uint16 ratingPower;             // b0-01  ��������
    Uint16 ratingVoltage;           // b0-02  ������ѹ
    Uint16 ratingCurrent;           // b0-03  ��������
    Uint16 ratingFrq;               // b0-04  ����Ƶ��
    Uint16 ratingSpeed;             // b0-05  ����ת��

    // �첽����г����
    Uint16 statorResistance;        // b0-06  �첽�����ӵ���
    Uint16 rotorResistance;         // b0-07  �첽��ת�ӵ���
    Uint16 leakInductance;          // b0-08  �첽��©�п�
    Uint16 mutualInductance;        // b0-09  �첽�����п�
	  Uint16 zeroLoadCurrent;       // b0-10  �첽�����ص���
    // ͬ������г����
    Uint16 pmsmRs;                  // b1-11  ͬ�������ӵ���
    Uint16 pmsmLd;                  // b1-12  ͬ����d����
    Uint16 pmsmLq;                  // b1-13  ͬ����q����
//    Uint16 pmsmRsLdUnit;            // b1-14  ͬ������е��赥λ
    Uint16 pmsmCoeff;               // b1-15  ͬ�������綯��ϵ��
    Uint16 pmsmCheckTime;           // b1-16  ͬ�����ȱ����ʱ��
    Uint16 pmsmRsLdUnit;            // b1-14  ͬ������е��赥λ 20170210

    Uint16 rsvdF11[5];
    Uint16 rsvdF12[5];
   
     
};

struct PG_PARA_STRUCT
{
   // PG������
    Uint16 pgType;                  // b0-28    ����������
    Uint16 encoderPulse;            // b0-29    ��������������
    Uint16 enCoderAngle;            // b0-30    ��������װ��
    Uint16 enCoderDir;              // b0-31    ����������/������
    Uint16 uvwSignAngle;            // b0-32    UVW�ź����λ�ý�
    Uint16 uvwSignDir;              // b0-33    UVW�źŷ���
    Uint16 fvcPgLoseTime;           // b0-34    �ٶȷ���PG���߼��ʱ��    
    Uint16 enCoderPole;             // b0-35    ���伫����
    Uint16 fvcPgSrc;                // b0-36    �ٶȷ���PG��ѡ��, 0-QEP1,1-QEP2(��չ)    
    Uint16 rsvdF11;                 // b0-37    UVW������ 

};

union MOTOR_PARA
{
    Uint16 all[sizeof(struct MOTOR_PARA_STRUCT)];
    
    struct MOTOR_PARA_STRUCT elem;
};

union PG_PARA
{
    Uint16 all[sizeof(struct PG_PARA_STRUCT)];
    
    struct PG_PARA_STRUCT elem;
};

//=================================

//=================================
enum MOTOR_SN
{
    MOTOR_SN_1,     // ��1���
    MOTOR_SN_2,     // ��2���
    MOTOR_SN_3,     // ��3���
    MOTOR_SN_4      // ��4���
};
//=================================


//=================================


//------------------------------------------------
struct VC_PARA
{

    //---rev by mrb 2013/01/08---�޸�20130228 by mrb
    Uint16 vcSpdLoopKp1;                 // F5-00  �ٶȻ���������1                F200
    Uint16 vcSpdLoopTi1;                 // F5-01  �ٶȻ�����ʱ��1                F201
    Uint16 vcSpdLoopChgFrq1;             // F5-02  �л�Ƶ��1                      F202
    Uint16 vcSpdLoopKp2;                 // F5-03  �ٶȻ���������2                F203
    Uint16 vcSpdLoopTi2;                 // F5-04  �ٶȻ�����ʱ��2                F204
    Uint16 vcSpdLoopChgFrq2;             // F5-05  �л�Ƶ��2                      F205

    Uint16 spdLoopI;                     // F5-06  �ٶȻ���������                 F217
    Uint16 spdCtrlDriveTorqueLimitSrc;   // F5-07  �ٶȿ���(����)ת������Դ       F209  
    Uint16 spdCtrlDriveTorqueLimit;      // F5-08  �ٶȿ���(����)ת�����������趨 F210

    Uint16 vcSlipCompCoef;               // F5-09  ת���ϵ��                   F206
    Uint16 vcSpdLoopFilterTime;          // F5-10  �ٶȻ��˲�ʱ�䳣��             F207
    Uint16 vcOverMagGain;                // F5-11  ʸ�����ƹ���������             F208
                                         
    Uint16 mAcrKp;                       // F5-12  M�������Kp                    F213
    Uint16 mAcrKi;                       // F5-13  M�������Ki                    F214
                                         
    Uint16 tAcrKp;                       // F5-14  T�������Kp                    F215
    Uint16 tAcrKi;                       // F5-15  T�������Ki                    F216
                                         
    Uint16 weakFlusMode;                 // F5-16 ͬ��������ģʽ                  F218
    Uint16 weakFlusCoef;                 // F5-17 ͬ��������ϵ��                  F219
    Uint16 weakFlusCurMax;               // F5-18 ������ŵ���                    F220
    Uint16 weakFlusAutoCoef;             // F5-19 �����Զ���гϵ��                F221
    Uint16 weakFlusIntegrMul;            // F5-20 ���Ż��ֱ���                    F222
    Uint16 spdCtrlBrakeTorqueLimitSrc;   // F5-21  �ٶȿ���(�ƶ�)ת������Դ       F211
    Uint16 spdCtrlBrakeTorqueLimit;      // F5-22  �ٶȿ���(�ƶ�)ת�����������趨 F212

	Uint16 pmsmSvcPar[27];  

};
//------------------------------------------------



//------------------------------------------------
// ��2����Ĺ����룬����������������Ʋ���
struct MOTOR_FC
{//---rev by mrb 2013/01/08
    union MOTOR_PARA motorPara;     // bx-00  bx-26 ��2/3/4�������. ͬ��1�������
    Uint16 tuneCmd;                 // bx-27  ��г����
    union PG_PARA    pgPara;        // bx-28  bx-37 ��1���PG������
    
    struct VC_PARA vcPara;          // bx-38  bx-60
    
    Uint16 motorCtrlMode;           // bx-61  ��2/3/4������Ʒ�ʽ
    Uint16 accDecTimeMotor;         // bx-62  ��2/3/4����Ӽ���ʱ��ѡ��
    Uint16 torqueBoost;             // bx-63  ת������
    Uint16 rsvdA21;                 // bx-64  ����������ģʽ/
    Uint16 antiVibrateGain;         // bx-65  ����������
};
//------------------------------------------------/





#define AI_NUMBER               3           // AI���Ӹ���

#define AO_NUMBER               2           // AO���Ӹ���

#define HDI_NUMBER              1           // HDI���Ӹ���

#define HDO_NUMBER              1           // HDO���Ӹ���

#define DI_NUMBER_PHSIC         10          // ����DI����
#define DI_NUMBER_V             5           // ����DI����
#define DI_NUMBER_AI_AS_DI      AI_NUMBER   // AI��ΪDI

#define DO_NUMBER_PHSIC         5           // ����DO����
#define DO_NUMBER_V             5           // ����DO����

#define PLC_STEP_MAX            16          // PLC�����ָ�����




//=====================================================================
// EEPROM��ʹ�ó��ȣ������м�Ԥ������
// EEPROM��ַ���䣬2010-08-13
// 0            -   ������rsvd4All
// 1,2          -   EEPROM-CHK
// 3            -   AIAO CHK
// 4-944        -   
// 945-1149     -   
// 1150-1329    -   
// ����,16-63   -   �������ʹ��
// rvb by laoqiuyi 2014 12 05 

#define EEPROM_INDEX_USE_LENGTH     1016//1012//1006//1003 //1002 //999 //998     // ���һ��eeprom��ַ+1
#define EEPROM_INDEX_USE_INDEX        4     // ���ܲ���ʹ��EEPROM��ʼ
// Ԥ��������ʹ�õĹ�����index
#define FUNCCODE_RSVD4ALL_INDEX     GetCodeIndex(funcCode.code.rsvd4All)
#define REM_P_OFF_MOTOR     5   // ����ʹ�õĵ�����䡣������CORE_TO_FUNC_DISP_DATA_NUM����

//20170210
//#define FUNCCODE_RSVD4ALL_INDEX     GetCodeIndex(funcCode.code.rsvd4All)
//#define EEPROM_INDEX_USE_LENGTH     FUNCCODE_RSVD4ALL_INDEX//1016//1012//1006//1003 //1002 //999 //998     // ���һ��eeprom��ַ+1
//#define EEPROM_INDEX_USE_INDEX        4     // ���ܲ���ʹ��EEPROM��ʼ
//#define REM_P_OFF_MOTOR     5   // ����ʹ�õĵ�����䡣������CORE_TO_FUNC_DISP_DATA_NUM����


// �����������
#define FUNCCODE_GROUP_NUM  84  // ����Ԥ������, ��funcCodeGradeCurrentMenuMode. 
// EEPROM��˳��: EEPROM_CHK, �������, FF, FP, F0-FE, A0-AF, B0-BF, C0-CF
// ��ʾ˳��:     F0-FF, E0-EF, B0-BF, Y0-Y7, L0-L9, EEPROM_CHK(����ʾ), �������(����ʾ), D0-DF
// D0-DF��ռ��EEPROM�ռ�





#define FUNCCODE_GROUP_F0   00      // F0��
#define FUNCCODE_GROUP_F1   01      // F1��
#define FUNCCODE_GROUP_F2   02      // F2��
#define FUNCCODE_GROUP_F3   03      // F3��
#define FUNCCODE_GROUP_F4   04      // F4��
#define FUNCCODE_GROUP_F5   05      // F5��
#define FUNCCODE_GROUP_F6   06      // F6��
#define FUNCCODE_GROUP_F7   07      // F7��
#define FUNCCODE_GROUP_F8   08      // F8��
#define FUNCCODE_GROUP_F9   09      // F9��
#define FUNCCODE_GROUP_FA   10      // FA��
#define FUNCCODE_GROUP_FB   11      // FB��
#define FUNCCODE_GROUP_FC   12      // FC��
#define FUNCCODE_GROUP_FD   13      // FD��
#define FUNCCODE_GROUP_FE   14      // FE��
#define FUNCCODE_GROUP_FF   15      // FF��

#define FUNCCODE_GROUP_E0   16      // E0�� FP
#define FUNCCODE_GROUP_E1   17      // E1�� A0
#define FUNCCODE_GROUP_E2   18      // E2�� A1
#define FUNCCODE_GROUP_E3   19      // E3�� A2
#define FUNCCODE_GROUP_E4   20      // E4�� A3
#define FUNCCODE_GROUP_E5   21      // E5�� A4
#define FUNCCODE_GROUP_E6   22      // E6�� A5
#define FUNCCODE_GROUP_E7   23      // E7�� A6
#define FUNCCODE_GROUP_E8   24      // E8�� A7
#define FUNCCODE_GROUP_E9   25      // E9�� A8
#define FUNCCODE_GROUP_EA   26      // EA�� A9
#define FUNCCODE_GROUP_EB   27      // EB�� AA
#define FUNCCODE_GROUP_EC   28      // EC�� AB
#define FUNCCODE_GROUP_ED   29      // ED�� AC 
#define FUNCCODE_GROUP_EE   30      // EE�� AD
#define FUNCCODE_GROUP_EF   31      // EF�� AE

#define FUNCCODE_GROUP_B0   32      // b0�� 
#define FUNCCODE_GROUP_B1   33      // b1��
#define FUNCCODE_GROUP_B2   34      // b2��
#define FUNCCODE_GROUP_B3   35      // b3��
#define FUNCCODE_GROUP_B4   36      // b4��
#define FUNCCODE_GROUP_B5   37      // b5��
#define FUNCCODE_GROUP_B6   38      // b6��
#define FUNCCODE_GROUP_B7   39      // b7��
#define FUNCCODE_GROUP_B8   40      // b8��
#define FUNCCODE_GROUP_B9   41      // b9��
#define FUNCCODE_GROUP_BA   42      // bA��
#define FUNCCODE_GROUP_BB   43      // bB��
#define FUNCCODE_GROUP_BC   44      // bC��
#define FUNCCODE_GROUP_BD   45      // bD��
#define FUNCCODE_GROUP_BE   46      // bE��
#define FUNCCODE_GROUP_BF   47      // bF��

#define FUNCCODE_GROUP_Y0   48      // y0��
#define FUNCCODE_GROUP_Y1   49      // y1��
#define FUNCCODE_GROUP_Y2   50      // y2��
#define FUNCCODE_GROUP_Y3   51      // y3��
#define FUNCCODE_GROUP_Y4   52      // y4��
#define FUNCCODE_GROUP_Y5   53      // y5��
#define FUNCCODE_GROUP_Y6   54      // y6��
#define FUNCCODE_GROUP_Y7   55      // y7��

#define FUNCCODE_GROUP_L0   56      // L0�� 
#define FUNCCODE_GROUP_L1   57      // L1�� 
#define FUNCCODE_GROUP_L2   58      // L2�� 
#define FUNCCODE_GROUP_L3   59      // L3�� 
#define FUNCCODE_GROUP_L4   60      // L4�� 
#define FUNCCODE_GROUP_L5   61      // L5�� 
#define FUNCCODE_GROUP_L6   62      // L6�� 
#define FUNCCODE_GROUP_L7   63      // L7�� 
#define FUNCCODE_GROUP_L8   64      // L8�� 
#define FUNCCODE_GROUP_L9   65      // L9�� 


#define FUNCCODE_GROUP_D0   68      // D0�飬��ʾ
#define FUNCCODE_GROUP_D3   71      // D3�飬��ʾ
#define FUNCCODE_GROUP_DF   (FUNCCODE_GROUP_NUM - 1)    // DF�����ܵ�����ʾ

// ÿ�鹦����ĸ���
// ��FF��֮�⣬ÿ��Ԥ��2�������롣Ϊ�������ӹ�����ʱ���������ûָ�����������
#define F0NUM           (28+ 0  )   // F0  ���������� 
#define F1NUM           (40+ 7  )   // F1  �������   
#define F2NUM           (23+ 0  )   // F2  �������   
#define F3NUM           (16+ 11 )   // F3  ��ͣ����   
#define F4NUM           (49+ 0  )   // F4  V/F���Ʋ���
#define F5NUM           (23+ 27 )   // F5  ʸ�����Ʋ���  
#define F6NUM           (15+ 0  )   // F6  ��������ʾ   
#define F7NUM           (52+ 0  )   // F7  ��������    
#define F8NUM           (36+ 2  )   // F8  �����뱣�� 
#define F9NUM           ( 9+ 0  )   // F9  ͨѶ����    
#define FANUM           ( 9+ 0  )   // FA  ת�ؿ��Ʋ���
#define FBNUM           (10+ 0  )   // FB  �����Ż�����
#define FCNUM           ( 3+ 0  )   // FC  
#define FDNUM           ( 0+ 1  )   // FD  
#define FENUM           ( 0+ 1  )   // FE  
#define FFNUM           ( 0+ 1  )   // FF  


#define E0NUM           (10+ 0  )//(10+ 0  )   // E0  ��Ƶ�������ͼ���
#define E1NUM           (52+ 0  )   // E1  ���ָ�����PLC
#define E2NUM           (29+ 0  )   // E2  PID����    
#define E3NUM           (22+ 0  )   // E3
#define E4NUM           (32+ 0  )   // E4
#define E5NUM           (30+ 0  )   // E5
#define E6NUM           ( 9+ 3  )   // E6
#define E7NUM           ( 8+ 0  )   // E7
#define E8NUM           (30     )   // E8
#define E9NUM           ( 1+ 0  )   // E9
#define EANUM           ( 1+ 0  )   // EA
#define EBNUM           ( 0+ 1  )   // EB
#define ECNUM           ( 1+ 0  )   // EC
#define EDNUM           ( 20    )   // ED  AIAO����У�� 
#define EENUM           ( 1+ 0  )   // EE  ������������
#define EFNUM           ( 0+ 1  )   // EF

#define B0NUM           (38+0   )          // B0  ��һ�������
#define B1NUM           (B0NUM+2+F5NUM+3)  // B1  �ڶ��������
#define B2NUM           ( B1NUM )          // B2  �����������
#define B3NUM           ( B1NUM )          // B3  ���ĵ������
#define B4NUM           ( 0+1   )   // B4
#define B5NUM           ( 0+1   )   // B5
#define B6NUM           ( 0+1   )   // B6
#define B7NUM           ( 0+1   )   // B7
#define B8NUM           ( 0+1   )   // B8
#define B9NUM           ( 0+1   )   // B9
#define BANUM           ( 0+1   )   // BA
#define BBNUM           ( 0+1   )   // BB
#define BCNUM           ( 0+1   )   // BC
#define BDNUM           ( 0+1   )   // BD
#define BENUM           ( 0+1   )   // BE
#define BFNUM           ( 0+1   )   // BF

#define Y0NUM           ( 6+0   )   // Y0  ���������
#define Y1NUM           ( 31+0  )   // Y1
#define Y2NUM           ( 0+1   )   // Y2
#define Y3NUM           ( 0+1   )   // Y3
#define Y4NUM           ( 0+1   )   // Y4
#define Y5NUM           ( 0+1   )   // Y5
#define Y6NUM           ( 0+1   )   // Y6
#define Y7NUM           ( 0+1   )   // Y7

#define L0NUM           (13+ 0  )   // L0 
#define L1NUM           ( EDNUM )   // L1
#define L2NUM           ( 0+1   )   // L2
#define L3NUM           ( 0+1   )   // L3
#define L4NUM           ( 0+1   )   // L4
#define L5NUM           ( 0+1   )   // L5
#define L6NUM           ( 0+1   )   // L6
#define L7NUM           ( 0+1   )   // L7
#define L8NUM           ( 0+1   )   // L8

#if DEBUG_F_MOTOR_FUNCCODE
#define L9NUM           FUNC_TO_CORE_DEBUG_DATA_NUM     // L9  ���ԣ�func2motor
#elif 1
#define L9NUM           ( 0+1   )   // L9
#endif

#define CHK_NUM   (4 +0  )  //     eepromCheckWord(2)��rsvd4All(1)�����������ǰ��, AIAOChk(1)Ҳ������
#define REM_NUM   (48)      // ������䣬��������ʹ�õĵ�����䡣(�������ܵĵ������)

#define D0NUM     (100+0)   // D0  ��ʾʹ�ã���ռ��EEPROM�������ٵ�ռ�ó���ռ�(�޳���ֵ�������ޣ���������)
#define D1NUM     ( 0+1)    // D1
#define D2NUM     ( 0+1)    // D2
#define D3NUM     (10+2)    // D3
#define D4NUM     ( 0+1)    // D4
#define D5NUM     ( 0+1)    // D5
#define D6NUM     ( 0+1)    // D6
#define D7NUM     ( 0+1)    // D7
#define D8NUM     ( 0+1)    // D8
#define D9NUM     ( 0+1)    // D9
#define DANUM     ( 0+1)    // DA
#define DBNUM     ( 0+1)    // DB
#define DCNUM     ( 0+1)    // DC
#define DDNUM     ( 0+1)    // DD
#define DENUM     ( 0+1)    // DE
#if DEBUG_F_MOTOR_FUNCCODE
#define DFNUM     CORE_TO_FUNC_DISP_DATA_NUM  // DF  ���ԣ�motor2func
#elif 1
#define DFNUM     ( 0+1)    // DF ���ܵ���ʹ��
#endif

#define FNUM_PARA      (F0NUM + F1NUM + F2NUM + F3NUM +     \
                        F4NUM + F5NUM + F6NUM + F7NUM +     \
                        F8NUM + F9NUM + FANUM + FBNUM +     \
                        FCNUM + FDNUM + FENUM + FFNUM +     \
                                                            \
                        E0NUM + E1NUM + E2NUM + E3NUM +     \
                        E4NUM + E5NUM + E6NUM + E7NUM +     \
                        E8NUM + E9NUM + EANUM + EBNUM +     \
                        ECNUM + EDNUM + EENUM + EFNUM +     \
                                                            \
                        B0NUM + B1NUM + B2NUM + B3NUM +     \
                        B4NUM + B5NUM + B6NUM + B7NUM +     \
                        B8NUM + B9NUM + BANUM + BBNUM +     \
                        BCNUM + BDNUM + BENUM + BFNUM +     \
                                                            \
                        Y0NUM + Y1NUM + Y2NUM + Y3NUM +     \
                        Y4NUM + Y5NUM + Y6NUM + Y7NUM +     \
                                                            \
                        L0NUM + L1NUM + L2NUM + L3NUM +     \
                        L4NUM + L5NUM + L6NUM + L7NUM +     \
                        L8NUM + L9NUM                       \
                        )                                   // ���й����룬��������ʾ
#define FNUM_EEPROM    (FNUM_PARA + CHK_NUM + REM_NUM)      // ��Ҫ�洢��EEPROM�е����в���
#define FNUM_ALL       (FNUM_EEPROM +                       \
                        D0NUM + D1NUM + D2NUM + D3NUM +     \
                        D4NUM + D5NUM + D6NUM + D7NUM +     \
                        D8NUM + D9NUM + DANUM + DBNUM +     \
                        DCNUM + DDNUM + DENUM + DFNUM       \
                       )                                    // ���й����롢������������ʾ


//=====================================================================
//
// 飬F0�飬F1��, ...
//
// eepromCheckWord������ǰ��
// ���Ҳ������û����롢�û����Ʋ˵�����ǰ�棬��ֹ��ɾ������ʱ�����޸ġ�
// fc��eeprom�ж�Ӧ��ϵ����������Ҫ����Ҫ��
//
//=====================================================================
struct FUNCCODE_GROUP 
{
//======================================
    Uint16 f0[F0NUM];               // F0 ����������
    Uint16 f1[F1NUM];               // F1 �������
    Uint16 f2[F2NUM];               // F2 �������
    Uint16 f3[F3NUM];               // F3 ��ͣ����
    
//======================================
    Uint16 f4[F4NUM];               // F4 V/F������?
    Uint16 f5[F5NUM];               // F5 ʸ?
    Uint16 f6[F6NUM];               // F6 ��������ʾ
    Uint16 f7[F7NUM];               // F7 ��������
    
//======================================
    Uint16 f8[F8NUM];               // F8 �����뱣��
    Uint16 f9[F9NUM];               // F9 ͨѶ����
    Uint16 fa[FANUM];               // FA ת�ؿ��Ʋ���
    Uint16 fb[FBNUM];               // FB �����Ż�����

//======================================
    Uint16 fc[FCNUM];               // 
    Uint16 fd[FDNUM];               // 
    Uint16 fe[FENUM];               //  
    Uint16 ff[FFNUM];               // 
    
 //======================================
    Uint16 e0[E0NUM];               // E0 ��Ƶ�������ͼ���
    Uint16 e1[E1NUM];               // E1 ���ָ�����PLC
    Uint16 e2[E2NUM];               // E2 PID����
    Uint16 e3[E3NUM];               // E3 

//======================================
    Uint16 e4[E4NUM];               // E4
    Uint16 e5[E5NUM];               // E5
    Uint16 e6[E6NUM];               // E6
    Uint16 e7[E7NUM];               // E7

//======================================
    Uint16 e8[E8NUM];               // E8
    Uint16 e9[E9NUM];               // E9
    Uint16 ea[EANUM];               // EA
    Uint16 eb[EBNUM];               // EB

//======================================
    Uint16 ec[ECNUM];               // EC
    Uint16 ed[EDNUM];               // ED 
    Uint16 ee[EENUM];               // EE ������������
    Uint16 ef[EFNUM];               // EF 
    
//======================================
    Uint16 b0[B0NUM];               // B0 ��һ�������
    Uint16 b1[B1NUM];               // B1 �ڶ��������
    Uint16 b2[B2NUM];               // B2 �����������
    Uint16 b3[B3NUM];               // B3 ���ĵ������

//======================================
    Uint16 b4[B4NUM];               // B4
    Uint16 b5[B5NUM];               // B5
    Uint16 b6[B6NUM];               // B6
    Uint16 b7[B7NUM];               // B7

//======================================
    Uint16 b8[B8NUM];               // B8
    Uint16 b9[B9NUM];               // B9
    Uint16 ba[BANUM];               // BA
    Uint16 bb[BBNUM];               // BB

//======================================
    Uint16 bc[BCNUM];               // BC
    Uint16 bd[BDNUM];               // BD
    Uint16 be[BENUM];               // BE
    Uint16 bf[BFNUM];               // BF

//======================================
    Uint16 y0[Y0NUM];               // Y0 ���������
    Uint16 y1[Y1NUM];               // Y1 
    Uint16 y2[Y2NUM];               // Y2
    Uint16 y3[Y3NUM];               // Y3

//======================================
    Uint16 y4[Y4NUM];               // Y4
    Uint16 y5[Y5NUM];               // Y5
    Uint16 y6[Y6NUM];               // Y6
    Uint16 y7[Y7NUM];               // Y7

//======================================
    Uint16 l0[L0NUM];               // L0 ���Ҳ���
    Uint16 l1[L1NUM];               // L1 AIAO����У��
    Uint16 l2[L2NUM];               // L2
    Uint16 l3[L3NUM];               // L3

//======================================
    Uint16 l4[L4NUM];               // L4   
    Uint16 l5[L5NUM];               // L5
    Uint16 l6[L6NUM];               // L6
    Uint16 l7[L7NUM];               // L7
    Uint16 l8[L8NUM];               // L8    
    Uint16 l9[L9NUM];               // L9    

// ֮ǰ�Ĺ������������ޣ����ԣ�֮��û�У���ʡ�ռ�
//======================================

//======================================
    Uint16 fChk[CHK_NUM];           // eepromCheckWord

//======================================
    Uint16 remember[REM_NUM];       // �������

// ֮ǰ������Ҫ����EEPROM��
//======================================

//======================================
// ֮������ݲ���Ҫ����EEPROM�У���RAM
    Uint16 d0[D0NUM];               // D0 ��ʾ
    Uint16 d1[D1NUM];               // D1
    Uint16 d2[D2NUM];               // D2
    Uint16 d3[D3NUM];               // D3

//======================================
    Uint16 d4[D4NUM];               // D4
    Uint16 d5[D5NUM];               // D5
    Uint16 d6[D6NUM];               // D6
    Uint16 d7[D7NUM];               // D7

//======================================
    Uint16 d8[D8NUM];               // D8
    Uint16 d9[D9NUM];               // D9
    Uint16 da[DANUM];               // DA
    Uint16 db[DBNUM];               // DB

//======================================
    Uint16 dc[DCNUM];               // DC
    Uint16 dd[DDNUM];               // DD
    Uint16 de[DENUM];               // DE
    Uint16 df[DFNUM];               // DF, ��ʾ�����ܵ���ʹ��
//======================================
};


//=====================================================================
//
// �����룬F0-00, F0-01, ..., F1-00, F1-01, ...
//
//=====================================================================
struct FUNCCODE_CODE 
{
//======================================
// F0 ����������----ԭF0------�иĶ�---20130228 by mrb
    Uint16 motorCtrlMode;                   // F0-00  (���)���Ʒ�ʽ
    Uint16 presetFrq;                       // F0-01  Ԥ��Ƶ��
    Uint16 frqPoint;                        // F0-02  Ƶ��ָ��С����
    Uint16 frqXSrc;                         // F0-03  ��Ƶ��ԴXѡ��
    Uint16 frqYSrc;                         // F0-04  ����Ƶ��ԴYѡ��
    Uint16 frqYRangeBase;                   // F0-05  ����Ƶ��ԴY��Χѡ��
    Uint16 frqYRange;                       // F0-06  ����Ƶ��ԴY��Χ
    Uint16 frqCalcSrc;                      // F0-07  Ƶ��Դ����ѡ��
    Uint16 frqYOffsetFc;                    // F0-08  ����Ƶ��Դƫ�õ������趨
    Uint16 frqRemMode;                      // F0-09  �����趨Ƶ�ʼ���ѡ��
    Uint16 updnBenchmark;                   // F0-10  ����ʱƵ��ָ��UP/DOWN��׼
    Uint16 runSrc;                          // F0-11  ����Դѡ��
    Uint16 frqRunCmdBind;                   // F0-12  ����Դ����Ƶ��Դ
    Uint16 accTime1;                        // F0-13  ����ʱ��1
    Uint16 decTime1;                        // F0-14  ����ʱ��1
    Uint16 accDecTimeUnit;                  // F0-15  �Ӽ���ʱ��ĵ�λ
    Uint16 accDecBenchmark;                 // F0-16  �Ӽ���ʱ���׼Ƶ��
    Uint16 varFcByTem;                      // F0-17  �ز�Ƶ�����¶ȵ���
    Uint16 carrierFrq;                      // F0-18  �ز�Ƶ��
    Uint16 maxFrq;                          // F0-19  ���Ƶ��
    Uint16 upperFrqSrc;                     // F0-20  ����Ƶ��Դ
    Uint16 upperFrq;                        // F0-21  ����Ƶ����ֵ�趨
    Uint16 upperFrqOffset;                  // F0-22  ����Ƶ��ƫ��
    Uint16 lowerFrq;                        // F0-23  ����Ƶ����ֵ�趨
    Uint16 runDir;                          // F0-24  ���з���
    enum MOTOR_SN motorSn;                  // F0-25  ���ѡ��
    Uint16 AiSet_Frq;                       // F0-26  AI����ѡ�� 0  0.01HZ  1  0.05  2 0.1 3 0.5  
    Uint16 inverterGpTypeDisp;              // F0-27  GP������ʾ


//======================================
// F1 �������--------ԭF4----�޸Ķ�---20130228 by mrb
    Uint16 diFunc[DI_NUMBER_PHSIC];         // F1-00  --F1-09   DI1���ӹ���ѡ��
                                            
    Uint16 diControlMode;                   // F1-10  �������ʽ
    Uint16 diUpDownSlope;                   // F1-11  ����UP/DOWN���ʣ���Ϊ0.001Hz
                                            
                                            
    Uint16 curveSet2P1[4];                  // F1-12,...,F1-15  ����1��2�㣬���ֵ����Сֵ
                                            
                                            
    Uint16 curveSet2P2[4];                  // F1-16,...,F1-19  ����2��2��
                                            
    Uint16 curveSet2P3[4];                  // F1-20,...,F1-23  ����3��2��
                                            
    Uint16 aiCurveSrc;                      // F1-24  AI�趨����ѡ��
                                            
    Uint16 aiLimitSrc;                      // F1-25  AI����ѡ��
                                            
                                            
    Uint16 curveSet2P4[4];                  // F1-26,...,F1-29  HDI���ߣ�2��
                                            
                                            
    Uint16 diFilterTime;                    // F1-30  DI�˲�ʱ��
    Uint16 ai1FilterTime;                   // F1-31  AI1�˲�ʱ��, 10ms
                                            
    Uint16 ai2FilterTime;                   // F1-32  AI2�˲�ʱ��, 10ms
                                            
    Uint16 ai3FilterTime;                   // F1-33  AI3�˲�ʱ��, 10ms
                                            
    Uint16 pulseInFilterTime;               // F1-34  PULSE�˲�ʱ��, 10ms
                                            
    Uint16 diLogic[2];                      // F1-35  DI��Ч״̬ѡ��1
                                            // F1-36  DI��Ч�̬ѡ�?
    Uint16 diDelayTime[3];                  // F1-37  DI1�ӳ�ʱ��
									                          // F1-38  DI1�ӳ�ʱ��
									                          // F1-39  DI1�ӳ�ʱ��
    Uint16 diDupSel;                        // F1-40 DI���ظ�����ѡ��0---���ɣ�1---����
    Uint16 keyAdX1;                         // F1-41 ���̵�λ��X1
    Uint16 keyAdX2;                         // F1-42 ���̵�λ��X2
    Uint16 keyAdValue;                      // F1-43 ���̵�λ���趨ֵ
    int16 keyAdY1;                          // F1-44 ���̵�λ��X1��Ӧֵ
    int16 keyAdY2;                          // F1-45 ���̵�λ��X2��Ӧֵ
	  Uint16 keyAdCtrl;                       // F1-46 ���̵�λ������ 

//======================================
// F2 �������--------ԭF5----�޸Ķ�---20130228 by mrb

    Uint16 fmOutSelect;                     // F2-00  �๦�ܶ������ѡ
    Uint16 doFunc[DO_NUMBER_PHSIC];         // F2-01  FMR���ѡ��
                                            // F2-02  ���ư�RELAY���ѡ��
                                            // F2-03  ��չ��RELAY���ѡ��
                                            // F2-04  DO1���ѡ��
                                            // F2-05  ��չ��DO2���ѡ��

    Uint16 aoFunc[AO_NUMBER+HDO_NUMBER];    // F2-06  FMP���ѡ��
                                            // F2-07  AO1���ѡ��
                                            // F2-08  ��չ��AO2���ѡ��
    Uint16 fmpOutMaxFrq;                    // F2-09  FMP������Ƶ��

    Uint16 doDelayTime[DO_NUMBER_PHSIC];    // F2-10  RELAY1����ӳ�ʱ��
                                            // F2-11  RELAY2����ӳ�ʱ��
                                            // F2-12  DO1����ӳ�ʱ��
                                            // F2-13  DO2����ӳ�ʱ��
                                            // F2-14  DO3����ӳ�ʱ��
    Uint16 doLogic;                         // F2-15  DO��Ч״̬ѡ��
    AO_PARA aoPara[AO_NUMBER];              // F2-16  AO1��ƫϵ��
                                            // F2-17  AO1����
                                            // F2-18  AO2��ƫϵ��
                                            // F2-19  AO2����
    Uint16 aoLpfTime[AO_NUMBER+HDO_NUMBER]; // F2-20  HDO,AO1,AO2����˲�ʱ��
    


//======================================
// F3 ��ͣ����--------ԭF6------�޸Ķ�---20130228 by mrb
    Uint16 startMode;                       // F3-00  ������ʽ
    Uint16 speedTrackMode;                  // F3-01  ת�ٸ��ٷ�ʽ
    Uint16 speedTrackVelocity;              // F3-02  ת�ٸ��ٿ���
    Uint16 startFrq;                        // F3-03  ����Ƶ��
    Uint16 startFrqTime;                    // F3-04  ����Ƶ�ʱ���ʱ��
    Uint16 startBrakeCurrent;               // F3-05  ����ֱ���ƶ�����
    Uint16 startBrakeTime;                  // F3-06  ����ֱ���ƶ�ʱ��
    Uint16 stopMode;                        // F3-07  ͣ����ʽ
    Uint16 stopBrakeFrq;                    // F3-08  ͣ��ֱ���ƶ���ʼƵ��
    Uint16 stopBrakeWaitTime;               // F3-09  ͣ��ֱ���ƶ��ȴ�ʱ��
    Uint16 stopBrakeCurrent;                // F3-10  ͣ��ֱ���ƶ�����
    Uint16 stopBrakeTime;                   // F3-11  ͣ��ֱ���ƶ�ʱ��
    Uint16 brakeDutyRatio;                  // F3-12  �ƶ�ʹ����
    Uint16 accDecSpdCurve;                  // F3-13  �Ӽ��ٷ�ʽ
    Uint16 sCurveStartPhaseTime;            // F3-14  S���߿�ʼ��ʱ�����
    Uint16 sCurveEndPhaseTime;              // F3-15  S���߽�����ʱ�����
    Uint16  startGroup[11];                 // F3-16~26 ���ز���  
//======================================
// F4 V/F���Ʋ���--------ԭF3--�иĶ�---20130228 by mrb

    Uint16 vfCurve;                         // F4-00  VF�����趨
    Uint16 torqueBoost;                     // F4-01  ת������
    Uint16 boostCloseFrq;                   // F4-02  ת��������ֹƵ��
    Uint16 vfFrq1;                          // F4-03  ���VFƵ�ʵ�1
    Uint16 vfVol1;                          // F4-04  ���VF��ѹ��1
    Uint16 vfFrq2;                          // F4-05  ���VFƵ�ʵ�2
    Uint16 vfVol2;                          // F4-06  ���VF��ѹ��2
    Uint16 vfFrq3;                          // F4-07  ���VFƵ�ʵ�3
    Uint16 vfVol3;                          // F4-08  ���VF��ѹ��3
    Uint16 slipCompCoef;                    // F4-09  ת���ϵ��
    Uint16 vfOverMagGain;                   // F4-10  VF����������
    Uint16 antiVibrateGain;                 // F4-11  ����������
//    Uint16 vfF312;                          // F4-12 
    Uint16 vfVoltageSrc;                    // F4-13  VF����ĵ�ѹԴ
    Uint16 vfVoltageDigtalSet;              // F4-14  VF����ĵ�ѹԴ�����趨
    Uint16 vfVoltageAccTime;                // F4-15  VF����ĵ�ѹ����ʱ��   
    Uint16 vfF312;                          // F4-12  20170503   
    Uint16 vfOcLevel;                       // F4-16  ����ʧ�ٵ��� MD500
	Uint16 vfOcEn;                          // F4-17  ����ʧ��ʹ�� MD500
	Uint16 vfOcGain;                        // F4-18  ����ʧ������ MD500
	Uint16 vfBsOcCmpCoff;                   // F4-19  ���ٹ������� MD500               
	Uint16 vfOvLevel;                       // F4-20  ��ѹʧ�ٵ�ѹ MD500
	Uint16 vfOvEn;                          // F4-21  ��ѹʧ��ʹ�� MD500
	Uint16 vfOvFgn;                         // F4-22  ��ѹƵ������ MD500
    Uint16 vfOvVgn;                         // F4-23  ��ѹ��ѹ���� MD500
	Uint16 vfOvFrqLmt;                      // F4-24  ��ѹƵ������ MD500
	Uint16 vfwsTrComp;                      // F4-25  ת������� MD500    
	Uint16 vfGroup[23];                     // F4-26~48 VF������   MD500
    
//======================================
// F5 ʸ�����Ʋ���--------ԭF2

    struct VC_PARA vcParaM1;        // ��1���ʸ�����Ʋ���


//======================================
// F6 ��������ʾ--------ԭF7----δ��20130228 by mrb
//    Uint16 rsvdF71;                 // F6-00  ����
//    Uint16 mfkKeyFunc;              // F6-01  MF.K������ѡ��
    Uint16 stopKeyFunc;                     // F6-00  STOP������
    Uint16 ledDispParaRun1;                 // F6-01  LED������ʾ����1
    Uint16 ledDispParaRun2;                 // F6-02  LED������ʾ����2
                                            
    Uint16 ledDispParaStop;                 // F6-03  LEDͣ����ʾ����
    Uint16 speedDispCoeff;                  // F6-04  �����ٶ���ʾϵ��
  	Uint16 speedDispPointPos;               // F6-05  �����ٶ���ʾС����λ��
    Uint16 radiatorTemp;                    // F6-06  �����ģ��ɢ�����¶�
                                            
    Uint16 runTimeAddup;                    // F6-07  �ۼ�����ʱ��, ��λ: h
    Uint16 powerUpTimeAddup;                // F6-08  �ۼ��ϵ�ʱ��
    Uint16 powerAddup;                      // F6-09  �ۼƺĵ���
                                            
    Uint16 productVersion;                  // F6-10  ��Ʒ��
    Uint16 softVersion;                     // F6-11  ����汾��
    Uint16 temp2;                           // F6-12  ������ɢ�����¶�
    Uint16 rsvdF71;                         // F6-13  ����
    Uint16 mfkKeyFunc;                      // F6-14  MF.K������ѡ��

//======================================
// F7 ��������--------ԭF8---��20130228 by mrb
    Uint16 jogFrq;                          // F7-00  �㶯����Ƶ��
    Uint16 jogAccTime;                      // F7-01  �㶯����ʱ��
    Uint16 jogDecTime;                      // F7-02  �㶯����ʱ��
    // ADD                                  
    Uint16 jogWhenRun;                      // F7-03  ���ӵ㶯����                                           
    Uint16 jumpFrq1;                        // F7-04  ��ԾƵ��1
    Uint16 jumpFrq2;                        // F7-05  ��ԾƵ��2
    Uint16 jumpFrqRange;                    // F7-06  ��ԾƵ�ʷ���
    Uint16 jumpFrqMode;                     // F7-07  �Ӽ��ٹ�������ԾƵ���Ƿ���Ч
    Uint16 accTime2;                        // F7-08  ����ʱ��2
    Uint16 decTime2;                        // F7-09  ����ʱ��2
    Uint16 accTime3;                        // F7-10  ����ʱ��3
    Uint16 decTime3;                        // F7-11  ����ʱ��3
    Uint16 accTime4;                        // F7-12  ����ʱ��4
    Uint16 decTime4;                        // F7-13  ����ʱ��4
    Uint16 accTimefrqChgValue;              // F7-14  ����ʱ��1/2�л�Ƶ�ʵ� 
    Uint16 decTimefrqChgValue;              // F7-15  ����ʱ��1/2�л�Ƶ�ʵ�                                             
    Uint16 zeroSpeedDeadTime;               // F7-16  ����ת����ʱ��
    Uint16 antiReverseRun;                  // F7-17  ��ת����, 0-����ת��1-��ֹ��ת
    Uint16 lowerDeal;                       // F7-18  Ƶ�ʵ�������Ƶ�����ж���
    Uint16 droopCtrl;                       // F7-19  �´�����
    Uint16 powerUpTimeArriveSet;            // F7-20  �趨�ϵ絽��ʱ��
    Uint16 runTimeArriveSet;                // F7-21  �趨���е���ʱ��
    Uint16 startProtect;                    // F7-22  ��������ѡ��                                            
    Uint16 frqFdtValue;                     // F7-23  Ƶ�ʼ��ֵ(FDT��ƽ)
    Uint16 frqFdtLag;                       // F7-24  Ƶ�ʼ���ͺ�ֵ
    Uint16 frqArriveRange;                  // F7-25  Ƶ�ʵ���������
    Uint16 frqFdt1Value;                    // F7-26  Ƶ�ʼ��ֵ(FDT1��ƽ)
    Uint16 frqFdt1Lag;                      // F7-27  Ƶ�ʼ��1�ͺ�ֵ
    Uint16 frqArriveValue1;                 // F7-28  Ƶ�ʵ�����ֵ1 
    Uint16 frqArriveRange1;                 // F7-29  Ƶ�ʵ����1����
    Uint16 frqArriveValue2;                 // F7-30  Ƶ�ʵ�����ֵ2 
    Uint16 frqArriveRange2;                 // F7-31  Ƶ�ʵ�����2����                                          
    Uint16 oCurrentChkValue;                // F7-32  ��������
    Uint16 oCurrentChkTime;                 // F7-33  ���������ӳ�ʱ��
    Uint16 softOCValue;                     // F7-34  ���������
    Uint16 softOCDelay;                     // F7-35  �����������ӳ�ʱ��                                           
    Uint16 currentArriveValue1;             // F7-36  ����������ֵ1
    Uint16 currentArriveRange1;             // F7-37  ����������1����
    Uint16 currentArriveValue2;             // F7-38  ����������ֵ2
    Uint16 currentArriveRange2;             // F7-39  ����������2����                                           
    Uint16 temperatureArrive;               // F7-40  ģ���¶ȵ���
    Uint16 fanControl;                      // F7-41  ���ȿ���                                           
    Uint16 setTimeMode;                     // F7-42  ��ʱ����ѡ��
    Uint16 setTimeSource;                   // F7-43  ��ʱʱ���趨ѡ��
    Uint16 setTimeValue;                    // F7-44  �趨����ʱ��
    Uint16 setTimeArrive;                   // F7-45  ��ǰ���е���ʱ��                                            
    Uint16 wakeUpFrq;                       // F7-46  ���Ƶ�?
    Uint16 wakeUpTime;                      // F7-47  �����ӳ�ʱ��
    Uint16 dormantFrq;                      // F7-48  ����Ƶ��
    Uint16 dormantTime;                     // F7-49  �����ӳ�ʱ��                                           
    Uint16 ai1VoltageLimit;                 // F7-50  AI1�����ѹ����
    Uint16 ai1VoltageUpper;                 // F7-51  AI1�����ѹ����


//======================================
// F8 �����뱣��--------ԭF9----��20130228 by mrb
    Uint16 ocGain;                          // F8-00  ����ʧ������
    Uint16 ocPoint;                         // F8-01  ����ʧ�ٱ�������
    Uint16 overloadMode;                    // F8-02  ������ر���ѡ��
    Uint16 overloadGain;                    // F8-03  ������ر�������
    Uint16 foreOverloadCoef;                // F8-04  �������Ԥ��ϵ��
    Uint16 ovGain;                          // F8-05  ��ѹʧ������
    Uint16 ovPoint;                         // F8-06  ��ѹʧ�ٱ�����ѹ                                            
    Uint16 inPhaseLossProtect;              // F8-07  ����ȱ�ౣ��ѡ��
    Uint16 outPhaseLossProtect;             // F8-08  ���ȱ�ౣ��ѡ��
    Uint16 shortCheckMode;                  // F8-09  �ϵ�Եض�·��������
    Uint16 errAutoRstNumMax;                // F8-10  �����Զ���λ����
    Uint16 errAutoRstRelayAct;              // F8-11  �����Զ���λ�ڼ���ϼ̵�������ѡ��
    Uint16 errAutoRstSpaceTime;             // F8-12  �����Զ���λ���ʱ��, 0.1s   
    Uint16 osChkValue;                      // F8-13 ���ٶȼ��ֵ
    Uint16 osChkTime;                       // F8-14 ���ٶȼ��ʱ��
    Uint16 devChkValue;                     // F8-15 �ٶ�ƫ�������ֵ
    Uint16 devChkTime;                      // F8-16 �ٶ�ƫ�������ʱ��                                            
    Uint16 errorAction[4];                  // F8-17  -F8-21  ����ʱ��������ѡ��1-4
    Uint16 breakStartVolt;                  // F8-21  ��ѹ�ƶ���ֵ
    Uint16 errorShow[2];                    // F8-22  -F8-23  ����ָʾѡ��1,2
    Uint16 errorRunFrqSrc;                  // F8-24  ����ʱ��������Ƶ��ѡ��
    Uint16 errorSecondFrq;                  // F8-25  �쳣����Ƶ���趨                                                                                        
    Uint16 pOffTransitoryNoStop;            // F8-26  ˲ͣ��ͣ����ѡ��
    Uint16 pOffTransitoryFrqDecSlope;       // F8-27  ˲ͣ������ͣ�жϵ�ѹ
    Uint16 pOffVolBackTime;                 // F8-28  ˲ͣ��ͣ��ѹ�����ж�ʱ��
    Uint16 pOffThresholdVol;                // F8-29  ˲ͣ��ͣ�����жϵ�ѹ                                           
    Uint16 loseLoadProtectMode;             // F8-30  ���ر���ѡ��
    Uint16 loseLoadLevel;                   // F8-31  ���ؼ��ˮƽ
    Uint16 loseLoadTime;                    // F8-32  ���ؼ��ʱ��                                            
    Uint16 motorOtMode;                     // F8-33  ����¶ȴ���������
    Uint16 motorOtProtect;                  // F8-34  ������ȱ�����ֵ
    Uint16 motorOtCoef;                     // F8-35  �������Ԥ������ֵ
 	  Uint16 pmsmEncProtect[2];               // F8-36  UVW����������(Err20)ʹ��
	                                          // F8-37  ���ϱ�������ѡ��5 ��λ��Err51��ʮλ��Err19��


//========================================================
// F9 ͨѶ����--------ԭFD--��20130228 by mrb
    Uint16 commBaudRate;                    // F9-00  ������
    Uint16 commParity;                      // F9-01  ���ݸ�ʽ
    Uint16 commSlaveAddress;                // F9-02  ������ַ
    Uint16 commDelay;                       // F9-03  Ӧ���ӳ�
    Uint16 commOverTime;                    // F9-04  ͨѶ��ʱʱ��
    Uint16 commProtocol;                    // F9-05  ͨѶ��ݴ���ʽѡ��   
    Uint16 commReadCurrentPoint;            // F9-06  ͨѶ��ȡ�����ֱ���
    Uint16 commProtocolSec;                 // F9-07  ͨѶЭ��ѡ��(��չ��)
    Uint16 commMaster;                      // F9-08  ͨѶ���ӷ�ʽ
    
//==================================================================
// FA ת�ؿ��ƺ��޶�����--------ԭA0--��20130228 by mrb

    Uint16 torqueCtrl;                      // FA-00  ת�ؿ���
    Uint16 driveUpperTorqueSrc;             // FA-01  ����ת������Դ���
    Uint16 driveUpperTorque;                // FA-02  ����ת�����������趨
    Uint16 torqueCtrlAccTime;               // FA-03  ת�ؼ���ʱ��
    Uint16 torqueCtrlDecTime;               // FA-04  ת�ؼ���ʱ��
    Uint16 torqueCtrlFwdMaxFrq;             // FA-05  ת�ؿ����������Ƶ��
    Uint16 torqueCtrlRevMaxFrq;             // FA-06  ת�ؿ��Ʒ������Ƶ��
    Uint16 torqZeroZone;                    // FA-07  ת���˲�
    Uint16 brakeUpperTorqueSrc;             // FA-08  �ƶ�ת������Դ
   
//==================================================================
// FB �����Ż�����--------ԭA5--δ��20130228 by mrb

    Uint16 cbcEnable;                       // FB-00    ������ʹ��
    Uint16 uvPoint;                         // FB-01    Ƿѹ������
    Uint16 ovPointSet;                      // FB-02    ��ѹ������    
    Uint16 deadCompMode;                    // FB-03    ��������ģʽѡ��
    Uint16 curSampleDelayComp;              // FB-04    ���������ʱ��?
    Uint16 svcMode;                         // FB-05    SVC�Ż�ѡ�� 0-���Ż�  1-�Ż�ģʽ1  2-�Ż�ģʽ2
    Uint16 pwmMode;                         // FB-06    DPWM�л�����Ƶ��
    Uint16 modulationMode;                  // FB-07    ���Ʒ�ʽ��0-�첽���ƣ�1-ͬ������
    Uint16 softPwm;                         // FB-08    ���PWM
    Uint16 deadTimeSet;                     // FB-09    ����ʱ�����-1140Vר��

//==================================================================
// FC  
    Uint16 FC_rev;                          // FC  ����
    Uint16 prsCoe;                          // FC-01  ��������ϵ��
    Uint16 pidErrStart;                     // FC-02  ��������ƫ��(%)

//==================================================================
// FD  
    Uint16 fd[FDNUM];                       // FD  ����
//==================================================================
// FE  
    Uint16 fe[FENUM];                       // FE  ����
//==================================================================
// FF  
    Uint16 ff[FFNUM];                       // FF  ����

//======================================

//======================================
// E0 ��Ƶ�������ͼ���--------ԭFB
//˳��û�б䶯--mrb 20130109
//δ��20130228 by mrb
    Uint16 swingBaseMode;                   // E0-00  ��Ƶ�趨��ʽ
    Uint16 swingAmplitude;                  // E0-01  ��Ƶ����
    Uint16 swingJumpRange;                  // E0-02  ͻ��Ƶ�ʷ���
    Uint16 swingPeriod;                     // E0-03  ��Ƶ����
    Uint16 swingRiseTimeCoeff;              // E0-04  ��Ƶ�����ǲ�����ʱ��
    Uint16 lengthSet;                       // E0-05  �趨����
    Uint16 lengthCurrent;                   // E0-06  ʵ�ʳ���
    Uint16 lengthPulsePerMeter;             // E0-07  ÿ������������λ: 0.1
    Uint16 counterSet;                      // E0-08  �趨����ֵ
    Uint16 counterPoint;                    // E0-09  ָ������ֵ

//======================================
// E1 ����١�PLC--------ԭFC
//˳��û�б䶯--mrb 20130109
//δ��20130228 by mrb
    Uint16 plcFrq[PLC_STEP_MAX];                  // E1-00  --E1-15   �ָ?-���ָ��15
    Uint16 plcRunMode;                            // E1-16  PLC���з�ʽ
    Uint16 plcPowerOffRemMode;                    // E1-17  PLC�������ѡ��
    struct PLC_STRUCT plcAttribute[PLC_STEP_MAX]; // E1-18  --E1-49   PLC����ʱ�䣬�Ӽ���ʱ��ѡ��
    Uint16 plcTimeUnit;                           // E1-50  PLC����ʱ�䵥λ
    Uint16 plcFrq0Src;                            // E1-51  ���ָ��0������ʽ

//======================================
// E2 PID����--------ԭFA--��20130228 by mrb
    Uint16 pidSetSrc;                       // E2-00  PID����Դ
    Uint16 pidSet;                          // E2-01  PID��ֵ����, 0.1%
    Uint16 pidFdbSrc;                       // E2-02  PID����Դ
    Uint16 pidDir;                          // E2-03  PID���÷���
    Uint16 pidDisp;                         // E2-04  PID������������
    Uint16 reverseCutOffFrq;                // E2-05  PID��ת��ֹƵ��
    Uint16 pidErrMin;                       // E2-06  PIDƫ���
    Uint16 pidDLimit;                       // E2-07  PID΢���޷�
    Uint16 pidSetChangeTime;                // E2-08  PID���仯ʱ��
    Uint16 pidFdbLpfTime;                   // E2-09  PID�����˲�ʱ��
    Uint16 pidOutLpfTime;                   // E2-10  PID����˲�ʱ��
    Uint16 pidFdbLoseDetect;                // E2-11  PID������ʧ���ֵ
    Uint16 pidFdbLoseDetectTime;            // E2-12  PID������ʧ���ʱ��
    Uint16 pidKp;                           // E2-13  ��������P
    Uint16 pidTi;                           // E2-14  ����ʱ��I
    Uint16 pidTd;                           // E2-15  ΢��ʱ��D
    Uint16 pidKp2;                          // E2-16  PID��������P2
    Uint16 pidTi2;                          // E2-17  PID����ʱ��I2
    Uint16 pidTd2;                          // E2-18  PID΢��ʱ��D2
    Uint16 pidParaChgCondition;             // E2-19  PID�����л�����
    Uint16 pidParaChgDelta1;                // E2-20  PID�����л�ƫ��1
    Uint16 pidParaChgDelta2;                // E2-21  PID�����л�ƫ��2
    Uint16 pidIAttribute;                   // E2-22  PID��������
    Uint16 pidInit;                         // E2-23  PID��ֵ
    Uint16 pidInitTime;                     // E2-24  PID��ֵ����ʱ��
    Uint16 pidOutDeltaMax;                  // E2-25  PID�������֮��ƫ������ֵ
    Uint16 pidOutDeltaMin;                  // E2-26  PID�������֮��ƫ�����Сֵ
    Uint16 pidCalcMode;                     // E2-27  PID����ģʽ(ͣ���Ƿ�����). ��ˮģʽ�£�ͣ��ʱPIDҲ����.
    Uint16 pidSampleTime;                   // E2-28  PID��������(��δ��)

//======================================
// E3 ����DI������DO--------ԭA1
//��20130409 by mrb
    Uint16 vdiFunc[5];              // E3-00  --E3-04 VDI1���ӹ���ѡ��
    Uint16 vdiFcSet;                // E3-05  VDI���ӹ������趨��Ч״̬
    Uint16 vdiSrc;                  // E3-06  VDI������Ч״̬��Դ
    Uint16 aiAsDiFunc[3];           // E3-07  --E3-09 AI1���ӹ���ѡ�񣨵���DI��
    Uint16 diLogicAiAsDi;           // E3-10  AI��ΪDI��Ч״̬ѡ��
    Uint16 vdoFunc[5];              // E3-11  --E3-15 ����VDO1��VDO5���ѡ��
    Uint16 vdoLogic;                // E3-16  VDO���������Ч״̬ѡ��
    Uint16 vdoDelayTime[5];         // E3-17  --E3-21 VDO1��VDO5�ӳ�ʱ��

//======================================
// E4 �û����ƹ�����--------ԭFE
//û�иı�----mrb20130109
    Uint16 userCustom[E4NUM];       // E4 �û����ƹ�������
//======================================
// E5 ģ��������--------ԭA6
    Uint16 curveSet4P1[8];          // E5-00    --E5-07  ����4��4�㣬���ֵ����Сֵ��2���м��
    Uint16 curveSet4P2[8];          // E5-08    --E5-15  ����5��4��
    Uint16 rsvdA41[8];

    struct AI_JUMP aiJumpSet[AI_NUMBER]; // E5-24 --E5-29, AI1, AI2, AI3��Ծ
//======================================
// E6 ����ѡ��--------ԭA7
    Uint16 plcEnable;               // E6-00 PLC������ѡ��
    Uint16 outPortControl;          // E6-01 ������ӿ���
    Uint16 plcAI3Cfg;               // E6-02 PLC AI3��������
    Uint16 fmpValue;                // E6-03 FMP��� 
    Uint16 ao1Value;                // E6-04 AO1���
    Uint16 inPortOut;               // E6-05 ���������
    Uint16 plcFrqSet;               // E6-06 PLC��Ƶ�ʸ���
    Uint16 plcTorqueSet;            // E6-07 PLC��ת�ظ���
    Uint16 plcCmd;                  // E6-08 PLC���������
    Uint16 plcErrorCode;            // E6-09 PLC�����ϸ���
    Uint16 rsvdA7[2];
//======================================
// E7 ��Ե�ͨѶ����--------ԭA8
    Uint16 p2pEnable;               // E7-00 ��Ե�ͨѶ���ܡ��
    Uint16 p2pTypeSel;              // E7-01 ����ѡ��
    Uint16 p2pSendDataSel;          // E7-02 ������������   0:���ת��  1:����Ƶ��  2:�趨Ƶ��  3:�����ٶ�
    Uint16 p2pRevDataSel;           // E7-03 �ӻ���������   0:ת�ظ���  1:Ƶ�ʸ���  
    Uint16 p2pRevOffset;            // E7-04 ����������ƫ
    Uint16 p2pRevGain;              // E7-05 ������������
    Uint16 p2pOverTime;             // E7-06 ��Ե�ͨѶ�жϼ��ʱ��
    Uint16 p2pSendPeriod;           // E7-07 ��Ե�ͨѶ�������ݷ�������
//======================================
// E8 ����--------ԭA9
    Uint16 e8[E8NUM];               // E8
//======================================
// E9 ʸ���Ż�����--------ԭAA
    Uint16 e9[E9NUM];               // E9
    #if 0
    Uint16 motorCtrlM1;             // E9-00 ���ŵ�����ʽ
    Uint16 motorCtrlM2;             // E9-01 ����ѹ���㷽ʽ
    Uint16 motorCtrlM3;             // E9-02 ���ŵ���������KP
    Uint16 motorCtrlM4;             // E9-03 ���ŵ���������KI
    Uint16 motorCtrlM5;             // E9-04 ���ŵ�������������
    Uint16 motorCtrlM6;             // E9-05 ���ŵ�������������
    Uint16 motorCtrlM7;             // E9-06 ת���������
    Uint16 motorCtrlM8;             // E9-07 ת��������
    Uint16 motorCtrlM9;             // E9-08 ��������
    Uint16 motorCtrlM10;            // E9-09 ���Ƶ������ģʽ
    Uint16 motorCtrlM11;            // E9-10 ��ѹ������ֵ����
    Uint16 motorCtrlM12;            // E9-11 ��ѹ��������
    Uint16 motorCtrlM13;            // E9-12 �ٶȻ�����
    Uint16 motorCtrlM14;            // E9-13 �������˲�
    Uint16 motorCtrlM15;            // E9-14 ����ǶȲ���
    Uint16 motorCtrlM16;            // E9-15 SVCת�ؿ����Ż�
    #endif
//======================================
// EA VF�Ż�����--------ԭAB
    Uint16 ea[EANUM];               // EA
    #if 0
    Uint16 vfCtrlM2;                // EA-01 DPWM�л�����Ƶ��   86
    Uint16 vfCtrlM3;                // EA-02 ���������Ż�����   87
    Uint16 vfCtrlM4;                // EA-03 ����ǯλ����ϵ��    1
    Uint16 vfCtrlM5;                // EA-04 ǯλ�Ż�����Ƶ��   101
    Uint16 vfCtrlM6;                // EA-05 ǯλ�Ż�����Ƶ��   102
    Uint16 vfCtrlM7;                // EA-06 ������ģʽ       89
    Uint16 vfCtrlM8;                // EA-07 �����Ʒ�ֵ����   90
    #endif
//======================================
// EB ����    
    Uint16 eb[EBNUM]; 
//======================================
// EC ����--------ԭAD
    Uint16 ec[ECNUM]; 
//======================================
// ED AIAOУ��------ԭAC
    struct ANALOG_CALIBRATE_CURVE aiCalibrateCurve[AI_NUMBER];  // ED-00    ----ED-11, AI1/2/3У������
    struct ANALOG_CALIBRATE_CURVE aoCalibrateCurve[AO_NUMBER];  // ED-12    ----ED-19, AO1/AO2У������
//======================================
// EE ����--------ԭAF
    Uint16 ee[EENUM];               // EE
//======================================
// EF ����
    Uint16 ef[EFNUM];               // EF

//======================================
// B0 ��һ�������------ԭF1,F1-37  ��гѡ���Ƶ�F7��--����������
    union MOTOR_PARA motorParaM1;   // B0-00  B0-26 ��1�������
    Uint16 tuneCmd;                 // B0-27  ��гѡ��-----�е�����ţ�37->27
    union PG_PARA    pgParaM1;      // B0-28  B0-37 ��1���PG������
//======================================
// B1 ��2�������------ԭA2
    struct MOTOR_FC motorFcM2;      // ��2�������
//======================================
// B2 ��3�������------ԭA3
    struct MOTOR_FC motorFcM3;      // ��3�������
    
// B3 ��4�������------ԭA4
    struct MOTOR_FC motorFcM4;      // ��4�������
//B4
    Uint16 b4[B4NUM];               // B4
                                         
//B5                                     
    Uint16 b5[B5NUM];               // B5 
                                         
//B6                                     
    Uint16 b6[B6NUM];               // B6
                                         
//B7                                     
    Uint16 b7[B7NUM];               // B7
                                         
//B8                                     
    Uint16 b8[B8NUM];               // B8
                                         
//B9                                     
    Uint16 b9[B9NUM];               // B9
                                         
//BA                                     
    Uint16 ba[BANUM];               // BA
                                         
//BB                                     
    Uint16 bb[BBNUM];               // BB
                                         
//BC                                     
    Uint16 bc[BCNUM];               // BC
                                         
//BD                                     
    Uint16 bd[BDNUM];               // BD
                                         
//BE                                     
    Uint16 be[BENUM];               // BE
                                         
//BF                                     
    Uint16 bf[BFNUM];               // BF
    

//======================================
// y0 �û�����, ������ʼ��--------ԭFP
//δ��20130228 by mrb
    Uint16 paraInitMode;            // Y0-00  ������ʼ��
    Uint16 userPassword;            // Y0-01  �û�����
    Uint16 funcParaView;            // Y0-02  ���ܲ���ģʽ����
    Uint16 menuMode;                // Y0-03  ���Ի�����ģʽѡ��
    Uint16 userPasswordReadOnly;    // Y0-04  ֻ���û�����
    Uint16 rsvdFp;                  // Y0-05  ����
//======================================
// y1 ������ʷ��----------ԭF9�е�F9.14~F9.44
    Uint16 errorLatest1;                // Y1-00  ��һ�ι�������
    
    Uint16 errorLatest2;                // Y1-01  �ڶ��ι�������
    Uint16 errorLatest3;                // Y1-02  ������(���һ��)��������

    union ERROR_SCENE errorScene3;      // Y1-03  ������(���һ��)����ʱƵ��                              
                                        // Y1-04  ������(���һ��)����ʱ����                             
                                        // Y1-05  ������(���һ��)����ʱĸ�ߵ�ѹ                                                                                  
                                        // Y1-06  ������(���һ��)����ʱ�������״̬                     
                                        // Y1-07  ������(���һ��)����ʱ�������״̬   
                                        
                                        // Y1-08  ������(���һ��)����ʱ��Ƶ��״̬                       
                                        // Y1-09  ������(���һ��)����ʱʱ�䣨�ӱ����ϵ翪ʼ��ʱ��       
                                        // Y1-10  ������(���һ��)����ʱʱ�䣨������ʱ��ʼ��ʱ��         

    Uint16 rsvdF92[2];                  // Y1-11  Y1-12
    
    union ERROR_SCENE errorScene2;      // Y1-13  �ڶ��ι����ֳ�
    Uint16 rsvdF921[2];                 // Y1-21  Y1-22
    union ERROR_SCENE errorScene1;      // Y1-23  ��һ�ι����ֳ�
//======================================
    Uint16 y2[Y2NUM];               // ����
    Uint16 y3[Y3NUM];               // ����
    Uint16 y4[Y4NUM];               // ����
    Uint16 y5[Y5NUM];               // ����
    Uint16 y6[Y6NUM];               // ����
    Uint16 y7[Y7NUM];               // ����
//======================================
// L0 ���Ҳ���--------ԭFF
    Uint16 factoryPassword;         // L0-00  ��������
    Uint16 inverterType;            // L0-01  ��Ƶ������
    Uint16 inverterGpType;          // L0-02  G/P�ͺ�
    Uint16 inverterPower;           // L0-03  ��Ƶ������
    Uint16 tempCurve;               // L0-04  �¶�����
    
    Uint16 uvGainWarp;              // L0-05  UV���������������ƫ��
    Uint16 funcSoftVersion;         // L0-06  ����
    Uint16 motorSoftVersion;        // L0-07  ��������汾��
    Uint16 volJudgeCoeff;           // L0-08  ��ѹУ��ϵ��
    Uint16 curJudgeCoeff;           // L0-09  ����У��ϵ��
    
    Uint16 motorDebugFc;            // L0-10  ���ܵ��Թ�������ʾ����
    Uint16 aiaoCalibrateDisp;       // L0-11  AIAOУ����������ʾ
    Uint16 memoryAddr;              // L0-12  �ڴ��ַ�鿴
//	Uint16 runTimeArriveSet1;       // L0-13  �����趨����ʱ��
//======================================
 
// L1 AIAO����У��ֵ--------ԭAE
    struct ANALOG_CALIBRATE_CURVE aiFactoryCalibrateCurve[AI_NUMBER];   // L1-00 
    struct ANALOG_CALIBRATE_CURVE aoFactoryCalibrateCurve[AO_NUMBER];   // L1-12
    
    Uint16 l2[L2NUM];               // ���� 
    Uint16 l3[L3NUM];               // ����
    Uint16 l4[L4NUM];               // ����
    Uint16 l5[L5NUM];               // ����
    Uint16 l6[L6NUM];               // ����
    Uint16 l7[L7NUM];               // ����
    Uint16 l8[L8NUM];               // ����   
    Uint16 l9[L9NUM];               // ����   



//======================================
// eepromCheckWord
    Uint16 rsvd4All;                // ������������ǰ��
    Uint16 eepromCheckWord1;        //        eepromCheckWord1
    Uint16 eepromCheckWord2;        //        eepromCheckWord2
    Uint16 aiaoChkWord;             // AIAO����У��

//======================================
// REMEMBER ���籣�棬��48��
    Uint16 extendType;                  // FR-00  extendType
    Uint16 plcStepRem;                  // FR-01  PLC��ǰstep
    Uint16 plcTimeHighRem;              // FR-02  PLC��ǰstep���е�ʱ�䣬��λ
    Uint16 plcTimeLowRem;               // FR-03  PLC��ǰstep���е�ʱ�䣬��λ
    Uint16 dispParaRunBit;              // FR-04  ����ʱLED��ʾ������bitλֵ
    Uint16 dispParaStopBit;             // FR-05  ͣ��ʱLED��ʾ������bitλ��
    Uint16 runTimeAddupSec;             // FR-06  �ۼ�����ʱ���s(��)
    Uint16 counterTicker;               // FR-07  �����������ticker
    Uint16 lengthTickerRemainder;       // FR-08  ���ȼ�������tickerDelta��Remainder
    Uint16 frqComm;                     // FR-09  ͨѶ�޸�Ƶ��ֵ, 100.00%-maxFrq
    Uint16 upDownFrqRem;                // FR-10  UP/DOWN��Ƶ��
    Uint16 pmsmRotorPos;                // FR-11  ͬ����ת��λ��
    Uint16 powerAddupInt;               // FR-12  �ۼƺĵ�����������
    Uint16 powerUpTimeAddupSec;         // FR-13  �ۼ��ϵ�ʱ���s(��)
    Uint16 errorFrqUnit;                // FR-14  ����ʱƵ�ʼ�¼
    Uint16 saveUserParaFlag1;           // FR-15  �ѱ����û�������־1
    Uint16 saveUserParaFlag2;           // FR-16  �ѱ����û�������־2
    Uint16 speedFdbDir;                 // FR-17  ��������ٶȷ���
    Uint16 rsvdRem[2];                  // FR-18~FR-19
    Uint16 rsvdRem1[23];                // Ԥ��
    Uint16 remPOffMotorCtrl[REM_P_OFF_MOTOR];  // FR-43~FR-47  ����ʹ�õĵ������
//======================================
// D0 ���Ӳ���--------ԭU0
    Uint16 d0[D0NUM];               // D0 ��ʾ
    Uint16 d1[D1NUM];               // D1
    Uint16 d2[D2NUM];               // D2
    Uint16 d3[D3NUM];               // D3

//======================================
    Uint16 d4[D4NUM];               // D4
    Uint16 d5[D5NUM];               // D5
    Uint16 d6[D6NUM];               // D6
    Uint16 d7[D7NUM];               // D7

//======================================
    Uint16 d8[D8NUM];               // D8
    Uint16 d9[D9NUM];               // D9
    Uint16 da[DANUM];               // DA
    Uint16 db[DBNUM];               // DB

//======================================
    Uint16 dc[DCNUM];               // DC
    Uint16 dd[DDNUM];               // DD
    Uint16 de[DENUM];               // DE
    Uint16 df[DFNUM];               // DF, ��ʾ�����ܵ���ʹ��
//======================================
};


//=====================================================================
//
// ������Ķ��塣
// �����壬��Ա�ֱ�Ϊ���飬�ṹ�壬�ṹ��
// ���ǣ�һ��������ķ��ʣ������ַ�ʽ:
// funcCode.all[index]     
// funcCode.group.f0[8]     index = GetCodeIndex(funcCode.group.f0[8]);
// funcCode.code.presetFrq   index = GetCodeIndex(funcCode.code.presetFrq);
// 
//=====================================================================
typedef union FUNCCODE_ALL_UNION
{
    Uint16 all[FNUM_ALL];

    struct FUNCCODE_GROUP group;

    struct FUNCCODE_CODE code;
} FUNCCODE_ALL;


// ����Ƶ��Դѡ��
#define FUNCCODE_frqXySrc_FC                0   // �������趨����粻���?
#define FUNCCODE_frqXySrc_FC_P_OFF_REM      1   // �������趨���������
#define FUNCCODE_frqXySrc_AI1               2   // AI1
#define FUNCCODE_frqXySrc_AI2               3   // AI2
#define FUNCCODE_frqXySrc_AI3               4   // keyPanel rev by qtl 20131218 for Keyboard potentiometer
#define FUNCCODE_frqXySrc_PULSE             5   // PULSE�����趨(DI5)
#define FUNCCODE_frqXySrc_MULTI_SET         6   // ���ָ��
#define FUNCCODE_frqXySrc_PLC               7   // PLC
#define FUNCCODE_frqXySrc_PID               8   // PID
#define FUNCCODE_frqXySrc_COMM              9   // ͨѶ�趨
//#define FUNCCODE_frqXySrc_AI3               10   // AI3,add by qtl 20131218 for Keyboard potentiometer

// ����Ƶ��ԴY��Χѡ��
#define FUNCCODE_frqYRangeBase_MAX_FRQ      0   // ��������Ƶ��
#define FUNCCODE_frqYRangeBase_FRQ_X        1   // �������Ƶ��ԴX

// Ƶ��Դ(�л���ϵ)ѡ��
#define FUNCCODE_frqCalcSrc_X               0   // ��Ƶ��ԴX
#define FUNCCODE_frqCalcSrc_COMPOSE         1   // ����������
#define FUNCCODE_frqCalcSrc_X_OR_Y          2   // �� <--> ��
#define FUNCCODE_frqCalcSrc_X_OR_COMPOSE    3   // �� <--> ����������
#define FUNCCODE_frqCalcSrc_Y_OR_COMPOSE    4   // �� <--> ����������

// ����Ƶ�������ϵ
#define FUNCCODE_frqCalcSrc_ADD             0   // �� + ��
#define FUNCCODE_frqCalcSrc_SUBTRATION      1   // �� - ��
#define FUNCCODE_frqCalcSrc_MAX             2   // MAX(��, ��)
#define FUNCCODE_frqCalcSrc_MIN             3   // MIN(��, ��)
#define FUNCCODE_frqCalcSrc_4               4   //��*��/���Ƶ�� rcv by qtl 20130912
#define FUNCCODE_frqCalcSrc_5               5   // 

// ����Ƶ��Դ
#define FUNCCODE_upperFrqSrc_FC         0   // �������趨
#define FUNCCODE_upperFrqSrc_AI1        1   // AI1
#define FUNCCODE_upperFrqSrc_AI2        2   // AI2
#define FUNCCODE_upperFrqSrc_AI3        3   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_upperFrqSrc_keyPanel    3   //keyPanel,rev by qtl 20131218 for Keyboard potentiometer
#define FUNCCODE_upperFrqSrc_PULSE      4   // PULSE�����趨(DI5)
#define FUNCCODE_upperFrqSrc_COMM       5   // ͨѶ����
//#define FUNCCODE_upperFrqSrc_AI3        6   // AI3,add by qtl 20131218 for Keyboard potentiometer

// Ƶ��ָ��С����
#define FUNCCODE_frqPoint_1             0   // 0: 0��С���㣬1Hz
#define FUNCCODE_frqPoint_0_1           1   // 1: 1��С���㣬0.1Hz
#define FUNCCODE_frqPoint_0_01          2   // 2: 2��С���㣬0.01Hz

// �ز�Ƶ�ʵ���ѡ��
//#define FUNCCODE_autoCarrierFrq_0

// �Ӽ���ʱ��ĵ�λ
#define FUNCCODE_accDecTimeUnit_0POINT  0   // 0��С���㣬1s
#define FUNCCODE_accDecTimeUnit_1POINT  1   // 1��С���㣬0.1s
#define FUNCCODE_accDecTimeUnit_2POINT  2   // 2��С���㣬0.01s

// ��ֵ�趨Ƶ�ʼ����趨
#define FUNCCODE_frqRemMode_POWEROFF_NO     0   // ���粻����
#define FUNCCODE_frqRemMode_POWEROFF_YES    1   // �������
#define FUNCCODE_frqRemMode_STOP_NO         0   // ͣ��������
#define FUNCCODE_frqRemMode_STOP_YES        1   // ͣ������

// �Ӽ��ٷ�ʽ
#define FUNCCODE_accDecSpdCurve_LINE        0   // ֱ�߼Ӽ���
#define FUNCCODE_accDecSpdCurve_S_CURVE_A   1   // S����1����ͨ���η�
#define FUNCCODE_accDecSpdCurve_S_CURVE_B   2   // S����2���ο�����S����B
#define ACC_DEC_LINE    FUNCCODE_accDecSpdCurve_LINE
#define ACC_DEC_SA      FUNCCODE_accDecSpdCurve_S_CURVE_A
#define ACC_DEC_SB      FUNCCODE_accDecSpdCurve_S_CURVE_B

// ת������Դ
#define FUNCCODE_upperTorqueSrc_FC      0   // �������趨
#define FUNCCODE_upperTorqueSrc_AI1     1   // AI1
#define FUNCCODE_upperTorqueSrc_AI2     2   // AI2
#define FUNCCODE_upperTorqueSrc_AI3     3 // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_upperTorqueSrc_keyPanel  3   // keyPanel,rev by qtl 20131218 for Keyboard potentiometer

#define FUNCCODE_upperTorqueSrc_PULSE   4   // PULSE
#define FUNCCODE_upperTorqueSrc_COMM    5   // ͨ�
#define FUNCCODE_upperTorqueSrc_MIN_AI1_AI2 6  // min(ai1,ai2)
#define FUNCCODE_upperTorqueSrc_MAX_AI1_AI2 7  // max(ai1,ai2)
//#define FUNCCODE_upperTorqueSrc_AI3     8   // AI3,add by qtl 20131218 for Keyboard potentiometer
// FVC��PG��ѡ��, 0-QEP1(����PG),1-QEP2(��չPG)
#define FUNCCODE_fvcPgSrc_QEP1          0   // QEP1
#define FUNCCODE_fvcPgSrc_QEP2          1   // QEP2, ��չPG��

#define TIME_UNIT_ACC_DEC_SPEED         100 // �Ӽ���ʱ��λ, ms


// VF�����趨
#define FUNCCODE_vfCurve_Line               0   // ֱ��VF
#define FUNCCODE_vfCurve_DOT                1   // ���VF
#define FUNCCODE_vfCurve_SQUARE             2   // ƽ��VF
#define FUNCCODE_vfCurve_ALL_SEPARATE       10  // VF��ȫ����ģʽ
#define FUNCCODE_vfCurve_HALF_SEPARATE      11  // VF�����ģʽ

// vfVoltageSrc, VF����ĵ�ѹԴ
#define FUNCCODE_vfVoltageSrc_FC            0   // �������趨
#define FUNCCODE_vfVoltageSrc_AI1           1   // AI1
#define FUNCCODE_vfVoltageSrc_AI2           2   // AI2
#define FUNCCODE_vfVoltageSrc_AI3           3   // AI3,del by qtl 20131218 for Keyboard potentiometer 
//#define FUNCCODE_vfVoltageSrc_keyPanel      3   //keyPane,rev by qtl 20131218 for Keyboard potentiometer 

#define FUNCCODE_vfVoltageSrc_PULSE         4   // PULSE�����趨(DI5)
#define FUNCCODE_vfVoltageSrc_MULTI_SET     5   // ���ָ��
#define FUNCCODE_vfVoltageSrc_PLC           6   // PLC
#define FUNCCODE_vfVoltageSrc_PID           7   // PID
#define FUNCCODE_vfVoltageSrc_COMM          8   // ͨѶ�趨
//#define FUNCCODE_vfVoltageSrc_AI3           9   // AI3,add by qtl 20131218 for Keyboard potentiometer 

// λ�ÿ���ѡ��
#define FUNCCODE_posCtrl_NONE               0   // ��λ�ÿ���
#define FUNCCODE_posCtrl_POSITION_CTRL      1   // λ�ÿ���
#define FUNCCODE_posCtrl_SWITCH_TO_PC       2   // �ٶ�/ת�ؿ���<->λ�ÿ���
#define FUNCCODE_posCtrl_SWITCH_FROM_PC     3   // λ�ÿ���<->�ٶ�/ת�ؿ���

// λ�ÿ���ģʽ
#define FUNCCODE_pcMode_PCMD            0   // Pcmd
#define FUNCCODE_pcMode_APTP            1   // APTP
#define FUNCCODE_pcMode_SWITCH_TO_APTP  2   // Pcmd<->AP2P

// λ��ָ�����巽ʽ
#define FUNCCODE_pcPulseType_PULSE_AND_DIR  0   // ����+����
#define FUNCCODE_pcPulseType_QUADRATURE     1   // 2·��������
#define FUNCCODE_pcPulseType_CW_AND_CCW     2   // CW+CCW

// ��λ����ģʽ
#define FUNCCODE_aptpMode_RELATIVE      0   // ���ʽ
#define FUNCCODE_aptpMode_ABSOLUTE      1   // ����ʽ
#define FUNCCODE_aptpMode_INDEX         2   // �ֶ���

// λ�ÿ������ѡ��
#define FUNCCODE_pcZeroSelect_ENCODER   0   // ������index�ź�
#define FUNCCODE_pcZeroSelect_DI        1   // DI����

// PG����װλ��
#define FUNCCODE_pgLocation_MOTOR       0   // �����
#define FUNCCODE_pgLocation_AXIS        1   // ��������

//=====================================================================
// (���)���Ʒ�ʽ

#define FUNCCODE_motorCtrlMode_SVC  0   // SVC
#define FUNCCODE_motorCtrlMode_FVC  1   // FVC
#define FUNCCODE_motorCtrlMode_VF   2   // VF

// ����Դѡ��
#define FUNCCODE_runSrc_PANEL       0   // ����������ͨ��
#define FUNCCODE_runSrc_DI          1   // ��������ͨ��
#define FUNCCODE_runSrc_COMM        2   // ���п�ͨѶ����ͨ��
#define FUNCCODE_runSrc_AUTO_RUN    3   // �ϵ�����

// ���з���
#define FUNCCODE_runDir_NO_REVERSE      0   // ��һ��
#define FUNCCODE_runDir_REVERSE         1   // �����෴

// ��гѡ��
#define FUNCCODE_tuneCmd_NONE           0   // �޲���
#define FUNCCODE_tuneCmd_ACI_STATIC     1   // �첽����ֹ��г
#define FUNCCODE_tuneCmd_ACI_WHOLE      2   // �첽��������г
#define FUNCCODE_tuneCmd_PMSM_11        11  // ͬ����
#define FUNCCODE_tuneCmd_PMSM_12        12  // ͬ����
#define FUNCCODE_tuneCmd_PMSM_13        13  // ͬ����

// �������ʽ
#define FUNCCODE_diControlMode_2LINE1   0   // ����ʽ1
#define FUNCCODE_diControlMode_2LINE2   1   // ����ʽ2
#define FUNCCODE_diControlMode_3LINE1   2   // ����ʽ1
#define FUNCCODE_diControlMode_3LINE2   3   // ����ʽ2

// �๦�ܶ������ѡ��
#define FUNCCODE_fmOutSelect_PULSE      0   // FMP�������
#define FUNCCODE_fmOutSelect_DO         1   // DO
#define FUNCCODE_fmOutSelect_AO         2   // AO

// ������ʽ
#define FUNCCODE_startMode_DIRECT_START 0   // ֱ������
#define FUNCCODE_startMode_SPEED_TRACK  1   // ת�ٸ���������
#define FUNCCODE_startMode_FORE_MAG     2   // �첽����������

// ͣ����ʽ
#define FUNCCODE_stopMode_DEC_STOP      0   // ����ͣ��
#define FUNCCODE_stopMode_FREESTOP      1   // ����ͣ��
#define FUNCCODE_stopMode_HURRY_STOP    2   // ��ͣͣ��

// Ƶ�ʵ�������Ƶ�����ж���
#define FUNCCODE_lowerDeal_RUN_LOWER    0   // ������Ƶ������
#define FUNCCODE_lowerDeal_DELAY_STOP   1   // ��ʱͣ��
#define FUNCCODE_lowerDeal_RUN_ZERO     2   // ��������

// �趨����ʱ�䵽�ﶯ��ѡ��
#define FUNCCODE_runTimeOverAct_RUN     0   // ��������
#define FUNCCODE_runTimeOverAct_STOP    1   // ͣ��

// �趨�ϵ�ʱ�䵽�ﶯ��ѡ��
#define FUNCCODE_powerUpTimeOverAct_RUN     0   // ��������
#define FUNCCODE_powerUpTimeOverAct_STOP    1   // ͣ��

// PID����Դ
#define FUNCCODE_pidSetSrc_FC               0   // �������趨
#define FUNCCODE_pidSetSrc_AI1              1   // AI1
#define FUNCCODE_pidSetSrc_AI2              2   // AI2
#define FUNCCODE_pidSetSrc_AI3              3   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_pidSetSrc_keyPanel              3   // keyPanel,rev by qtl 20131218 for Keyboard potentiometer

#define FUNCCODE_pidSetSrc_PULSE            4   // PULSE
#define FUNCCODE_pidSetSrc_COMM             5   // ͨ?
#define FUNCCODE_pidSetSrc_MULTI_SET        6   // ���ָ��
//#define FUNCCODE_pidSetSrc_AI3              7   // AI3,add by qtl 20131218 for Keyboard potentiometer

// PID����Դ
#define FUNCCODE_pidFdbSrc_AI1              0   // AI1
#define FUNCCODE_pidFdbSrc_AI2              1   // AI2
#define FUNCCODE_pidFdbSrc_AI3              2   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_pidFdbSrc_keyPanel         2   // AI3,rev by qtl 20131218 for Keyboard potentiometer
#define FUNCCODE_pidFdbSrc_AI1_SUB_AI2      3   // AI1-AI2
#define FUNCCODE_pidFdbSrc_PULSE            4   // PULSE
#define FUNCCODE_pidFdbSrc_COMM             5   // ͨѶ
#define FUNCCODE_pidFdbSrc_AI1_ADD_AI2      6   // AI1+AI2
#define FUNCCODE_pidFdbSrc_MAX_AI           7   // MAX(|AI1|, |AI2|)
#define FUNCCODE_pidFdbSrc_MIN_AI           8   // MIN(|AI1|, |AI2|)
//#define FUNCCODE_pidFdbSrc_AI3              9   // AI3,add by qtl 20131218 for Keyboard potentiometer

// PID�����л�����
#define FUNCCODE_pidParaChgCondition_NO         0   // ���л�
#define FUNCCODE_pidParaChgCondition_DI         1   // DI����
#define FUNCCODE_pidParaChgCondition_PID_ERROR  2   // ����ƫ���Զ��л�

// PID����ģʽ
#define FUNCCODE_pidCalcMode_NO             0   // ͣ��ʱ������
#define FUNCCODE_pidCalcMode_YES            1   // ͣ��ʱ����

// ��Ƶ�趨��ʽ
#define FUNCCODE_swingBaseMode_AGAIN_FRQSETAIM  0   // ���������Ƶ��(�趨Ƶ��)
#define FUNCCODE_swingBaseMode_AGAIN_MAXFRQ     1   // ��������Ƶ��

// MF.K������ѡ��
#define FUNCCODE_mfkKeyFunc_NONE        0   // MF.K��������Ч
#define FUNCCODE_mfkKeyFunc_SWITCH      1   // ��������ͨ���л�
#define FUNCCODE_mfkKeyFunc_REVERSE     2   // ����ת�л�
#define FUNCCODE_mfkKeyFunc_FWD_JOG     3   // ��ת�㶯����
#define FUNCCODE_mfkKeyFunc_REV_JOG     4   // ��ת�㶯����

// STOP/RES������
#define FUNCCODE_stopKeyFunc_KEYBOARD   0   // ͣ�����ܽ��ڼ��̿��Ʒ�ʽʱ��Ч
#define FUNCCODE_stopKeyFunc_ALL        1   // ����Ч

// ���ָ��0������ʽ
#define FUNCCODE_plcFrq0Src_FC          0   // ������FC-00����
#define FUNCCODE_plcFrq0Src_AI1         1   // AI1
#define FUNCCODE_plcFrq0Src_AI2         2   // AI2
#define FUNCCODE_plcFrq0Src_AI3         3   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_plcFrq0Src_keyPanel    3   // keyPanel,rev by qtl 20131218 for Keyboard potentiometer 
#define FUNCCODE_plcFrq0Src_PULSE       4   // PULSE
#define FUNCCODE_plcFrq0Src_PID         5   // PID����
#define FUNCCODE_plcFrq0Src_PRESET_FRQ  6   // ���Ƶ��
//#define FUNCCODE_plcFrq0Src_AI3         7   // AI3,add by qtl 20131218 for Keyboard potentiometer   
// PLC���з�ʽ
#define FUNCCODE_plcRunMode_ONCE_STOP   0   // �������н���ͣ��
#define FUNCCODE_plcRunMode_ONCE_RUN    1   // �������н���������ֵ
#define FUNCCODE_plcRunMode_REPEAT      2   // һֱѭ��

// PLC�������ѡ��
#define FUNCCODE_plcPowerOffRemMode_NO_REM  0   // ���粻����
#define FUNCCODE_plcPowerOffRemMode_REM     1   // �������
// PLCͣ������ѡ��
#define FUNCCODE_plcStopRemMode_NO_REM  0   // ���粻����
#define FUNCCODE_plcStopRemMode_REM     1   // �������

// PLC����ʱ�䵥λ
#define FUNCCODE_plcTimeUnit_S      0   // S(��)
#define FUNCCODE_plcTimeUnit_H      1   // H(Сʱ)

// ����¶ȴ���������
#define FUNCCODE_tempSenorType_NONE         0       // PTC100
#define FUNCCODE_tempSenorType_PTC100       1       // PTC100
#define FUNCCODE_tempSenorType_PTC1000      2       // PTC1000
#define FUNCCODE_tempSenorType_NTC          3       // NTC

// ���ݸ�ʽ
#define FUNCCODE_sciParity_NONE     0   // ��У��(8-N-2)
#define FUNCCODE_sciParity_EVEN     1   // żУ��(8-E-1)
#define FUNCCODE_sciParity_ODD      2   // ��У��(8-O-1)
#define FUNCCODE_sciParity_NONE1    3   // ��У��(8-N-1)



// �������ʱ�䵥λ
// ע�⣬������Ϊ�˼�С����Ҫ�ļ���Ϳռ�ռ�ã�����ʹ����
// X * (TIME_UNIT_WAIT_STOP_BRAKE / RUN_CTRL_PERIOD) �ķ�ʽ
// �����ǣ�(X * TIME_UNIT_WAIT_STOP_BRAKE) / RUN_CTRL_PERIOD
// ֮���޸���Щʱ�䵥λ�������б�Ҫ�޸ġ�
#define TIME_UNIT_SEC_PER_HOUR          3600    // 1hour = 3600sec
#define TIME_UNIT_MIN_PER_HOUR          60      // 1hour = 60min
#define TIME_UNIT_SEC_PER_MIN           60      // 1min  = 60sec
#define TIME_UNIT_MS_PER_SEC            1000    // 1s = 1000ms

#define TIME_UNIT_VF_VOL_ACC_TIME       100     // VF����ĵ�ѹ����ʱ��

#define TIME_UNIT_AI_PULSE_IN_FILTER    10      // AI,pulseIn�˲�ʱ��, ms
#define TIME_UNIT_DI_DELAY              100     // DI����ӳ�ʱ��, ms
#define TIME_UNIT_DO_DELAY              100     // DO����ӳ�ʱ��, ms
#define TIME_UNIT_START_FRQ_WAIT        100      // ����Ƶ�ʱ���ʱ�䣬ms
#define TIME_UNIT_START_BRAKE           10      // ����ֱ���ƶ�ʱ�䣬ms
#define TIME_UNIT_WAIT_STOP_BRAKE       10      // ͣ��ֱ���ƶ��ȴ�ʱ�䣬ms
#define TIME_UNIT_STOP_BRAKE            10      // ͣ��ֱ���ƶ�ʱ�䣬ms
#define TIME_UNIT_ZERO_SPEED_DEAD       100     // ����ת����ʱ��
#define TIME_UNIT_LOWER_STOP_DELAY      100     // Ƶ�ʵ�������Ƶ��ʱͣ�����ӳ�ʱ��
#define TIME_UNIT_PID_SET_CHANGE        10      // PID�����仯ʱ��
#define TIME_UNIT_PID_FILTER            10      // PID����������˲�ʱ��
#define TIME_UNIT_PID_INIT              10      // PID��ֵ����ʱ��
#define TIME_UNIT_PID_FDB_LOSE          100     // PID������ʧ���ʱ��
#define TIME_UNIT_SWING_PERIOD          100      // ��Ƶ����
#define TIME_UNIT_sciCommOverTime       100     // ͨѶ��ʱʱ��
#define TIME_UNIT_ERR_AUTO_RST_DELAY    100     // �����Զ���λ���ʱ�䣬ms
#define TIME_UNIT_ERR_AUTO_RST_CLR      (TIME_UNIT_SEC_PER_HOUR*100UL) // �����Զ���λ�������ʱ��, 0.1h
#define TIME_UNIT_P_OFF_VOL_BACK        10      // ˲ͣ��ͣ��ѹ�����ж�ʱ��
#define TIME_UNIT_PLC                   100     // PLC����ʱ�䵥λ

#define TIME_UNIT_ACC_DEC_SPEED_SERVO   10      // �ŷ��Ӽ���ʱ�䵥λ
#define TIME_UNIT_WAKE_UP               100     // ���ʱ��ĵλ
#define TIME_UNIT_DORMANT               100     // ����ʱ��ĵ�λ
#define TIME_UNIT_CURRENT_CHK           10      // �������ʱ�䵥λ
#define TIME_UNIT_TORQUE_CTRL_ACC_DEC   10      // ת�ؿ���ʱ�䵥λ
//=====================================================================

#if 0//F_DEBUG_RAM
#define ACC_DEC_T_INIT1  ((Uint32)2*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)    // �Ӽ���ʱ�����ֵ��2s������ <= 20
#define ACC_DEC_T_INIT2  ((Uint32)5*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)    // 5s������ > 20
#else
//============rev by xyw 20131216 for ACC_DEC time=================
#define ACC_DEC_T_INIT1  ((Uint32)10*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // �Ӽ���ʱ�����ֵ��20s������ <= 20
#define ACC_DEC_T_INIT2  ((Uint32)30*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 50s������ > 20
//#define ACC_DEC_T_INIT1  ((Uint32)10*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // �Ӽ���ʱ�����ֵ��10s������ <= 12---5.5KW
//#define ACC_DEC_T_INIT2  ((Uint32)20*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 20s������ 13~18----7.5~30KW
//#define ACC_DEC_T_INIT3  ((Uint32)40*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 40s������ >=19---37KW
//============rev by xyw 20131216 for ACC_DEC time=================


#endif

#define RATING_FRQ_INIT_0   50      // ����Ƶ�ʣ�0��С����
#define RATING_FRQ_INIT_1   500     // ����Ƶ�ʣ�1��С����
#define RATING_FRQ_INIT_2   5000    // ����Ƶ�ʣ�2��С����
#define BAUD_NUM_MAX        12   // ������ѡ��Χ�����ֵ
//#define BAUD_NUM_MAX 10
#define PARA_INIT_MODE_MAX  501       // ������ʼ������ֵ
#define INV_TYPE_MAX   30

#define MENU_MODE_MAX       3   // �˵�ģʽ�����ֵ

//=====================================================================
// ��Щ�������������������ĳ�������룬������funcCode�е�index

// ������ʹ�õ�һЩ�������index
//= �������/ɾ���˹���룬����Ҫ�޸?

#define FACTORY_PWD_INDEX      (GetCodeIndex(funcCode.code.factoryPassword))   // L0-00 ��������
#define INV_TYPE_INDEX         (GetCodeIndex(funcCode.code.inverterType))      // L0-01 ��Ƶ������
#define RATING_POWER_INVERTER_INDEX  (GetCodeIndex(funcCode.code.inverterPower))     // L0-03 ��Ƶ������
#define FUNCCODE_FACTORY_START_INDEX     (GetCodeIndex(funcCode.group.l0[0]))            // L0�鳧�Ҳ����Ŀ�ʼ
#define FUNCCODE_FACTORY_END_INDEX       (GetCodeIndex(funcCode.group.l0[L0NUM - 1]))    // L0�鳧�Ҳ����Ľ���

#define FC_MOTOR1_START_INDEX   (GetCodeIndex(funcCode.code.motorParaM1.all[0]))      // ��1�����������ʼ
#define FC_MOTOR1_END_INDEX     (GetCodeIndex(funcCode.code.pgParaM1.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // ��1��������Ľ���

#define FC_MOTOR2_START_INDEX   (GetCodeIndex(funcCode.code.motorFcM2.motorPara.all[0]))      // ��2�����������ʼ
#define FC_MOTOR2_END_INDEX     (GetCodeIndex(funcCode.code.motorFcM2.pgPara.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // ��2��������Ľ���

#define FC_MOTOR3_START_INDEX   (GetCodeIndex(funcCode.code.motorFcM3.motorPara.all[0]))      // ��3�����������ʼ
#define FC_MOTOR3_END_INDEX     (GetCodeIndex(funcCode.code.motorFcM3.pgPara.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // ��3��������Ľ���

#define FC_MOTOR4_START_INDEX   (GetCodeIndex(funcCode.code.motorFcM4.motorPara.all[0]))      // ��4�����������ʼ
#define FC_MOTOR4_END_INDEX     (GetCodeIndex(funcCode.code.motorFcM4.pgPara.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // ��4��������Ľ���



#define PRESET_FRQ_INDEX        (GetCodeIndex(funcCode.code.presetFrq))      // F0-08   Ԥ��Ƶ��
#define MAX_FRQ_INDEX           (GetCodeIndex(funcCode.code.maxFrq))         // F0-10   ���Ƶ��
#define UPPER_FRQ_INDEX         (GetCodeIndex(funcCode.code.upperFrq))       // F0-12   ����Ƶ��
#define LOWER_FRQ_INDEX         (GetCodeIndex(funcCode.code.lowerFrq))       // F0-14   ���Ƶ�?
#define ACC_TIME1_INDEX         (GetCodeIndex(funcCode.code.accTime1))       // F0-17   ����ʱ��1
#define DEC_TIME1_INDEX         (GetCodeIndex(funcCode.code.decTime1))       // F0-18   ����ʱ��1
#define CARRIER_FRQ_INDEX       (GetCodeIndex(funcCode.code.carrierFrq))     // F0-15   �ز�Ƶ��

#define RATING_POWER_INDEX      (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingPower))    // ��������
#define RATING_VOL_INDEX        (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingVoltage))  // ������ѹ
#define RATING_CUR_INDEX        (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingCurrent))  // ��������
#define RATING_CUR_INDEX2       (GetCodeIndex(funcCode.code.motorFcM2.motorPara.elem.ratingCurrent))    // ��2��������
#define RATING_CUR_INDEX3       (GetCodeIndex(funcCode.code.motorFcM3.motorPara.elem.ratingCurrent))   // ��3��������
#define RATING_CUR_INDEX4       (GetCodeIndex(funcCode.code.motorFcM4.motorPara.elem.ratingCurrent))   // ��4�������


#define RATING_FRQ_INDEX        (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingFrq))      // ����Ƶ��

#define ZERO_LOAD_CURRENT_INDEX (GetCodeIndex(funcCode.code.motorParaM1.elem.zeroLoadCurrent))// ���ص���
#define STATOR_RESISTANCE_INDEX (GetCodeIndex(funcCode.code.motorParaM1.elem.statorResistance))// ���ӵ���

#define ANTI_VIBRATE_GAIN_INDEX (GetCodeIndex(funcCode.code.antiVibrateGain))// F3-11   ����������
#define ANTI_VIBRATE_GAIN_MOTOR2_INDEX (GetCodeIndex(funcCode.code.motorFcM2.antiVibrateGain))// A4-52   ����������
#define ANTI_VIBRATE_GAIN_MOTOR3_INDEX (GetCodeIndex(funcCode.code.motorFcM3.antiVibrateGain))// A5-52   ����������
#define ANTI_VIBRATE_GAIN_MOTOR4_INDEX (GetCodeIndex(funcCode.code.motorFcM4.antiVibrateGain))// A6-52   ����������

#define TUNE_CMD_INDEX_1  (GetCodeIndex(funcCode.code.tuneCmd))  // ��г
#define TUNE_CMD_INDEX_2  (GetCodeIndex(funcCode.code.motorFcM2.tuneCmd))  // ��г
#define TUNE_CMD_INDEX_3  (GetCodeIndex(funcCode.code.motorFcM3.tuneCmd))  // ��г
#define TUNE_CMD_INDEX_4  (GetCodeIndex(funcCode.code.motorFcM4.tuneCmd))  // ��г

#define VC_CHG_FRQ1_INDEX (GetCodeIndex(funcCode.code.vcParaM1.vcSpdLoopChgFrq1))  // ʸ�������ٶȻ� �л����1
#define VC_CHG_FRQ2_INDEX (GetCodeIndex(funcCode.code.vcParaM1.vcSpdLoopChgFrq2))  // ʸ�������ٶȻ� �л�Ƶ��2

#define VC_CHG_FRQ1_INDEX2 (GetCodeIndex(funcCode.code.motorFcM2.vcPara.vcSpdLoopChgFrq1))  // ʸ�������ٶȻ� �л�Ƶ��1
#define VC_CHG_FRQ2_INDEX2 (GetCodeIndex(funcCode.code.motorFcM2.vcPara.vcSpdLoopChgFrq2))  // ʸ�������ٶȻ� �л�Ƶ��2

#define VC_CHG_FRQ1_INDEX3 (GetCodeIndex(funcCode.code.motorFcM3.vcPara.vcSpdLoopChgFrq1))  // ʸ�������ٶȻ� �л�Ƶ��1
#define VC_CHG_FRQ2_INDEX3 (GetCodeIndex(funcCode.code.motorFcM3.vcPara.vcSpdLoopChgFrq2))  // ʸ�������ٶȻ� �л�Ƶ��2

#define VC_CHG_FRQ1_INDEX4 (GetCodeIndex(funcCode.code.motorFcM4.vcPara.vcSpdLoopChgFrq1))  // ʸ�������ٶȻ� �л�Ƶ��1
#define VC_CHG_FRQ2_INDEX4 (GetCodeIndex(funcCode.code.motorFcM4.vcPara.vcSpdLoopChgFrq2))  // ʸ�������ٶȻ� �л�Ƶ��2


#define TORQUE_BOOST_INDEX      (GetCodeIndex(funcCode.code.torqueBoost))    // F1-05   ת������

#define TORQUE_BOOST_MOTOR2_INDEX      (GetCodeIndex(funcCode.code.motorFcM2.torqueBoost))    // D0-52   ��2���ת������
#define TORQUE_BOOST_MOTOR3_INDEX      (GetCodeIndex(funcCode.code.motorFcM3.torqueBoost))    // D0-52   ��3���ת������
#define TORQUE_BOOST_MOTOR4_INDEX      (GetCodeIndex(funcCode.code.motorFcM4.torqueBoost))    // D0-52   ?���ת������

#define SVC_MODE_INDX           (GetCodeIndex(funcCode.code.svcMode))        // A5-07 SVCģʽѡ��
#define OV_POINT_SET_INDEX      (GetCodeIndex(funcCode.code.ovPointSet))     // A5-09 ��ѹ������
#define UV_POINT_SET_INDEX      (GetCodeIndex(funcCode.code.uvPoint))        // A5-06 ��ѹ������ 20170525

#define VF_FRQ1_INDEX           (GetCodeIndex(funcCode.code.vfFrq1))         // F3-03   ���VFƵ�ʵ�1
#define VF_FRQ2_INDEX           (GetCodeIndex(funcCode.code.vfFrq2))         // F3-05   ���VFƵ�ʵ�2
#define VF_FRQ3_INDEX           (GetCodeIndex(funcCode.code.vfFrq3))         // F3-07   ���VFƵ�ʵ�3


#define CURVE1_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet2P1[0]))       // F2-08   AI1��С����
#define CURVE1_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet2P1[2]))       // F2-10   AI1�������
#define CURVE2_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet2P2[0]))       // F2-14   AI2��С����
#define CURVE2_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet2P2[2]))       // F2-16   AI2�������
#define CURVE3_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet2P3[0]))       // F2-14   AI3��С����
#define CURVE3_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet2P3[2]))       // F2-16   AI3�������
#define PULSE_IN_MIN_INDEX      (GetCodeIndex(funcCode.code.curveSet2P4[0]))     // F2-20   PULSE��С����
#define PULSE_IN_MAX_INDEX      (GetCodeIndex(funcCode.code.curveSet2P4[2]))     // F2-22   PULSE�������

#define CURVE4_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet4P1[0]))       // A8-00   AI4��С����
#define CURVE4_INFLEX1_INDEX    (GetCodeIndex(funcCode.code.curveSet4P1[2]))       // A8-02   AI4�յ�1����
#define CURVE4_INFLEX2_INDEX    (GetCodeIndex(funcCode.code.curveSet4P1[4]))       // A8-04   AI4�յ�2����
#define CURVE4_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet4P1[6]))       // A8-06   AI4�������
#define CURVE5_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet4P2[0]))       // A8-08   AI5��С����
#define CURVE5_INFLEX1_INDEX    (GetCodeIndex(funcCode.code.curveSet4P2[2]))       // A8-10   AI5�յ�1����
#define CURVE5_INFLEX2_INDEX    (GetCodeIndex(funcCode.code.curveSet4P2[4]))       // A8-12   AI5�յ�2����
#define CURVE5_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet4P2[6]))       // A8-14   AI5�������


#define ACC_TIME2_INDEX         (GetCodeIndex(funcCode.code.accTime2))       // F8-03 ����ʱ��2
#define DEC_TIME2_INDEX         (GetCodeIndex(funcCode.code.decTime2))       // F8-04 ����ʱ��2

#define ACC_TIME3_INDEX         (GetCodeIndex(funcCode.code.accTime3))       // F8-05 ����ʱ��3
#define DEC_TIME3_INDEX         (GetCodeIndex(funcCode.code.decTime3))       // F8-06 ����ʱ��3

#define ACC_TIME4_INDEX         (GetCodeIndex(funcCode.code.accTime4))       // F8-07 ����ʱ��4
#define DEC_TIME4_INDEX         (GetCodeIndex(funcCode.code.decTime4))       // F8-08 ����ʱ��4

#define RUN_TIME_ADDUP_INDEX    (GetCodeIndex(funcCode.code.runTimeAddup))     // F7-09  �ۼ�����ʱ��
#define POWER_TIME_ADDUP_INDEX  (GetCodeIndex(funcCode.code.powerUpTimeAddup)) // F7-13  �ۼ��ϵ�ʱ��
#define POWER_ADDUP_INDEX       (GetCodeIndex(funcCode.code.powerAddup))       // F7-14  �ۼƺĵ���


#define AI1_LIMIT               (GetCodeIndex(funcCode.code.ai1VoltageLimit)) //  F8-45  AI��������
#define AI1_UPPER               (GetCodeIndex(funcCode.code.ai1VoltageUpper)) //  F8-46  AI��������

#define PID_PARA_CHG_DELTA1_MAX (GetCodeIndex(funcCode.code.pidParaChgDelta2))  // FA-20  PID�����л�ƫ��2
#define PID_PARA_CHG_DELTA2_MIN (GetCodeIndex(funcCode.code.pidParaChgDelta1))  // FA-19  PID�����л���?

#define DORMANT_UPPER           (GetCodeIndex(funcCode.code.wakeUpFrq))       // ����Ƶ������
#define WAKE_UP_LIMIT           (GetCodeIndex(funcCode.code.dormantFrq))      // ����Ƶ������
#define RADIATOR_TEMP_INDEX     (GetCodeIndex(funcCode.code.radiatorTemp))   // FB-19   �����ģ��ɢ�����¶�
#define ERROR_LATEST1_INDEX     (GetCodeIndex(funcCode.code.errorLatest1))   // FB-20   ��һ�ι�������
#define ERROR_LATEST2_INDEX     (GetCodeIndex(funcCode.code.errorLatest2))   // FB-21   �ڶ��ι�������
#define ERROR_LATEST3_INDEX     (GetCodeIndex(funcCode.code.errorLatest3))   // FB-22   (���һ��)�����ι�������
#define ERROR_FRQ_INDEX         (GetCodeIndex(funcCode.code.errorScene3.elem.errorFrq))       // FB-23   ����ʱƵ��
#define ERROR_CURRENT_INDEX     (GetCodeIndex(funcCode.code.errorScene3.elem.errorCurrent))   // FB-24   ����ʱ����
#define ERROR_UDC_INDEX         (GetCodeIndex(funcCode.code.errorScene3.elem.errorGeneratrixVoltage)) // FB-25 ����ʱĸ�ߵ�ѹ
#define ERROR_DI_STATUS_INDEX   (GetCodeIndex(funcCode.code.errorScene3.elem.errorDiStatus))  // FB-26   ����ʱ�������״̬
#define ERROR_DO_STATUS_INDEX   (GetCodeIndex(funcCode.code.errorScene3.elem.errorDoStatus))  // FB-27   ����ʱ�������״̬
#define LAST_ERROR_RECORD_INDEX (GetCodeIndex(funcCode.code.errorScene1.all[sizeof(struct ERROR_SCENE_STRUCT) - 1]))  // ���һ�����ϼ�¼

#define MIN_CBC_TIME_INDEX       (GetCodeIndex(funcCode.code.cbcMinTime))         // A0-14   ������ʱ������
#define MAX_CBC_TIME_INDEX       (GetCodeIndex(funcCode.code.cbcMaxTime))         // A0-15   ������ʱ������



#define PC_LOOP_CHG_FRQ1_I      (GetCodeIndex(funcCode.code.pcLoopChgFrq1))     //          �л�Ƶ��1
#define PC_LOOP_CHG_FRQ2_I      (GetCodeIndex(funcCode.code.pcLoopChgFrq2))     //          л��2

#define EEPROM_CHECK_INDEX      (GetCodeIndex(funcCode.code.eepromCheckWord1))  // eepromCheckWord1

#define RUN_TIME_ADDUP_SEC_INDEX    (GetCodeIndex(funcCode.code.runTimeAddupSec))   // FR-07 F209  �ۼ�����ʱ���s


#define EEPROM_CHECK_INDEX1     (GetCodeIndex(funcCode.code.eepromCheckWord1))  // eepromCheckWord1
#define EEPROM_CHECK_INDEX2     (GetCodeIndex(funcCode.code.eepromCheckWord2))  // eepromCheckWord2

#define SAVE_USER_PARA_PARA1    (GetCodeIndex(funcCode.code.saveUserParaFlag1))
#define SAVE_USER_PARA_PARA2    (GetCodeIndex(funcCode.code.saveUserParaFlag2))

#define AI_AO_CHK_FLAG          (GetCodeIndex(funcCode.code.aiaoChkWord))       // AIAOУ����־
#define AI_AO_CALIB_START       (GetCodeIndex(funcCode.code.aiFactoryCalibrateCurve[0].before1))  // aiao����У����ʼ
#define AI_AO_CALIB_STOP        (GetCodeIndex(funcCode.code.aoFactoryCalibrateCurve[0].after2))   // aiao����У������

#define AI1_CALB_START          (GetCodeIndex(funcCode.code.aiFactoryCalibrateCurve[0].before1))
#define AI2_CALB_STOP           (GetCodeIndex(funcCode.code.aiFactoryCalibrateCurve[1].after2))
#define AO1_CALB_START          (GetCodeIndex(funcCode.code.aoFactoryCalibrateCurve[0].before1))
#define AO1_CALB_STOP           (GetCodeIndex(funcCode.code.aoFactoryCalibrateCurve[0].after2)) 

//-------------------------------
#define FC_GROUP_FACTORY    FUNCCODE_GROUP_L0   // ���Ҳ�����
#define FC_GROUP_FC_MANAGE  FUNCCODE_GROUP_Y0   // ���������
#define FC_START_GROUP      FUNCCODE_GROUP_F0   // ����������ʾ�ĵ�1��
#define USER_DISPLAY1_INDEX      (GetCodeIndex(funcCode.code.disPara))  // ����ѡ��2
//--------------------------------


extern FUNCCODE_ALL funcCode;           // �������RAMֵ
//extern FUNCCODE_ALL funcCodeEeprom;     // �������EEPROMֵ

extern const Uint16 funcCodeGradeSum[];

extern Uint16 saveEepromIndex;    // DP������洢
extern const Uint16 ovVoltageInitValue[];
extern const Uint16 uvVoltageInitValue[]; //20170525
extern const Uint16 funcCodeGradeAll[];
extern Uint16 funcCodeGradeCurMenuMode[];
int32 keyLineCal(void);//����x��Χadd by qtl for Keyboard potentiometer 20131218
#endif  // __F_FUNCCODE_H__




