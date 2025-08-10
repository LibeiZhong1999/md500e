//======================================================================
//
// Time-stamp: <2007-12-20 14:01:46  Shisheng.Zhi, 0354>
//
// ÿ����������������
//
//======================================================================


//-------------------------------------------------
// ���˵���д���
//
// ֮�����ӹ����룬��Ҫ��EEPROM��ַ��ʹ�ã�
// 1��ÿ�鹦�����е�Ԥ���ռ䡣
// 2��238-260����23��
// 3��950-1007��

// EEPROM��ַ�ķ��䣺
// 0    - 15��  ��16����������ʹ��
// 16   - 1031����1016����ʹ�á�
// 1032 - 2047����1026�������ݡ�

// 238-260����23����Ԥ����
// 830-889����60�������EEPROM����� ���ϼ�¼ ʹ��
// 890-949����60�������EEPROM����� ������� ʹ��
//-------------------------------------------------

#include "f_funcCode.h"
#include "f_io.h"
#include "f_main.h"
#include "f_runSrc.h"
#include "f_frqSrc.h"
 
#pragma DATA_SECTION(funcCodeGradeCurMenuMode, ".funR2");  
#pragma DATA_SECTION(funcCode, ".fcode"); 
 
FUNCCODE_ALL funcCode;

// ��ǰ�˵�ģʽ�£���ÿһgroup���û����Բ�����grade������
// Ϊ0����ʾ���ɷ��ʸ��鹦���롣
Uint16 funcCodeGradeCurMenuMode[FUNCCODE_GROUP_NUM];

const Uint16 funcCodeGradeAll[FUNCCODE_GROUP_NUM] =
{//��ʾ��������
//===========================
    F0NUM - 0,  F1NUM - 0,  F2NUM - 0,  F3NUM - 0,  // F0-F3
    F4NUM - 22, F5NUM - 0,  F6NUM - 0,  F7NUM - 0,  // F4-F7
    F8NUM - 0,  F9NUM - 0,  FANUM - 0,  FBNUM - 0,  // F8-FB
    FCNUM - 0,  FDNUM - 1,  FENUM - 1,  FFNUM - 1,  // FC-FF

//===========================
    E0NUM - 0,  E1NUM - 0,  E2NUM - 0,  E3NUM - 0,  // E0-E3
    E4NUM - 2,  E5NUM - 0,  E6NUM - 2,  E7NUM - 0,  // E4-E7
    E8NUM - 15, E9NUM - 1,  EANUM - 1,  EBNUM - 1,  // E8-EB
    ECNUM - 1,  0,  EENUM - 1,  EFNUM - 1,  // EC-EF

//===========================
    B0NUM - 0,  B1NUM - 0,  B2NUM - 0,  B3NUM - 0,  // B0-B3
    B4NUM - 1,  B5NUM - 1,  B6NUM - 1,  B7NUM - 1,  // B4-B7
    B8NUM - 1,  B9NUM - 1,  BANUM - 1,  BBNUM - 1,  // B8-BB
    BCNUM - 1,  BDNUM - 1,  BENUM - 1,  BFNUM - 1,  // BC-BF

//===========================
    Y0NUM -	0,  Y1NUM - 0,  Y2NUM - 1,  Y3NUM - 1,  // Y0-Y3
    Y4NUM - 1,  Y5NUM - 1,  Y6NUM - 1,  Y7NUM - 1,  // Y4-Y7
    
//===========================                                      
    L0NUM -	0,  0,          L2NUM - 1,  L3NUM - 1,  // L0-L3       
    L4NUM - 1,  L5NUM - 1,  L6NUM - 1,  L7NUM - 1,  // L4-L7       
    L8NUM - 1,  0,                                  // L8-L9

//===========================
    0,      //  eepromCheckWord

//===========================
    0,      //  �������

//===========================
    D0NUM -31, D1NUM - 1, D2NUM - 1, D3NUM - 0,     // d0, ��ʾ
    D4NUM - 1, D5NUM - 1, D6NUM - 1, D7NUM - 1,
    D8NUM - 1, D9NUM - 1, DANUM - 1, DBNUM - 1,
    DCNUM - 1, DDNUM - 1, DENUM - 1, 0,
};

// ÿһgroup�Ŀ�ʼ�������index
// ��ÿһ�����룬currentIndex = GetGradeIndex(group, grade) = funcCodeGradeSum[group]+grade;
const Uint16 funcCodeGradeSum[FUNCCODE_GROUP_NUM] =
{
//============================================
    GetCodeIndex(funcCode.group.f0[0]),         // F0
    GetCodeIndex(funcCode.group.f1[0]),         // F1
    GetCodeIndex(funcCode.group.f2[0]),         // F2
    GetCodeIndex(funcCode.group.f3[0]),         // F3

    GetCodeIndex(funcCode.group.f4[0]),         // F4
    GetCodeIndex(funcCode.group.f5[0]),         // F5
    GetCodeIndex(funcCode.group.f6[0]),         // F6
    GetCodeIndex(funcCode.group.f7[0]),         // F7

    GetCodeIndex(funcCode.group.f8[0]),         // F8
    GetCodeIndex(funcCode.group.f9[0]),         // F9
    GetCodeIndex(funcCode.group.fa[0]),         // FA
    GetCodeIndex(funcCode.group.fb[0]),         // FB

    GetCodeIndex(funcCode.group.fc[0]),         // FC
    GetCodeIndex(funcCode.group.fd[0]),         // FD
    GetCodeIndex(funcCode.group.fe[0]),         // FE
    GetCodeIndex(funcCode.group.ff[0]),         // FF

//============================================
    GetCodeIndex(funcCode.group.e0[0]),         // A0
    GetCodeIndex(funcCode.group.e1[0]),         // A1
    GetCodeIndex(funcCode.group.e2[0]),         // A2
    GetCodeIndex(funcCode.group.e3[0]),         // A3

    GetCodeIndex(funcCode.group.e4[0]),         // A4
    GetCodeIndex(funcCode.group.e5[0]),         // A5
    GetCodeIndex(funcCode.group.e6[0]),         // A6
    GetCodeIndex(funcCode.group.e7[0]),         // A7

    GetCodeIndex(funcCode.group.e8[0]),         // A8
    GetCodeIndex(funcCode.group.e9[0]),         // A9
    GetCodeIndex(funcCode.group.ea[0]),         // AA
    GetCodeIndex(funcCode.group.eb[0]),         // AB

    GetCodeIndex(funcCode.group.ec[0]),         // AC
    GetCodeIndex(funcCode.group.ed[0]),         // AD
    GetCodeIndex(funcCode.group.ee[0]),         // AE
    GetCodeIndex(funcCode.group.ef[0]),         // AF

//============================================
    GetCodeIndex(funcCode.group.b0[0]),         // B0
    GetCodeIndex(funcCode.group.b1[0]),         // B1
    GetCodeIndex(funcCode.group.b2[0]),         // B2
    GetCodeIndex(funcCode.group.b3[0]),         // B3

    GetCodeIndex(funcCode.group.b4[0]),         // B4
    GetCodeIndex(funcCode.group.b5[0]),         // B5
    GetCodeIndex(funcCode.group.b6[0]),         // B6
    GetCodeIndex(funcCode.group.b7[0]),         // B7

    GetCodeIndex(funcCode.group.b8[0]),         // B8
    GetCodeIndex(funcCode.group.b9[0]),         // B9
    GetCodeIndex(funcCode.group.ba[0]),         // BA
    GetCodeIndex(funcCode.group.bb[0]),         // BB

    GetCodeIndex(funcCode.group.bc[0]),         // BC
    GetCodeIndex(funcCode.group.bd[0]),         // BD
    GetCodeIndex(funcCode.group.be[0]),         // BE
    GetCodeIndex(funcCode.group.bf[0]),         // BF

//============================================
    GetCodeIndex(funcCode.group.y0[0]),         // C0
    GetCodeIndex(funcCode.group.y1[0]),         // C1
    GetCodeIndex(funcCode.group.y2[0]),         // C2
    GetCodeIndex(funcCode.group.y3[0]),         // C3

    GetCodeIndex(funcCode.group.y4[0]),         // C4
    GetCodeIndex(funcCode.group.y5[0]),         // C5
    GetCodeIndex(funcCode.group.y6[0]),         // C6
    GetCodeIndex(funcCode.group.y7[0]),         // C7

    GetCodeIndex(funcCode.group.l0[0]),         // C8
    GetCodeIndex(funcCode.group.l1[0]),         // C9
    GetCodeIndex(funcCode.group.l2[0]),         // CA
    GetCodeIndex(funcCode.group.l3[0]),         // CB

    GetCodeIndex(funcCode.group.l4[0]),         // CC
    GetCodeIndex(funcCode.group.l5[0]),         // CD
    GetCodeIndex(funcCode.group.l6[0]),         // CE
    GetCodeIndex(funcCode.group.l7[0]),         // CF
    GetCodeIndex(funcCode.group.l8[0]),         // CF   
    GetCodeIndex(funcCode.group.l9[0]),         // CF

//============================================
    GetCodeIndex(funcCode.group.fChk[0]),       // eepromCheckWord

//============================================
    GetCodeIndex(funcCode.group.remember[0]),   // �������

//============================================
    GetCodeIndex(funcCode.group.d0[0]),         // U0
    GetCodeIndex(funcCode.group.d1[0]),         // U1
    GetCodeIndex(funcCode.group.d2[0]),         // U2
    GetCodeIndex(funcCode.group.d3[0]),         // U3

    GetCodeIndex(funcCode.group.d4[0]),         // U4
    GetCodeIndex(funcCode.group.d5[0]),         // U5
    GetCodeIndex(funcCode.group.d6[0]),         // U6
    GetCodeIndex(funcCode.group.d7[0]),         // U7

    GetCodeIndex(funcCode.group.d8[0]),         // U8
    GetCodeIndex(funcCode.group.d9[0]),         // U9
    GetCodeIndex(funcCode.group.da[0]),         // UA
    GetCodeIndex(funcCode.group.db[0]),         // UB

    GetCodeIndex(funcCode.group.dc[0]),         // UC
    GetCodeIndex(funcCode.group.dd[0]),         // UD
    GetCodeIndex(funcCode.group.de[0]),         // UE
    GetCodeIndex(funcCode.group.df[0]),         // UF
};


// ����������Ա����ޡ����ޡ����ԡ�
// ������ (EEPROM_CHK��������䡢)��ʾ
const FUNCCODE_ATTRIBUTE funcCodeAttribute[FNUM_PARA] =
{
// F0����                      F0����                        F0����ֵ            F0����                                                                         
{  0,                          2,                            2,                  0x0840,   },  //F0-00  F001 (���)���Ʒ�ʽ                                               
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //F0-01  Ԥ��Ƶ��                                                     
{  1,                          2,                            2,                  0x0840,   },  //F0-02  Ƶ��ָ��С����                                               
{  0,                          9,                            1,                  0x0840,   },  //F0-03  ��Ƶ��ԴXѡ��                                             
{  0,                          9,                            2,                  0x0840,   },  //F0-04  ����Ƶ��ԴYѡ��                                              
{  0,                          2,                            0,                  0x0040,   },  //F0-05  ����Ƶ��ԴY��Χѡ��                                          
{  0,                          150,                          100,                0x00F0,   },  //F0-06  ����Ƶ��ԴY��Χ                                              
{  0,                          44,                           0,                  0x4080,   },  //F0-07  Ƶ��Դѡ��                                                   
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F0-08  ����Ƶ��Դƫ�õ������趨                                     
{  0,                          0x1,                          1,                  0x0040,   },  //F0-09  �����趨Ƶ�ʼ���ѡ��                                         
{  0,                          1,                            0,                  0x0840,   },  //F0-10  ����ʱƵ��ָ��UP/DOWN��׼                                    
{  0,                          2,                            0,                  0x0040,   },  //F0-11  F002 ����Դѡ��                                                                                                      
{  0,                          9999,                         0,                  0x4040,   },  //F0-12  ����Դ����Ƶ��Դ                                             
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F0-13  ����ʱ��1                                                    
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F0-14  ����ʱ��1                                                    
{  0,                          2,                            1,                  0x0840,   },  //F0-15  �Ӽ���ʱ��ĵ�λ                                             
{  0,                          2,                            0,                  0x0840,   },  //F0-16  �Ӽ���ʱ���׼Ƶ��                                           
{  0,                          1,                            0,                  0x0040,   },  //F0-17  �ز�Ƶ�����¶ȵ���                                          
{  5,                          160,                          60,                 0x00C1,   },  //F0-18  �ز�Ƶ��                                                     
{  500,                        32000,                        5000,               0x094A,   },  //F0-19  ���Ƶ��                                                     
{  0,                          6,                            0,                  0x0840,   },  //F0-20  ����Ƶ��Դ                                                   
{  LOWER_FRQ_INDEX,            MAX_FRQ_INDEX,                5000,               0x074A,   },  //F0-21  ����Ƶ����ֵ�趨                                             
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F0-22  ����Ƶ��ƫ��                                                 
{  0,                          UPPER_FRQ_INDEX,              0,                  0x034A,   },  //F0-23  ����Ƶ����ֵ�趨                                             
{  0,                          1,                            0,                  0x0040,   },  //F0-24  ���з���                                                     
{  0,                          1,                            0,                  0x0840,   },  //F0-25  ���ѡ��                                                     
{  0,                          3,                            1,                  0x0840,   },  //F0-26  ����Ƶ��Դƫ��ѡ��                                           
{  1,                          2,                            1,                  0x1040,   },  //F0-27  F000 GP������ʾ                                                    
                                                                                                                          

//  F1 ����             F1 ����             F1 ����ֵ       F1 ���� 
{  0,                          DI_FUNC_NUMBER-1,             1,                  0x0880,   },  //F1-00  DI1���ӹ���ѡ��                                              
{  0,                          DI_FUNC_NUMBER-1,             2,                  0x0880,   },  //F1-01  DI2���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             8,                  0x0880,   },  //F1-02  DI3���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             9,                  0x0880,   },  //F1-03  DI4���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             12,                 0x0880,   },  //F1-04  DI5���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             13,                 0x0880,   },  //F1-05  DI6���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             14,                 0x0880,   },  //F1-06  DI7���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             15,                 0x0880,   },  //F1-07  DI8���ӹ���ѡ��                                                           
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //F1-08  ���ұ���δ����                                                           
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //F1-09  ���ұ���δ����                                                           
{  0,                          3,                            0,                  0x0840,   },  //F1-10  �������ʽ                                                 
{  1,                          65535,                        1000,               0x014B,   },  //F1-11  ����UP/DOWN���ʣ���Ϊ0.001Hz                                 
{  0,                          CURVE1_MAX_INDEX,             30,                 0x2322,   },  //F1-12  AI����1��С����                                   
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //F1-13  AI����1��С�����Ӧ�趨                                                            
{  CURVE1_MIN_INDEX,           1000,                         1000,               0x2522,   },  //F1-14  AI����1�������                                                                    
{  (Uint16)(-1000),            1000,                         1000,               0x2131,   },  //F1-15  AI����1��������Ӧ�趨                                                            
{  0,                          CURVE2_MAX_INDEX,             0,                  0x2322,   },  //F1-16  AI����2��С����                                                          
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //F1-17  AI����2��С�����Ӧ�趨                                                  
{  CURVE2_MIN_INDEX,           1000,                         1000,               0x2522,   },  //F1-18  AI����2�������                                                          
{  (Uint16)(-1000),            1000,                         1000,               0x2131,   },  //F1-19  AI����2��������Ӧ�趨                                                  
{  (Uint16)(-1000),            CURVE3_MAX_INDEX,             0,                  0x2322,   },  //F1-20  AI����3��С����                                                          
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //F1-21  AI����3��С�����Ӧ�趨                                                  
{  CURVE3_MIN_INDEX,           1000,                         1000,               0x2522,   },  //F1-22  AI����3�������                                                          
{  (Uint16)(-1000),            1000,                         1000,               0x2131,   },  //F1-23  AI����3��������Ӧ�趨                                                  
{  0x0111,                     0x0333,                       0x0321,             0x8140,   },  //F1-24  AI�趨����ѡ��                                               
{  0x0000,                     0x0111,                       0x0000,             0x8080,   },  //F1-25  AI����ѡ��   20170210                                                 
{  0,                          PULSE_IN_MAX_INDEX,           0,                  0x0302,   },  //F1-26  HDI������С����                                                          
{  0xFC18,                     1000,                         0,                  0x2131,   },  //F1-27  HDI������С�����Ӧ�趨                                                  
{  PULSE_IN_MIN_INDEX,         10000,                        5000,               0x0502,   },  //F1-28  HDI�����������                                                          
{  0xFC18,                     1000,                         1000,               0x2131,   },  //F1-29  HDI������������Ӧ�趨                                                  
{  0,                          1000,                         10,                 0x0083,   },  //F1-30  DI�˲�ʱ��                                                   
{  0,                          1000,                         10,                 0x0102,   },  //F1-31  AI1�˲�ʱ��, 10ms                                            
{  0,                          1000,                         10,                 0x0102,   },  //F1-32  AI2�˲�ʱ��, 10ms                                            
{  0,                          1000,                         10,                 0x0102,   },  //F1-33  AI3�˲�ʱ��, 10ms                                            
{  0,                          1000,                          0,                 0x0102,   },  //F1-34  HDI�˲�ʱ��, 10ms                                          
{  0,                          11111,                        0,                  0x4940,   },  //F1-35  DI��Ч״̬ѡ��1                                              
{  0,                          11111,                        0,                  0x4940,   },  //F1-36  DI��Ч״̬ѡ��2                                              
{  0,                          36000,                        0,                  0x0141,   },  //F1-37  DI1�ӳ�ʱ��                                                  
{  0,                          36000,                        0,                  0x0141,   },  //F1-38  DI2�ӳ�ʱ��                                                            
{  0,                          36000,                        0,                  0x0141,   },  //F1-39  DI2�ӳ�ʱ�� 
{  0,                          1,                            0,                  0x0040,   },  //F1-40  DI���ظ�����ѡ��0---���ɣ�1---���� (����)                                                                                                                                                                                                                    
{  0,                          10000,                        0,                  0x00F2,   },  //F1-41  keyAdX1     ���̵�λ��X1             (����)  
{  0,                          10000,                        10000,              0x00F2,   },  //F1-42  keyAdX2     ���̵�λ��X2             (����)  
{  0,                          10000,                        0,                  0x00F2,   },  //F1-43  keyAdValue  ���̵�λ���趨ֵ         (����) 
{  0xB1E0,                     20000,                        0,                  0x2132,   },  //F1-44  keyAdY1     ���̵�λ��X1��Ӧֵ       (����) 
{  0xB1E0,                     20000,                        10000,              0x2132,   },  //F1-45  keyAdY2     ���̵�λ��X2��Ӧֵ       (����) 
{  0,                          21,                           00,                 0x2040,   },  //F1-46  keyAdCntl   ���̵�λ������           (����) 
                                                                                                                                             
               
// F2����                      F2����                        F2����ֵ            F2����                                                                          
{  0,                          FUNCCODE_FM_OUT_SELECT_MAX,   0,                  0x0040,   },  //F2-00  �๦�ܶ������ѡ��                                           
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //F2-01  FMR���ѡ��                                                  
{  0,                          (DO_FUNC_NUMBER-1),           2,                  0x0080,   },  //F2-02  ���ư�RELAY���ѡ��                                          
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //F2-03  ��չ��RELAY���ѡ��                                          
{  0,                          (DO_FUNC_NUMBER-1),           1,                  0x0080,   },  //F2-04  DO1���ѡ��                                                  
{  0,                          (DO_FUNC_NUMBER-1),           1,                  0x0080,   },  //F2-05  ��չ��DO2���ѡ��                                            
{  0,                          17,                           0,                  0x0080,   },  //F2-06  FMP���ѡ��                                                  
{  0,                          17,                           2,                  0x0080,   },  //F2-07  AO1���ѡ��                                                  
{  0,                          17,                           13,                 0x0080,   },  //F2-08  ��չ��AO2���ѡ��                                            
{  1,                          10000,                        5000,               0x00C2,   },  //F2-09  FMP������Ƶ��                                              
{  0,                          36000,                        0,                  0x0141,   },  //F2-10  RELAY1����ӳ�ʱ��                                           
{  0,                          36000,                        0,                  0x0141,   },  //F2-11  RELAY2����ӳ�ʱ��                                           
{  0,                          36000,                        0,                  0x0141,   },  //F2-12  DO1����ӳ�ʱ��                                              
{  0,                          36000,                        0,                  0x0141,   },  //F2-13  DO2����ӳ�ʱ��                                              
{  0,                          36000,                        0,                  0x0141,   },  //F2-14  DO3����ӳ�ʱ��                                              
{  0,                          11111,                        0,                  0x4140,   },  //F2-15  DO��Ч״̬ѡ��                                               
{  0xFC18,                     1000,                         0,                  0x2131,   },  //F2-16  AO1��ƫϵ��                                                  
{  0xFC18,                     1000,                         100,                0x2102,   },  //F2-17  AO1����                                                      
{  0xFC18,                     1000,                         200,                0x2131,   },  //F2-18  AO2��ƫϵ��                                                  
{  0xFC18,                     1000,                         80,                 0x2102,   },  //F2-19  AO2����                                                      
{  0,                          1000,                         0,                  0x1102,   },  //F2-20  HDO,AO1,AO2����˲�ʱ��              (����)                                  
{  0,                          1000,                         0,                  0x1102,   },  //F2-21  AO1����˲�ʱ��                      (����)                                           
{  0,                          1000,                         0,                  0x1102,   },  //F2-22  AO2����˲�ʱ��                      (����) 

// F3����                      F3����                        F3����ֵ            F3����                                                                          
{  0,                          3,                            0,                  0x1040,   },  //F3-00  ������ʽ              //201612                                               
{  0,                          2,                            0,                  0x1040,   },  //F3-01  ת�ٸ��ٷ�ʽ          //20161201 PT                                              
{  1,                          100,                          20,                 0x10C0,   },  //F3-02  ת�ٸ��ٿ���          //201612                                                     
{  0,                          1000,                         0,                  0x010A,   },  //F3-03  ����Ƶ��                                                        
{  0,                          1000,                         0,                  0x08C1,   },  //F3-04  ����Ƶ�ʱ���ʱ��                                                
{  0,                          100,                          0,                  0x08F0,   },  //F3-05  ����ֱ���ƶ�����                                                
{  0,                          10000,                        0,                  0x0942,   },  //F3-06  ����ֱ���ƶ�ʱ��      //20161201 PT                                          
{  0,                          1,                            0,                  0x0040,   },  //F3-07  ͣ����ʽ                                                     
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F3-08  ͣ��ֱ���ƶ���ʼƵ��                                         
{  0,                          10000,                        0,                  0x0142,   },  //F3-09  ͣ��ֱ���ƶ��ȴ�ʱ��  //20161201 PT                                       
{  0,                          100,                          0,                  0x00F0,   },  //F3-10  ͣ��ֱ���ƶ�����                                             
{  0,                          10000,                        0,                  0x0142,   },  //F3-11  ͣ��ֱ���ƶ�ʱ��                                             
{  0,                          100,                          100,                0x00F0,   },  //F3-12  �ƶ�ʹ����                                                   
{  0,                          2,                            0,                  0x0840,   },  //F3-13  �Ӽ��ٷ�ʽ                                                   
{  0,                          1000,                         300,                0x0931,   },  //F3-14  S���߿�ʼ��ʱ�����                                          
{  0,                          1000,                         300,                0x0931,   },  //F3-15  S���߽�����ʱ�����   
{  0x0000,                     0x03E8,                       0x01F4,             0x0040,   },  //F3-16  ����
{  0x0000,   			             0x03E8,   			               0x0320,  		       0x0040,   },  //F3-17  ����
{  0x001E,   			             0x00C8,   			               0x0064,  		       0x0040,   },  //F3-18  ����
{  0x000A,   			             0x0064,   			               0x001E,  		       0x0040,   },  //F3-19  ����
{  0x0005,   			             0x001E,   			               0x000A,  		       0x0041,   },  //F3-20  ����
{  0x0000,   			             0x01F4,   			               0x0064,  		       0x0042,   },  //F3-21  ����
{  0x0000,   			             0xFDE8,   			               0x0000,  		       0x0140,   },  //F3-22  ����
{  0x0000,   			             0xFFFF,   			               0x0000,  		       0x0040,   },  //F3-23  ����
{  0x0000,   			             0xFFFF,   			               0x0000,  		       0x0040,   },  //F3-24  ����
{  0x0000,   			             0xFFFF,   			               0x0000,  		       0x0040,   },  //F3-25  ����
{  0x0000,   			             0xFFFF,   			               0x0000,  		       0x0040,   },  //F3-26  ����
                                                                                                        
// F4����                      F4����                        F4����ֵ            F4����                                                                           
{  0,                          11,                           0,                  0x0840,   },   //F4-00    VF�����趨                                                   
{  0,                          300,                          10,                 0x00F1,   },   //F4-01    ת������                                                   
{  0,                          MAX_FRQ_INDEX,                5000,               0x0B4A,   },   //F4-02    ת��������ֹƵ��                                             
{  0,                          VF_FRQ2_INDEX,                0,                  0x0B4A,   },   //F4-03    ���VFƵ�ʵ�1                                                
{  0,                          1000,                         0,                  0x0931,   },   //F4-04    ���VF��ѹ��1                                                
{  VF_FRQ1_INDEX,              VF_FRQ3_INDEX,                0,                  0x0F4A,   },   //F4-05    ���VFƵ�ʵ�2                                                
{  0,                          1000,                         0,                  0x0931,   },   //F4-06    ���VF��ѹ��2                                                
{  VF_FRQ2_INDEX,              RATING_FRQ_INDEX,             0,                  0x0F4A,   },   //F4-07    ���VFƵ�ʵ�3                                                
{  0,                          1000,                         0,                  0x0931,   },   //F4-08    ���VF��ѹ��3                                                
{  0,                          2000,                         0,                  0x0131,   },   //F4-09    ת���ϵ��                                                 
{  0,                          200,                          64,                 0x00C0,   },   //F4-10    VF����������                                                 
{  0,                          100,                          0,                  0x00C0,   },   //F4-11    ����������  
//{  0,                          4,                            3,                  0x0840,   },   //F4-12    ������ģʽѡ��                                               
{  0,                          8,                            0,                  0x0140,   },   //F4-13    VF����ĵ�ѹԴ    //20161201 PT                                                                    
{  0,                          RATING_VOL_INDEX,             0,                  0x0340,   },   //F4-14    VF����ĵ�ѹԴ�����趨                                       
{  0,                          10000,                        0,                  0x0141,   },   //F4-15    VF����ĵ�ѹ����ʱ��                                                                                   
{  0,                          4,                            3,                  0x0840,   },   //F4-12    ������ģʽѡ��   20170503
{  0x0032,                     0x00C8,                       0x0082,             0x0840,   },   //F4-16    ����ʧ�ٵ��� MD500
{  0x0000,                     0x0001,                       0x0001,             0x0840,   },   //F4-17    ����ʧ��ʹ�� MD500
{  0x0000,                     0x0064,                       0x0014,             0x0040,   },   //F4-18    ����ʧ������ MD500
{  0x0032,                     0x00C8,                       0x0032,             0x0840,   },   //F4-19    ���ٹ������� MD500
{  0x1964,                     0x1F40,                       0x1DB0,             0x0841,   },   //F4-20    ��ѹʧ�ٵ�ѹ MD500
{  0x0000,                     0x0001,                       0x0000,             0x0840,   },   //F4-21    ��ѹʧ��ʹ�� MD500
{  0x0000,                     0x0064,                       0x001E,             0x0040,   },   //F4-22    ��ѹƵ������ MD500
{  0x0000,                     0x0064,                       0x001E,             0x0040,   },   //F4-23    ��ѹ��ѹ���� MD500
{  0x0000,                     0x0032,                       0x0005,             0x0840,   },   //F4-24    ��ѹƵ������ MD500
{  0x0001,                     0x0064,                       0x0005,             0x0041,   },   //F4-25    ת������� MD500  

{  0x0000,                     0x0001,                       0x0000,             0x0840,   },   //F4-26~48 VF������   MD500
{  0x000A,                     0x0064,                       0x0032,             0x0840,   },   //F4-27
{  0x000A,                     0x0064,                       0x0014,             0x0840,   },   //F4-28
{  0x0000,                     0x0064,                       0x0032,             0x0040,   },   //F4-29
{  0x0000,                     0x0064,                       0x0032,             0x0040,   },   //F4-30
{  0x0050,                     0x0096,                       0x0064,             0x0840,   },   //F4-31
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-32
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-33
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-34
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-35
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-36
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-37
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-38
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-39
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-40
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-41
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-42
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-43
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-44
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-45
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-46
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-47
{  0x0000,                     0xFFFF,                       0x0000,             0x0040,   },   //F4-48                                       

// F5����                      F5����                        F5����ֵ            F5����                                                                          

{  1,                          100,                          30,                 0x00C0,   },  //F5-00  �ٶȻ���������1                 F2-00                                           
{  1,                          1000,                         50,                 0x0102,   },  //F5-01  �ٶȻ�����ʱ��1                 F2-01                                           
{  0,                          VC_CHG_FRQ2_INDEX,            500,                0x034A,   },  //F5-02  �л�Ƶ��1                       F2-02                          
{  1,                          100,                          20,                 0x00C0,   },  //F5-03  �ٶȻ���������2                 F2-03                          
{  1,                          1000,                         100,                0x0102,   },  //F5-04  �ٶȻ�����ʱ��2                 F2-04                          
{  VC_CHG_FRQ1_INDEX,          MAX_FRQ_INDEX,                1000,               0x074A,   },  //F5-05  �л�Ƶ��2                       F2-05                          
{  0,                          1,                            0,                  0x4080,   },  //F5-06  �ٶȻ���������                  F2-17                          
{  0,                          7,                            0,                  0x0040,   },  //F5-07  �ٶȿ���(����)ת������Դ        F2-09                          
{  0,                          2000,                         1500,               0x0131,   },  //F5-08  �ٶȿ���(����)ת�����������趨  F2-10                          
{  50,                         200,                          100,                0x00F0,   },  //F5-09  ת���ϵ��                    F2-06                          
{  0,                          100,                          15,                 0x0143,   },  //F5-10  �ٶȻ��˲�ʱ�䳣��              F2-07                          
{  0,                          200,                          64,                 0x00C0,   },  //F5-11  ʸ�����ƹ���������              F2-08                          
{  0,                          60000,                        2000,               0x0140,   },  //F5-12  M�������Kp                     F2-13                          
{  0,                          60000,                        1300,               0x0140,   },  //F5-13  M�������Ki                     F2-14                          
{  0,                          60000,                        2000,               0x0140,   },  //F5-14  T�������Kp                     F2-15                          
{  0,                          60000,                        1300,               0x0140,   },  //F5-15  T�������Ki                     F2-16                          
{  0,                          2,                            1,                  0x0040,   },  //F5-16  ͬ��������ģʽ                  F2-18                          
//{  50,                         500,                          100,                0x00F0,   },  //F5-17  ͬ��������ϵ��                  F2-19   20170213
{  0x0001,                     0x0032,                       0x0005,             0x00F0,   },  //F5-17  ͬ��������ϵ��                  F2-19                      
{  1,                          300,                          50,                 0x08F0,   },  //F5-18  ������ŵ���                    F2-20                          
{  10,                         500,                          100,                0x00F0,   },  //F5-19  �����Զ���гϵ��                F2-21                          
{  0,                          1,                            0,                  0x0080,   },  //F5-20  ����ת��������Чʹ��            F2-22                                  
{  0,                          7,                            0,                  0x0040,   },  //F5-21  �ٶȿ���(�ƶ�)ת������Դ        F2-11                          
{  0,                          2000,                         1500,               0x0131,   },  //F5-22  �ٶȿ���(�ƶ�)ת�����������趨  F2-12                          

{  0x0000,                     0x0032,                       0x0005,             0x0140,   },  //F5-23  ͬ���������ѹ����ԣ��          F2-23
{  0x0032,                     0x00B4,                       0x0050,             0x0130,   },  //F5-24  ͬ������ʼλ�ýǼ�����        F2-24
{  0x0000,                     0x0002,                       0x0000,             0x0140,   },  //F5-25  ͬ������ʼλ�ýǼ��            F2-25
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //F5-26  ����                            F2-26
{  0x0032,                     0x01F4,                       0x0064,             0x0130,   },  //F5-27  ͬ����͹���ʵ�������            F2-27
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //F5-28  ���ת�ص����ȿ���              F2-28
{  0x0000,                     0x0002,                       0x0000,             0x0140,   },  //F5-29  ����                            F2-29
{  0x0001,                     0x0064,                       0x0006,             0x0140,   },  //F5-30  ����                            F2-30
{  0x0001,                     0x0064,                       0x0006,             0x0140,   },  //F5-31  ����                            F2-31
{  0x0000,                     0x0001,                       0x0001,             0x0140,   },  //F5-32  Z�ź�У��                       F2-32
{  0x000A,                     0x03E8,                       0x0064,             0x0140,   },  //F5-33  ����                            F2-33
{  0x0005,                     0x00C8,                       0x0028,             0x0140,   },  //F5-34  ����                            F2-34
{  0x0005,                     0x01F4,                       0x001E,             0x0140,   },  //F5-35  ����                            F2-35
{  0x0000,                     0x0050,                       0x001E,             0x0140,   },  //F5-36  �������ŵ���                    F2-36
{  0x0008,                     0x0012,                       0x000F,             0x0341,   },  //F5-37  ������Ƶ   20170410 PT ����F0.18                     F2-37
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //F5-38  SVC�����ƶ���ʽ                 F2-38
{  0x0000,                     0x03E8,                       0x00C8,             0x0142,   },  //F5-39  SVC�����ƶ���ЧƵ��             F2-39
//{  0x0005,                     0x2710,                       0x000A,             0x0144,   },  //F5-40  SVC��Ƶ�ƶ�Ƶ�ʱ仯����         F2-40
{  0x0005,                     0x2328,                       0x000A,             0x0144,   },  //F5-40  20170410 PT
{  0x0000,                     0x0050,                       0x0032,             0x0140,   },  //F5-41  SVC��Ƶ�ƶ�����                 F2-41
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //F5-42  ͬ����SVC�ٶȸ���               F2-42
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //F5-43  ���ŷ�ʹ��                      F2-43
{  0x0000,                     VC_CHG_FRQ1_INDEX,            0x001E,             0x034A,   },  //F5-44  �л�Ƶ��   20170410 PT                     F2-44
{  0x0001,                     0x0064,                       0x000A,             0x0140,   },  //F5-45  ���ŷ��ٶȻ���������            F2-45
{  0x0001,                     0x03E8,                       0x0032,             0x0142,   },  //F5-46  ���ŷ��ٶȻ�����ʱ��            F2-46
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //F5-47  ֹͣ����תʹ��                  F2-47
{  0x0000,                     0x0064,                       0x0008,             0x0141,   },  //F5-48  ͣ���Ƕ�                        F2-48
{  0x001E,                     0x012C,                       0x0078,             0x0140,   },  //F5-49  ����                            F2-49                    

//  F6 ����             F6 ����             F6 ����ֵ       F6 ����  
{  0,                          1,                            1,                  0x0040,   },  //F6-00  STOP������                                                   
{  0,                          0xFFFF,                       0x1F,               0x8140,   },  //F6-01  LED������ʾ����1                                             
{  0,                          0xFFFF,                       0,                  0x8140,   },  //F6-02  LED������ʾ����2                                             
{  1,                          0xFFFF,                       0x33,               0x8140,   },  //F6-03  LEDͣ����ʾ����                                              
{  1,                          65000,                        30000,              0x0144,   },  //F6-04  �����ٶ���ʾϵ�� rev by llh 20140815 for ����ֵ                                            
{  0,                          23,                           21,                  0x0040,   },  //F6-05  �����ٶ���ʾС����λ��                                       
{  0,                          999,                          0,                  0x30C0,   },  //F6-06  �����ģ��ɢ�����¶�                                         
{  0,                          65535,                        0,                  0x1140,   },  //F6-07  �ۼ�����ʱ��, ��λ: h                                        
{  0,                          65535,                        0,                  0x1140,   },  //F6-08  �ۼ��ϵ�ʱ��                                                 
{  0,                          65535,                        0,                  0x1140,   },  //F6-09  �ۼƺĵ���                                                   
{  0,                          65535,                        0,                  0x1140,   },  //F6-10  ��Ʒ��                                                       
{  0,                          65535,                        0,                  0x1142,   },  //F6-11  ����汾��                                                   
{  0,                          999,                          0,                  0x30C0,   },  //F6-12  ������ɢ�����¶�                                             
{  0,                          1,                            0,                  0x1040,   },  //F6-13  ����                                                         
{  0,                          4,                            0,                  0x1140,   },  //F6-14  MF.K������ѡ��

// F7����                      F7����                        F7����ֵ            F7����                                                                          
{  0,                          MAX_FRQ_INDEX,                600,                0x034A,   },  //F7-00  �㶯����Ƶ�� rev by llh 20140815 for ����ֵ                                                
{  0,                          65000,                        50,                 0x0141,   },  //F7-01  �㶯����ʱ�� rev by llh 20140815 for ����ֵ                                                 
{  0,                          65000,                        50,                 0x0141,   },  //F7-02  �㶯����ʱ�� rev by llh 20140815 for ����ֵ                                                 
{  0,                          1,                            1,                  0x0040,   },  //F7-03  ���ӵ㶯����                                                 
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F7-04  ��ԾƵ��1                                                    
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F7-05  ��ԾƵ��2                                                    
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F7-06  ��ԾƵ�ʷ���                                                 
{  0,                          1,                            0,                  0x0040,   },  //F7-07  �Ӽ��ٹ�������ԾƵ���Ƿ���Ч                                 
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F7-08  ����ʱ��2                                                    
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F7-09  ����ʱ��2                                                    
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F7-10  ����ʱ��3                                                    
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F7-11  ����ʱ��3                                                    
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F7-12  ����ʱ��4                                                    
{  0,                          65000,                        ACC_DEC_T_INIT1,    0x0141,   },  //F7-13  ����ʱ��4                                                    
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F7-14  ����ʱ��1/2�л�Ƶ�ʵ�                                        
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //F7-15  ����ʱ��1/2�л�Ƶ�ʵ�                                        
{  0,                          30000,                        0,                  0x0141,   },  //F7-16  ����ת����ʱ��                                               
{  0,                          1,                            0,                  0x0040,   },  //F7-17  ��ת����, 0-����ת��1-��ֹ��ת                             
{  0,                          2,                            0,                  0x0040,   },  //F7-18  Ƶ�ʵ�������Ƶ�����ж���                                     
{  0,                          1000,                         0,                  0x010A,   },  //F7-19  �´�����                                                     
{  0,                          65000,                        0,                  0x0040,   },  //F7-20  �趨�ϵ絽��ʱ��                                             
{  0,                          65000,                        0,                  0x0140,   },  //F7-21  �趨���е����                                             
{  0,                          1,                            0,                  0x0040,   },  //F7-22  ��������ѡ��                                                 
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //F7-23  Ƶ�ʼ��ֵ(FDT��ƽ)                                          
{  0,                          1000,                         50,                 0x0131,   },  //F7-24  Ƶ�ʼ���ͺ�ֵ                                               
{  0,                          1000,                         0,                  0x0131,   },  //F7-25  Ƶ�ʵ���������                                             
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //F7-26  Ƶ�ʼ��ֵ(FDT1��ƽ)                                         
{  0,                          1000,                         50,                 0x0131,   },  //F7-27  Ƶ�ʼ��1�ͺ�ֵ                                              
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //F7-28  Ƶ�ʵ�����ֵ1                                              
{  0,                          1000,                         0,                  0x0131,   },  //F7-29  Ƶ�ʵ�����1����                                            
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //F7-30  Ƶ�ʵ�����ֵ2                                              
{  0,                          1000,                         0,                  0x0131,   },  //F7-31  Ƶ�ʵ�����2����                                            
{  0,                          3000,                         50,                 0x0131,   },  //F7-32  ��������ֵ                                                 
{  1,                          60000,                        10,                 0x0142,   },  //F7-33  ���������ӳ�ʱ��                                           
{  0,                          3000,                         2000,               0x0131,   },  //F7-34  ���������                                                   
{  0,                          60000,                        0,                  0x0142,   },  //F7-35  �����������ӳ�ʱ��                                         
{  0,                          3000,                         1000,               0x0131,   },  //F7-36  ����������ֵ1                                              
{  0,                          3000,                         0,                  0x0131,   },  //F7-37  ����������1����                                            
{  0,                          3000,                         1000,               0x0131,   },  //F7-38  ����������ֵ1                                              
{  0,                          3000,                         0,                  0x0131,   },  //F7-39  ����������1����                                            
{  0,                          100,                          75,                 0x0100,   },  //F7-40  ģ���¶ȵ���                                                 
{  0,                          1,                            0,                  0x0840,   },  //F7-41  ���ȿ���                                                     
{  0,                          1,                            0,                  0x0840,   },  //F7-42  ��ʱ����ѡ��                                                 
{  0,                          3,                            0,                  0x0840,   },  //F7-43  ��ʱʱ���趨ѡ��                                             
{  0,                          65000,                        0,                  0x0941,   },  //F7-44  �趨����ʱ��                                                 
{  0,                          65000,                        0,                  0x0941,   },  //F7-45  ��ǰ���е���ʱ��                                             
{  WAKE_UP_LIMIT,              MAX_FRQ_INDEX,                0,                  0x074A,   },  //F7-46  ����Ƶ��                                                     
{  0,                          65000,                        0,                  0x0141,   },  //F7-47  �����ӳ�ʱ��                                                 
{  0,                          DORMANT_UPPER,                0,                  0x034A,   },  //F7-48  ����Ƶ��                                                     
{  0,                          65000,                        0,                  0x0141,   },  //F7-49  �����ӳ�ʱ��                                                 
{  0,                          AI1_UPPER,                    310,                0x0322,   },  //F7-50  AI1�����ѹ����                                              
{  AI1_LIMIT,                  1100,                         680,                0x0522,   },  //F7-51  AI1�����ѹ����   

// F8����                      F8����                        F8����ֵ            F8����                                                                          
{  0,                          100,                          20,                 0x00C0,   },  //F8-00  ����ʧ������                                                 
{  100,                        200,                          150,                0x00F0,   },  //F8-01  ����ʧ�ٱ�������                                             
{  0,                          1,                            1,                  0x0040,   },  //F8-02  ������ر���ѡ��                                             
{  20,                         1000,                         100,                0x0102,   },  //F8-03  ������ر�������                                             
{  50,                         100,                          80,                 0x00F0,   },  //F8-04  �������Ԥ��ϵ��                                             
{  0,                          100,                          0,                  0x00C0,   },  //F8-05  ��ѹʧ������                                                 
{  120,                        150,                          130,                0x00F0,   },  //F8-06  ��ѹʧ�ٱ�����ѹ                                             
{  0,                          11,                           11,                 0x4040,   },  //F8-07  ����ȱ�ౣ��ѡ��                                             
{  0,                          1,                            1,                  0x0040,   },  //F8-08  ���ȱ�ౣ��ѡ��                                             
{  0,                          1,                            1,                  0x0040,   },  //F8-09  �ϵ�Եض�·��������                                         
{  0,                          32767,                        0,                  0x0040,   },  //F8-10  �����Զ���λ����                                             
{  0,                          1,                            0,                  0x0040,   },  //F8-11  �����Զ���λ�ڼ���ϼ̵�������ѡ��                           
{  1,                          1000,                         10,                 0x0101,   },  //F8-12  �����Զ���λ���ʱ��, 0.1s                                   
{  0,                          500,                          200,                0x0131,   },  //F8-13  ���ٶȼ��ֵ                                                 
{  0,                          600,                          10,                 0x00C1,   },  //F8-14  ���ٶȼ��ʱ��                                               
{  0,                          500,                          200,                0x0131,   },  //F8-15  �ٶ�ƫ�������ֵ                                           
{  0,                          600,                          50,                 0x00C1,   },  //F8-16  �ٶ�ƫ�������ʱ��                                         
{  0,                          22222,                        0,                  0x4040,   },  //F8-17  ����ʱ��������ѡ��1                                        
{  0,                          22210,                        0,                  0x4040,   },  //F8-18  ����ʱ��������ѡ��2                                                            
{  0,                          22222,                        0,                  0x4040,   },  //F8-19  ����ʱ��������ѡ��3                                                            
{  0,                          22222,                        0,                  0x4040,   },  //F8-20  ����ʱ��������ѡ��4                                                            
{  6500,                       8000,                         6800,               0x0061,   },  //F8-21  �ƶ���ѹ��ֵ                                                          
{  0,                          65535,                        0,                  0x1040,   },  //F8-22  ����ָʾѡ��1,2                                              
{  0,                          65535,                        0,                  0x1040,   },  //F8-23  0                                                            
{  0,                          4,                            0,                  0x0040,   },  //F8-24  ����ʱ��������Ƶ��ѡ��                                       
{  0,                          1000,                         1000,               0x0131,   },  //F8-25  �쳣����Ƶ���趨                                             
{  0,                          2,                            0,                  0x4040,   },  //F8-26  ˲ͣ��ͣ����ѡ��                                             
{  500,                        1000,                         900,                0x0131,   },  //F8-27  ˲ͣ������ͣ�жϵ�ѹ                                         
{  0,                          10000,                        50,                 0x0142,   },  //F8-28  ˲ͣ��ͣ��ѹ�����ж�ʱ��                                     
{  500,                        1000,                         800,                0x0131,   },  //F8-29  ˲ͣ��ͣ�����жϵ�ѹ                                         
{  0,                          1,                            0,                  0x0040,   },  //F8-30  ���ر���ѡ��                                                 
{  0,                          1000,                         100,                0x0131,   },  //F8-31  ���ؼ��ˮƽ                                                 
{  0,                          600,                          10,                 0x00C1,   },  //F8-32  ���ؼ��ʱ��                                                 
{  0,                          2,                            0,                  0x0040,   },  //F8-33  ����¶ȴ���������                                           
{  0,                          200,                          110,                0x00C0,   },  //F8-34  ������ȱ�����ֵ                                             
{  0,                          200,                          90,                 0x00C0,   },  //F8-35  �������Ԥ������ֵ   
{  0x0000,                     0x0001,                       0x0001,             0x0140,   },  //F8-36  UVW����������(Err20)ʹ��
{  0x0000,                     0x000B,                       0x000B,             0x4040,   },  //F8-37  ���ϱ�������ѡ��5 ��λ��Err51��ʮλ��Err19�� 
                                                                                                                                                  
// F9����                      F9����                        F9����ֵ            F9����                                                                          
{  0,                          6039,                         6005,               0x6040,   },  //F9-00  ������                                                       
{  0,                          3,                            0,                  0x0040,   },  //F9-01  ���ݸ�ʽ                                                     
{  1,                          250,                          1,                  0x00C0,   },  //F9-02  ������ַ                                                     
{  0,                          20,                           2,                  0x0080,   },  //F9-03  Ӧ���ӳ�                                                     
{  0,                          600,                          0,                  0x00C1,   },  //F9-04  ͨѶ��ʱʱ��                                                 
{  0,                          31,                           31,                 0x2040,   },  //F9-05  ͨѶ���ݴ��͸�ʽѡ��                                         
{  0,                          1,                            0,                  0x0040,   },  //F9-06  ͨѶ���ӷ�ʽ                                                 
{  0,                          3,                            0,                  0x0040,   },  //F9-07  ͨѶЭ��ѡ��                                                 
{  0,                          1,                            0,                  0x0040,   },  //F9-08  0                                                            
                                                                                                                                                  
// FA����                      FA����                        FA����ֵ            FA����                                                                          

{  0,                          1,                            0,                  0x0840,   },  //FA-00  ת�ؿ���                                                     
{  0,                          7,                            0,                  0x0840,   },  //FA-01  ����ת������Դ                                               
{  (Uint16)(-2000),            2000,                         1500,               0x2131,   },  //FA-02  ����ת������                                                 
{  0,                          65000,                        0,                  0x0142,   },  //FA-03  ת�ؼ���ʱ��                                                 
{  0,                          65000,                        0,                  0x0142,   },  //FA-04  ת�ؼ���ʱ��                                                 
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //FA-05  ת�ؿ����������Ƶ��                                         
{  0,                          MAX_FRQ_INDEX,                5000,               0x034A,   },  //FA-06  ת�ؿ��Ʒ������Ƶ��                                         
{  0,                          1000,                         0,                  0x0142,   },  //FA-07  ת���˲�                                                     
{  0,                          5,                            0,                  0x1040,   },  //FA-08  �ƶ�ת������Դ                                               
                                                                                                                                                 
// FB����                      FB����                        FB����ֵ            FB����                                                                          

{  0,                          1,                            1,                  0x0140,   },  //FB-00  ������ʹ��                                                 
{  1000,                       4200,                         3500,               0x0131,   },  //FB-01  Ƿѹ������                                                   
{  2000,                       25000,                        8200,               0x0921,   },  //FB-02  ��ѹ������                                                    
{  0,                          2,                            1,                  0x0140,   },  //FB-03  ��������ģʽѡ��                                            
{  100,                        120,                          110,                0x0100,   },  //FB-04  ���������ʱ��?                                              
{  0,                          2,                            1,                  0x0940,   },  //FB-05  SVC�Ż�ѡ�� 0-���Ż�  1-�Ż�ģʽ1  2-�Ż�ģʽ2               
{  0,                          1500,                         1200,               0x010A,   },  //FB-06  DPWM�л�����Ƶ��                                             
{  0,                          1,                            0,                  0x0140,   },  //FB-07  ���Ʒ�ʽ��0-�첽���ƣ�1-ͬ������                             
{  0,                          10,                           0,                  0x0140,   },  //FB-08  ���PWM                                                      
{  100,                        200,                          150,                0x08F0,   },  //FB-09  ����ʱ�����-1140Vר��                                       
                                                                                                                                              
// FC����                      FC����                        FC����ֵ            FC����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //FC-00  0                                                            
{  0,                          1000,                         0,                  0x0102,   },  //FC-01  ��������ϵ��                                                                                                                                                                                                                                                                                                                                                                                                                           
{  0,                          1000,                         0,                  0x0131,   },  //FC-02  ��������ƫ��

// FD����                      FD?                       FD����ֵ                FD����                                                                    
{  0,                          65535,                        0,                  0x0140,   },  //FD-00  0                                                            
                                                                                                                                                  
// FE����                      FE����                        FE����ֵ            FE����                                                                        
{  0,                          65535,                        0,                  0x0140,   },  //FE-00  0                                                            
                                                                                                                                                  
// FF����                      FF����                        FF����?             FF����                                                                      
{  0,                          65535,                        0,                  0x0140,   },  //FF-00  0                                                            
                                                                                                                                                   
// E0����                      E0����                        E0����ֵ            E0����                                                                          
{  0,                          1,                            0,                  0x0040,   },  //E0-00  ��Ƶ��ʽ                                                 
{  0,                          1000,                         0,                  0x0131,   },  //E0-01  ��Ƶ����                                                     
{  0,                          500,                          0,                  0x00F1,   },  //E0-02  ͻ��Ƶ�ʷ���                                                 
{  1,                          30000,                        100,                0x0141,   },  //E0-03  ��Ƶ����                                                     
{  1,                          1000,                         500,                0x0131,   },  //E0-04  ��Ƶ�����ǲ�����ʱ��                                         
{  0,                          65535,                        1000,               0x0140,   },  //E0-05  �趨����                                                     
{  0,                          65535,                        0,                  0x0140,   },  //E0-06  ʵ�ʳ���                                                     
{  1,                          65535,                        1000,               0x0141,   },  //E0-07  ÿ������������λ: 0.1                                        
{  1,                          65535,                        1000,               0x0140,   },  //E0-08  �趨����ֵ                                                   
{  1,                          65535,                        1000,               0x0140,   },  //E0-09  ָ������ֵ                                                   
                                                                                                                                                
// E1����                      E1����                        E1����ֵ            E1����                                                                          
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-00  ���ָ��0-���ָ��15                                         
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-01  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-02  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-03  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-04  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-05  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-06  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-07  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-08  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-09  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-10  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-11  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-12  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-13  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-14  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E1-15  0                                                            
{  0,                          2,                            0,                  0x0040,   },  //E1-16  PLC���нʽ                                                  
{  0,                          11,                           11,                 0x4080,   },  //E1-17  PLC�������ѡ��                                              
{  0,                          65000,                        0,                  0x0141,   },  //E1-18  PLC����ʱ�䣬�Ӽ���ʱ��ѡ��                                  
{  0,                          3,                            0,                  0x0040,   },  //E1-19  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-20  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-21  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-22  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-23  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-24  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-25  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-26  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-27  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-28  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-29  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-30  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-31  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-32  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-33  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-34  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-35  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-36  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-37  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-38  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-39  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-40  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-41  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-42  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-43  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-44  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-45  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-46  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-47  0                                                            
{  0,                          65000,                        0,                  0x0141,   },  //E1-48  0                                                            
{  0,                          3,                            0,                  0x0040,   },  //E1-49  0                                                            
{  0,                          1,                            0,                  0x0040,   },  //E1-50  PLC����ʱ�䵥λ                                              
{  0,                          7,                            0,                  0x0040,   },  //E1-51  ���ָ��0������ʽ                                            
                                                                                                                                                  
// E2����                      E2����                        E2����ֵ            E2����                                                                          
{  0,                          7,                            0,                  0x0040,   },  //E2-00  PID����Դ                                                    
{  0,                          1000,                         500,                0x0131,   },  //E2-01  PID��ֵ����, 0.1%                                            
{  0,                          9,                            0,                  0x0040,   },  //E2-02  PID����Դ                                                    
{  0,                          1,                            0,                  0x0040,   },  //E2-03  PID���÷���                                                  
{  0,                          65535,                        1000,               0x0140,   },  //E2-04  PID������������                                              
{  0,                          MAX_FRQ_INDEX,                0,                  0x034A,   },  //E2-05  PID��ת��ֹƵ�� rev by llh 20140815 for ����ֵ                                             
{  0,                          1000,                         0,                  0x0131,   },  //E2-06  PIDƫ���                                                  
{  0,                          10000,                        10,                 0x00F2,   },  //E2-07  PID΢���޷�                                                  
{  0,                          65000,                        0,                  0x0102,   },  //E2-08  PID�����仯ʱ��                                              
{  0,                          6000,                         0,                  0x0102,   },  //E2-09  PID�����˲�ʱ��                                              
{  0,                          6000,                         0,                  0x0102,   },  //E2-10  PID����˲�ʱ��                                              
{  0,                          1000,                         0,                  0x0131,   },  //E2-11  PID����������ֵ                                            
{  0,                          200,                          0,                  0x00C1,   },  //E2-12  PID������ʧ���ʱ��                                          
{  0,                          2000,                         800,                0x0101,   },  //E2-13  ��������P                                                    
{  1,                          1000,                         50,                 0x0102,   },  //E2-14  ����ʱ��I                                                    
{  0,                          10000,                        0,                  0x0103,   },  //E2-15  ΢��ʱ��D                                                    
{  0,                          2000,                         200,                0x0101,   },  //E2-16  PID��������P2                                                
{  1,                          1000,                         200,                0x0102,   },  //E2-17  PID���ֱ��I2                                                
{  0,                          10000,                        0,                  0x0103,   },  //E2-18  PID΢��ʱ��D2                                                
{  0,                          2,                            0,                  0x0040,   },  //E2-19  PID�����л�����                                              
{  0,                          PID_PARA_CHG_DELTA1_MAX,      200,                0x0331,   },  //E2-20  PID�����л�ƫ��1                                             
{  PID_PARA_CHG_DELTA2_MIN,    1000,                         800,                0x0531,   },  //E2-21  PID�����л�ƫ��2                                             
{  0,                          11,                           0,                  0x4040,   },  //E2-22  PID��������                                                  
{  0,                          1000,                         0,                  0x0131,   },  //E2-23  PID��ֵ                                                      
{  0,                          65000,                        0,                  0x0102,   },  //E2-24  PID��ֵ����ʱ��                                              
{  0,                          10000,                        100,                0x00F2,   },  //E2-25  PID�������֮��ƫ������ֵ                                  
{  0,                          10000,                        100,                0x00F2,   },  //E2-26  PID�������֮��ƫ�����Сֵ                                  
{  0,                          1,                            1,                  0x0040,   },  //E2-27  PID����ģʽ(ͣ���Ƿ�����). ��ˮģʽ�£�ͣ��ʱPIDҲ����. rev by llh 20140815 for ����ֵ      
{  0,                          65535,                        0,                  0x0140,   },  //E2-28  PID��������(��δ��)                                          

// E3����                      E3����                        E3����ֵ            E3����                                                                          
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-00  0                                                            
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-01  0                                                            
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-02  0                                                            
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-03  0                                                            
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-04  0                                                            
{  0,                          11111,                        0,                  0x4140,   },  //E3-05  VDI���ӹ������趨��Ч״̬                                    
{  0,                          22222,                        11111,              0x4940,   },  //E3-06  VDI������Ч״̬��Դ                                          
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-07  AI1���ӹ���ѡ�񣨵���DI��                                    
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-08  0                                                            
{  0,                          DI_FUNC_NUMBER-1,             0,                  0x0880,   },  //E3-09  0                                                            
{  0,                          111,                          0,                  0x4940,   },  //E3-10  AI��ΪDI��Ч״̬ѡ��                                         
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //E3-11  ����VDO1��VDO5���ѡ��                                       
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //E3-12  0                                                            
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //E3-13  0                                                            
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //E3-14  0                                                            
{  0,                          (DO_FUNC_NUMBER-1),           0,                  0x0080,   },  //E3-15  0
{  0,                          11111,                        0,                  0x4080,   },  //E3-16  VDO���������Ч״̬ѡ��                                                                                                  
{  0,                          36000,                        0,                  0x0141,   },  //E3-17  VDO1��VDO5�ӳ�ʱ��                                           
{  0,                          36000,                        0,                  0x0141,   },  //E3-18  0                                                            
{  0,                          36000,                        0,                  0x0141,   },  //E3-19  0                                                            
{  0,                          36000,                        0,                  0x0141,   },  //E3-20  0                                                            
{  0,                          36000,                        0,                  0x0141,   },  //E3-21  0                                                            
                                                                                                                                                  
// E4����                      E4����                        E4����ֵ            E4����                                                                          
{  0,                          65535,                        1,                  0x0140,   },  //E4-00  �û����ƹ�������                                             
{  0,                          65535,                        2,                  0x0140,   },  //E4-01  0                                                            
{  0,                          65535,                        3,                  0x0140,   },  //E4-02  0                                                            
{  0,                          65535,                        7,                  0x0140,   },  //E4-03  0                                                            
{  0,                          65535,                        8,                  0x0140,   },  //E4-04  0                                                            
{  0,                          65535,                        17,                 0x0140,   },  //E4-05  0                                                            
{  0,                          65535,                        18,                 0x0140,   },  //E4-06  0                                                            
{  0,                          65535,                        300,                0x0140,   },  //E4-07  0                                                            
{  0,                          65535,                        301,                0x0140,   },  //E4-08  0                                                            
{  0,                          65535,                        400,                0x0140,   },  //E4-09  0                                                            
{  0,                          65535,                        401,                0x0140,   },  //E4-10  0                                                            
{  0,                          65535,                        402,                0x0140,   },  //E4-11  0                                                            
{  0,                          65535,                        504,                0x0140,   },  //E4-12  0                                                            
{  0,                          65535,                        507,                0x0140,   },  //E4-13  0                                                            
{  0,                          65535,                        600,                0x0140,   },  //E4-14  0                                                            
{  0,                          65535,                        610,                0x0140,   },  //E4-15  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-16  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-17  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-18  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-19  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-20  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-21  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-22  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-23  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-24  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-25  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-26  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-27  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-28  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-29  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-30  0                                                            
{  0,                          65535,                        0,                  0x0140,   },  //E4-31  0                                                            
                                                                                                                                                  
// E5����                      E5����                        E5����ֵ            E5����                                                                          
{  (Uint16)(-1000),            CURVE4_INFLEX1_INDEX,         0,                  0x2322,   },  //E5-00  ����4��4�㣬���ֵ����Сֵ��2���м��                        
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E5-01  0                                                            
{  CURVE4_MIN_INDEX,           CURVE4_INFLEX2_INDEX,         300,                0x2722,   },  //E5-02  0                                                            
{  (Uint16)(-1000),            1000,                         300,                0x2131,   },  //E5-03  0                                                            
{  CURVE4_INFLEX1_INDEX,       CURVE4_MAX_INDEX,             600,                0x2722,   },  //E5-04  0                                                            
{  (Uint16)(-1000),            1000,                         600,                0x2131,   },  //E5-05  0                                                            
{  CURVE4_INFLEX2_INDEX,       1000,                         1000,               0x2522,   },  //E5-06  0                                                            
{  (Uint16)(-1000),            1000,                         1000,               0x2131,   },  //E5-07  0                                                            
{  (Uint16)(-1000),            CURVE5_INFLEX1_INDEX,         (Uint16)-1000,      0x2322,   },  //E5-08  ����5��4��                                                   
{  (Uint16)(-1000),            1000,                         (Uint16)-1000,      0x2131,   },  //E5-09  0                                                            
{  CURVE5_MIN_INDEX,           CURVE5_INFLEX2_INDEX,         (Uint16)-300,       0x2722,   },  //E5-10  0                                                            
{  (Uint16)(-1000),            1000,                         (Uint16)-300,       0x2131,   },  //E5-11  0                                                            
{  CURVE5_INFLEX1_INDEX,       CURVE5_MAX_INDEX,             300,                0x2722,   },  //E5-12  0                                                            
{  (Uint16)(-1000),            1000,                         300,                0x2131,   },  //E5-13  0                                                            
{  CURVE5_INFLEX2_INDEX,       1000,                         1000,               0x2522,   },  //E5-14  0                                                            
{  (Uint16)(-1000),            1000,                         1000,               0x2131,   },  //E5-15  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-16  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-17  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-18  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-19  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-20  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-21  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-22  0                                                            
{  0,                          0,                            0,                  0x1040,   },  //E5-23  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E5-24   AI1, AI2, AI3��Ծ                                           
{  0,                          1000,                         5,                  0x0131,   },  //E5-25  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E5-26  0                                                            
{  0,                          1000,                         5,                  0x0131,   },  //E5-27  0                                                            
{  (Uint16)(-1000),            1000,                         0,                  0x2131,   },  //E5-28  0                                                            
{  0,                          1000,                         5,                  0x0131,   },  //E5-29  0                                                            
                                                                                                                                                  
// E6����                      E6����                        E6����ֵ            E6����                                                                          
{  0,                          1,                            0,                  0x0840,   },  //E6-00  PLC������ѡ��                                                
{  0,                          11111,                        0,                  0x4940,   },  //E6-01  ������ӿ���                                                 
{  0,                          7,                            0,                  0x0840,   },  //E6-02  PLC AI3��������                                              
{  0,                          1000,                         0,                  0x0131,   },  //E6-03  FMP���                                                      
{  0,                          1000,                         0,                  0x0131,   },  //E6-04  AO1���                                                      
{  0,                          11,                           0,                  0x40C0,   },  //E6-05  ���������                                                   
{  (Uint16)(-10000),           10000,                        0,                  0x2172,   },  //E6-06  PLC��Ƶ�ʸ���                                                
{  (Uint16)(-2000),            2000,                         0,                  0x2131,   },  //E6-07  PLC��ת�ظ���                                                
{  0,                          65535,                        0,                  0x0140,   },  //E6-08  PLC���������                                                
{  0,                          65535,                        0,                  0x0140,   },  //E6-09  PLC�����ϸ���                                                
{  0,                          1,                            0,                  0x0840,   },  //E6-10  0                                                            
{  0,                          1,                            0,                  0x0840,   },  //E6-11  0                                                            
                                                                                                                                                  
// E7����                      E7����                        E7����ֵ            E7����                                                                          
{  0,                          1,                            0,                  0x0800,   },  //E7-00  ��Ե�ͨѶ����ѡ��                                           
{  0,                          1,                            0,                  0x0800,   },  //E7-01  ����ѡ��                                                     
{  0,                          3,                            0,                  0x0800,   },  //E7-02  ����������?  0:���ת��  1:����Ƶ��  2:�趨Ƶ��  3:�����ٶ�
{  0,                          1,                            0,                  0x0800,   },  //E7-03  �ӻ���������   0:ת�ظ���  1:Ƶ�ʸ���                        
{  (Uint16)(-10000),           10000,                        0,                  0x2132,   },  //E7-04  ����������ƫ                                                 
{  (Uint16)(-1000),            1000,                         100,                0x2102,   },  //E7-05  ������������                                                 
{  0,                          100,                          10,                 0x0941,   },  //E7-06  ��Ե�ͨѶ�жϼ��ʱ��                                       
{  1,                          10000,                        1,                  0x0943,   },  //E7-07  ��Ե�ͨѶ�������ݷ�������                                   
                                                                                                                                                  
// E8����                      E8����                        E8����ֵ            E8����                                                                          
{  0,                          65535,                        0,                  0x0040,   },  //E8-00  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-01  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-02  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-03  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-04  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-05  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-06  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-07  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-08  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-09  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-10  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-11  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-12  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-13  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-14  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-15  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-16  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-17  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-18  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-19  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-20  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-21  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-22  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-23  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-24  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-25  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-26  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-27  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-28  0                                                            
{  0,                          65535,                        0,                  0x0040,   },  //E8-29  0                                                            
                                                                                                                                                  
// E9����                      E9����                        E9����ֵ            E9����                                                                          
{  0,                          65535,                        0,                  0x0940,   },  //E9-00  0                                                            
                                                                                                                                                  
// EA����                      EA����                        EA����ֵ            EA����                                                                          
{  0,                          1500,                         1200,               0x010A,   },  //EA-00  0                                                            
                                                                                                                                                  
// EB����                      EB����                        EB����ֵ            EB����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //EB-00  0                                                            
                                                                                                                                                  
// EC����                      EC����                        EC����ֵ            EC����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //EC-00  0                                                            
                                                                                                                                                  
// ED����                      ED����                        ED����ֵ            ED����                                                                          
{  500,                        4000,                         2000,               0x2163,   },  //ED-00  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-01  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-02  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-03  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-04  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-05  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-06  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-07  0                                                            
{  (Uint16)(-9999),            10000,                        2000,               0x2163,   },  //ED-08  0                                                            
{  (Uint16)(-9999),            10000,                        2000,               0x2163,   },  //ED-09  0                                                            
{  (Uint16)(-9999),            10000,                        8000,               0x2163,   },  //ED-10  0                                                            
{  (Uint16)(-9999),            10000,                        8000,               0x2163,   },  //ED-11  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-12  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-13  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-14  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-15  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-16  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //ED-17  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-18  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //ED-19  0                                                            
                                                                                                                                                  
// EE����                      EE����                        EE����ֵ            EE����                                                                          
{  0,                          65535,                        1,                  0x0140,   },  //EE-00  0                                                            
                                                                                                                                                  
// EF����                      EF����                        EF����ֵ            EF����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //EF-00  0                                                            
                                                                                                                                                  
// b0����                      b0����                        b0����ֵ            b0����                                                                          
                                                                                                                               
{  2,                          2,                            2,                  0x0840,   },  //b0-00  �������ѡ��                                                 
{  1,                          10000,                        37,                 0x0941,   },  //b0-01  ��������                                                 
{  1,                          2000,                         380,                0x08E0,   },  //b0-02  ������ѹ                                                 
{  1,                          65535,                        900,                0x0952,   },  //b0-03  ��������                                                 
{  1,                          MAX_FRQ_INDEX,                5000,               0x0B4A,   },  //b0-04  ����Ƶ��                                                 
{  1,                          65535,                        1500,               0x0958,   },  //b0-05  ����ת��                                                 
   //�첽����г����                                                                                                                                
{  1,                          65535,                        1204,               0x0943,   },  //b0-06  �첽�����ӵ���                                               
{  1,                          65535,                        908,                0x0943,   },  //b0-07  �첽��ת�ӵ���                                               
{  1,                          65535,                        528,                0x0942,   },  //b0-08  �첽��©�п�                                                 
{  1,                          65535,                        1586,               0x0941,   },  //b0-09  �첽�����п�                                                 
{  1,                          RATING_CUR_INDEX,             424,                0x0B52,   },  //b0-10  �첽�����ص��� 
                                              
// ͬ������г����
{  1,                          65535,                        1204,           	 0x0943,   },  //b0-11  ͬ�������ӵ���                    F1-16
{  1,                          65535,              		     1586,               0x0942,   },  //b0-12  ͬ����D����                     F1-17
{  1,                          65535,              			 1586,          	 0x0942,   },  //b0-13  ͬ����Q����                     F1-18
//{  0,                          12   ,                  	  0,           		  0x1180,   },  //b0-14  ͬ��������衢��е�λ 20170210  
{  0,                          65535,              			 3000,           	 0x0961,   },  //b0-15  ͬ�������綯��ϵ��	0~6000.0V	  F1-20
{  0,                          60000,                 		 0,           		 0x1943,   },  //b0-16  ͬ�������ȱ����ʱ��            F1-21                                                                                                                  
{  0,                          12   ,                  		 0,           		 0x1180,   },  //b0-14  ͬ��������衢��е�λ 20170210   F1-19
                                         
{  0,                          65535,                        0,                  0x1140,   },  //b0-17  F1-11                                                            
{  0,                          65535,                        0,                  0x1140,   },  //b0-18  F1-12                                                             
{  0,                          65535,                        0,                  0x1140,   },  //b0-19  F1-13                                                             
{  0,                          65535,                        0,                  0x1140,   },  //b0-20  F1-14                                                             
{  0,                          65535,                        0,                  0x1140,   },  //b0-21  F1-15                                                            
{  0,                          65535,                        0,                  0x1140,   },  //b0-22  F1-22                                                            
{  0,                          65535,                        0,                  0x1140,   },  //b0-23  F1-23                                                            
{  0,                          65535,                        0,                  0x1140,   },  //b0-24  F1-24                                                            
{  0,                          65535,                        0,                  0x1140,   },  //b0-25  F1-25                                                            
{  0,                          65535,                        0,                  0x1140,   },  //b0-26  F1-26                                                            
   //��г                                                                                                                                          
{  0,                          12,                           0,                  0x0840,   },  //b0-27  F1-37                                                            
   //PG������                                                                                                                                      
{  0,                          4,                            0,                  0x0940,   },  //b0-28  ����������              F1-28                                                 
{  1,                          65535,                        2500,               0x0940,   },  //b0-29  ��������������          F1-27                                     
{  0,                          3599,                         0,                  0x0901,   },  //b0-30  ��������װ��            F1-31                                     
{  0,                          1,                            0,                  0x0840,   },  //b0-31  ����������/������       F1-30                                     
{  0,                          3599,                         0,                  0x0901,   },  //b0-32  UVW�ź����λ�ý�       F1-33                                     
{  0,                          1,                            0,                  0x0840,   },  //b0-33  UVW�źŷ���             F1-32                                     
{  0,                          100,                          0,                  0x08C1,   },  //b0-34  �ٶȷ���PG���߼��ʱ��  F1-36                                     
{  1,                          65535,                        1,                  0x0940,   },  //b0-35  ���伫����              F1-34                                     
{  0,                          2,                            0,                  0x0940,   },  //b0-36  �ٶȷ���PG��ѡ��, 0-QEP1,1-QEP2(��չ) F1-29                        
{  1,                          65535,                        4,                  0x1140,   },  //b0-37  UVW������               F1-35

//  b1 ����                    b1 ����                       b1 ����ֵ           b1 ��� 
{   2,                         2,                            2,                  0x0840,   },  // 0
{   1,                         65535,                        37,                 0x0941,   },  // 1
{   1,                         2000,                         380,                0x08E0,   },  // 2
{   1,                         65535,                        900,                0x0952,   },  // 3
{   1,                         MAX_FRQ_INDEX,                5000,               0x0B4A,   },  // 4
{   1,                         65535,                        1460,               0x0958,   },  // 5
{   1,                         65535,                        1204,               0x0943,   },  // 6  �첽�����ӵ���
{   1,                         65535,                        908,                0x0943,   },  // 7
{   1,                         65535,                        528,                0x0942,   },  // 8
{   1,                         65535,                        1586,               0x0941,   },  // 9
{   1,                         RATING_CUR_INDEX2,            424,                0x0B52,   },  // 10 ���ص���

                                              
{   1,                         65535,                        1204,               0x0943,   },  //b1-11  ͬ�������ӵ���                                               
{   1,                         65535,                        1586,               0x0942,   },  //b1-12  ͬ����d����                                                
{   1,                         65535,                        1586,               0x0942,   },  //b1-13  ͬ����q����  
{   0,                         12,                           0,                  0x1180,   },  //b1-14  ͬ������е��赥λ                                               
{   0,                         65535,                        3000,               0x0961,   },  //b1-15  ͬ�������綯��ϵ��                                           
{   0,                         60000,                        0,                  0x1943,   },  //b1-16  ͬ�������ȱ����ʱ��                                        

{   0,                         65535,                        0,                  0x1140,   },  // 11
{   0,                         65535,                        0,                  0x1140,   },  // 12
{   0,                         65535,                        0,                  0x1140,   },  // 13
{   0,                         65535,                        0,                  0x1140,   },  // 14
{   0,                         65535,                        0,                  0x1140,   },  // 15  
                                                                                           
{   0,                         65535,                        0,                  0x1140,   },  // 22
{   0,                         65535,                        0,                  0x1140,   },  // 23
{   0,                         65535,                        0,                  0x1140,   },  // 24
{   0,                         65535,                        0,                  0x1140,   },  // 25
{   0,                         65535,                        0,                  0x1140,   },  // 26
// ��г                                                                                    
{   0,                         12,                           0,                  0x0840,   },  // 37 ��гѡ�� 
                                                                                           
// PG������                                                                                
{   1,                         65535,                        1024,               0x0940,   },  // 27
{   0,                         4,                            0,                  0x0940,   },  // 28
{   0,                         2,                            0,                  0x0940,   },  // 29
{   0,                         1,                            0,                  0x0840,   },  // 30 ���䷽��
{   0,                         3599,                         0,                  0x0901,   },  // 31 ��������װλ�ý�
{   0,                         1,                            0,                  0x0840,   },  // 32 UVW�źŷ���
{   0,                         3599,                         0,                  0x0901,   },  // 33 UVW�ź����λ�ý�
{   1,                         65535,                        1,                  0x0940,   },  // 34 ���伫����
{   1,                         65535,                        4,                  0x1140,   },  // 35 UVW������
{   0,                         100,                          0,                  0x08C1,   },  // 36 �ٶȷ���PG���߼��ʱ��
                                                                                           
// ʸ�����Ʋ���                                                                            
{   1,                         100,                          30,                 0x00C0,   },  // 38
{   1,                         1000,                         50,                 0x0102,   },  // 39
{   0,                         VC_CHG_FRQ2_INDEX2,           500,                0x034A,   },  // 40
{   1,                         100,                          20,                 0x00C0,   },  // 41
{   1,                         1000,                         100,                0x0102,   },  // 42
{   VC_CHG_FRQ1_INDEX2,        MAX_FRQ_INDEX,                1000,               0x074A,   },  // 43
{   50,                        200,                          100,                0x00F0,   },  // 44
{   0,                         100,                          15,                 0x0143,   },  // 45
{   0,                         200,                          64,                 0x0140,   },  // 46
{   0,                         7,                            0,                  0x0040,   },  // 47
{   0,                         2000,                         1500,               0x0131,   },  // 48
{   0,                         8,                            0,                  0x1040,   },  // 49
{   0,                         2000,                         0,                  0x1131,   },  // 50
{   0,                         60000,                        2000,               0x0140,   },  // 51
{   0,                         60000,                        1300,               0x0140,   },  // 52
{   0,                         60000,                        2000,               0x0140,   },  // 53
{   0,                         60000,                        1300,               0x0140,   },  // 54
{   0,                         65535,                        0,                  0x1140,   },  // 55
{   0,                         2,                            1,                  0x0040,   },  // 56
 
{   0x0001,                    0x0032,                       0x0005,             0x00F0,   },  // 57
{   0x0001,                    0x012C,                       0x0032,             0x08F0,   },  // 58
{   0x000A,                    0x01F4,                       0x0064,             0x00F0,   },  // 59
{   0x0000,                    0x0001,                       0x0000,             0x0840,   },  // 60
{   0x0000,                    0x0032,                       0x0005,             0x0140,   },  // 61
{   0x0032,                    0x00B4,                       0x0050,             0x0130,   },  // 62
{   0x0000,                    0x0002,                       0x0000,             0x0140,   },  // 63
{   0x0000,                    0x0001,                       0x0000,             0x0140,   },  // 64
{   0x0032,                    0x01F4,                       0x0064,             0x0130,   },  // 65
{   0x0000,                    0x0001,                       0x0000,             0x0140,   },  // 66
{   0x0000,                    0x0002,                       0x0000,             0x0140,   },  // 67
{   0x0001,                    0x0064,                       0x0006,             0x0140,   },  // 68
{   0x0001,                    0x0064,                       0x0006,             0x0140,   },  // 69
{   0x0000,                    0x0001,                       0x0001,             0x0140,   },  // 70
{   0x000A,                    0x03E8,                       0x0064,             0x0140,   },  // 71
{   0x0005,                    0x00C8,                       0x0028,             0x0140,   },  // 72
{   0x0005,                    0x01F4,                       0x001E,             0x0140,   },  // 73
{   0x0000,                    0x0050,                       0x001E,             0x0140,   },  // 74
{   0x0008,                    0x000F,                       0x000F,             0x0341,   },  // 75
{   0x0000,                    0x0001,                       0x0000,             0x0140,   },  // 76
{   0x0000,                    0x03E8,                       0x00C8,             0x0142,   },  // 77
{   0x0005,                    0x2710,                       0x000A,             0x0144,   },  // 78
{   0x0000,                    0x0050,                       0x0032,             0x0140,   },  // 79
{   0x0000,                    0x0001,                       0x0000,             0x0140,   },  // 80
{   0x0000,                    0x0001,                       0x0000,             0x0140,   },  // 81
{   0x0000,                    0x0045,                       0x001E,             0x034A,   },  // 82
{   0x0001,                    0x0064,                       0x000A,             0x0140,   },  // 83
{   0x0001,                    0x03E8,                       0x0032,             0x0142,   },  // 84
{   0x0000,                    0x0001,                       0x0000,             0x0140,   },  // 85
{   0x0000,                    0x0064,                       0x0008,             0x0141,   },  // 86
{   0x001E,                    0x012C,                       0x0078,             0x0140,   },  // 87
// ���Ʒ�ʽ                                                                                    
{   0,                         2,                            0,                  0x0840,   },  // 88
{   0,                         4,                            0,                  0x0040,   },  // 89
{   0,                         300,                          10,                 0x00F1,   },  // 90
{   0,                         11,                           0,                  0x5040,   },  // 91
{   0,                         100,                          0,                  0x00C0,   },  // 92
                                                                                 
//  b2 ����                    b2 ����                       b2 ����ֵ           b2 ����                                                                     
{   2,                         2,                            2,                  0x0840,   },  // 0
{   1,                         65535,                        37,                 0x0941,   },  // 1
{   1,                         2000,                         380,                0x08E0,   },  // 2
{   1,                         65535,                        900,                0x0952,   },  // 3
{   1,                         MAX_FRQ_INDEX,                5000,               0x0B4A,   },  // 4
{   1,                         65535,                        1460,               0x0958,   },  // 5
{   1,                         65535,                        1204,               0x0943,   },  // 6  �첽�����ӵ���
{   1,                         65535,                        908,                0x0943,   },  // 7
{   1,                         65535,                        528,                0x0942,   },  // 8
{   1,                         65535,                        1586,               0x0941,   },  // 9
{   1,                         RATING_CUR_INDEX2,            424,                0x0B52,   },  // 10 ���ص���                                             
{   1,                         65535,                        1204,               0x0943,   },  //b2-11  ͬ�������ӵ���                                               
{   1,                         65535,                        1586,               0x0942,   },  //b2-12  ͬ����d����                                                
{   1,                         65535,                        1586,               0x0942,   },  //b2-13  ͬ����q����  
//{   0,                         12,                           0,                  0x1180,   },  //b2-14  ͬ������е��赥λ  20170210                                             
{   0,                         65535,                        3000,               0x0961,   },  //b2-15  ͬ�������綯��ϵ��                                           
{   0,                         60000,                        0,                  0x1943,   },  //b2-16  ͬ�������ȱ����ʱ��  
{   0,                         12,                           0,                  0x1180,   },  //b2-14  ͬ������е��赥λ    20170210                                   
{   0,                         65535,                        0,                  0x1140,   },  // 11
{   0,                         65535,                        0,                  0x1140,   },  // 12
{   0,                         65535,                        0,                  0x1140,   },  // 13
{   0,                         65535,                        0,                  0x1140,   },  // 14
{   0,                         65535,                        0,                  0x1140,   },  // 15                                                                                             
{   0,                         65535,                        0,                  0x1140,   },  // 22
{   0,                         65535,                        0,                  0x1140,   },  // 23
{   0,                         65535,                        0,                  0x1140,   },  // 24
{   0,                         65535,                        0,                  0x1140,   },  // 25
{   0,                         65535,                        0,                  0x1140,   },  // 26
// ��г                                                                                                                                                                               
{   0,                         12,                           0,                  0x0840,   },  // 37 ��гѡ�� 
                                                                                           
// PG������                                                                                
{   1,                         65535,                        1024,               0x0940,   },  // 27
{   0,                         4,                            0,                  0x0940,   },  // 28
{   0,                         2,                            0,                  0x0940,   },  // 29
{   0,                         1,                            0,                  0x0840,   },  // 30 ���䷽��
{   0,                         3599,                         0,                  0x0901,   },  // 31 ��������װλ�ý�
{   0,                         1,                            0,                  0x0840,   },  // 32 UVW�źŷ���
{   0,                         3599,                         0,                  0x0901,   },  // 33 UVW�ź����λ�ý�
{   1,                         65535,                        1,                  0x0940,   },  // 34 ���伫����
{   1,                         65535,                        4,                  0x1140,   },  // 35 UVW������
{   0,                         100,                          0,                  0x08C1,   },  // 36 �ٶȷ���PG���߼��ʱ��
                               
// ʸ�����Ʋ���                
{   1,                         100,                          30,                 0x00C0,   },  // 38
{   1,                         1000,                         50,                 0x0102,   },  // 39
{   0,                         VC_CHG_FRQ2_INDEX2,           500,                0x034A,   },  // 40
{   1,                         100,                          20,                 0x00C0,   },  // 41
{   1,                         1000,                         100,                0x0102,   },  // 42
{   VC_CHG_FRQ1_INDEX2,        MAX_FRQ_INDEX,                1000,               0x074A,   },  // 43
{   50,                        200,                          100,                0x00F0,   },  // 44
{   0,                         100,                          15,                 0x0143,   },  // 45
{   0,                         200,                          64,                 0x0140,   },  // 46
{   0,                         7,                            0,                  0x0040,   },  // 47
{   0,                         2000,                         1500,               0x0131,   },  // 48
{   0,                         8,                            0,                  0x1040,   },  // 49
{   0,                         2000,                         0,                  0x1131,   },  // 50
{   0,                         60000,                        2000,               0x0140,   },  // 51
{   0,                         60000,                        1300,               0x0140,   },  // 52
{   0,                         60000,                        2000,               0x0140,   },  // 53
{   0,                         60000,                        1300,               0x0140,   },  // 54
{   0,                         65535,                        0,                  0x1140,   },  // 55
{   0,                         2,                            1,                  0x0040,   },  // 56
 
{   0x0001,                    0x0032,                       0x0005,             0x00F0, },
{   0x0001,                    0x012C,                       0x0032,             0x08F0, },
{   0x000A,                    0x01F4,                       0x0064,             0x00F0, },
{   0x0000,                    0x0001,                       0x0000,             0x0840, },
{   0x0000,                    0x0032,                       0x0005,             0x0140, },
{   0x0032,                    0x00B4,                       0x0050,             0x0130, },
{   0x0000,                    0x0002,                       0x0000,             0x0140, },
{   0x0000,                    0x0001,                       0x0000,             0x0140, },
{   0x0032,                    0x01F4,                       0x0064,             0x0130, },
{   0x0000,                    0x0001,                       0x0000,             0x0140, },
{   0x0000,                    0x0002,                       0x0000,             0x0140, },
{   0x0001,                    0x0064,                       0x0006,             0x0140, },
{   0x0001,                    0x0064,                       0x0006,             0x0140, },
{   0x0000,                    0x0001,                       0x0001,             0x0140, },
{   0x000A,                    0x03E8,                       0x0064,             0x0140, },
{   0x0005,                    0x00C8,                       0x0028,             0x0140, },
{   0x0005,                    0x01F4,                       0x001E,             0x0140, },
{   0x0000,                    0x0050,                       0x001E,             0x0140, },
{   0x0008,                    0x000F,                       0x000F,             0x0341, },
{   0x0000,                    0x0001,                       0x0000,             0x0140, },
{   0x0000,                    0x03E8,                       0x00C8,             0x0142, },
{   0x0005,                    0x2710,                       0x000A,             0x0144, },
{   0x0000,                    0x0050,                       0x0032,             0x0140, },
{   0x0000,                    0x0001,                       0x0000,             0x0140, },
{   0x0000,                    0x0001,                       0x0000,             0x0140, },
{   0x0000,                    0x0045,                       0x001E,             0x034A, },
{   0x0001,                    0x0064,                       0x000A,             0x0140, },
{   0x0001,                    0x03E8,                       0x0032,             0x0142, },
{   0x0000,                    0x0001,                       0x0000,             0x0140, },
{   0x0000,                    0x0064,                       0x0008,             0x0141, },
{   0x001E,                    0x012C,                       0x0078,             0x0140, },  
// ���Ʒ�ʽ
{   0,                         2,                            0,                  0x0840,        },  
{   0,                         4,                            0,                  0x0040,        },  
{   0,                         300,                          10,                 0x00F1,        },  
{   0,                         11,                           0,                  0x5040,        },  
{   0,                         100,                          0,                  0x00C0,        },  
                                                                                 
// b3����                      b3����                        b3����ֵ            b3����                                                                          
{   2,                         2,                            2,                  0x0840,         },  // 0
{   1,                         65535,                        37,                 0x0941,         },  // 1
{   1,                         2000,                         380,                0x08E0,         },  // 2
{   1,                         65535,                        900,                0x0952,         },  // 3
{   1,                         MAX_FRQ_INDEX,                5000,               0x0B4A,         },  // 4
{   1,                         65535,                        1460,               0x0958,         },  // 5
{   1,                         65535,                        1204,               0x0943,         },  // 6  �첽�����ӵ���
{   1,                         65535,                        908,                0x0943,         },  // 7
{   1,                         65535,                        528,                0x0942,         },  // 8
{   1,                         65535,                        1586,               0x0941,         },  // 9
{   1,                         RATING_CUR_INDEX2,            424,                0x0B52,         },  // 10 ���ص���

                                              
{   1,                         65535,                        1204,               0x0943,         },  //b3-11  ͬ�������ӵ���                                               
{   1,                         65535,                        1586,               0x0942,         },  //b3-12  ͬ����d���                                                
{   1,                         65535,                        1586,               0x0942,         },  //b3-13  ͬ����q����  
{   0,                         12,                           0,                  0x1180,         },  //b3-14  ͬ������е��赥λ                                               
{   0,                         65535,                        3000,               0x0961,         },  //b3-15  ͬ�������綯��ϵ��                                           
{   0,                         60000,                        0,                  0x1943,         },  //b3-16  ͬ�������ȱ����ʱ��                                        

{   0,                         65535,                        0,                  0x1140,         },  // 11
{   0,                         65535,                        0,                  0x1140,         },  // 12
{   0,                         65535,                        0,                  0x1140,         },  // 13
{   0,                         65535,                        0,                  0x1140,         },  // 14
{   0,                         65535,                        0,                  0x1140,         },  // 15  
                                                                                 
{   0,                         65535,                        0,                  0x1140,         },  // 22
{   0,                         65535,                        0,                  0x1140,         },  // 23
{   0,                         65535,                        0,                  0x1140,         },  // 24
{   0,                         65535,                        0,                  0x1140,         },  // 25
{   0,                         65535,                        0,                  0x1140,         },  // 26
// ��г                                                                          
{   0,                         12,                           0,                  0x0840,         },  // 37 ��гѡ�� 
                                                                                 
// PG������                                                                      
{   1,                         65535,                        1024,               0x0940,         },  // 27
{   0,                         4,                            0,                  0x0940,         },  // 28
{   0,                         2,                            0,                  0x0940,         },  // 29
{   0,                         1,                            0,                  0x0840,         },  // 30 ���䷽��
{   0,                         3599,                         0,                  0x0901,         },  // 31 ��������װλ�ý�
{   0,                         1,                            0,                  0x0840,         },  // 32 UVW�źŷ���
{   0,                         3599,                         0,                  0x0901,         },  // 33 UVW�ź����λ�ý�
{   1,                         65535,                        1,                  0x0940,         },  // 34 ���伫����
{   1,                         65535,                        4,                  0x1140,         },  // 35 UVW������
{   0,                         100,                          0,                  0x08C1,         },  // 36 �ٶȷ���PG���߼��ʱ��
                                                                                                 
// ʸ�����Ʋ���                                                                                  
{   1,                         100,                          30,                 0x00C0,         },  // 38
{   1,                         1000,                         50,                 0x0102,         },  // 39
{   0,                         VC_CHG_FRQ2_INDEX2,           500,                0x034A,         },  // 40
{   1,                         100,                          20,                 0x00C0,         },  // 41
{   1,                         1000,                         100,                0x0102,         },  // 42
{   VC_CHG_FRQ1_INDEX2,        MAX_FRQ_INDEX,                1000,               0x074A,         },  // 43
{   50,                        200,                          100,                0x00F0,         },  // 44
{   0,                         100,                          15,                 0x0143,         },  // 45
{   0,                         200,                          64,                 0x0140,         },  // 46
{   0,                         7,                            0,                  0x0040,         },  // 47
{   0,                         2000,                         1500,               0x0131,         },  // 48
{   0,                         8,                            0,                  0x1040,         },  // 49
{   0,                         2000,                         0,                  0x1131,         },  // 50
{   0,                         60000,                        2000,               0x0140,         },  // 51
{   0,                         60000,                        1300,               0x0140,         },  // 52
{   0,                         60000,                        2000,               0x0140,         },  // 53
{   0,                         60000,                        1300,               0x0140,         },  // 54
{   0,                         65535,                        0,                  0x1140,         },  // 55
{   0,                         2,                            1,                  0x0040,         },  // 56
 
{   0x0001,                    0x0032,                       0x0005,             0x00F0,         },
{   0x0001,                    0x012C,                       0x0032,             0x08F0,         },
{   0x000A,                    0x01F4,                       0x0064,             0x00F0,         },
{   0x0000,                    0x0001,                       0x0000,             0x0840,         },
{   0x0000,                    0x0032,                       0x0005,             0x0140,         },
{   0x0032,                    0x00B4,                       0x0050,             0x0130,         },
{   0x0000,                    0x0002,                       0x0000,             0x0140,         },
{   0x0000,                    0x0001,                       0x0000,             0x0140,         },
{   0x0032,                    0x01F4,                       0x0064,             0x0130,         },
{   0x0000,                    0x0001,                       0x0000,             0x0140,         },
{   0x0000,                    0x0002,                       0x0000,             0x0140,         },
{   0x0001,                    0x0064,                       0x0006,             0x0140,         },
{   0x0001,                    0x0064,                       0x0006,             0x0140,         },
{   0x0000,                    0x0001,                       0x0001,             0x0140,         },
{   0x000A,                    0x03E8,                       0x0064,             0x0140,         },
{   0x0005,                    0x00C8,                       0x0028,             0x0140,         },
{   0x0005,                    0x01F4,                       0x001E,             0x0140,         },
{   0x0000,                    0x0050,                       0x001E,             0x0140,         },
{   0x0008,                    0x000F,                       0x000F,             0x0341,         },
{   0x0000,                    0x0001,                       0x0000,             0x0140,         },
{   0x0000,                    0x03E8,                       0x00C8,             0x0142,         },
{   0x0005,                    0x2710,                       0x000A,             0x0144,         },
{   0x0000,                    0x0050,                       0x0032,             0x0140,         },
{   0x0000,                    0x0001,                       0x0000,             0x0140,         },
{   0x0000,                    0x0001,                       0x0000,             0x0140,         },
{   0x0000,                    0x0045,                       0x001E,             0x034A,         },
{   0x0001,                    0x0064,                       0x000A,             0x0140,         },
{   0x0001,                    0x03E8,                       0x0032,             0x0142,         },
{   0x0000,                    0x0001,                       0x0000,             0x0140,         },
{   0x0000,                    0x0064,                       0x0008,             0x0141,         },
{   0x001E,                    0x012C,                       0x0078,             0x0140,         },  

// ���Ʒ�ʽ
{   0,                         2,                            0,                  0x0840,         },  
{   0,                         4,                            0,                  0x0040,         },  
{   0,                         300,                          10,                 0x00F1,         },  
{   0,                         11,                           0,                  0x5040,         },  
{   0,                         100,                          0,                  0x00C0,         },  
 
// b4����                      b4����                        b4����ֵ            b4����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //b4-00  0                                                            
                                                                                                                                                  
// b5����                      y3����                        y3����ֵ            y3����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //b5-00  0                                                            
                                                                                                                                                  
// b6����                      b6����                        b6����ֵ            b6����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //b6-00  0                                                            
                                                                                                                                                  
// b7����                      b7����                        b7����ֵ            b7����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //b7-00  0                                                            
                                                                                                                                                  
// b8����                      b8����                        b8����ֵ            b8����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //b8-00  0                                                            
                                                                                                                                                  
// b9����                      b9����                        b9����ֵ            b9����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //b9-00  0                                                            
                                                                                                                                                  
// bA����                      bA����                        bA����ֵ            bA����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //bA-00  0                                                            
                                                                                                                                                  
// bB����                      bB����                        bB����ֵ            bB����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //bB-00  0                                                            
                                                                                                                                                  
// bC����                      bC����                        bC����ֵ            bC����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //bC-00  0                                                            
                                                                                                                                                  
// bD����                      bD����                        bD����ֵ            bD����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //bD-00  0                                                            
                                                                                                                                                  
// bE����                      bE����                        bE����ֵ            bE����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //bE-00  0                                                            
                                                                                                                                                  
// bF����                      bF����                        bF����ֵ            bF����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //bF-00  0                                                            
                                                                                                                                                  
// y0����                      y0����                        y0����ֵ            y0����                                                                          
{  0,                          PARA_INIT_MODE_MAX,           0,                  0x0840,   },  //Y0-00  ���ʼ��                                                   
{  0,                          65535,                        0,                  0x0140,   },  //Y0-01  �û�����                                                     
{  0,                          11111,                        11111,              0x4040,   },  //Y0-02  ���ܲ���ģʽ����                                             
{  0,                          11,                           0,                  0x4040,   },  //Y0-03  ���Ի�����ģʽѡ��                                           
{  0,                          1,                            0,                  0x0140,   },  //Y0-04  ֻ���û�����                                                 
{  0,                          65535,                        0,                  0x0140,   },  //Y0-05  ����                                                         
                                                                                                                                                  
// y1����                      y1����                        y1����ֵ          y1����                                                                          
{  0,                          99,                           0,                  0x1080,   },  //Y1-00  ��һ�ι�������                                               
{  0,                          99,                           0,                  0x1080,   },  //Y1-01  �ڶ��ι�������                                               
{  0,                          99,                           0,                  0x1080,   },  //Y1-02  ������(���һ��)������?                                    
{  0,                          65535,                        0,                  0x114A,   },  //Y1-03  ������(���һ�?����                                   
{  0,                          65535,                        0,                  0x1152,   },  //Y1-04  ������(���һ��)����ʱ����                                   
{  0,                          65535,                        0,                  0x1161,   },  //Y1-05  ������(���һ��)����ʱĸ�ߵ�ѹ                               
{  0,                          9999,                         0,                  0x1100,   },  //Y1-06  ������(���һ��)����ʱ�������״̬                           
{  0,                          9999,                         0,                  0x1100,   },  //Y1-07  ������(���һ��)����ʱ�������״̬                           
{  0,                          65535,                        0,                  0x1100,   },  //Y1-08  ������(���һ��)����ʱ��Ƶ��״̬                             
{  0,                          65535,                        0,                  0x1100,   },  //Y1-09  ������(���һ��)����ʱʱ�䣨�ӱ����ϵ翪ʼ��ʱ��             
{  0,                          65535,                        0,                  0x1101,   },  //Y1-10  ������(���һ��)����ʱʱ�䣨������ʱ��ʼ��ʱ��               
{  0,                          65535,                        0,                  0x1040,   },  //Y1-11  0                                                            
{  0,                          65535,                        0,                  0x1040,   },  //Y1-12  0                                                            
{  0,                          65535,                        0,                  0x114A,   },  //Y1-13  �ڶ��ι����ֳ�                                               
{  0,                          65535,                        0,                  0x1152,   },  //Y1-14  0                                                            
{  0,                          65535,                        0,                  0x1161,   },  //Y1-15  0                                                            
{  0,                          9999,                         0,                  0x1100,   },  //Y1-16  0                                                            
{  0,                          9999,                         0,                  0x1100,   },  //Y1-17  0                                                            
{  0,                          65535,                        0,                  0x1100,   },  //Y1-18  0                                                            
{  0,                          65535,                        0,                  0x1100,   },  //Y1-19  0                                                            
{  0,                          65535,                        0,                  0x1101,   },  //Y1-20  0                                                            
{  0,                          65535,                        0,                  0x1040,   },  //Y1-21  0                                                            
{  0,                          65535,                        0,                  0x1040,   },  //Y1-22                                                               
{  0,                          65535,                        0,                  0x114A,   },  //Y1-23  ��һ�ι����ֳ�                                               
{  0,                          65535,                        0,                  0x1152,   },  //Y1-24  0                                                            
{  0,                          65535,                        0,                  0x1161,   },  //Y1-25  0                                                            
{  0,                          9999,                         0,                  0x1100,   },  //Y1-26  0                                                            
{  0,                          9999,                         0,                  0x1100,   },  //Y1-27  0                                                            
{  0,                          65535,                        0,                  0x1100,   },  //Y1-28  0                                                            
{  0,                          65535,                        0,                  0x1100,   },  //Y1-29  0                                                            
{  0,                          65535,                        0,                  0x1101,   },  //Y1-30  0                                                            
                                                                                                                                                  
// y2����                      y2����                        y2����ֵ            y2����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //Y2-00  0                                                            
                                                                                                                                                  
// y3����                      y3����                        y3����ֵ            y3����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //Y3-00  0                                                            
                                                                                                                                                  
// y4����                      y4����                        y4����ֵ            y4����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //Y4-00  0                                                            
                                                                                                                                                  
// y5����                      y5����                        y5����ֵ            y5����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //Y5-00  0                                                            
                                                                                                                                                  
// y6����                      y6����                        y6����ֵ            y6����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //Y6-00  0                                                            
                                                                                                                                                  
// y7����                      y7����                        y7����ֵ            y7����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //Y7-00  0                                                            
                                                                                                                                                  
// L0����                      L0����                        L0����ֵ            L0����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //L0-00  ��������                                                     
{  1,                          537,                          11,                 0x0880,   },  //L0-01  ��Ƶ������                                               
{  1,                          2,                            1,                  0x0840,   },  //L0-02  G/P�ͺ�                                                      
{  0,                          65535,                        37,                 0x1141,   },  //L0-03  ��Ƶ������                                                   
{  0,                          4,                            4,                  0x0840,   },  //L0-04  �¶�����                                                     
{  900,                        1100,                         1000,               0x0931,   },  //L0-05  UV���������������ƫ��                                       
{  0,                          65535,                        0,                  0x1902,   },  //L0-06  ����                                                         
{  0,                          65535,                        0,                  0x1902,   },  //L0-07  �������汾��                                               
{  350,                        2000,                         1000,               0x0131,   },  //L0-08  ��ѹУ��ϵ��                                                 
{  350,                        2000,                         1000,               0x0131,   },  //L0-09  ����У��ϵ��                                                 
{  0,                          3040,                         0,                  0x0140,   },  //L0-10  ���ܵ��Թ�������ʾ����                                       
{  0,                          0x1111,                       0,                  0x8840,   },  //L0-11  AIAOУ����������ʾ                                           
{  0x0000,                     0xBFFF,                       0x8836,             0x8140,   },  //L0-12  �ڴ��ַ�鿴                                                 
                                                                                                                                                             
// L1����                      L1����                        L1����ֵ            L1����                                                                          
{  500,                        4000,                         2000,               0x2163,   },  //L1-00  0                                                            
{  500,                        4000,                         1929,               0x2163,   },  //L1-01  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //L1-02  0                                                            
{  6000,                       9999,                         7844,               0x2163,   },  //L1-03  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //L1-04  0                                                            
{  500,                        4000,                         1928,               0x2163,   },  //L1-05  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //L1-06  0                                                            
{  6000,                       9999,                         7853,               0x2163,   },  //L1-07  0                                                            
{  (Uint16)(-9999),            10000,                        2000,               0x2163,   },  //L1-08  0                                                            
{  (Uint16)(-9999),            10000,                        1928,               0x2163,   },  //L1-09  0                                                            
{  (Uint16)(-9999),            10000,                        8000,               0x2163,   },  //L1-10  0                                                            
{  (Uint16)(-9999),            10000,                        7857,               0x2163,   },  //L1-11  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //L1-12  0                                                            
{  500,                        4000,                         1980,               0x2163,   },  //L1-13  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //L1-14  0                                                            
{  6000,                       9999,                         7880,               0x2163,   },  //L1-15  0                                                            
{  500,                        4000,                         2000,               0x2163,   },  //L1-16  0                                                            
{  500,                        4000,                         1980,               0x2163,   },  //L1-17  0                                                            
{  6000,                       9999,                         8000,               0x2163,   },  //L1-18  0                                                            
{  6000,                       9999,                         7880,               0x2163,   },  //L1-19  0                                                            
                                                                                                                                                  
// L2����                      L2����                        L2����ֵ            L2����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //L2-00  0                                                            
                                                                                                                                                  
// L3����                      L3����                        L3����ֵ            L3����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //L3-00  0                                                            
                                                                                                                                                  
// L4����                      L4����                        L4����ֵ            L4����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //L4-00  0                                                            
                                                                                                                                                  
// L5 ����                     L5 ����                       L5 ����ֵ           L5 ����                                                                       
{  0,                          65535,                        0,                  0x0140,   },  //L5-00  0                                                            
                                                                                                                                                  
// L6����                      L6����                        L6����ֵ            L6����                                                                          
{  0,                          1,                            0,                  0x0140,   },  //L6-00  0                                                            
                                                                                                                                                  
// L7����                      L7����                        L7����ֵ            L7����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //L7-00  0                                                            
                                                                                                                                                  
// L8����                      L8����                        L8����ֵ            L8����                                                                          
{  0,                          65535,                        0,                  0x0140,   },  //L8-00  0                                                            
                                                                                                                                                  
// L9����                      L9����                        L9����ֵ                                                                           
{  0x0000,                     0x0001,                       0x0000,             0x0140,   },  //L9-00 
{  0x0000,   			       0x0001,   			         0x0000,   		     0x0140,   },  //L9-01
{  0x0000,   			       0x0001,   			         0x0000,   		     0x0170,   },  //L9-02
{  0x0064,   			       0x0078,   			         0x006E,   		     0x0140,   },  //L9-03
{  0x0000,   			       0x0001,   			         0x0000,   		     0x0140,   },  //L9-04
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-05
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x00F1,   },  //L9-06
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0B4A,   },  //L9-07
{  0x0000,   			       0x0064,   			         0x000A,   		     0x0103,   },  //L9-08
{  0x0000,   			       0x0001,   			         0x0000,   		     0x0140,   },  //L9-09
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0103,   },  //L9-10
{  0x0001,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-11
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-12
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-13
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-14
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-15
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-16
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-17
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-18
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-19
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-20
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-21
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-22
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-23
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-24
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-25
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-26
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-27
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-28
{  0x0000,   			       0xFFFF,   			         0x0000,   		     0x0140,   },  //L9-29 
   

};



// fChk��remember��Ӧ��EEPROM index  
// �밴�չ�����union��˳������
const Uint16 fcNoAttri2Eeprom[CHK_NUM + REM_NUM] =                                               
{
// ����
    0 ,     // rsvd4All
    1 ,     // fChk-EEPROM��ַ
    2 ,     // 
    3 ,     // AIAO_CHK
    
// remember-EEPROM��ַ
    16  ,   // 0                                             
    17  ,   // 1                                           
    18  ,   // 2                                           
    19  ,   // 3                                           
    20  ,   // 4  
    21  ,   // 5                                            
    22  ,   // 6                                           
    23  ,   // 7                                           
    24  ,   // 8                                           
    25  ,   // 9  
    26  ,   // 10                                           
    27  ,   // 11                                          
    28  ,   // 12                                          
    29  ,   // 13                                          
    30  ,   // 14                                          
    31  ,   // 15 
    32  ,   // 16 
    33  ,   // 17 
    34  ,   // 18 
    35  ,   // 19 
    36  ,   // 20 
    37  ,   // 21 
    38  ,   // 22 
    39  ,   // 23 
    40  ,   // 24 
    41  ,   // 25
    42  ,   // 26
    43  ,   // 27
    44  ,   // 28
    45  ,   // 29
    46  ,   // 30
    47  ,   // 31
    48  ,   // 32
    49  ,   // 33
    50  ,   // 34
    51  ,   // 35
    52  ,   // 36
    53  ,   // 37
    54  ,   // 38
    55  ,   // 39
    56  ,   // 40
    57  ,   // 41
    58  ,   // 42
    59  ,   // 43
    60  ,   // 44
    61  ,   // 45
    62  ,   // 46
    63  ,   // 47

};  



