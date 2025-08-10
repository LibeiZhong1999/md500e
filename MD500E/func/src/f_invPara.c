//======================================================================
//
// ��Ƶ���������Լ���ز���
// 
// ������
// ��Ƶ�����͡���Ƶ�����ʡ���Ƶ�����ѹ
// �ز�Ƶ�ʡ�
// �������ʡ��������������ӵ��衢ת�ӵ��衢©�п������п������ص���
// 
// ����������
//
// Time-stamp: <2008-08-14 12:01:32  Shisheng.Zhi, 0354>
//
//======================================================================


#include "f_menu.h"
#include "f_eeprom.h"
#include "f_comm.h"
#include "f_invPara.h"

#if F_DEBUG_RAM
#define DEBUG_F_TUNE                0       // ��г
#elif 1
#define DEBUG_F_TUNE                1       // ��г
#endif

struct INV_PARA invPara;

// ���͹�ѹ��A5-09��Ƿѹ��A5-06����ѹ���Ƶ�F3-22(F9-04)

#if DEBUG_MD500_SEARIS
const Uint16 ovVoltageInitValue[INV_TYPE_VOLTAGE_NUM] = {8200,4000,4000,8900,13000,20000};
const Uint16 uvVoltageInitValue[INV_TYPE_VOLTAGE_NUM] = {3500,2000,2000,4500,6500,13500};
const Uint16 ovVdcLimitPoint[INV_TYPE_VOLTAGE_NUM]    = {7600,3700,3700,8300,13000,19000};
const Uint16 brakeStartVoltage[INV_TYPE_VOLTAGE_NUM]  = {6900,3700,3700,8000,11600,18500};
#elif DEBUG_MD290_SEARIS
const Uint16 ovVoltageInitValue[INV_TYPE_VOLTAGE_NUM] = {8200,4000,4000,8900,13000,20000};
const Uint16 uvVoltageInitValue[INV_TYPE_VOLTAGE_NUM] = {3500,2000,2000,4500,6500,13500};	
const Uint16 ovVdcLimitPoint[INV_TYPE_VOLTAGE_NUM]    = {7600,3700,3700,8300,13000,19000};
const Uint16 brakeStartVoltage[INV_TYPE_VOLTAGE_NUM]  = {6900,3700,3700,8000,11600,18500};
#else
const Uint16 ovVoltageInitValue[INV_TYPE_VOLTAGE_NUM] = {8100,4000,4000,8900,13000,20000};
const Uint16 uvVoltageInitValue[INV_TYPE_VOLTAGE_NUM] = {3500,2000,2000,3500,6500,11000};	
const Uint16 ovVdcLimitPoint[INV_TYPE_VOLTAGE_NUM]    = {7600,3800,3800,8500,12500,19000};
const Uint16 brakeStartVoltage[INV_TYPE_VOLTAGE_NUM]  = {6900,3600,3600,8000,11600,18500};
#endif
// ע��! ��������Ҫ��invTypeParaTablexxxx����ʼ����һ��
const struct INV_TYPE_LIMIT invTypeLimitTable[INV_TYPE_VOLTAGE_NUM] = 
{
    {380,  INV_TYPE_LOWER_LIMIT_T380, INV_TYPE_UPPER_LIMIT_T380},   // 0, T380
    {220,  INV_TYPE_LOWER_LIMIT_T220, INV_TYPE_UPPER_LIMIT_T220},   // 1, T220
    {220,  INV_TYPE_LOWER_LIMIT_S220, INV_TYPE_UPPER_LIMIT_S220},   // 2, S220
    {480,  INV_TYPE_LOWER_LIMIT_T480, INV_TYPE_UPPER_LIMIT_T480},   // 3, T480
    {690,  INV_TYPE_LOWER_LIMIT_T690, INV_TYPE_UPPER_LIMIT_T690},   // 4, T690
    {1140,  INV_TYPE_LOWER_LIMIT_T1140, INV_TYPE_UPPER_LIMIT_T1140},// 5, T1140
};

// ��1���������index
// ������أ�ʹ�� 0-6�� �������ʣ��������������ӵ��裬ת�ӵ��裬©�У����У����ص���
// ������أ�ʹ�� 1-6�� ����������ӵ��裬ת�ӵ��裬©�У����У����ص���
// ��г��    ʹ�� 2-10�����ӵ��裬ת�ӵ��裬©�У����У����ص�����ͬ����Rs��Ld, Lq, UV��������ƫ��
// Ҫ�� INV_TYPE_RELATED_PARA ��˳��һ��
const Uint16 motor1ParaIndex[] = 
{
    // �����������
    GetCodeIndex(funcCode.code.motorParaM1.elem.ratingPower),        // 0  F1-01  �������� 
    GetCodeIndex(funcCode.code.motorParaM1.elem.ratingCurrent),      // 1  F1-03  ��������
    // �첽������
    GetCodeIndex(funcCode.code.motorParaM1.elem.statorResistance),   // 2  F1-06  ���ӵ���
    GetCodeIndex(funcCode.code.motorParaM1.elem.rotorResistance),    // 3  F1-07  ת�ӵ���
    GetCodeIndex(funcCode.code.motorParaM1.elem.leakInductance),     // 4  F1-08  ©�п�
    GetCodeIndex(funcCode.code.motorParaM1.elem.mutualInductance),   // 5  F1-09  ���п�   
    GetCodeIndex(funcCode.code.motorParaM1.elem.zeroLoadCurrent),    // 6  F1-10  ���ص���
    // ͬ��������
    GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmRs),             // 7  F1-16  ͬ�������ӵ���
    GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmLd),             // 8  F1-17  ͬ����d����
    GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmLq),             // 9  F1-18  ͬ����q����
    GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmRsLdUnit),       // 10 F1-19  ͬ��������衢��е�λ
    GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmCoeff),          // 11 F1-20  ͬ�������綯��ϵ��
    // PI����
    GetCodeIndex(funcCode.code.vcParaM1.mAcrKp),                     // 12 F2-13  M�������Kp
    GetCodeIndex(funcCode.code.vcParaM1.mAcrKi),                     // 13 F2-14  M�������Ki
    GetCodeIndex(funcCode.code.vcParaM1.tAcrKp),                     // 14 F2-15  T�������Kp
    GetCodeIndex(funcCode.code.vcParaM1.tAcrKi),                     // 15 F2-16  T�������Ki    
    GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmCheckTime),      // 16 F1-21  ͬ�������ȱ����ʱ��
    // PG������
    GetCodeIndex(funcCode.code.pgParaM1.elem.enCoderDir),            // 17 F1-30  ���䷽��
    GetCodeIndex(funcCode.code.pgParaM1.elem.enCoderAngle),          // 18 F1-31  ��������װ��
    GetCodeIndex(funcCode.code.uvGainWarp),                          // 19 FF-05  UV��������ƫ��
    GetCodeIndex(funcCode.code.pgParaM1.elem.uvwSignDir),            // 20 F1-32  UVW�źŷ���
    GetCodeIndex(funcCode.code.pgParaM1.elem.uvwSignAngle),          // 21 F1-33  UVW�ź����λ�ý�
};


//---------------------------------------------------------------------------------------------------
// ��Ƶ��������صĲ���
#if (DEBUG_F_MOTOR_POWER_RELATE || DEBUG_F_INV_TYPE_RELATE)
// ������ز���������380V
const INV_TYPE_RELATED_PARA invTypeParaTableT380[INV_TYPE_LENGTH_T380] =
{
// �ز�Ƶ�� ����  ����  ���ӵ��� ת�ӵ��� ©�п� ���п� ���ص��� 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 60,   7,     210,    9134,   4281,   3799,  4077,  155,   40,          },  // 0.75    // 8  
    { 60,   15,    380,    4421,   2813,   2065,  3457,  187,   40,          },  // 1.5     // 9  
    { 60,   22,    510,    2706,   1543,   1011,  2450,  267,   40,          },  // 2.2     // 10 
    { 60,   37,    900,    1204,   908,    528,   1586,  424,   40,          },  // 3.7     // 11 
    { 60,   55,    1300,   804,    708,    475,   1206,  518,   40,          },  // 5.5     // 12 
    { 60,   75,    1700,   610,    584,    430,   1069,  630,   40,          },  // 7.5     // 13 
    { 60,   110,   2500,   410,    220,    260,   776,   890,   40,          },  // 11      // 14 
    { 40,   150,   3200,   273,    200,    220,   578,   1160,  40,          },  // 15      // 15 
#if DEBUG_MD500_SEARIS||DEBUG_MD290_SEARIS
    { 60,   185,   3700,   200,    130,    150,   537,   1280,  40,          },  // 18.5    // 16 
    { 60,   220,   4500,   152,    138,    115,   342,   1965,  40,          },  // 22      // 17 
    { 60,   300,   6000,   110,    98,     100,   322,   2070,  40,          },  // 30      // 18 
    { 50,   370,   7500,   69,     74,     60,    308,   2190,  40,         },  // 37      // 19 
    { 50,   450,   9000,   61,     66,     52,    223,   3020,  40,         },  // 45      // 20 
    { 40,   550,   11000,  50,     60,     41,    183,   3690,  40,         },  // 55      // 21 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 40,   750,   1520,   300,    460,    290,   1640,  420 ,  40,         },  // 75      // 22 
    { 40,   900,   1760,   220,    380,    200,   1400,  490 ,  40,         },  // 90      // 23 
    { 40,   1100,  2100,   120,    320,    150,   1200,  580 ,  40,         },  // 110     // 24 
    { 40,   1320,  2530,   100,    270,    130,   1060,  680 ,  40,         },  // 132     // 25 
    { 30,   1600,  3040,   70,     240,    110,   800,   815 ,  40,         },  // 160     // 26 
    { 30,   2000,  3800,   40,     210,    90,    600,   950 ,  40,         },  // 200     // 27
    { 30,   2200,  4260,   28,     190,    75,    450,   1050,  40,         },  // 220     // 28 
    { 30,   2500,  4650,   20,     170,    55,    350,   1200,  40,         },  // 250     // 29 
    { 30,   2800,  5200,   14,     150,    45,    280,   1300,  40,         },  // 280     // 30
    { 30,   3150,  5850,   10,     120,    30,    200,   1500,  40,         },  // 315     // 31
    { 30,   3550,  6500,   8,      100,    22,    140,   1640,  40,         },  // 355     // 32
    { 30,   4000,  7250,   6,      85,     17,    100,   1750,  40,         },  // 400     // 33
    { 30,   4500,  8200,   4,      63,     13,    70,    1900,  40,         },  // 450     // 34

#else
    { 40,   185,   3700,   200,    130,    150,   537,   1280,  40,          },  // 18.5    // 16 
    { 40,   220,   4500,   152,    138,    115,   342,   1965,  40,          },  // 22      // 17 
    { 40,   300,   6000,   110,    98,     100,   322,   2070,  40,          },  // 30      // 18 
    { 40,   370,   7500,   69,     74,     60,    308,   2190,  40,         },  // 37      // 19 
    { 40,   450,   9000,   61,     66,     52,    223,   3020,  40,         },  // 45      // 20 
    { 30,   550,   11000,  50,     60,     41,    183,   3690,  40,         },  // 55      // 21 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 20,   750,   1520,   300,    460,    290,   1640,  420 ,  40,         },  // 75      // 22 
    { 20,   900,   1760,   220,    380,    200,   1400,  490 ,  40,         },  // 90      // 23 
    { 20,   1100,  2100,   120,    320,    150,   1200,  580 ,  40,         },  // 110     // 24 
    { 20,   1320,  2530,   100,    270,    130,   1060,  680 ,  40,         },  // 132     // 25 
    { 20,   1600,  3040,   70,     240,    110,   800,   815 ,  40,         },  // 160     // 26 
    { 20,   2000,  3800,   40,     210,    90,    600,   950 ,  40,         },  // 200     // 27
    { 20,   2200,  4260,   28,     190,    75,    450,   1050,  40,         },  // 220     // 28 
    { 20,   2500,  4650,   20,     170,    55,    350,   1200,  40,         },  // 250     // 29 
    { 20,   2800,  5200,   14,     150,    45,    280,   1300,  40,         },  // 280     // 30
    { 20,   3150,  5850,   10,     120,    30,    200,   1500,  40,         },  // 315     // 31
    { 20,   3550,  6500,   8,      100,    22,    140,   1640,  40,         },  // 355     // 32
    { 20,   4000,  7250,   6,      85,     17,    100,   1750,  40,         },  // 400     // 33
    { 20,   4500,  8200,   4,      63,     13,    70,    1900,  40,         },  // 450     // 34

#endif

#if 0    
    { 20,   5000,  7250,   4,      63,     13,    70,    1900,  50,         },  // 500     // 35
    { 20,   5500,  8200,   4,      63,     13,    70,    1900,  50,         },  // 550     // 36
#endif        
};

// ������ز���������220V
const INV_TYPE_RELATED_PARA invTypeParaTableT220[INV_TYPE_LENGTH_T220] = 
{
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ����  // ��Ӧ380V���� ����
    { 60,   4,     210,    9134,   4281,   3799,  4077,  155,   40,          },  // 0.4kw   // 107      8  0.75kw
    { 60,   7,     380,    4421,   2813,   2065,  3457,  187,   40,          },  // 0.75kw  // 108      9  1.5kw
    { 60,   11,    510,    2706,   1543,   1011,  2450,  267,   40,          },  // 1.1kw   // 109      10 2.2kw
    { 60,   22,    900,    1204,   908,    528,   1586,  424,   40,          },  // 2.2kw   // 110      11 3.7kw
    { 60,   37,    1300,   804,    708,    475,   1206,  518,   40,          },  // 3.7kw   // 111      12 5.5kw
    { 60,   55,    2500,   410,    220,    260,   776,   890,   40,          },  // 5.5kw   // 112      14 11kw
    { 40,   75,    3200,   273,    200,    220,   578,   1160,  40,          },  // 7.5kw   // 113      15 15kw
    { 40,   110,   4500,   152,    138,    115,   342,   1965,  40,          },  // 11kw    // 114      17 22kw
    { 40,   150,   6000,   110,    98,     100,   322,   2070,  40,          },  // 15kw    // 115      18 30kw
    { 40,   185,   7500,   69,     74,     60,    308,   2190,  40,         },  // 18.5kw  // 116      19 37kw
    { 40,   220,   9000,   61,     66,     52,    223,   3020,  40,         },  // 22kw    // 117      20 45kw
    { 30,   300,   11000,  50,     60,     41,    183,   3690,  40,         },  // 30kw    // 118      21 55kw
    // carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 20,   370,   1520,   300,    460,    290,   1640,  420 ,  40,         },  // 37kw    // 119      22 75kw
    { 20,   450,   1760,   220,    380,    200,   1400,  490 ,  40,         },  // 45kw    // 120      23 90kw
    { 20,   550,   2100,   120,    320,    150,   1200,  580 ,  40,         },  // 55kw    // 121      24 110kw
    { 20,   750,   3040,   70,     240,    110,   800,   815 ,  40,         },  // 75kw    // 122      26 160kw
    { 20,   900,   3800,   40,     210,    90,    600,   950 ,  40,         },  // 90KW    // 123      27 200KW
    { 20,   1100,  4260,   28,     190,    75,    450,   1050,  40,         },  // 110KW   // 124      28 220KW
    { 20,   1320,  4650,   20,     170,    55,    350,   1200,  40,         },  // 130KW   // 125      29 250KW 
    { 20,   1600,  5850,   10,     120,    30,    200,   1500,  40,         },  // 160KW   // 126      31 315KW
    { 20,   2000,  7250,   6,      85,     17,    100,   1750,  40,         },  // 200KW   // 127      33 400KW      
    { 20,   2200,  8200,   4,      63,     13,    70,    1900,  40,         },  // 220KW   // 128      34 450KW    
};

// ������ز���������220V
const INV_TYPE_RELATED_PARA invTypeParaTableS220[INV_TYPE_LENGTH_S220] =
{
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 60,   2,     170,    15330,  10440,  4700,  3720,  90,    40,          },  // 0.2kw   // 201  
    { 60,   5,     230,    4007,   2462,   1358,  1821,  202,   40,          },  // 0.55kw  // 202  
    { 60,   7,     400,    2963,   1697,   1008,  1472,  255,   40,          },  // 0.75kw  // 203  
    { 60,   15,    700,    1534,   1043,   628,   1308,  292,   40,          },  // 1.5kw   // 204  
    { 60,   22,    960,    1025,   680,    340,   885,   450,   40,          },  // 2.2kw   // 205  
    { 60,   37,    1700,   735,    450,    268,   598,   637,   40,          },  // 3.7kw   // 206  
    { 60,   55,    2500,   435,    350,    180,   350,   1100,  40,          },  // 5.5kw   // 207  
};


#if 0

// ������ز���������480V
const INV_TYPE_RELATED_PARA invTypeParaTableT480[INV_TYPE_LENGTH_T480] = 
{
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
//    { 60,   2,     170,    15330,  10440,  4700,  3720,  90,    0,          },  // 0.2kw   // 301  

// �ز�Ƶ�� ����  ����  ���ӵ��� ת�ӵ��� ©�п� ���п� ���ص��� 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 60,   7,     210,    9134,   4281,   3799,  4077,  155,   0,          },  // 0.75    // 308  
    { 60,   15,    380,    4421,   2813,   2065,  3457,  187,   0,          },  // 1.5     // 309  
    { 60,   22,    510,    2706,   1543,   1011,  2450,  267,   0,          },  // 2.2     // 310 
    { 60,   37,    900,    1204,   908,    528,   1586,  424,   0,          },  // 3.7     // 311 
    { 60,   55,    1300,   804,    708,    475,   1206,  518,   0,          },  // 5.5     // 312 
    { 60,   75,    1700,   610,    584,    430,   1069,  630,   0,          },  // 7.5     // 313 
    { 60,   110,   2500,   410,    220,    260,   776,   890,   0,          },  // 11      // 314 
    { 40,   150,   3200,   273,    200,    220,   578,   1160,  0,          },  // 15      // 315 
    { 40,   185,   3700,   200,    130,    150,   537,   1280,  0,          },  // 18.5    // 316 
    { 40,   220,   4500,   152,    138,    115,   342,   1965,  0,          },  // 22      // 317 
    { 40,   300,   6000,   110,    98,     100,   322,   2070,  0,          },  // 30      // 318 
    { 40,   370,   7500,   69,     74,     60,    308,   2190,  20,         },  // 37      // 319 
    { 40,   450,   9000,   61,     66,     52,    223,   3020,  20,         },  // 45      // 320 
    { 30,   550,   11000,  50,     60,     41,    183,   3690,  30,         },  // 55      // 321 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 20,   750,   1520,   300,    460,    290,   1640,  420 ,  30,         },  // 75      // 322 
    { 20,   900,   1760,   220,    380,    200,   1400,  490 ,  40,         },  // 90      // 323 
    { 20,   1100,  2100,   120,    320,    150,   1200,  580 ,  40,         },  // 110     // 324 
    { 20,   1320,  2530,   100,    270,    130,   1060,  680 ,  40,         },  // 132     // 325 
    { 20,   1600,  3040,   70,     240,    110,   800,   815 ,  50,         },  // 160     // 326 
    { 20,   2000,  3800,   40,     210,    90,    600,   950 ,  50,         },  // 200     // 327
    { 20,   2200,  4260,   28,     190,    75,    450,   1050,  50,         },  // 220     // 328 
    { 20,   2500,  4650,   20,     170,    55,    350,   1200,  50,         },  // 250     // 329 
    { 20,   2800,  5200,   14,     150,    45,    280,   1300,  50,         },  // 280     // 330
    { 20,   3150,  5850,   10,     120,    30,    200,   1500,  50,         },  // 315     // 331
    { 20,   3550,  6500,   8,      100,    22,    140,   1640,  50,         },  // 355     // 332
    { 20,   4000,  7250,   6,      85,     17,    100,   1750,  50,         },  // 400     // 333
    { 20,   4500,  8200,   4,      63,     13,    70,    1900,  50,         },  // 450     // 334
#if 0    
    { 20,   5000,  7250,   4,      63,     13,    70,    1900,  50,         },  // 500     // 335
    { 20,   5500,  8200,   4,      63,     13,    70,    1900,  50,         },  // 550     // 336
#endif        
};

#endif

// ������ز���������690V
const INV_TYPE_RELATED_PARA invTypeParaTableT690[INV_TYPE_LENGTH_T690] = 
{
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
#if 0
    { 60,   7,     80,     9134,   4281,   3799,  4077,  155,   0,          },  // 0.75    // 408  
    { 60,   15,    160,    4000,   2653,   2130,  3622,  187,   0,          },  // 1.5     // 409  
    { 60,   22,    240,    2706,   1543,   1011,  2450,  267,   0,          },  // 2.2     // 410 
    { 60,   37,    400,    1050,   1000,   750,   1870,  380,   0,          },  // 3.7     // 411 
    { 60,   55,    600,    680,    640,    600,   1250,  518,   0,          },  // 5.5     // 412 
    { 60,   75,    800,    610,    584,    430,   1069,  630,   0,          },  // 7.5     // 413 
    { 40,   110,   1200,   410,    220,    260,   776,   890,   0,          },  // 11      // 414 
    { 40,   150,   1600,   273,    200,    220,   578,   1160,  0,          },  // 15      // 415 
    { 40,   185,   2000,   200,    130,    150,   537,   1280,  0,          },  // 18.5    // 416 
    { 40,   220,   2400,   152,    138,    115,   342,   1965,  0,          },  // 22      // 417 
    { 40,   300,   3300,   110,    98,     100,   322,   2070,  0,          },  // 30      // 418 
    { 40,   370,   4100,   69,     74,     60,    308,   2190,  20,         },  // 37      // 419 
    { 40,   450,   5000,   61,     66,     52,    223,   3020,  20,         },  // 45      // 420 
#endif        
    { 20,   550,   6033,   132,    119,     210,    581,   2082,  40,         },  // 55      // 421 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 20,   750,   825,  1380,    1240,    1404,   3885,  311,   40,         },  // 75      // 422 
    { 20,   900,   990,   840,    760,    1264,   3493,  346,   40,         },  // 90      // 423 
    { 20,   1100,  1206,   690,    620,    1029,   2849,  424,   40,         },  // 110     // 424 
    { 20,   1320,  1416,   540,    490,    868,   2408,  502,   40,         },  // 132     // 425 
    { 20,   1600,  1714,   420,     380,    720,   1995,   606,   50,         },  // 160     // 426 
    { 20,   2000,  1968,   360,     320,    631,    1722,   702,   50,         },  // 200     // 427
    { 20,   2200,  2349,   300,     270,    526,    1456,   830,  50,         },  // 220     // 428 
    { 20,   2500,  2461,   210,     190,    388,    1274,   905,  50,         },  // 250     // 429 
    { 20,   2800,  2752,   140,     150,    300,    1150,   1050,  50,         },  // 280     // 430
    { 20,   3150,  3096,   100,     120,    250,    1030,   1100,  50,         },  // 315     // 431
    { 20,   3550,  3440,   80,      100,    220,    900,   1240,  50,         },  // 355     // 432
    { 20,   4000,  3837,   60,      850,    170,    800,   1350,  50,         },  // 400     // 433
    { 20,   4500,  4058,   40,      630,    130,    700,   1500,  50,         },  // 450     // 434
    { 20,   5000,  4500,   40,      630,    130,    700,   1600,  50,         },  // 500     // 435
    { 20,   5500,  4800,   30,      600,    120,    690,   1700,  50,         },  // 550     // 436
};

// ������ز���������1140V
const INV_TYPE_RELATED_PARA invTypeParaTableT1140[INV_TYPE_LENGTH_T1140] = 
{
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������   // ����    // ����  
    { 10,  370,  2405,    705,     635,     735,    2195,   954,  40,         },  // 37      // 519 
    { 10,   450,  2922,    545,     495,     685,    1915,   1096,  40,         },  // 45      // 520 
    { 10,   550,  3483,    395,     355,     635,    1745,   1202,  40,         },  // 55      // 521 
// carrier  power  rateCur Rs      rr      lo     lm     IM     ����������    // ����    // ���� 
    { 10,   750,   476,    4145,    3735,    4215,   11655,  179,   40,         },  // 75      // 522 
    { 10,   900,   572,    2525,    2275,    3795,   10495,  199,   40,         },  // 90      // 523 
    { 10,   1100,  696,    2075,    1865,    3085,   8545,  245,   40,         },  // 110     // 524 
    { 10,   1320,  817,    1625,    1465,    2605,   7225,  290,   40,         },  // 132     // 525 
    { 10,   1600,  990,   1265,     1135,    2165,   5985,   350,   50,         },  // 160     // 526 
    { 10,   2000,  1136,   1085,     975,    1895,    5165,   405,   50,         },  // 200     // 527
    { 10,   2200,  1356,   905,     815,    1575,    4365,   479,  50,         },  // 220     // 528 
    { 10,   2500,  1425,   635,     575,    1165,    3225,   497,  50,         },  // 250     // 529 
    { 10,   2800,  1550,   405,     385,    845,    2105,   530,  50,         },  // 280     // 530
    { 10,   3150,  1650,   245,     265,    505,    1005,   560,  50,         },  // 315     // 531
    { 10,   3550,  1760,   175,      185,    305,    1005,   590,  50,         },  // 355     // 532
    { 10,   4000,  1880,   155,      165,     185,    1005,   640,  50,         },  // 400     // 533
    { 10,   4500,  1910,   85,      155,     105,    1005,    680,  50,         },  // 450     // 534
    { 10,   5000,  2030,   85,      155,     105,    1005,    720,  50,         },  // 500     // 535
    { 10,   5600,  2180,   85,      155,     105,    1005,    760,  50,         },  // 560     // 536
    { 10,   6300,  2300,   85,      155,     105,    1005,    800,  50,         },  // 630     // 537
};

// ������ز���
const INV_TYPE_RELATED_PARA * pInvTypeParaTable[INV_TYPE_VOLTAGE_NUM] = 
{
    invTypeParaTableT380,       // ����380V
    invTypeParaTableT220,       // ����220V
    invTypeParaTableS220,       // ����220V
    invTypeParaTableT380,       // ����480V
    invTypeParaTableT690,       // ����690V
    invTypeParaTableT1140,      // ����1140V
};

// ��ͬ��ѹ�ȼ�����С�����ٽ��
const Uint16 INV_TYPE_POINT_LIMIT_TABLE[INV_TYPE_VOLTAGE_NUM] = 
{
    INV_TYPE_POINT_LIMIT_T380,       // ����380V
    INV_TYPE_POINT_LIMIT_T220,       // ����220V
    INV_TYPE_POINT_LIMIT_T380,       // ����220V
    INV_TYPE_POINT_LIMIT_T380,       // ����480V
    INV_TYPE_POINT_LIMIT_T380,       // ����690V
    INV_TYPE_POINT_LIMIT_T380,       // ����1140V
};

// ��ͬ��ѹ�ȼ��Ӽ���ʱ�����ֵΪ��ֵ����ʼ����
const Uint16 INV_TYPE_BIG_ACC_DEC_START_TABLE[INV_TYPE_VOLTAGE_NUM] = 
{
    INV_TYPE_BIG_ACC_DEC_START_T380,       // ����380V
    INV_TYPE_BIG_ACC_DEC_START_T220,       // ����220V
    INV_TYPE_BIG_ACC_DEC_START_T380,       // ����220V
    INV_TYPE_BIG_ACC_DEC_START_T380,       // ����480V
    INV_TYPE_BIG_ACC_DEC_START_T380,       // ����690V
    INV_TYPE_BIG_ACC_DEC_START_T380,       // ����1140V
};


#endif
//---------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------
// ���2������index������ڵ��1����index������
#define MOTOR2_INDEX_INC_TO_MOTOR1                          \
(GetCodeIndex(funcCode.code.motorFcM2.motorPara.all[0]) -    \
GetCodeIndex(funcCode.code.motorParaM1.all[0]))              \


// ��1/2����ĵ��������������1���������CodeIndex����
const Uint16 motorSnIndexIncToMotor1[2] = 
{
    0,                              // ��1���
    MOTOR2_INDEX_INC_TO_MOTOR1,     // ��2���
};
//---------------------------------------------------------------------------------------------------





//=====================================================================
//
// �޸ı�Ƶ������ʱ�Ĵ���
//
// �ı��Ƶ������ʱ����Ҫ�޸ĵĹ�����(MD280Ŀǰ��14��)��
// 1���������������ʡ����ѹ����������ת�١��Ƶ�ʡ����ص��������ӵ��衢ת�ӵ��裬©�У�����
// 2����Ƶ������FF-03
// 3��ת������
// 4������������
// 5���ز�Ƶ��
// 6���Ӽ���ʱ��1���Ӽ���ʱ��2
//
//=====================================================================
void InverterTypeRelatedParaDeal(void)
{
#if DEBUG_F_INV_TYPE_RELATE
    Uint16 ratingFrq;
    Uint16 invType;
    int16 i;

    UpdateInvType();            // ����ʵ�ʻ��͡���ѹ�ȼ�
    invType = invPara.type;     // ����

    switch(invPara.volLevel)
    {
        case 0:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 3000;
            break;
        case 1:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 1800;
            break;
        case 2:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 1800;
            break;
        case 3:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 3000;
            break;
        case 4:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 5500;
            break;
        case 5:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 9100;
            break;
        default:
            funcCode.code.motorParaM1.elem.pmsmCoeff = 3000;
            break;
    }
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorParaM1.elem.pmsmCoeff));

// �������ʣ��������������ӵ��裬ת�ӵ��裬©�У����У����ص���
// ��Щ������ÿ��������ǵ��������룬INV_TYPE_RELATED_PARA ��Ҳ�С�
// 
    for (i = 6; i >= 0; i--)
    {
        Uint16 index1;          // ��1�������index
        Uint16 index2;          // ��2�������index
        Uint16 tmp;

        index1 = motor1ParaIndex[i];
        index2 = motor1ParaIndex[i] + MOTOR2_INDEX_INC_TO_MOTOR1;

        tmp = GetInvParaPointer(invType)->all[i+1];
        
        funcCode.all[index1] = tmp;    // ��1�������
        funcCode.all[index2] = tmp;    // ��2�������
        
        SaveOneFuncCode(index1);
        SaveOneFuncCode(index2);
    }
    
// ������ѹ������Ƶ�ʣ��ת��
    ratingFrq = RATING_FRQ_INIT_0 * decNumber[funcCode.code.frqPoint];


    // ��1���
    funcCode.code.motorParaM1.elem.ratingVoltage = invPara.ratingVoltage;// ������ѹ
    funcCode.code.motorParaM1.elem.ratingFrq = ratingFrq;                // ����Ƶ��
    funcCode.code.motorParaM1.elem.ratingSpeed = RATING_SPEED_RPM;       // �ת��
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorParaM1.elem.ratingVoltage));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorParaM1.elem.ratingFrq));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorParaM1.elem.ratingSpeed));

    // ��2���
    funcCode.code.motorFcM2.motorPara.elem.ratingVoltage = invPara.ratingVoltage;
    funcCode.code.motorFcM2.motorPara.elem.ratingFrq = ratingFrq;   
    funcCode.code.motorFcM2.motorPara.elem.ratingSpeed = RATING_SPEED_RPM;
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorFcM2.motorPara.elem.ratingVoltage));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorFcM2.motorPara.elem.ratingFrq));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorFcM2.motorPara.elem.ratingSpeed));

// ��Ƶ������
    funcCode.code.inverterPower = funcCode.code.motorParaM1.elem.ratingPower;
    SaveOneFuncCode(GetCodeIndex(funcCode.code.inverterPower));

// �������صķǵ������������
    InverterTypeRelatedNoMotorParaDeal(invType);

// ����ֵ�������صķǵ�����������룬������EEPROM
    InverterTypeRelatedNoMotorParaSaveDeal();
#endif
}


//=====================================================================
// 
// ��Ҫ������
// 1. inverterType
// 2. ratingPower����������
// 
// �������صĹ����룬�ǵ������
// 
//=====================================================================

LOCALF void InverterTypeRelatedNoMotorParaDeal(Uint16 invType)
{
#if DEBUG_F_INV_TYPE_RELATE
    Uint16 accDecInit;
    if (invType < invPara.bitAccDecStart)  // ���� < 21
    {
        accDecInit = ACC_DEC_T_INIT1;
    }
    else
    {
        accDecInit = ACC_DEC_T_INIT2;
    }

    funcCode.all[ACC_TIME1_INDEX] = accDecInit; // ����ʱ��1
    funcCode.all[DEC_TIME1_INDEX] = accDecInit; // ����ʱ��1
    funcCode.all[ACC_TIME2_INDEX] = accDecInit; // ����ʱ��2
    funcCode.all[DEC_TIME2_INDEX] = accDecInit; // ����ʱ��2
    funcCode.all[ACC_TIME3_INDEX] = accDecInit; // ����ʱ��3
    funcCode.all[DEC_TIME3_INDEX] = accDecInit; // ����ʱ��3
    funcCode.all[ACC_TIME4_INDEX] = accDecInit; // ����ʱ��4
    funcCode.all[DEC_TIME4_INDEX] = accDecInit; // ����ʱ��4
    
// �ز�Ƶ��
    funcCode.all[CARRIER_FRQ_INDEX] = GetInvParaPointer(invType)->elem.carrierFrq;

// ����������
    funcCode.all[ANTI_VIBRATE_GAIN_INDEX] = GetInvParaPointer(invType)->elem.antiVibrateGain;
    funcCode.code.motorFcM2.antiVibrateGain = funcCode.code.antiVibrateGain;

// ת��������Ҫ�ڹ��ʴ���֮��
    funcCode.all[TORQUE_BOOST_INDEX] = TorqueBoostDeal(funcCode.code.motorParaM1.elem.ratingPower);
    funcCode.code.motorFcM2.torqueBoost = funcCode.code.torqueBoost;

#if DEBUG_MD500_SEARIS
// ��ѹ��
    funcCode.code.ovPointSet = ovVoltageInitValue[invPara.volLevel];

// Ƿѹ��
    funcCode.code.uvPoint = uvVoltageInitValue[invPara.volLevel];

// �ƶ����迪����
    funcCode.code.brakeStartVoltage = brakeStartVoltage[invPara.volLevel];
#elif DEBUG_MD290_SEARIS
// ��ѹ��
    funcCode.code.ovPointSet = ovVoltageInitValue[invPara.volLevel];

// Ƿѹ��
    funcCode.code.uvPoint = uvVoltageInitValue[invPara.volLevel];

// �ƶ����迪����
    funcCode.code.brakeStartVoltage = brakeStartVoltage[invPara.volLevel];
#else
// ��ѹ��
    funcCode.code.ovPointSet = ovVoltageInitValue[invPara.volLevel];
// Ƿѹ��
    funcCode.code.uvPoint = uvVoltageInitValue[invPara.volLevel];

// �ƶ����迪����
    funcCode.code.brakeStartVoltage = brakeStartVoltage[invPara.volLevel];
#endif

#if DEBUG_MD500_SEARIS
// ��ѹ���Ƶ�
    funcCode.code.VFOverVdcLimitPoint = ovVdcLimitPoint[invPara.volLevel];
#elif DEBUG_MD290_SEARIS
// ��ѹ���Ƶ�
    funcCode.code.VFOverVdcLimitPoint = ovVdcLimitPoint[invPara.volLevel];
#else
// ��ѹ���Ƶ�
    funcCode.code.VFOverVdcLimitPoint = ovVdcLimitPoint[invPara.volLevel];
    funcCode.code.ovPoint             = ovVdcLimitPoint[invPara.volLevel];
#endif

    if(invPara.volLevel == 5) // 1140v
    {
    	if(invType>=34)//450KW
    	{
    		funcCode.code.cf[8] = 50;
    	}
    	else
    	{
    		funcCode.code.cf[8] = 0;
    	}
    }
#endif
}



// ����ֵ�������صķǵ�����������룬������EEPROM
void InverterTypeRelatedNoMotorParaSaveDeal(void)
{
#if DEBUG_F_INV_TYPE_RELATE
    SaveOneFuncCode(ACC_TIME1_INDEX);
    SaveOneFuncCode(DEC_TIME1_INDEX);
    SaveOneFuncCode(ACC_TIME2_INDEX);
    SaveOneFuncCode(DEC_TIME2_INDEX);
    SaveOneFuncCode(ACC_TIME3_INDEX);
    SaveOneFuncCode(DEC_TIME3_INDEX);
    SaveOneFuncCode(ACC_TIME4_INDEX);
    SaveOneFuncCode(DEC_TIME4_INDEX);

    SaveOneFuncCode(CARRIER_FRQ_INDEX);

    SaveOneFuncCode(ANTI_VIBRATE_GAIN_INDEX);
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorFcM2.antiVibrateGain));

    SaveOneFuncCode(TORQUE_BOOST_INDEX);
    SaveOneFuncCode(GetCodeIndex(funcCode.code.motorFcM2.torqueBoost));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.ovPointSet));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.cf[8]));
#if DEBUG_MD500_SEARIS
    SaveOneFuncCode(GetCodeIndex(funcCode.code.ovPointSet));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.uvPoint));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.VFOverVdcLimitPoint));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.brakeStartVoltage));
	
#elif DEBUG_MD290_SEARIS
    SaveOneFuncCode(GetCodeIndex(funcCode.code.ovPointSet));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.uvPoint));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.VFOverVdcLimitPoint));
	SaveOneFuncCode(GetCodeIndex(funcCode.code.brakeStartVoltage));
#else
    SaveOneFuncCode(GetCodeIndex(funcCode.code.ovPointSet));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.uvPoint));
    SaveOneFuncCode(GetCodeIndex(funcCode.code.VFOverVdcLimitPoint));
	SaveOneFuncCode(GetCodeIndex(funcCode.code.ovPoint));
	SaveOneFuncCode(GetCodeIndex(funcCode.code.brakeStartVoltage));
#endif

#endif
}



//=====================================================================
//
// �޸ĵ������ʵĴ���
//
// �޸ĵ�������ʱ����Ҫ��Ӧ�޸ĵ��������������
// 1����������
// 2�����ص���
// 3�����ӵ���
// 4��ת������
// 5��ת�ӵ���
// 6��©��
// 7������
//
//=====================================================================
// ���ݵ�ǰ��ѹ�ȼ��ͻ��ͣ���ö�Ӧ�ṹ�����ʼ��ַ
void MotorPowerRelatedParaDeal(Uint16 power, Uint16 motorSn4Power)
{
#if DEBUG_F_MOTOR_POWER_RELATE
    Uint16 i;
    Uint32 flag;
    Uint16 start;           // ��������ʼ����
    Uint16 end;             // �����Ľ�������
    Uint16 invTypeOfPower;  // ��ǰ���������Ļ���λ��
    int16 j;
    Uint16 dataPointFormat[6] = {1,1,1,1,1,1};
    
    start = invTypeLimitTable[invPara.volLevel].lower;
    end = invTypeLimitTable[invPara.volLevel].upper;

// ��֪�����ʹ�С���󣬹���Ҳ��С����
    for (i = start; i <= end; i++)
    {
        if (GetInvParaPointer(i)->elem.ratingPower >= power)
            break;
    }

// �Ƚ���˭��
    invTypeOfPower = i;
    if (end < i)            // i = end + 1�����������
    {
        invTypeOfPower--;
    }
    else if (start < i)     // start < i
    {
        if (power - GetInvParaPointer(i-1)->elem.ratingPower
            < GetInvParaPointer(i)->elem.ratingPower - power) // ע���С
        {
            invTypeOfPower--;
        }
    }

// ת������
    if (MOTOR_SN_1 == motorSn4Power)        // ��1���
    {
        funcCode.code.torqueBoost = TorqueBoostDeal(power);
        SaveOneFuncCode(GetCodeIndex(funcCode.code.torqueBoost));
    }
    else if (MOTOR_SN_2 == motorSn4Power)   // ��2���
    {
        funcCode.code.motorFcM2.torqueBoost = TorqueBoostDeal(power);
        SaveOneFuncCode(GetCodeIndex(funcCode.code.motorFcM2.torqueBoost));
    }

// CF-00 ����ʸ��ʹ�ò���
    funcCode.code.cf[0] = Cf00Deal(power);
    SaveOneFuncCode(GetCodeIndex(funcCode.code.cf[0]));

    if ((invPara.type > invPara.pointLimit) 
        && (invTypeOfPower <= invPara.pointLimit))
    {
        flag = 10;
        dataPointFormat[0] = 100; 
        dataPointFormat[5] = 100; 
        
    }
    else if ((invPara.type <= invPara.pointLimit) 
        && (invTypeOfPower > invPara.pointLimit))
    {
        flag = 10;
		dataPointFormat[1] = 100; 
        dataPointFormat[2] = 100; 
        dataPointFormat[3] = 100; 
        dataPointFormat[4] = 100; 
    }
    else
    {
        flag = 1;
    }
    
// ����������ӵ��裬ת�ӵ��裬©�У����У����ص���
    for (j = 6; j >= 1; j--)
    {
        Uint16 index = motor1ParaIndex[j] + motorSnIndexIncToMotor1[motorSn4Power];
        
        funcCode.all[index] = (flag*(GetInvParaPointer(invTypeOfPower)->all[j+1]))/dataPointFormat[j-1];    // ��x�������
        SaveOneFuncCode(index);
    }
#endif
}



//=====================================================================
//
// ת�����������빦���ж�Ӧ��ϵ
// 0.4kW --- 0.75kW 6%
// 1.5kW --- 3.7kW  4%
// 5.5kW --- 7.5kW  3%
//  11kW ---  37kW  2%
//  45kW ---        1%
// 
//=====================================================================
Uint16 TorqueBoostDeal(Uint16 power)
{
#if (DEBUG_F_INV_TYPE_RELATE || DEBUG_F_MOTOR_POWER_RELATE)
    Uint16 torqueBoost;
    
    if (power <= 10)        // P <= 1.0kW
    {
        torqueBoost = 60;   // 6.0%
    }
    else if (power <= 40)   // p <= 4.0kW
    {
        torqueBoost = 40;   // 4.0%
    }
    else if (power <= 75)   // p <= 7.5kW
    {
        torqueBoost = 30;   // 3.0%
    }
    else if (power <= 370)  // p <= 37kW
    {
        torqueBoost = 20;   // 2.0%
    }
    else
    {
        torqueBoost = 10;   // 1.0%
    }

    return torqueBoost;
#endif
}

// CF-00����ֵ����
Uint16 Cf00Deal(Uint16 power)
{
    Uint16 data;
    if (power < 550)        // P < 55.0kW
    {
        //data = 20; 
		data = 0; 
    }
    else                    // p >= 55.0kW
    {
        //data = 12;  
		data = 0; 
    }

    return data;
}
// 1140v��������ֵ����
Uint16 initFor1140vDeal(Uint16 invType,Uint16 volLevel,Uint16 index)
{
    Uint16 data;
    if(volLevel != 5)           // ����1140v��ѹ�ȼ�
    {
        data = GetFuncCodeInitOriginal(index);
        return data;
    }
    
    if(index == SPEED_FILTER_COEFF)
    {
        if (invType >= 34)     // P < 450.0kW
        {
            data = 50; 
        }
        else                        // p >= 450.0kW
        {
            data = 0;  
        }
        
    }
    else if(index == QUICK_CURRENT_LIMIT_INDEX)
    {
        data = 0;               // 1140v Ĭ�Ϲر�������
    }
    return data;
}

// ȷ�ϻ����Ƿ���Ч
Uint16 ValidateInvType(void)
{
#if DEBUG_F_INV_TYPE_RELATE
    Uint16 i;
    Uint16 upper;
    Uint16 lower;
    Uint16 ret = COMM_ERR_NONE;
    Uint16 invType = funcCode.code.inverterType;

    i = invType / 100;

    upper = invTypeLimitTable[i].upper + (i * 100);
    lower = invTypeLimitTable[i].lower + (i * 100);

    if (!((lower <= invType) && (invType <= upper)))  // ���ʲ��ڹ涨��Χ
    {
        ret = COMM_ERR_PARA;
    }
    
    return ret;
#endif
}



// input:  funcCode.code.inverterType
// output: invPara
// �����û�������ͣ����ʵ�ʻ���(��Ƶ������)����Ƶ�����ѹ
void UpdateInvType(void)
{
    invPara.type = funcCode.code.inverterType % 100;                    // ��λ��ʮλ����ʾ����
    invPara.volLevel = funcCode.code.inverterType / 100;                // ��λ��ʾ��ѹ�ȼ�
    invPara.ratingVoltage = invTypeLimitTable[invPara.volLevel].vol;    // 
    invPara.pointLimit = INV_TYPE_POINT_LIMIT_TABLE[invPara.volLevel];  
    invPara.bitAccDecStart = INV_TYPE_BIG_ACC_DEC_START_TABLE[invPara.volLevel];
}



// �����г����
extern enum MOTOR_SN motorSn;
void SaveTuneData(void)
{
#if DEBUG_F_TUNE
    int16 i;
    
    // ���ӵ��裬ת�ӵ��裬©�У����У����ص���
    // ͬ����Rs, Ld, Lq
    // UV��������ƫ��
    // UVW�źŷ��� UVW�ź����λ�ý�
    for (i = sizeof(motor1ParaIndex)-1; i >= 2; i--)
    {
        Uint16 index;               // ��Ҫ����Ĳ���index
        Uint16 index4motorTune;     // ��Ӧ������ݵ�index
        Uint16 add = motorSnIndexIncToMotor1[motorSn];

        if (i == (sizeof(motor1ParaIndex)-3))    // UV��������ƫ��
        {
            add = 0;    // ���е��ʹ��ͬһ��������
        }
        
        index = *(motor1ParaIndex + i) + add;   // ��г����������첽�����ӵ��迪ʼ
        index4motorTune = i - 2;
        if(funcCode.all[index] != gParaIdToFunctionDataBuff[index4motorTune])
        {
            funcCode.all[index] = gParaIdToFunctionDataBuff[index4motorTune];
            SaveOneFuncCode(index);
        }
        /*else
        {
            funcCode.all[index] = gParaIdToFunctionDataBuff[index4motorTune];
            SaveOneFuncCode(index);
        }*/
    }
#endif
}







