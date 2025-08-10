/*****************************************************************
 *
 * ¹¦ÄÜÂë¶¨ÒåµÄÍ·ÎÄ¼þ
 * 
 * Time-stamp: <2008-08-20 08:57:13  Shisheng.Zhi, 0354>
 * 
 *
 *
 *
 *****************************************************************/

#ifndef __F_FUNCCODE_H__
#define __F_FUNCCODE_H__


#if defined(DSP2803X)         // 2803x»¹ÊÇ2808Æ½Ì¨
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

 

// Çë²»ÒªËæÒâÐÞ¸ÄÕâÁ½¸öÖµ£¬·ñÔòEEPROMÖÐµÄÖµ»áÈ«²¿(°üÀ¨P0×é¡¢P1×é¡¢¸÷ÖÖ¼ÇÂ¼)»Ö¸´³ö³§Öµ¡£
#define EEPROM_CHECK1       (0x0022)                        // EEPROMÐ£Ñé×Ö1
#define EEPROM_CHECK2       (0xFFFF - EEPROM_CHECK1)        // EEPROMÐ£Ñé×Ö2




#define USER_MENU_GROUP     FUNCCODE_GROUP_E4

// »ñµÃ¹¦ÄÜÂëcodeÔÚÈ«²¿¹¦ÄÜÂëÖÐµÄindex£¬¼´¹¦ÄÜÂëÔÚÊý×éfuncCode.all[]ÖÐµÄÏÂ±ê¡£
// ¸ù¾Ý FUNCCODE_ALL µÄ¶¨Òå£¬Ò»¸ö¹¦ÄÜÂëµÄ±íÊ¾ÓÐ4ÖÖ°ì·¨£¬¶ÔÓ¦µÄ»ñµÃindexµÄ°ì·¨£º
// 1. funcCode.all[i]     ---- i
// 2. funcCode.f5[7]      ---- GetCodeIndex(funcCode.group.f5[7])
// 3. group, grade        ---- GetGradeIndex(group, grade)
// 4. funcCode.code.maxFrq---- GetCodeIndex(funcCode.code.maxFrq)
#define GetCodeIndex(code)    ((Uint16)((&(code)) - (&(funcCode.all[0]))))
#define GetGradeIndex(group, grade)  (funcCodeGradeSum[group] + (grade))


//=====================================================================
// ¹¦ÄÜÂëÊôÐÔÎ»¶¨Òå
//
// µ¥Î»
#define ATTRIBUTE_UNIT_HZ_BIT           0
#define ATTRIBUTE_UNIT_A_BIT            1
#define ATTRIBUTE_UNIT_V_BIT            2

// ¶ÁÐ´ÊôÐÔ
#define ATTRIBUTE_READ_AND_WRITE        0   // (ÈÎºÎÊ±ºò)¿ÉÐ´
#define ATTRIBUTE_READ_ONLY_WHEN_RUN    1   // ÔËÐÐÊ±Ö»¶Á
#define ATTRIBUTE_READ_ONLY_ANYTIME     2   // Ö»¶Á

// ¶à¹¦ÄÜÂë×éºÏÊôÐÔ
#define ATTRIBUTE_MULTI_LIMIT_SINGLE    0   // µ¥¶ÀµÄ¹¦ÄÜÂë
#define ATTRIBUTE_MULTI_LIMIT_DEC       1   // ¶à¸ö¹¦ÄÜÂë£¬Ê®½øÖÆ
#define ATTRIBUTE_MULTI_LIMIT_HEX       2   // ¶à¸ö¹¦ÄÜÂë£¬Ê®Áù½øÖÆ

struct  FUNC_ATTRIBUTE_BITS
{                           // bits   description
    Uint16 point:3;         // 2:0    radix point,Ð¡Êýµã¡£
                            //        0-ÎÞÐ¡Êýµã£¬1-1Î»Ð¡Êý£¬...£¬4-4Î»Ð¡Êý
                            //        (0.0000-100,00.000-011,000.00-010,0000.0-001,00000-000)
    Uint16 unit:3;          // 5:3    unit,µ¥Î»
                            //        1-hz, 2-A, 3-RPM, 4-V, 6-%; 001-Hz, 010-A, 100-V
    Uint16 displayBits:3;   // 8:6    5¸öÊýÂë¹ÜÒªÏÔÊ¾µÄÎ»Êý¡£0-ÏÔÊ¾0Î»£¬1-ÏÔÊ¾1Î»£¬...£¬5-ÏÔÊ¾5Î»
    Uint16 upperLimit:1;    // 9      1-²ÎÊýÓÉÉÏÏÞÏà¹Ø¹¦ÄÜÂëÏÞÖÆ£¬0-Ö±½ÓÓÉÉÏÏÞÏÞÖÆ
    Uint16 lowerLimit:1;    // 10     1-²ÎÊýÓÉÏÂÏÞÏà¹Ø¹¦ÄÜÂëÏÞÖÆ£¬0-Ö±½ÓÓÉÏÂÏÞÏÞÖÆ
    Uint16 writable:2;      // 12:11  ²ÎÊý¶ÁÐ´ÌØÐÔ£¬00-¿ÉÒÔ¶ÁÐ´, 01-ÔËÐÐÖÐÖ»¶Á£¬10-²ÎÊýÖ»¶Á
    Uint16 signal:1;        // 13     ·ûºÅ£¬unsignal-0; signal-1
    Uint16 multiLimit:2;    // 15:14  ¸Ã¹¦ÄÜÂëÎª¶à¸ö¹¦ÄÜÂëµÄ×éºÏ. 
                            //        00-µ¥¶À¹¦ÄÜÂë(·Ç×éºÏ); 
                            //        01-Ê®½øÖÆ,  ¶à¸ö¹¦ÄÜÂëµÄ×éºÏ; 
                            //        10-Ê®Áù½øÖÆ,¶à¸ö¹¦ÄÜÂëµÄ×éºÏ; 
};

union FUNC_ATTRIBUTE
{
   Uint16                      all;
   struct FUNC_ATTRIBUTE_BITS  bit;
};
//=====================================================================



//=====================================================================
// ¹¦ÄÜÂëÊôÐÔ±í£ºÉÏÏÞ¡¢ÏÂÏÞ¡¢ÊôÐÔ
// ¹¦ÄÜÂëµÄ³ö³§Öµ£¬°üÀ¨EEPROM_CHECK¡¢µôµç¼ÇÒä£¬µ«²»°üÀ¨ÏÔÊ¾×é
typedef struct FUNCCODE_ATTRIBUTE_STRUCT
{
    Uint16                  lower;          // ÏÂÏÞ
    Uint16                  upper;          // ÉÏÏÞ
    Uint16                  init;           // ³ö³§Öµ
    union FUNC_ATTRIBUTE    attribute;      // ÊôÐÔ

//    Uint16                  eepromIndex;    // ¶ÔÓ¦EEPROM´æ´¢µÄindex
} FUNCCODE_ATTRIBUTE;

extern const FUNCCODE_ATTRIBUTE funcCodeAttribute[];
//=====================================================================



//=====================================================================
// ¹¦ÄÜÂëµÄcodeµÄÒ»Ð©Êý¾Ý½á¹¹¶¨Òå
struct PLC_STRUCT
{
    Uint16 runTime;         // PLCµÚ_¶ÎÔËÐÐÊ±¼ä
    Uint16 accDecTimeSet;   // PLCµÚ_¶Î¼Ó¼õËÙÊ±¼äÑ¡Ôñ
};
//=================================


//=================================
struct AI_SET_CURVE  // AIÉè¶¨ÇúÏß
{
    Uint16 minIn;       // ÇúÏß×îÐ¡ÊäÈë(µçÑ¹Öµ)
    Uint16 minInSet;    // ÇúÏß×îÐ¡ÊäÈë¶ÔÓ¦Éè¶¨(°Ù·Ö±È)
    Uint16 maxIn;       // ÇúÏß×î´óÊäÈë(µçÑ¹Öµ)
    Uint16 maxInSet;    // ÇúÏß×î´óÊäÈë¶ÔÓ¦Éè¶¨(°Ù·Ö±È)
};
//=================================


//=================================
struct AI_JUMP
{
    Uint16 point;   // Éè¶¨ÌøÔ¾µã
    Uint16 arrange; // Éè¶¨ÌøÔ¾·ù¶È
};
//=================================


//=================================
struct ANALOG_CALIBRATE_CURVE  // Ä£ÄâÁ¿Ð£ÕýÇúÏß£¬AIAO
{
    Uint16 before1;     // Ð£ÕýÇ°µçÑ¹1
    Uint16 after1;      // Ð£ÕýºóµçÑ¹1
    
    Uint16 before2;     // Ð£ÕýÇ°µçÑ¹2
    Uint16 after2;      // Ð£ÕýºóµçÑ¹2
};
//=================================


//=================================
typedef struct AO_PARA_STRUCT
{
    Uint16 offset;          // AOÁãÆ«ÏµÊý
    Uint16 gain;            // AOÔöÒæ
} AO_PARA;
//=================================


//=================================
struct FC_GROUP_HIDE_STRUCT //LLH 20130129
{
    Uint16 password;    // ¹¦ÄÜÂë×éÒþ²ØÌØÐÔÃÜÂë
    
    Uint16 f;           // F×éÒþ²ØÌØÐÔ£¨F0-FF£©
    Uint16 e;           // E×éÒþ²ØÌØÐÔ£¨E0-EF£©
    Uint16 b;           // B×éÒþ²ØÌØÐÔ£¨B0-BF£©
    Uint16 y;           // Y×éÒþ²ØÌØÐÔ£¨Y0-Y7£©
    Uint16 l;           // L×éÒþ²ØÌØÐÔ£¨L0-L9£©
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
    Uint16 errorFrq;                    // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±ÆµÂÊ
    Uint16 errorCurrent;                // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±µçÁ÷
    Uint16 errorGeneratrixVoltage;      // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Ä¸ÏßµçÑ¹
    Uint16 errorDiStatus;               // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±ÊäÈë¶Ë×Ó×´Ì¬
    Uint16 errorDoStatus;               // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Êä³ö¶Ë×Ó×´Ì¬
    
    Uint16 errorInverterStatus;         // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±±äÆµÆ÷×´Ì¬
    Uint16 errorTimeFromPowerUp;        // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Ê±¼ä£¨´Ó±¾´ÎÉÏµç¿ªÊ¼¼ÆÊ±£©
    Uint16 errorTimeFromRun;            // µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Ê±¼ä£¨´ÓÔËÐÐÊ±¿ªÊ¼¼ÆÊ±£©
};

union ERROR_SCENE
{
    Uint16 all[sizeof(struct ERROR_SCENE_STRUCT)];

    struct ERROR_SCENE_STRUCT elem;
};
//=================================


//=================================
#define APTP_NUM    32  // aptpµãÊý
typedef struct
{
    Uint16 low;     // aptpµÍÎ»£¬0-9999
    Uint16 high;    // aptp¸ßÎ»£¬0-65535
} LENGTH_SET;
// ·¶Î§: 0-655359999
//=================================


//=================================
#define MOTOR_TYPE_ACI_GENERAL  0   // ÆÕÍ¨Òì²½µç»ú
#define MOTOR_TYPE_ACI_INV      1   // ±äÆµÒì²½µç»ú
#define MOTOR_TYPE_PMSM         2   // ÓÀ´ÅÍ¬²½µç»ú
struct MOTOR_PARA_STRUCT
{
    // µç»ú»ù±¾²ÎÊý
    Uint16 motorType;               // b0-00  µç»úÀàÐÍÑ¡Ôñ
    Uint16 ratingPower;             // b0-01  µç»ú¶î¶¨¹¦ÂÊ
    Uint16 ratingVoltage;           // b0-02  µç»ú¶î¶¨µçÑ¹
    Uint16 ratingCurrent;           // b0-03  µç»ú¶î¶¨µçÁ÷
    Uint16 ratingFrq;               // b0-04  µç»ú¶î¶¨ÆµÂÊ
    Uint16 ratingSpeed;             // b0-05  µç»ú¶î¶¨×ªËÙ

    // Òì²½»úµ÷Ð³²ÎÊý
    Uint16 statorResistance;        // b0-06  Òì²½»ú¶¨×Óµç×è
    Uint16 rotorResistance;         // b0-07  Òì²½»ú×ª×Óµç×è
    Uint16 leakInductance;          // b0-08  Òì²½»úÂ©¸Ð¿¹
    Uint16 mutualInductance;        // b0-09  Òì²½»ú»¥¸Ð¿¹
	  Uint16 zeroLoadCurrent;       // b0-10  Òì²½»ú¿ÕÔØµçÁ÷
    // Í¬²½»úµ÷Ð³²ÎÊý
    Uint16 pmsmRs;                  // b1-11  Í¬²½»ú¶¨×Óµç×è
    Uint16 pmsmLd;                  // b1-12  Í¬²½»údÖáµç¸Ð
    Uint16 pmsmLq;                  // b1-13  Í¬²½»úqÖáµç¸Ð
//    Uint16 pmsmRsLdUnit;            // b1-14  Í¬²½»úµç¸Ðµç×èµ¥Î»
    Uint16 pmsmCoeff;               // b1-15  Í¬²½»ú·´µç¶¯ÊÆÏµÊý
    Uint16 pmsmCheckTime;           // b1-16  Í¬²½Êä³öÈ±Ïà¼ì²âÊ±¼ä
    Uint16 pmsmRsLdUnit;            // b1-14  Í¬²½»úµç¸Ðµç×èµ¥Î» 20170210

    Uint16 rsvdF11[5];
    Uint16 rsvdF12[5];
   
     
};

struct PG_PARA_STRUCT
{
   // PG¿¨²ÎÊý
    Uint16 pgType;                  // b0-28    ±àÂëÆ÷ÀàÐÍ
    Uint16 encoderPulse;            // b0-29    ±àÂëÆ÷Âö³åÏßÊý
    Uint16 enCoderAngle;            // b0-30    ±àÂëÆ÷°²×°½Ç
    Uint16 enCoderDir;              // b0-31    ±àÂëÆ÷ÏàÐò/Ö÷·½Ïò
    Uint16 uvwSignAngle;            // b0-32    UVWÐÅºÅÁãµãÎ»ÖÃ½Ç
    Uint16 uvwSignDir;              // b0-33    UVWÐÅºÅ·½Ïò
    Uint16 fvcPgLoseTime;           // b0-34    ËÙ¶È·´À¡PG¶ÏÏß¼ì²âÊ±¼ä    
    Uint16 enCoderPole;             // b0-35    Ðý±ä¼«¶ÔÊý
    Uint16 fvcPgSrc;                // b0-36    ËÙ¶È·´À¡PG¿¨Ñ¡Ôñ, 0-QEP1,1-QEP2(À©Õ¹)    
    Uint16 rsvdF11;                 // b0-37    UVW¼«¶ÔÊý 

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
    MOTOR_SN_1,     // µÚ1µç»ú
    MOTOR_SN_2,     // µÚ2µç»ú
    MOTOR_SN_3,     // µÚ3µç»ú
    MOTOR_SN_4      // µÚ4µç»ú
};
//=================================


//=================================


//------------------------------------------------
struct VC_PARA
{

    //---rev by mrb 2013/01/08---ÐÞ¸Ä20130228 by mrb
    Uint16 vcSpdLoopKp1;                 // F5-00  ËÙ¶È»·±ÈÀýÔöÒæ1                F200
    Uint16 vcSpdLoopTi1;                 // F5-01  ËÙ¶È»·»ý·ÖÊ±¼ä1                F201
    Uint16 vcSpdLoopChgFrq1;             // F5-02  ÇÐ»»ÆµÂÊ1                      F202
    Uint16 vcSpdLoopKp2;                 // F5-03  ËÙ¶È»·±ÈÀýÔöÒæ2                F203
    Uint16 vcSpdLoopTi2;                 // F5-04  ËÙ¶È»·»ý·ÖÊ±¼ä2                F204
    Uint16 vcSpdLoopChgFrq2;             // F5-05  ÇÐ»»ÆµÂÊ2                      F205

    Uint16 spdLoopI;                     // F5-06  ËÙ¶È»·»ý·ÖÊôÐÔ                 F217
    Uint16 spdCtrlDriveTorqueLimitSrc;   // F5-07  ËÙ¶È¿ØÖÆ(Çý¶¯)×ª¾ØÉÏÏÞÔ´       F209  
    Uint16 spdCtrlDriveTorqueLimit;      // F5-08  ËÙ¶È¿ØÖÆ(Çý¶¯)×ª¾ØÉÏÏÞÊý×ÖÉè¶¨ F210

    Uint16 vcSlipCompCoef;               // F5-09  ×ª²î²¹³¥ÏµÊý                   F206
    Uint16 vcSpdLoopFilterTime;          // F5-10  ËÙ¶È»·ÂË²¨Ê±¼ä³£Êý             F207
    Uint16 vcOverMagGain;                // F5-11  Ê¸Á¿¿ØÖÆ¹ýÀø´ÅÔöÒæ             F208
                                         
    Uint16 mAcrKp;                       // F5-12  MÖáµçÁ÷»·Kp                    F213
    Uint16 mAcrKi;                       // F5-13  MÖáµçÁ÷»·Ki                    F214
                                         
    Uint16 tAcrKp;                       // F5-14  TÖáµçÁ÷»·Kp                    F215
    Uint16 tAcrKi;                       // F5-15  TÖáµçÁ÷»·Ki                    F216
                                         
    Uint16 weakFlusMode;                 // F5-16 Í¬²½»úÈõ´ÅÄ£Ê½                  F218
    Uint16 weakFlusCoef;                 // F5-17 Í¬²½»úÈõ´ÅÏµÊý                  F219
    Uint16 weakFlusCurMax;               // F5-18 ×î´óÈõ´ÅµçÁ÷                    F220
    Uint16 weakFlusAutoCoef;             // F5-19 Èõ´Å×Ô¶¯µ÷Ð³ÏµÊý                F221
    Uint16 weakFlusIntegrMul;            // F5-20 Èõ´Å»ý·Ö±¶Êý                    F222
    Uint16 spdCtrlBrakeTorqueLimitSrc;   // F5-21  ËÙ¶È¿ØÖÆ(ÖÆ¶¯)×ª¾ØÉÏÏÞÔ´       F211
    Uint16 spdCtrlBrakeTorqueLimit;      // F5-22  ËÙ¶È¿ØÖÆ(ÖÆ¶¯)×ª¾ØÉÏÏÞÊý×ÖÉè¶¨ F212

	Uint16 pmsmSvcPar[27];  

};
//------------------------------------------------



//------------------------------------------------
// µÚ2µç»úµÄ¹¦ÄÜÂë£¬°üÀ¨µç»ú²ÎÊý¡¢¿ØÖÆ²ÎÊý
struct MOTOR_FC
{//---rev by mrb 2013/01/08
    union MOTOR_PARA motorPara;     // bx-00  bx-26 µÚ2/3/4µç»ú²ÎÊý. Í¬µÚ1µç»ú²ÎÊý
    Uint16 tuneCmd;                 // bx-27  µ÷Ð³ÃüÁî
    union PG_PARA    pgPara;        // bx-28  bx-37 µÚ1µç»úPG¿¨²ÎÊý
    
    struct VC_PARA vcPara;          // bx-38  bx-60
    
    Uint16 motorCtrlMode;           // bx-61  µÚ2/3/4µç»ú¿ØÖÆ·½Ê½
    Uint16 accDecTimeMotor;         // bx-62  µÚ2/3/4µç»ú¼Ó¼õËÙÊ±¼äÑ¡Ôñ
    Uint16 torqueBoost;             // bx-63  ×ª¾ØÌáÉý
    Uint16 rsvdA21;                 // bx-64  Õñµ´ÒÖÖÆÔöÒæÄ£Ê½/
    Uint16 antiVibrateGain;         // bx-65  Õñµ´ÒÖÖÆÔöÒæ
};
//------------------------------------------------/





#define AI_NUMBER               3           // AI¶Ë×Ó¸öÊý

#define AO_NUMBER               2           // AO¶Ë×Ó¸öÊý

#define HDI_NUMBER              1           // HDI¶Ë×Ó¸öÊý

#define HDO_NUMBER              1           // HDO¶Ë×Ó¸öÊý

#define DI_NUMBER_PHSIC         10          // ÎïÀíDI¶Ë×Ó
#define DI_NUMBER_V             5           // ÐéÄâDI¶Ë×Ó
#define DI_NUMBER_AI_AS_DI      AI_NUMBER   // AI×÷ÎªDI

#define DO_NUMBER_PHSIC         5           // ÎïÀíDO¶Ë×Ó
#define DO_NUMBER_V             5           // ÐéÄâDO¶Ë×Ó

#define PLC_STEP_MAX            16          // PLC¡¢¶à¶ÎÖ¸Áî¶ÎÊý




//=====================================================================
// EEPROMµÄÊ¹ÓÃ³¤¶È£¬°üÀ¨ÖÐ¼äÔ¤Áô²¿·Ö
// EEPROMµØÖ··ÖÅä£¬2010-08-13
// 0            -   ±£Áô£¬rsvd4All
// 1,2          -   EEPROM-CHK
// 3            -   AIAO CHK
// 4-944        -   
// 945-1149     -   
// 1150-1329    -   
// ÆäÖÐ,16-63   -   µôµç¼ÇÒäÊ¹ÓÃ
// rvb by laoqiuyi 2014 12 05 

#define EEPROM_INDEX_USE_LENGTH     1016//1012//1006//1003 //1002 //999 //998     // ×îºóÒ»¸öeepromµØÖ·+1
#define EEPROM_INDEX_USE_INDEX        4     // ¹¦ÄÜ²ÎÊýÊ¹ÓÃEEPROMÆðÊ¼
// Ô¤Áô¸øÆäËüÊ¹ÓÃµÄ¹¦ÄÜÂëindex
#define FUNCCODE_RSVD4ALL_INDEX     GetCodeIndex(funcCode.code.rsvd4All)
#define REM_P_OFF_MOTOR     5   // ÐÔÄÜÊ¹ÓÃµÄµôµç¼ÇÒä¡£°üº¬ÔÚCORE_TO_FUNC_DISP_DATA_NUMÀïÃæ

//20170210
//#define FUNCCODE_RSVD4ALL_INDEX     GetCodeIndex(funcCode.code.rsvd4All)
//#define EEPROM_INDEX_USE_LENGTH     FUNCCODE_RSVD4ALL_INDEX//1016//1012//1006//1003 //1002 //999 //998     // ×îºóÒ»¸öeepromµØÖ·+1
//#define EEPROM_INDEX_USE_INDEX        4     // ¹¦ÄÜ²ÎÊýÊ¹ÓÃEEPROMÆðÊ¼
//#define REM_P_OFF_MOTOR     5   // ÐÔÄÜÊ¹ÓÃµÄµôµç¼ÇÒä¡£°üº¬ÔÚCORE_TO_FUNC_DISP_DATA_NUMÀïÃæ


// ¹¦ÄÜÂëµÄ×éÊý
#define FUNCCODE_GROUP_NUM  84  // °üÀ¨Ô¤ÁôµÄ×é, ¼ûfuncCodeGradeCurrentMenuMode. 
// EEPROMÖÐË³Ðò: EEPROM_CHK, µôµç¼ÇÒä, FF, FP, F0-FE, A0-AF, B0-BF, C0-CF
// ÏÔÊ¾Ë³Ðò:     F0-FF, E0-EF, B0-BF, Y0-Y7, L0-L9, EEPROM_CHK(²»ÏÔÊ¾), µôµç¼ÇÒä(²»ÏÔÊ¾), D0-DF
// D0-DF²»Õ¼ÓÃEEPROM¿Õ¼ä





#define FUNCCODE_GROUP_F0   00      // F0×é
#define FUNCCODE_GROUP_F1   01      // F1×é
#define FUNCCODE_GROUP_F2   02      // F2×é
#define FUNCCODE_GROUP_F3   03      // F3×é
#define FUNCCODE_GROUP_F4   04      // F4×é
#define FUNCCODE_GROUP_F5   05      // F5×é
#define FUNCCODE_GROUP_F6   06      // F6×é
#define FUNCCODE_GROUP_F7   07      // F7×é
#define FUNCCODE_GROUP_F8   08      // F8×é
#define FUNCCODE_GROUP_F9   09      // F9×é
#define FUNCCODE_GROUP_FA   10      // FA×é
#define FUNCCODE_GROUP_FB   11      // FB×é
#define FUNCCODE_GROUP_FC   12      // FC×é
#define FUNCCODE_GROUP_FD   13      // FD×é
#define FUNCCODE_GROUP_FE   14      // FE×é
#define FUNCCODE_GROUP_FF   15      // FF×é

#define FUNCCODE_GROUP_E0   16      // E0×é FP
#define FUNCCODE_GROUP_E1   17      // E1×é A0
#define FUNCCODE_GROUP_E2   18      // E2×é A1
#define FUNCCODE_GROUP_E3   19      // E3×é A2
#define FUNCCODE_GROUP_E4   20      // E4×é A3
#define FUNCCODE_GROUP_E5   21      // E5×é A4
#define FUNCCODE_GROUP_E6   22      // E6×é A5
#define FUNCCODE_GROUP_E7   23      // E7×é A6
#define FUNCCODE_GROUP_E8   24      // E8×é A7
#define FUNCCODE_GROUP_E9   25      // E9×é A8
#define FUNCCODE_GROUP_EA   26      // EA×é A9
#define FUNCCODE_GROUP_EB   27      // EB×é AA
#define FUNCCODE_GROUP_EC   28      // EC×é AB
#define FUNCCODE_GROUP_ED   29      // ED×é AC 
#define FUNCCODE_GROUP_EE   30      // EE×é AD
#define FUNCCODE_GROUP_EF   31      // EF×é AE

#define FUNCCODE_GROUP_B0   32      // b0×é 
#define FUNCCODE_GROUP_B1   33      // b1×é
#define FUNCCODE_GROUP_B2   34      // b2×é
#define FUNCCODE_GROUP_B3   35      // b3×é
#define FUNCCODE_GROUP_B4   36      // b4×é
#define FUNCCODE_GROUP_B5   37      // b5×é
#define FUNCCODE_GROUP_B6   38      // b6×é
#define FUNCCODE_GROUP_B7   39      // b7×é
#define FUNCCODE_GROUP_B8   40      // b8×é
#define FUNCCODE_GROUP_B9   41      // b9×é
#define FUNCCODE_GROUP_BA   42      // bA×é
#define FUNCCODE_GROUP_BB   43      // bB×é
#define FUNCCODE_GROUP_BC   44      // bC×é
#define FUNCCODE_GROUP_BD   45      // bD×é
#define FUNCCODE_GROUP_BE   46      // bE×é
#define FUNCCODE_GROUP_BF   47      // bF×é

#define FUNCCODE_GROUP_Y0   48      // y0×é
#define FUNCCODE_GROUP_Y1   49      // y1×é
#define FUNCCODE_GROUP_Y2   50      // y2×é
#define FUNCCODE_GROUP_Y3   51      // y3×é
#define FUNCCODE_GROUP_Y4   52      // y4×é
#define FUNCCODE_GROUP_Y5   53      // y5×é
#define FUNCCODE_GROUP_Y6   54      // y6×é
#define FUNCCODE_GROUP_Y7   55      // y7×é

#define FUNCCODE_GROUP_L0   56      // L0×é 
#define FUNCCODE_GROUP_L1   57      // L1×é 
#define FUNCCODE_GROUP_L2   58      // L2×é 
#define FUNCCODE_GROUP_L3   59      // L3×é 
#define FUNCCODE_GROUP_L4   60      // L4×é 
#define FUNCCODE_GROUP_L5   61      // L5×é 
#define FUNCCODE_GROUP_L6   62      // L6×é 
#define FUNCCODE_GROUP_L7   63      // L7×é 
#define FUNCCODE_GROUP_L8   64      // L8×é 
#define FUNCCODE_GROUP_L9   65      // L9×é 


#define FUNCCODE_GROUP_D0   68      // D0×é£¬ÏÔÊ¾
#define FUNCCODE_GROUP_D3   71      // D3×é£¬ÏÔÊ¾
#define FUNCCODE_GROUP_DF   (FUNCCODE_GROUP_NUM - 1)    // DF£¬ÐÔÄÜµ÷ÊÔÏÔÊ¾

// Ã¿×é¹¦ÄÜÂëµÄ¸öÊý
// ³ýFF×éÖ®Íâ£¬Ã¿×éÔ¤Áô2¸ö¹¦ÄÜÂë¡£ÎªÁËÔÚÔö¼Ó¹¦ÄÜÂëÊ±£¬¾¡Á¿²»ÓÃ»Ö¸´³ö³§²ÎÊý¡£
#define F0NUM           (28+ 0  )   // F0  »ù±¾¹¦ÄÜ×é 
#define F1NUM           (40+ 7  )   // F1  ÊäÈë¶Ë×Ó   
#define F2NUM           (23+ 0  )   // F2  Êä³ö¶Ë×Ó   
#define F3NUM           (16+ 11 )   // F3  ÆôÍ£¿ØÖÆ   
#define F4NUM           (49+ 0  )   // F4  V/F¿ØÖÆ²ÎÊý
#define F5NUM           (23+ 27 )   // F5  Ê¸Á¿¿ØÖÆ²ÎÊý  
#define F6NUM           (15+ 0  )   // F6  ¼üÅÌÓëÏÔÊ¾   
#define F7NUM           (52+ 0  )   // F7  ¸¨Öú¹¦ÄÜ    
#define F8NUM           (36+ 2  )   // F8  ¹ÊÕÏÓë±£»¤ 
#define F9NUM           ( 9+ 0  )   // F9  Í¨Ñ¶²ÎÊý    
#define FANUM           ( 9+ 0  )   // FA  ×ª¾Ø¿ØÖÆ²ÎÊý
#define FBNUM           (10+ 0  )   // FB  ¿ØÖÆÓÅ»¯²ÎÊý
#define FCNUM           ( 3+ 0  )   // FC  
#define FDNUM           ( 0+ 1  )   // FD  
#define FENUM           ( 0+ 1  )   // FE  
#define FFNUM           ( 0+ 1  )   // FF  


#define E0NUM           (10+ 0  )//(10+ 0  )   // E0  °ÚÆµ¡¢¶¨³¤ºÍ¼ÆÊý
#define E1NUM           (52+ 0  )   // E1  ¶à¶ÎÖ¸Áî¡¢¼òÒ×PLC
#define E2NUM           (29+ 0  )   // E2  PID¹¦ÄÜ    
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
#define EDNUM           ( 20    )   // ED  AIAO³ö³§Ð£Õý 
#define EENUM           ( 1+ 0  )   // EE  ¹¦ÄÜÂë×éÒþ²Ø
#define EFNUM           ( 0+ 1  )   // EF

#define B0NUM           (38+0   )          // B0  µÚÒ»µç»ú²ÎÊý
#define B1NUM           (B0NUM+2+F5NUM+3)  // B1  µÚ¶þµç»ú²ÎÊý
#define B2NUM           ( B1NUM )          // B2  µÚÈýµç»ú²ÎÊý
#define B3NUM           ( B1NUM )          // B3  µÚËÄµç»ú²ÎÊý
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

#define Y0NUM           ( 6+0   )   // Y0  ¹¦ÄÜÂë¹ÜÀí
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
#define L9NUM           FUNC_TO_CORE_DEBUG_DATA_NUM     // L9  µ÷ÊÔ£¬func2motor
#elif 1
#define L9NUM           ( 0+1   )   // L9
#endif

#define CHK_NUM   (4 +0  )  //     eepromCheckWord(2)£¬rsvd4All(1)·ÅÔÚÕâÀïµÄ×îÇ°Ãæ, AIAOChk(1)Ò²ÔÚÕâÀï
#define REM_NUM   (48)      // µôµç¼ÇÒä£¬°üÀ¨ÐÔÄÜÊ¹ÓÃµÄµôµç¼ÇÒä¡£(°üÀ¨ÐÔÄÜµÄµôµç¼ÇÒä)

#define D0NUM     (100+0)   // D0  ÏÔÊ¾Ê¹ÓÃ£¬²»Õ¼ÓÃEEPROM£¬¾¡Á¿ÉÙµÄÕ¼ÓÃ³ÌÐò¿Õ¼ä(ÎÞ³ö³§Öµ¡¢ÉÏÏÂÏÞ£¬µ«ÓÐÊôÐÔ)
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
#define DFNUM     CORE_TO_FUNC_DISP_DATA_NUM  // DF  µ÷ÊÔ£¬motor2func
#elif 1
#define DFNUM     ( 0+1)    // DF ÐÔÄÜµ÷ÊÔÊ¹ÓÃ
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
                        )                                   // ËùÓÐ¹¦ÄÜÂë£¬²»°üÀ¨ÏÔÊ¾
#define FNUM_EEPROM    (FNUM_PARA + CHK_NUM + REM_NUM)      // ÐèÒª´æ´¢ÔÚEEPROMÖÐµÄËùÓÐ²ÎÊý
#define FNUM_ALL       (FNUM_EEPROM +                       \
                        D0NUM + D1NUM + D2NUM + D3NUM +     \
                        D4NUM + D5NUM + D6NUM + D7NUM +     \
                        D8NUM + D9NUM + DANUM + DBNUM +     \
                        DCNUM + DDNUM + DENUM + DFNUM       \
                       )                                    // ËùÓÐ¹¦ÄÜÂë¡¢²ÎÊý£¬°üÀ¨ÏÔÊ¾


//=====================================================================
//
// é£¬F0×é£¬F1×é, ...
//
// eepromCheckWord·ÅÔÚ×îÇ°Ãæ
// ³§¼Ò²ÎÊý¡¢ÓÃ»§ÃÜÂë¡¢ÓÃ»§¶¨ÖÆ²Ëµ¥·ÅÔÚÇ°Ãæ£¬·ÀÖ¹ÔöÉ¾¹¦ÄÜÂëÊ±±»ÎóÐÞ¸Ä¡£
// fcÓëeepromÓÐ¶ÔÓ¦¹ØÏµ±í£¬ËùÒÔÒÔÉÏÒªÇó²»ÐèÒªÁË
//
//=====================================================================
struct FUNCCODE_GROUP 
{
//======================================
    Uint16 f0[F0NUM];               // F0 »ù±¾¹¦ÄÜ×é
    Uint16 f1[F1NUM];               // F1 ÊäÈë¶Ë×Ó
    Uint16 f2[F2NUM];               // F2 Êä³ö¶Ë×Ó
    Uint16 f3[F3NUM];               // F3 ÆôÍ£¿ØÖÆ
    
//======================================
    Uint16 f4[F4NUM];               // F4 V/F¿ØÖÆÎÊ?
    Uint16 f5[F5NUM];               // F5 Ê¸?
    Uint16 f6[F6NUM];               // F6 ¼üÅÌÓëÏÔÊ¾
    Uint16 f7[F7NUM];               // F7 ¸¨Öú¹¦ÄÜ
    
//======================================
    Uint16 f8[F8NUM];               // F8 ¹ÊÕÏÓë±£»¤
    Uint16 f9[F9NUM];               // F9 Í¨Ñ¶²ÎÊý
    Uint16 fa[FANUM];               // FA ×ª¾Ø¿ØÖÆ²ÎÊý
    Uint16 fb[FBNUM];               // FB ¿ØÖÆÓÅ»¯²ÎÊý

//======================================
    Uint16 fc[FCNUM];               // 
    Uint16 fd[FDNUM];               // 
    Uint16 fe[FENUM];               //  
    Uint16 ff[FFNUM];               // 
    
 //======================================
    Uint16 e0[E0NUM];               // E0 °ÚÆµ¡¢¶¨³¤ºÍ¼ÆÊý
    Uint16 e1[E1NUM];               // E1 ¶à¶ÎÖ¸Áî¡¢¼òÒ×PLC
    Uint16 e2[E2NUM];               // E2 PID¹¦ÄÜ
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
    Uint16 ee[EENUM];               // EE ¹¦ÄÜÂë×éÒþ²Ø
    Uint16 ef[EFNUM];               // EF 
    
//======================================
    Uint16 b0[B0NUM];               // B0 µÚÒ»µç»ú²ÎÊý
    Uint16 b1[B1NUM];               // B1 µÚ¶þµç»ú²ÎÊý
    Uint16 b2[B2NUM];               // B2 µÚÈýµç»ú²ÎÊý
    Uint16 b3[B3NUM];               // B3 µÚËÄµç»ú²ÎÊý

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
    Uint16 y0[Y0NUM];               // Y0 ¹¦ÄÜÂë¹ÜÀí
    Uint16 y1[Y1NUM];               // Y1 
    Uint16 y2[Y2NUM];               // Y2
    Uint16 y3[Y3NUM];               // Y3

//======================================
    Uint16 y4[Y4NUM];               // Y4
    Uint16 y5[Y5NUM];               // Y5
    Uint16 y6[Y6NUM];               // Y6
    Uint16 y7[Y7NUM];               // Y7

//======================================
    Uint16 l0[L0NUM];               // L0 ³§¼Ò²ÎÊý
    Uint16 l1[L1NUM];               // L1 AIAO³ö³§Ð£Õý
    Uint16 l2[L2NUM];               // L2
    Uint16 l3[L3NUM];               // L3

//======================================
    Uint16 l4[L4NUM];               // L4   
    Uint16 l5[L5NUM];               // L5
    Uint16 l6[L6NUM];               // L6
    Uint16 l7[L7NUM];               // L7
    Uint16 l8[L8NUM];               // L8    
    Uint16 l9[L9NUM];               // L9    

// Ö®Ç°µÄ¹¦ÄÜÂëÓÐÉÏÏÂÏÞ£¬ÊôÐÔ£»Ö®ºóÃ»ÓÐ£¬½ÚÊ¡¿Õ¼ä
//======================================

//======================================
    Uint16 fChk[CHK_NUM];           // eepromCheckWord

//======================================
    Uint16 remember[REM_NUM];       // µôµç¼ÇÒä

// Ö®Ç°µÄÊý¾ÝÒª·ÅÔÚEEPROMÖÐ
//======================================

//======================================
// Ö®ºóµÄÊý¾Ý²»ÐèÒª·ÅÔÚEEPROMÖÐ£¬½öRAM
    Uint16 d0[D0NUM];               // D0 ÏÔÊ¾
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
    Uint16 df[DFNUM];               // DF, ÏÔÊ¾£¬ÐÔÄÜµ÷ÊÔÊ¹ÓÃ
//======================================
};


//=====================================================================
//
// ¹¦ÄÜÂë£¬F0-00, F0-01, ..., F1-00, F1-01, ...
//
//=====================================================================
struct FUNCCODE_CODE 
{
//======================================
// F0 »ù±¾¹¦ÄÜ×é----Ô­F0------ÓÐ¸Ä¶¯---20130228 by mrb
    Uint16 motorCtrlMode;                   // F0-00  (µç»ú)¿ØÖÆ·½Ê½
    Uint16 presetFrq;                       // F0-01  Ô¤ÖÃÆµÂÊ
    Uint16 frqPoint;                        // F0-02  ÆµÂÊÖ¸ÁîÐ¡Êýµã
    Uint16 frqXSrc;                         // F0-03  Ö÷ÆµÂÊÔ´XÑ¡Ôñ
    Uint16 frqYSrc;                         // F0-04  ¸¨ÖúÆµÂÊÔ´YÑ¡Ôñ
    Uint16 frqYRangeBase;                   // F0-05  ¸¨ÖúÆµÂÊÔ´Y·¶Î§Ñ¡Ôñ
    Uint16 frqYRange;                       // F0-06  ¸¨ÖúÆµÂÊÔ´Y·¶Î§
    Uint16 frqCalcSrc;                      // F0-07  ÆµÂÊÔ´µþ¼ÓÑ¡Ôñ
    Uint16 frqYOffsetFc;                    // F0-08  ¸¨ÖúÆµÂÊÔ´Æ«ÖÃµÄÊý×ÖÉè¶¨
    Uint16 frqRemMode;                      // F0-09  Êý×ÖÉè¶¨ÆµÂÊ¼ÇÒäÑ¡Ôñ
    Uint16 updnBenchmark;                   // F0-10  ÔËÐÐÊ±ÆµÂÊÖ¸ÁîUP/DOWN»ù×¼
    Uint16 runSrc;                          // F0-11  ÃüÁîÔ´Ñ¡Ôñ
    Uint16 frqRunCmdBind;                   // F0-12  ÃüÁîÔ´À¦°óÆµÂÊÔ´
    Uint16 accTime1;                        // F0-13  ¼ÓËÙÊ±¼ä1
    Uint16 decTime1;                        // F0-14  ¼õËÙÊ±¼ä1
    Uint16 accDecTimeUnit;                  // F0-15  ¼Ó¼õËÙÊ±¼äµÄµ¥Î»
    Uint16 accDecBenchmark;                 // F0-16  ¼Ó¼õËÙÊ±¼ä»ù×¼ÆµÂÊ
    Uint16 varFcByTem;                      // F0-17  ÔØ²¨ÆµÂÊËæÎÂ¶Èµ÷Õû
    Uint16 carrierFrq;                      // F0-18  ÔØ²¨ÆµÂÊ
    Uint16 maxFrq;                          // F0-19  ×î´óÆµÂÊ
    Uint16 upperFrqSrc;                     // F0-20  ÉÏÏÞÆµÂÊÔ´
    Uint16 upperFrq;                        // F0-21  ÉÏÏÞÆµÂÊÊýÖµÉè¶¨
    Uint16 upperFrqOffset;                  // F0-22  ÉÏÏÞÆµÂÊÆ«ÖÃ
    Uint16 lowerFrq;                        // F0-23  ÏÂÏÞÆµÂÊÊýÖµÉè¶¨
    Uint16 runDir;                          // F0-24  ÔËÐÐ·½Ïò
    enum MOTOR_SN motorSn;                  // F0-25  µç»úÑ¡Ôñ
    Uint16 AiSet_Frq;                       // F0-26  AI½ø¶ÈÑ¡Ôñ 0  0.01HZ  1  0.05  2 0.1 3 0.5  
    Uint16 inverterGpTypeDisp;              // F0-27  GPÀàÐÍÏÔÊ¾


//======================================
// F1 ÊäÈë¶Ë×Ó--------Ô­F4----ÎÞ¸Ä¶¯---20130228 by mrb
    Uint16 diFunc[DI_NUMBER_PHSIC];         // F1-00  --F1-09   DI1¶Ë×Ó¹¦ÄÜÑ¡Ôñ
                                            
    Uint16 diControlMode;                   // F1-10  ¶Ë×ÓÃüÁî·½Ê½
    Uint16 diUpDownSlope;                   // F1-11  ¶Ë×ÓUP/DOWNËÙÂÊ£¬¸ÄÎª0.001Hz
                                            
                                            
    Uint16 curveSet2P1[4];                  // F1-12,...,F1-15  ÇúÏß1£¬2µã£¬×î´óÖµ£¬×îÐ¡Öµ
                                            
                                            
    Uint16 curveSet2P2[4];                  // F1-16,...,F1-19  ÇúÏß2£¬2µã
                                            
    Uint16 curveSet2P3[4];                  // F1-20,...,F1-23  ÇúÏß3£¬2µã
                                            
    Uint16 aiCurveSrc;                      // F1-24  AIÉè¶¨ÇúÏßÑ¡Ôñ
                                            
    Uint16 aiLimitSrc;                      // F1-25  AIÏÂÏÞÑ¡Ôñ
                                            
                                            
    Uint16 curveSet2P4[4];                  // F1-26,...,F1-29  HDIÇúÏß£¬2µã
                                            
                                            
    Uint16 diFilterTime;                    // F1-30  DIÂË²¨Ê±¼ä
    Uint16 ai1FilterTime;                   // F1-31  AI1ÂË²¨Ê±¼ä, 10ms
                                            
    Uint16 ai2FilterTime;                   // F1-32  AI2ÂË²¨Ê±¼ä, 10ms
                                            
    Uint16 ai3FilterTime;                   // F1-33  AI3ÂË²¨Ê±¼ä, 10ms
                                            
    Uint16 pulseInFilterTime;               // F1-34  PULSEÂË²¨Ê±¼ä, 10ms
                                            
    Uint16 diLogic[2];                      // F1-35  DIÓÐÐ§×´Ì¬Ñ¡Ôñ1
                                            // F1-36  DIÓÐÐ§´Ì¬Ñ¡Ô?
    Uint16 diDelayTime[3];                  // F1-37  DI1ÑÓ³ÙÊ±¼ä
									                          // F1-38  DI1ÑÓ³ÙÊ±¼ä
									                          // F1-39  DI1ÑÓ³ÙÊ±¼ä
    Uint16 diDupSel;                        // F1-40 DI¿ÉÖØ¸´¶¨ÒåÑ¡Ôñ£¬0---²»¿É£¬1---¿ÉÒÔ
    Uint16 keyAdX1;                         // F1-41 ¼üÅÌµçÎ»Æ÷X1
    Uint16 keyAdX2;                         // F1-42 ¼üÅÌµçÎ»Æ÷X2
    Uint16 keyAdValue;                      // F1-43 ¼üÅÌµçÎ»Æ÷Éè¶¨Öµ
    int16 keyAdY1;                          // F1-44 ¼üÅÌµçÎ»Æ÷X1¶ÔÓ¦Öµ
    int16 keyAdY2;                          // F1-45 ¼üÅÌµçÎ»Æ÷X2¶ÔÓ¦Öµ
	  Uint16 keyAdCtrl;                       // F1-46 ¼üÅÌµçÎ»Æ÷¿ØÖÆ 

//======================================
// F2 Êä³ö¶Ë×Ó--------Ô­F5----ÎÞ¸Ä¶¯---20130228 by mrb

    Uint16 fmOutSelect;                     // F2-00  ¶à¹¦ÄÜ¶Ë×ÓÊä³öÑ¡
    Uint16 doFunc[DO_NUMBER_PHSIC];         // F2-01  FMRÊä³öÑ¡Ôñ
                                            // F2-02  ¿ØÖÆ°åRELAYÊä³öÑ¡Ôñ
                                            // F2-03  À©Õ¹¿¨RELAYÊä³öÑ¡Ôñ
                                            // F2-04  DO1Êä³öÑ¡Ôñ
                                            // F2-05  À©Õ¹¿¨DO2Êä³öÑ¡Ôñ

    Uint16 aoFunc[AO_NUMBER+HDO_NUMBER];    // F2-06  FMPÊä³öÑ¡Ôñ
                                            // F2-07  AO1Êä³öÑ¡Ôñ
                                            // F2-08  À©Õ¹¿¨AO2Êä³öÑ¡Ôñ
    Uint16 fmpOutMaxFrq;                    // F2-09  FMPÊä³ö×î´óÆµÂÊ

    Uint16 doDelayTime[DO_NUMBER_PHSIC];    // F2-10  RELAY1Êä³öÑÓ³ÙÊ±¼ä
                                            // F2-11  RELAY2Êä³öÑÓ³ÙÊ±¼ä
                                            // F2-12  DO1Êä³öÑÓ³ÙÊ±¼ä
                                            // F2-13  DO2Êä³öÑÓ³ÙÊ±¼ä
                                            // F2-14  DO3Êä³öÑÓ³ÙÊ±¼ä
    Uint16 doLogic;                         // F2-15  DOÓÐÐ§×´Ì¬Ñ¡Ôñ
    AO_PARA aoPara[AO_NUMBER];              // F2-16  AO1ÁãÆ«ÏµÊý
                                            // F2-17  AO1ÔöÒæ
                                            // F2-18  AO2ÁãÆ«ÏµÊý
                                            // F2-19  AO2ÔöÒæ
    Uint16 aoLpfTime[AO_NUMBER+HDO_NUMBER]; // F2-20  HDO,AO1,AO2Êä³öÂË²¨Ê±¼ä
    


//======================================
// F3 ÆôÍ£¿ØÖÆ--------Ô­F6------ÎÞ¸Ä¶¯---20130228 by mrb
    Uint16 startMode;                       // F3-00  Æô¶¯·½Ê½
    Uint16 speedTrackMode;                  // F3-01  ×ªËÙ¸ú×Ù·½Ê½
    Uint16 speedTrackVelocity;              // F3-02  ×ªËÙ¸ú×Ù¿ìÂý
    Uint16 startFrq;                        // F3-03  Æô¶¯ÆµÂÊ
    Uint16 startFrqTime;                    // F3-04  Æô¶¯ÆµÂÊ±£³ÖÊ±¼ä
    Uint16 startBrakeCurrent;               // F3-05  Æô¶¯Ö±Á÷ÖÆ¶¯µçÁ÷
    Uint16 startBrakeTime;                  // F3-06  Æô¶¯Ö±Á÷ÖÆ¶¯Ê±¼ä
    Uint16 stopMode;                        // F3-07  Í£»ú·½Ê½
    Uint16 stopBrakeFrq;                    // F3-08  Í£»úÖ±Á÷ÖÆ¶¯ÆðÊ¼ÆµÂÊ
    Uint16 stopBrakeWaitTime;               // F3-09  Í£»úÖ±Á÷ÖÆ¶¯µÈ´ýÊ±¼ä
    Uint16 stopBrakeCurrent;                // F3-10  Í£»úÖ±Á÷ÖÆ¶¯µçÁ÷
    Uint16 stopBrakeTime;                   // F3-11  Í£»úÖ±Á÷ÖÆ¶¯Ê±¼ä
    Uint16 brakeDutyRatio;                  // F3-12  ÖÆ¶¯Ê¹ÓÃÂÊ
    Uint16 accDecSpdCurve;                  // F3-13  ¼Ó¼õËÙ·½Ê½
    Uint16 sCurveStartPhaseTime;            // F3-14  SÇúÏß¿ªÊ¼¶ÎÊ±¼ä±ÈÀý
    Uint16 sCurveEndPhaseTime;              // F3-15  SÇúÏß½áÊø¶ÎÊ±¼ä±ÈÀý
    Uint16  startGroup[11];                 // F3-16~26 Òþ²Ø²ÎÊý  
//======================================
// F4 V/F¿ØÖÆ²ÎÊý--------Ô­F3--ÓÐ¸Ä¶¯---20130228 by mrb

    Uint16 vfCurve;                         // F4-00  VFÇúÏßÉè¶¨
    Uint16 torqueBoost;                     // F4-01  ×ª¾ØÌáÉý
    Uint16 boostCloseFrq;                   // F4-02  ×ª¾ØÌáÉý½ØÖ¹ÆµÂÊ
    Uint16 vfFrq1;                          // F4-03  ¶àµãVFÆµÂÊµã1
    Uint16 vfVol1;                          // F4-04  ¶àµãVFµçÑ¹µã1
    Uint16 vfFrq2;                          // F4-05  ¶àµãVFÆµÂÊµã2
    Uint16 vfVol2;                          // F4-06  ¶àµãVFµçÑ¹µã2
    Uint16 vfFrq3;                          // F4-07  ¶àµãVFÆµÂÊµã3
    Uint16 vfVol3;                          // F4-08  ¶àµãVFµçÑ¹µã3
    Uint16 slipCompCoef;                    // F4-09  ×ª²î²¹³¥ÏµÊý
    Uint16 vfOverMagGain;                   // F4-10  VF¹ýÀø´ÅÔöÒæ
    Uint16 antiVibrateGain;                 // F4-11  Õñµ´ÒÖÖÆÔöÒæ
//    Uint16 vfF312;                          // F4-12 
    Uint16 vfVoltageSrc;                    // F4-13  VF·ÖÀëµÄµçÑ¹Ô´
    Uint16 vfVoltageDigtalSet;              // F4-14  VF·ÖÀëµÄµçÑ¹Ô´Êý×ÖÉè¶¨
    Uint16 vfVoltageAccTime;                // F4-15  VF·ÖÀëµÄµçÑ¹ÉÏÉýÊ±¼ä   
    Uint16 vfF312;                          // F4-12  20170503   
    Uint16 vfOcLevel;                       // F4-16  ¹ýÁ÷Ê§ËÙµçÁ÷ MD500
	Uint16 vfOcEn;                          // F4-17  ¹ýÁ÷Ê§ËÙÊ¹ÄÜ MD500
	Uint16 vfOcGain;                        // F4-18  ¹ýÁ÷Ê§ËÙÔöÒæ MD500
	Uint16 vfBsOcCmpCoff;                   // F4-19  ±¶ËÙ¹ýÁ÷²¹³¥ MD500               
	Uint16 vfOvLevel;                       // F4-20  ¹ýÑ¹Ê§ËÙµçÑ¹ MD500
	Uint16 vfOvEn;                          // F4-21  ¹ýÑ¹Ê§ËÙÊ¹ÄÜ MD500
	Uint16 vfOvFgn;                         // F4-22  ¹ýÑ¹ÆµÂÊÔöÒæ MD500
    Uint16 vfOvVgn;                         // F4-23  ¹ýÑ¹µçÑ¹ÔöÒæ MD500
	Uint16 vfOvFrqLmt;                      // F4-24  ¹ýÑ¹ÆµÂÊÏÞÖÆ MD500
	Uint16 vfwsTrComp;                      // F4-25  ×ª²î²¹³¥³£Êý MD500    
	Uint16 vfGroup[23];                     // F4-26~48 VF²ÎÊý×é   MD500
    
//======================================
// F5 Ê¸Á¿¿ØÖÆ²ÎÊý--------Ô­F2

    struct VC_PARA vcParaM1;        // µÚ1µç»úÊ¸Á¿¿ØÖÆ²ÎÊý


//======================================
// F6 ¼üÅÌÓëÏÔÊ¾--------Ô­F7----Î´¸Ä20130228 by mrb
//    Uint16 rsvdF71;                 // F6-00  ±£Áô
//    Uint16 mfkKeyFunc;              // F6-01  MF.K¼ü¹¦ÄÜÑ¡Ôñ
    Uint16 stopKeyFunc;                     // F6-00  STOP¼ü¹¦ÄÜ
    Uint16 ledDispParaRun1;                 // F6-01  LEDÔËÐÐÏÔÊ¾²ÎÊý1
    Uint16 ledDispParaRun2;                 // F6-02  LEDÔËÐÐÏÔÊ¾²ÎÊý2
                                            
    Uint16 ledDispParaStop;                 // F6-03  LEDÍ£»úÏÔÊ¾²ÎÊý
    Uint16 speedDispCoeff;                  // F6-04  ¸ºÔØËÙ¶ÈÏÔÊ¾ÏµÊý
  	Uint16 speedDispPointPos;               // F6-05  ¸ºÔØËÙ¶ÈÏÔÊ¾Ð¡ÊýµãÎ»ÖÃ
    Uint16 radiatorTemp;                    // F6-06  Äæ±äÆ÷Ä£¿éÉ¢ÈÈÆ÷ÎÂ¶È
                                            
    Uint16 runTimeAddup;                    // F6-07  ÀÛ¼ÆÔËÐÐÊ±¼ä, µ¥Î»: h
    Uint16 powerUpTimeAddup;                // F6-08  ÀÛ¼ÆÉÏµçÊ±¼ä
    Uint16 powerAddup;                      // F6-09  ÀÛ¼ÆºÄµçÁ¿
                                            
    Uint16 productVersion;                  // F6-10  ²úÆ·ºÅ
    Uint16 softVersion;                     // F6-11  Èí¼þ°æ±¾ºÅ
    Uint16 temp2;                           // F6-12  ÕûÁ÷ÇÅÉ¢ÈÈÆ÷ÎÂ¶È
    Uint16 rsvdF71;                         // F6-13  ±£Áô
    Uint16 mfkKeyFunc;                      // F6-14  MF.K¼ü¹¦ÄÜÑ¡Ôñ

//======================================
// F7 ¸¨Öú¹¦ÄÜ--------Ô­F8---¸Ä20130228 by mrb
    Uint16 jogFrq;                          // F7-00  µã¶¯ÔËÐÐÆµÂÊ
    Uint16 jogAccTime;                      // F7-01  µã¶¯¼ÓËÙÊ±¼ä
    Uint16 jogDecTime;                      // F7-02  µã¶¯¼õËÙÊ±¼ä
    // ADD                                  
    Uint16 jogWhenRun;                      // F7-03  ¶Ë×Óµã¶¯ÓÅÏÈ                                           
    Uint16 jumpFrq1;                        // F7-04  ÌøÔ¾ÆµÂÊ1
    Uint16 jumpFrq2;                        // F7-05  ÌøÔ¾ÆµÂÊ2
    Uint16 jumpFrqRange;                    // F7-06  ÌøÔ¾ÆµÂÊ·ù¶È
    Uint16 jumpFrqMode;                     // F7-07  ¼Ó¼õËÙ¹ý³ÌÖÐÌøÔ¾ÆµÂÊÊÇ·ñÓÐÐ§
    Uint16 accTime2;                        // F7-08  ¼ÓËÙÊ±¼ä2
    Uint16 decTime2;                        // F7-09  ¼õËÙÊ±¼ä2
    Uint16 accTime3;                        // F7-10  ¼ÓËÙÊ±¼ä3
    Uint16 decTime3;                        // F7-11  ¼õËÙÊ±¼ä3
    Uint16 accTime4;                        // F7-12  ¼ÓËÙÊ±¼ä4
    Uint16 decTime4;                        // F7-13  ¼õËÙÊ±¼ä4
    Uint16 accTimefrqChgValue;              // F7-14  ¼ÓËÙÊ±¼ä1/2ÇÐ»»ÆµÂÊµã 
    Uint16 decTimefrqChgValue;              // F7-15  ¼õËÙÊ±¼ä1/2ÇÐ»»ÆµÂÊµã                                             
    Uint16 zeroSpeedDeadTime;               // F7-16  Õý·´×ªËÀÇøÊ±¼ä
    Uint16 antiReverseRun;                  // F7-17  ·´×ª¿ØÖÆ, 0-ÔÊÐí·´×ª£¬1-½ûÖ¹·´×ª
    Uint16 lowerDeal;                       // F7-18  ÆµÂÊµÍÓÚÏÂÏÞÆµÂÊÔËÐÐ¶¯×÷
    Uint16 droopCtrl;                       // F7-19  ÏÂ´¹¿ØÖÆ
    Uint16 powerUpTimeArriveSet;            // F7-20  Éè¶¨ÉÏµçµ½´ïÊ±¼ä
    Uint16 runTimeArriveSet;                // F7-21  Éè¶¨ÔËÐÐµ½´ïÊ±¼ä
    Uint16 startProtect;                    // F7-22  Æô¶¯±£»¤Ñ¡Ôñ                                            
    Uint16 frqFdtValue;                     // F7-23  ÆµÂÊ¼ì²âÖµ(FDTµçÆ½)
    Uint16 frqFdtLag;                       // F7-24  ÆµÂÊ¼ì²âÖÍºóÖµ
    Uint16 frqArriveRange;                  // F7-25  ÆµÂÊµ½´ï¼ì³ö·ù¶È
    Uint16 frqFdt1Value;                    // F7-26  ÆµÂÊ¼ì²âÖµ(FDT1µçÆ½)
    Uint16 frqFdt1Lag;                      // F7-27  ÆµÂÊ¼ì²â1ÖÍºóÖµ
    Uint16 frqArriveValue1;                 // F7-28  ÆµÂÊµ½´ï¼ì²âÖµ1 
    Uint16 frqArriveRange1;                 // F7-29  ÆµÂÊµ½ï¼ì³ö1·ù¶È
    Uint16 frqArriveValue2;                 // F7-30  ÆµÂÊµ½´ï¼ì²âÖµ2 
    Uint16 frqArriveRange2;                 // F7-31  ÆµÂÊµ½´ï¼ì³ö2·ù¶È                                          
    Uint16 oCurrentChkValue;                // F7-32  ÁãµçÁ÷¼ì²â
    Uint16 oCurrentChkTime;                 // F7-33  ÁãµçÁ÷¼ì²âÑÓ³ÙÊ±¼ä
    Uint16 softOCValue;                     // F7-34  Èí¼þ¹ýÁ÷µã
    Uint16 softOCDelay;                     // F7-35  Èí¼þ¹ýÁ÷¼ì²âÑÓ³ÙÊ±¼ä                                           
    Uint16 currentArriveValue1;             // F7-36  µçÁ÷µ½´ï¼ì²âÖµ1
    Uint16 currentArriveRange1;             // F7-37  µçÁ÷µ½´ï¼ì²â1·ù¶È
    Uint16 currentArriveValue2;             // F7-38  µçÁ÷µ½´ï¼ì²âÖµ2
    Uint16 currentArriveRange2;             // F7-39  µçÁ÷µ½´ï¼ì²â2·ù¶È                                           
    Uint16 temperatureArrive;               // F7-40  Ä£¿éÎÂ¶Èµ½´ï
    Uint16 fanControl;                      // F7-41  ·çÉÈ¿ØÖÆ                                           
    Uint16 setTimeMode;                     // F7-42  ¶¨Ê±¹¦ÄÜÑ¡Ôñ
    Uint16 setTimeSource;                   // F7-43  ¶¨Ê±Ê±¼äÉè¶¨Ñ¡Ôñ
    Uint16 setTimeValue;                    // F7-44  Éè¶¨ÔËÐÐÊ±¼ä
    Uint16 setTimeArrive;                   // F7-45  µ±Ç°ÔËÐÐµ½´ïÊ±¼ä                                            
    Uint16 wakeUpFrq;                       // F7-46  ½ÐÑÆµÂ?
    Uint16 wakeUpTime;                      // F7-47  »½ÐÑÑÓ³ÙÊ±¼ä
    Uint16 dormantFrq;                      // F7-48  ÐÝÃßÆµÂÊ
    Uint16 dormantTime;                     // F7-49  ÐÝÃßÑÓ³ÙÊ±¼ä                                           
    Uint16 ai1VoltageLimit;                 // F7-50  AI1ÊäÈëµçÑ¹ÏÂÏÞ
    Uint16 ai1VoltageUpper;                 // F7-51  AI1ÊäÈëµçÑ¹ÉÏÏÞ


//======================================
// F8 ¹ÊÕÏÓë±£»¤--------Ô­F9----¸Ä20130228 by mrb
    Uint16 ocGain;                          // F8-00  ¹ýÁ÷Ê§ËÙÔöÒæ
    Uint16 ocPoint;                         // F8-01  ¹ýÁ÷Ê§ËÙ±£»¤µçÁ÷
    Uint16 overloadMode;                    // F8-02  µç»ú¹ýÔØ±£»¤Ñ¡Ôñ
    Uint16 overloadGain;                    // F8-03  µç»ú¹ýÔØ±£»¤ÔöÒæ
    Uint16 foreOverloadCoef;                // F8-04  µç»ú¹ýÔØÔ¤¾¯ÏµÊý
    Uint16 ovGain;                          // F8-05  ¹ýÑ¹Ê§ËÙÔöÒæ
    Uint16 ovPoint;                         // F8-06  ¹ýÑ¹Ê§ËÙ±£»¤µçÑ¹                                            
    Uint16 inPhaseLossProtect;              // F8-07  ÊäÈëÈ±Ïà±£»¤Ñ¡Ôñ
    Uint16 outPhaseLossProtect;             // F8-08  Êä³öÈ±Ïà±£»¤Ñ¡Ôñ
    Uint16 shortCheckMode;                  // F8-09  ÉÏµç¶ÔµØ¶ÌÂ·±£»¤¹¦ÄÜ
    Uint16 errAutoRstNumMax;                // F8-10  ¹ÊÕÏ×Ô¶¯¸´Î»´ÎÊý
    Uint16 errAutoRstRelayAct;              // F8-11  ¹ÊÕÏ×Ô¶¯¸´Î»ÆÚ¼ä¹ÊÕÏ¼ÌµçÆ÷¶¯×÷Ñ¡Ôñ
    Uint16 errAutoRstSpaceTime;             // F8-12  ¹ÊÕÏ×Ô¶¯¸´Î»¼ä¸ôÊ±¼ä, 0.1s   
    Uint16 osChkValue;                      // F8-13 ¹ýËÙ¶È¼ì²âÖµ
    Uint16 osChkTime;                       // F8-14 ¹ýËÙ¶È¼ì²âÊ±¼ä
    Uint16 devChkValue;                     // F8-15 ËÙ¶ÈÆ«²î¹ý´ó¼ì²âÖµ
    Uint16 devChkTime;                      // F8-16 ËÙ¶ÈÆ«²î¹ý´ó¼ì²âÊ±¼ä                                            
    Uint16 errorAction[4];                  // F8-17  -F8-21  ¹ÊÕÏÊ±±£»¤¶¯×÷Ñ¡Ôñ1-4
    Uint16 breakStartVolt;                  // F8-21  ¹ýÑ¹ÖÆ¶¯·§Öµ
    Uint16 errorShow[2];                    // F8-22  -F8-23  ¹ÊÕÏÖ¸Ê¾Ñ¡Ôñ1,2
    Uint16 errorRunFrqSrc;                  // F8-24  ¹ÊÕÏÊ±¼ÌÐøÔËÐÐÆµÂÊÑ¡Ôñ
    Uint16 errorSecondFrq;                  // F8-25  Òì³£±¸ÓÃÆµÂÊÉè¶¨                                                                                        
    Uint16 pOffTransitoryNoStop;            // F8-26  Ë²Í£²»Í£¹¦ÄÜÑ¡Ôñ
    Uint16 pOffTransitoryFrqDecSlope;       // F8-27  Ë²Í£¶¯×÷ÔÝÍ£ÅÐ¶ÏµçÑ¹
    Uint16 pOffVolBackTime;                 // F8-28  Ë²Í£²»Í£µçÑ¹»ØÉýÅÐ¶ÏÊ±¼ä
    Uint16 pOffThresholdVol;                // F8-29  Ë²Í£²»Í£¶¯×÷ÅÐ¶ÏµçÑ¹                                           
    Uint16 loseLoadProtectMode;             // F8-30  µôÔØ±£»¤Ñ¡Ôñ
    Uint16 loseLoadLevel;                   // F8-31  µôÔØ¼ì³öË®Æ½
    Uint16 loseLoadTime;                    // F8-32  µôÔØ¼ì³öÊ±¼ä                                            
    Uint16 motorOtMode;                     // F8-33  µç»úÎÂ¶È´«¸ÐÆ÷ÀàÐÍ
    Uint16 motorOtProtect;                  // F8-34  µç»ú¹ýÈÈ±£»¤ãÐÖµ
    Uint16 motorOtCoef;                     // F8-35  µç»ú¹ýÈÈÔ¤±¨¾¯ãÐÖµ
 	  Uint16 pmsmEncProtect[2];               // F8-36  UVW±àÂëÆ÷¹ÊÕÏ(Err20)Ê¹ÄÜ
	                                          // F8-37  ¹ÊÕÏ±£»¤¶¯×÷Ñ¡Ôñ5 ¸öÎ»£¨Err51£©Ê®Î»£¨Err19£©


//========================================================
// F9 Í¨Ñ¶²ÎÊý--------Ô­FD--¸Ä20130228 by mrb
    Uint16 commBaudRate;                    // F9-00  ²¨ÌØÂÊ
    Uint16 commParity;                      // F9-01  Êý¾Ý¸ñÊ½
    Uint16 commSlaveAddress;                // F9-02  ±¾»úµØÖ·
    Uint16 commDelay;                       // F9-03  Ó¦´ðÑÓ³Ù
    Uint16 commOverTime;                    // F9-04  Í¨Ñ¶³¬Ê±Ê±¼ä
    Uint16 commProtocol;                    // F9-05  Í¨Ñ¶ÊýÝ´ËÍñÊ½Ñ¡Ôñ   
    Uint16 commReadCurrentPoint;            // F9-06  Í¨Ñ¶¶ÁÈ¡µçÁ÷·Ö±æÂÊ
    Uint16 commProtocolSec;                 // F9-07  Í¨Ñ¶Ð­ÒéÑ¡Ôñ(À©Õ¹¿¨)
    Uint16 commMaster;                      // F9-08  Í¨Ñ¶Ö÷´Ó·½Ê½
    
//==================================================================
// FA ×ª¾Ø¿ØÖÆºÍÏÞ¶¨²ÎÊý--------Ô­A0--¸Ä20130228 by mrb

    Uint16 torqueCtrl;                      // FA-00  ×ª¾Ø¿ØÖÆ
    Uint16 driveUpperTorqueSrc;             // FA-01  Çý¶¯×ª¾ØÉÏÏÞÔ´¡Ôñ
    Uint16 driveUpperTorque;                // FA-02  Çý¶¯×ª¾ØÉÏÏÞÊý×ÖÉè¶¨
    Uint16 torqueCtrlAccTime;               // FA-03  ×ª¾Ø¼ÓËÙÊ±¼ä
    Uint16 torqueCtrlDecTime;               // FA-04  ×ª¾Ø¼õËÙÊ±¼ä
    Uint16 torqueCtrlFwdMaxFrq;             // FA-05  ×ª¾Ø¿ØÖÆÕýÏò×î´óÆµÂÊ
    Uint16 torqueCtrlRevMaxFrq;             // FA-06  ×ª¾Ø¿ØÖÆ·´Ïò×î´óÆµÂÊ
    Uint16 torqZeroZone;                    // FA-07  ×ª¾ØÂË²¨
    Uint16 brakeUpperTorqueSrc;             // FA-08  ÖÆ¶¯×ª¾ØÉÏÏÞÔ´
   
//==================================================================
// FB ¿ØÖÆÓÅ»¯²ÎÊý--------Ô­A5--Î´¸Ä20130228 by mrb

    Uint16 cbcEnable;                       // FB-00    Öð²¨ÏÞÁ÷Ê¹ÄÜ
    Uint16 uvPoint;                         // FB-01    Ç·Ñ¹µãÉèÖÃ
    Uint16 ovPointSet;                      // FB-02    ¹ýÑ¹µãÉèÖÃ    
    Uint16 deadCompMode;                    // FB-03    ËÀÇø²¹³¥Ä£Ê½Ñ¡Ôñ
    Uint16 curSampleDelayComp;              // FB-04    µçÁ÷¼ì²âÑÓÊ±¹³?
    Uint16 svcMode;                         // FB-05    SVCÓÅ»¯Ñ¡Ôñ 0-²»ÓÅ»¯  1-ÓÅ»¯Ä£Ê½1  2-ÓÅ»¯Ä£Ê½2
    Uint16 pwmMode;                         // FB-06    DPWMÇÐ»»ÉÏÏÞÆµÂÊ
    Uint16 modulationMode;                  // FB-07    µ÷ÖÆ·½Ê½£¬0-Òì²½µ÷ÖÆ£¬1-Í¬²½µ÷ÖÆ
    Uint16 softPwm;                         // FB-08    Ëæ»úPWM
    Uint16 deadTimeSet;                     // FB-09    ËÀÇøÊ±¼äµ÷Õû-1140V×¨ÓÃ

//==================================================================
// FC  
    Uint16 FC_rev;                          // FC  ±£Áô
    Uint16 prsCoe;                          // FC-01  ±ÈÀýÁª¶¯ÏµÊý
    Uint16 pidErrStart;                     // FC-02  »½ÐÑÆô¶¯Æ«²î(%)

//==================================================================
// FD  
    Uint16 fd[FDNUM];                       // FD  ±£Áô
//==================================================================
// FE  
    Uint16 fe[FENUM];                       // FE  ±£Áô
//==================================================================
// FF  
    Uint16 ff[FFNUM];                       // FF  ±£Áô

//======================================

//======================================
// E0 °ÚÆµ¡¢¶¨³¤ºÍ¼ÆÊý--------Ô­FB
//Ë³ÐòÃ»ÓÐ±ä¶¯--mrb 20130109
//Î´¸Ä20130228 by mrb
    Uint16 swingBaseMode;                   // E0-00  °ÚÆµÉè¶¨·½Ê½
    Uint16 swingAmplitude;                  // E0-01  °ÚÆµ·ù¶È
    Uint16 swingJumpRange;                  // E0-02  Í»ÌøÆµÂÊ·ù¶È
    Uint16 swingPeriod;                     // E0-03  °ÚÆµÖÜÆÚ
    Uint16 swingRiseTimeCoeff;              // E0-04  °ÚÆµµÄÈý½Ç²¨ÉÏÉýÊ±¼ä
    Uint16 lengthSet;                       // E0-05  Éè¶¨³¤¶È
    Uint16 lengthCurrent;                   // E0-06  Êµ¼Ê³¤¶È
    Uint16 lengthPulsePerMeter;             // E0-07  Ã¿Ã×Âö³åÊý£¬µ¥Î»: 0.1
    Uint16 counterSet;                      // E0-08  Éè¶¨¼ÆÊýÖµ
    Uint16 counterPoint;                    // E0-09  Ö¸¶¨¼ÆÊýÖµ

//======================================
// E1 ¶à¶ËËÙ¡¢PLC--------Ô­FC
//Ë³ÐòÃ»ÓÐ±ä¶¯--mrb 20130109
//Î´¸Ä20130228 by mrb
    Uint16 plcFrq[PLC_STEP_MAX];                  // E1-00  --E1-15   à¶Ö¸?-¶à¶ÎÖ¸Áî15
    Uint16 plcRunMode;                            // E1-16  PLCÔËÐÐ·½Ê½
    Uint16 plcPowerOffRemMode;                    // E1-17  PLCµôµç¼ÇÒäÑ¡Ôñ
    struct PLC_STRUCT plcAttribute[PLC_STEP_MAX]; // E1-18  --E1-49   PLCÔËÐÐÊ±¼ä£¬¼Ó¼õËÙÊ±¼äÑ¡Ôñ
    Uint16 plcTimeUnit;                           // E1-50  PLCÔËÐÐÊ±¼äµ¥Î»
    Uint16 plcFrq0Src;                            // E1-51  ¶à¶ÎÖ¸Áî0¸ø¶¨·½Ê½

//======================================
// E2 PID¹¦ÄÜ--------Ô­FA--¸Ä20130228 by mrb
    Uint16 pidSetSrc;                       // E2-00  PID¸ø¶¨Ô´
    Uint16 pidSet;                          // E2-01  PIDÊýÖµ¸ø¶¨, 0.1%
    Uint16 pidFdbSrc;                       // E2-02  PID·´À¡Ô´
    Uint16 pidDir;                          // E2-03  PID×÷ÓÃ·½Ïò
    Uint16 pidDisp;                         // E2-04  PID¸ø¶¨·´À¡Á¿³Ì
    Uint16 reverseCutOffFrq;                // E2-05  PID·´×ª½ØÖ¹ÆµÂÊ
    Uint16 pidErrMin;                       // E2-06  PIDÆ«²î¼«ÏÞ
    Uint16 pidDLimit;                       // E2-07  PIDÎ¢·ÖÏÞ·ù
    Uint16 pidSetChangeTime;                // E2-08  PID¸ø±ä»¯Ê±¼ä
    Uint16 pidFdbLpfTime;                   // E2-09  PID·´À¡ÂË²¨Ê±¼ä
    Uint16 pidOutLpfTime;                   // E2-10  PIDÊä³öÂË²¨Ê±¼ä
    Uint16 pidFdbLoseDetect;                // E2-11  PID·´À¡¶ªÊ§¼ì²âÖµ
    Uint16 pidFdbLoseDetectTime;            // E2-12  PID·´À¡¶ªÊ§¼ì²âÊ±¼ä
    Uint16 pidKp;                           // E2-13  ±ÈÀýÔöÒæP
    Uint16 pidTi;                           // E2-14  »ý·ÖÊ±¼äI
    Uint16 pidTd;                           // E2-15  Î¢·ÖÊ±¼äD
    Uint16 pidKp2;                          // E2-16  PID±ÈÀýÔöÒæP2
    Uint16 pidTi2;                          // E2-17  PID»ý·ÖÊ±¼äI2
    Uint16 pidTd2;                          // E2-18  PIDÎ¢·ÖÊ±¼äD2
    Uint16 pidParaChgCondition;             // E2-19  PID²ÎÊýÇÐ»»Ìõ¼þ
    Uint16 pidParaChgDelta1;                // E2-20  PID²ÎÊýÇÐ»»Æ«²î1
    Uint16 pidParaChgDelta2;                // E2-21  PID²ÎÊýÇÐ»»Æ«²î2
    Uint16 pidIAttribute;                   // E2-22  PID»ý·ÖÊôÐÔ
    Uint16 pidInit;                         // E2-23  PID³õÖµ
    Uint16 pidInitTime;                     // E2-24  PID³õÖµ±£³ÖÊ±¼ä
    Uint16 pidOutDeltaMax;                  // E2-25  PIDÁ½´ÎÊä³öÖ®¼äÆ«²îµÄ×î´óÖµ
    Uint16 pidOutDeltaMin;                  // E2-26  PIDÁ½´ÎÊä³öÖ®¼äÆ«²îµÄ×îÐ¡Öµ
    Uint16 pidCalcMode;                     // E2-27  PIDÔËËãÄ£Ê½(Í£»úÊÇ·ñÔËËã). ¹©Ë®Ä£Ê½ÏÂ£¬Í£»úÊ±PIDÒ²¼ÆËã.
    Uint16 pidSampleTime;                   // E2-28  PID²ÉÑùÖÜÆÚ(ÔÝÎ´×ö)

//======================================
// E3 ÐéÄâDI¡¢ÐéÄâDO--------Ô­A1
//¸Ä20130409 by mrb
    Uint16 vdiFunc[5];              // E3-00  --E3-04 VDI1¶Ë×Ó¹¦ÄÜÑ¡Ôñ
    Uint16 vdiFcSet;                // E3-05  VDI¶Ë×Ó¹¦ÄÜÂëÉè¶¨ÓÐÐ§×´Ì¬
    Uint16 vdiSrc;                  // E3-06  VDI¶Ë×ÓÓÐÐ§×´Ì¬À´Ô´
    Uint16 aiAsDiFunc[3];           // E3-07  --E3-09 AI1¶Ë×Ó¹¦ÄÜÑ¡Ôñ£¨µ±×÷DI£©
    Uint16 diLogicAiAsDi;           // E3-10  AI×÷ÎªDIÓÐÐ§×´Ì¬Ñ¡Ôñ
    Uint16 vdoFunc[5];              // E3-11  --E3-15 ÐéÄâVDO1¡«VDO5Êä³öÑ¡Ôñ
    Uint16 vdoLogic;                // E3-16  VDOÊä³ö¶Ë×ÓÓÐÐ§×´Ì¬Ñ¡Ôñ
    Uint16 vdoDelayTime[5];         // E3-17  --E3-21 VDO1¡«VDO5ÑÓ³ÙÊ±¼ä

//======================================
// E4 ÓÃ»§¶¨ÖÆ¹¦ÄÜÂë--------Ô­FE
//Ã»ÓÐ¸Ä±ä----mrb20130109
    Uint16 userCustom[E4NUM];       // E4 ÓÃ»§¶¨ÖÆ¹¦ÄÜÂë×é
//======================================
// E5 Ä£ÄâÁ¿ÇúÏß--------Ô­A6
    Uint16 curveSet4P1[8];          // E5-00    --E5-07  ÇúÏß4£¬4µã£¬×î´óÖµ£¬×îÐ¡Öµ£¬2¸öÖÐ¼äµã
    Uint16 curveSet4P2[8];          // E5-08    --E5-15  ÇúÏß5£¬4µã
    Uint16 rsvdA41[8];

    struct AI_JUMP aiJumpSet[AI_NUMBER]; // E5-24 --E5-29, AI1, AI2, AI3ÌøÔ¾
//======================================
// E6 ¹¦ÄÜÑ¡Ôñ--------Ô­A7
    Uint16 plcEnable;               // E6-00 PLC¿¨¹¦ÄÜÑ¡Ôñ
    Uint16 outPortControl;          // E6-01 Êä³ö¶Ë×Ó¿ØÖÆ
    Uint16 plcAI3Cfg;               // E6-02 PLC AI3¹¦ÄÜÅäÖÃ
    Uint16 fmpValue;                // E6-03 FMPÊä³ö 
    Uint16 ao1Value;                // E6-04 AO1Êä³ö
    Uint16 inPortOut;               // E6-05 ¿ª¹ØÁ¿Êä³ö
    Uint16 plcFrqSet;               // E6-06 PLC¿¨ÆµÂÊ¸ø¶¨
    Uint16 plcTorqueSet;            // E6-07 PLC¿¨×ª¾Ø¸ø¶¨
    Uint16 plcCmd;                  // E6-08 PLC¿¨ÃüÁî¸ø¶¨
    Uint16 plcErrorCode;            // E6-09 PLC¿¨¹ÊÕÏ¸ø¶¨
    Uint16 rsvdA7[2];
//======================================
// E7 µã¶ÔµãÍ¨Ñ¶¹¦ÄÜ--------Ô­A8
    Uint16 p2pEnable;               // E7-00 µã¶ÔµãÍ¨Ñ¶¹¦ÄÜ¡Ôñ
    Uint16 p2pTypeSel;              // E7-01 Ö÷´ÓÑ¡Ôñ
    Uint16 p2pSendDataSel;          // E7-02 Ö÷»ú·¢ËÍÊý¾Ý   0:Êä³ö×ª¾Ø  1:ÔËÐÐÆµÂÊ  2:Éè¶¨ÆµÂÊ  3:·´À¡ËÙ¶È
    Uint16 p2pRevDataSel;           // E7-03 ´Ó»ú½ÓÊÕÊý¾Ý   0:×ª¾Ø¸ø¶¨  1:ÆµÂÊ¸ø¶¨  
    Uint16 p2pRevOffset;            // E7-04 ½ÓÊÜÊý¾ÝÁãÆ«
    Uint16 p2pRevGain;              // E7-05 ½ÓÊÕÊý¾ÝÔöÒæ
    Uint16 p2pOverTime;             // E7-06 µã¶ÔµãÍ¨Ñ¶ÖÐ¶Ï¼ì²âÊ±¼ä
    Uint16 p2pSendPeriod;           // E7-07 µã¶ÔµãÍ¨Ñ¶Ö÷»úÊý¾Ý·¢ËÍÖÜÆÚ
//======================================
// E8 ±£Áô--------Ô­A9
    Uint16 e8[E8NUM];               // E8
//======================================
// E9 Ê¸Á¿ÓÅ»¯²ÎÊý--------Ô­AA
    Uint16 e9[E9NUM];               // E9
    #if 0
    Uint16 motorCtrlM1;             // E9-00 Àø´Åµ÷Õû·½Ê½
    Uint16 motorCtrlM2;             // E9-01 ×î´óµçÑ¹¼ÆËã·½Ê½
    Uint16 motorCtrlM3;             // E9-02 Àø´ÅµçÁ÷µ÷½ÚÆ÷KP
    Uint16 motorCtrlM4;             // E9-03 Àø´ÅµçÁ÷µ÷½ÚÆ÷KI
    Uint16 motorCtrlM5;             // E9-04 Àø´ÅµçÁ÷ÕýÏòÐÞÕýÁ¿
    Uint16 motorCtrlM6;             // E9-05 Àø´ÅµçÁ÷·´ÏòÐÞÕýÁ¿
    Uint16 motorCtrlM7;             // E9-06 ×ª²îµ÷½ÚÉÏÏÞ
    Uint16 motorCtrlM8;             // E9-07 ×ª²îµ÷ÚÔöÒæ
    Uint16 motorCtrlM9;             // E9-08 »¥¸ÐÔöÒæ
    Uint16 motorCtrlM10;            // E9-09 Êä³öÆµÂÊÐÞÕýÄ£Ê½
    Uint16 motorCtrlM11;            // E9-10 µçÑ¹ÐÞÕýãÐÖµµ÷Õû
    Uint16 motorCtrlM12;            // E9-11 µçÑ¹ÐÞÕýÔöÒæ
    Uint16 motorCtrlM13;            // E9-12 ËÙ¶È»·µ÷Õû
    Uint16 motorCtrlM14;            // E9-13 Ðý±ä¼ì²âÂË²¨
    Uint16 motorCtrlM15;            // E9-14 Ðý±ä½Ç¶È²¹³¥
    Uint16 motorCtrlM16;            // E9-15 SVC×ª¾Ø¿ØÖÆÓÅ»¯
    #endif
//======================================
// EA VFÓÅ»¯²ÎÊý--------Ô­AB
    Uint16 ea[EANUM];               // EA
    #if 0
    Uint16 vfCtrlM2;                // EA-01 DPWMÇÐ»»ÏÂÏÞÆµÂÊ   86
    Uint16 vfCtrlM3;                // EA-02 ËÀÇø²¹³¥ÓÅ»¯¿ªÆô   87
    Uint16 vfCtrlM4;                // EA-03 ËÀÇøÇ¯Î»²¹³¥ÏµÊý    1
    Uint16 vfCtrlM5;                // EA-04 Ç¯Î»ÓÅ»¯ÏÂÏÞÆµÂÊ   101
    Uint16 vfCtrlM6;                // EA-05 Ç¯Î»ÓÅ»¯ÉÏÏÞÆµÂÊ   102
    Uint16 vfCtrlM7;                // EA-06 Õñµ´ÒÖÖÆÄ£Ê½       89
    Uint16 vfCtrlM8;                // EA-07 Õñµ´ÒÖÖÆ·ùÖµµ÷Õû   90
    #endif
//======================================
// EB ±£Áô    
    Uint16 eb[EBNUM]; 
//======================================
// EC ±£Áô--------Ô­AD
    Uint16 ec[ECNUM]; 
//======================================
// ED AIAOÐ£Õý------Ô­AC
    struct ANALOG_CALIBRATE_CURVE aiCalibrateCurve[AI_NUMBER];  // ED-00    ----ED-11, AI1/2/3Ð£ÕýÇúÏß
    struct ANALOG_CALIBRATE_CURVE aoCalibrateCurve[AO_NUMBER];  // ED-12    ----ED-19, AO1/AO2Ð£ÕýÇúÏß
//======================================
// EE ±£Áô--------Ô­AF
    Uint16 ee[EENUM];               // EE
//======================================
// EF ±£Áô
    Uint16 ef[EFNUM];               // EF

//======================================
// B0 µÚÒ»µç»ú²ÎÊý------Ô­F1,F1-37  µ÷Ð³Ñ¡ÔñÒÆµ½F7×é--¸¨Öú¹¦ÄÜ×é
    union MOTOR_PARA motorParaM1;   // B0-00  B0-26 µÚ1µç»ú²ÎÊý
    Uint16 tuneCmd;                 // B0-27  µ÷Ð³Ñ¡Ôñ-----ÓÐµ÷ÕûÐòºÅ£º37->27
    union PG_PARA    pgParaM1;      // B0-28  B0-37 µÚ1µç»úPG¿¨²ÎÊý
//======================================
// B1 µÚ2µç»ú²ÎÊý------Ô­A2
    struct MOTOR_FC motorFcM2;      // µÚ2µç»ú²ÎÊý
//======================================
// B2 µÚ3µç»ú²ÎÊý------Ô­A3
    struct MOTOR_FC motorFcM3;      // µÚ3µç»ú²ÎÊý
    
// B3 µÚ4µç»ú²ÎÊý------Ô­A4
    struct MOTOR_FC motorFcM4;      // µÚ4µç»ú²ÎÊý
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
// y0 ÓÃ»§ÃÜÂë, ²ÎÊý³õÊ¼»¯--------Ô­FP
//Î´¸Ä20130228 by mrb
    Uint16 paraInitMode;            // Y0-00  ²ÎÊý³õÊ¼»¯
    Uint16 userPassword;            // Y0-01  ÓÃ»§ÃÜÂë
    Uint16 funcParaView;            // Y0-02  ¹¦ÄÜ²ÎÊýÄ£Ê½ÊôÐÔ
    Uint16 menuMode;                // Y0-03  ¸öÐÔ»¯²ÎÊýÄ£Ê½Ñ¡Ôñ
    Uint16 userPasswordReadOnly;    // Y0-04  Ö»¶ÁÓÃ»§ÃÜÂë
    Uint16 rsvdFp;                  // Y0-05  ±£Áô
//======================================
// y1 ¹ÊÕÏÀúÊ·²é----------Ô­F9ÖÐµÄF9.14~F9.44
    Uint16 errorLatest1;                // Y1-00  µÚÒ»´Î¹ÊÕÏÀàÐÍ
    
    Uint16 errorLatest2;                // Y1-01  µÚ¶þ´Î¹ÊÕÏÀàÐÍ
    Uint16 errorLatest3;                // Y1-02  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÀàÐÍ

    union ERROR_SCENE errorScene3;      // Y1-03  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±ÆµÂÊ                              
                                        // Y1-04  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±µçÁ÷                             
                                        // Y1-05  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Ä¸ÏßµçÑ¹                                                                                  
                                        // Y1-06  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±ÊäÈë¶Ë×Ó×´Ì¬                     
                                        // Y1-07  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Êä³ö¶Ë×Ó×´Ì¬   
                                        
                                        // Y1-08  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±±äÆµÆ÷×´Ì¬                       
                                        // Y1-09  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Ê±¼ä£¨´Ó±¾´ÎÉÏµç¿ªÊ¼¼ÆÊ±£©       
                                        // Y1-10  µÚÈý´Î(×î½üÒ»´Î)¹ÊÕÏÊ±Ê±¼ä£¨´ÓÔËÐÐÊ±¿ªÊ¼¼ÆÊ±£©         

    Uint16 rsvdF92[2];                  // Y1-11  Y1-12
    
    union ERROR_SCENE errorScene2;      // Y1-13  µÚ¶þ´Î¹ÊÕÏÏÖ³¡
    Uint16 rsvdF921[2];                 // Y1-21  Y1-22
    union ERROR_SCENE errorScene1;      // Y1-23  µÚÒ»´Î¹ÊÕÏÏÖ³¡
//======================================
    Uint16 y2[Y2NUM];               // ±£Áô
    Uint16 y3[Y3NUM];               // ±£Áô
    Uint16 y4[Y4NUM];               // ±£Áô
    Uint16 y5[Y5NUM];               // ±£Áô
    Uint16 y6[Y6NUM];               // ±£Áô
    Uint16 y7[Y7NUM];               // ±£Áô
//======================================
// L0 ³§¼Ò²ÎÊý--------Ô­FF
    Uint16 factoryPassword;         // L0-00  ³§¼ÒÃÜÂë
    Uint16 inverterType;            // L0-01  ±äÆµÆ÷»úÐÍ
    Uint16 inverterGpType;          // L0-02  G/PÐÍºÅ
    Uint16 inverterPower;           // L0-03  ±äÆµÆ÷¹¦ÂÊ
    Uint16 tempCurve;               // L0-04  ÎÂ¶ÈÇúÏß
    
    Uint16 uvGainWarp;              // L0-05  UVÁ½ÏàµçÁ÷²ÉÑùÔöÒæÆ«²î
    Uint16 funcSoftVersion;         // L0-06  ±£Áô
    Uint16 motorSoftVersion;        // L0-07  ÐÔÄÜÈí¼þ°æ±¾ºÅ
    Uint16 volJudgeCoeff;           // L0-08  µçÑ¹Ð£ÕýÏµÊý
    Uint16 curJudgeCoeff;           // L0-09  µçÁ÷Ð£ÕýÏµÊý
    
    Uint16 motorDebugFc;            // L0-10  ÐÔÄÜµ÷ÊÔ¹¦ÄÜÂëÏÔÊ¾¸öÊý
    Uint16 aiaoCalibrateDisp;       // L0-11  AIAOÐ£Õý¹¦ÄÜÂëÏÔÊ¾
    Uint16 memoryAddr;              // L0-12  ÄÚ´æµØÖ·²é¿´
//	Uint16 runTimeArriveSet1;       // L0-13  ³§¼ÒÉè¶¨ÔËÐÐÊ±¼ä
//======================================
 
// L1 AIAO³ö³§Ð£ÕýÖµ--------Ô­AE
    struct ANALOG_CALIBRATE_CURVE aiFactoryCalibrateCurve[AI_NUMBER];   // L1-00 
    struct ANALOG_CALIBRATE_CURVE aoFactoryCalibrateCurve[AO_NUMBER];   // L1-12
    
    Uint16 l2[L2NUM];               // ±£Áô 
    Uint16 l3[L3NUM];               // ±£Áô
    Uint16 l4[L4NUM];               // ±£Áô
    Uint16 l5[L5NUM];               // ±£Áô
    Uint16 l6[L6NUM];               // ±£Áô
    Uint16 l7[L7NUM];               // ±£Áô
    Uint16 l8[L8NUM];               // ±£Áô   
    Uint16 l9[L9NUM];               // ±£Áô   



//======================================
// eepromCheckWord
    Uint16 rsvd4All;                // ±£Áô£¬·ÅÔÚ×îÇ°Ãæ
    Uint16 eepromCheckWord1;        //        eepromCheckWord1
    Uint16 eepromCheckWord2;        //        eepromCheckWord2
    Uint16 aiaoChkWord;             // AIAO³ö³§Ð£Õý

//======================================
// REMEMBER µôµç±£´æ£¬¹²48¸ö
    Uint16 extendType;                  // FR-00  extendType
    Uint16 plcStepRem;                  // FR-01  PLCµ±Ç°step
    Uint16 plcTimeHighRem;              // FR-02  PLCµ±Ç°stepÔËÐÐµÄÊ±¼ä£¬¸ßÎ»
    Uint16 plcTimeLowRem;               // FR-03  PLCµ±Ç°stepÔËÐÐµÄÊ±¼ä£¬µÍÎ»
    Uint16 dispParaRunBit;              // FR-04  ÔËÐÐÊ±LEDÏÔÊ¾²ÎÊýµÄbitÎ»Öµ
    Uint16 dispParaStopBit;             // FR-05  Í£»úÊ±LEDÏÔÊ¾²ÎÊýµÄbitÎ»ÖÃ
    Uint16 runTimeAddupSec;             // FR-06  ÀÛ¼ÆÔËÐÐÊ±¼äµÄs(Ãë)
    Uint16 counterTicker;               // FR-07  ¼ÆÊýÆ÷ÊäÈëµÄticker
    Uint16 lengthTickerRemainder;       // FR-08  ³¤¶È¼ÆÊýÆ÷µÄtickerDeltaµÄRemainder
    Uint16 frqComm;                     // FR-09  Í¨Ñ¶ÐÞ¸ÄÆµÂÊÖµ, 100.00%-maxFrq
    Uint16 upDownFrqRem;                // FR-10  UP/DOWNµÄÆµÂÊ
    Uint16 pmsmRotorPos;                // FR-11  Í¬²½»ú×ª×ÓÎ»ÖÃ
    Uint16 powerAddupInt;               // FR-12  ÀÛ¼ÆºÄµçÁ¿¸¨Öú¼ÆËã
    Uint16 powerUpTimeAddupSec;         // FR-13  ÀÛ¼ÆÉÏµçÊ±¼äµÄs(Ãë)
    Uint16 errorFrqUnit;                // FR-14  ¹ÊÕÏÊ±ÆµÂÊ¼ÇÂ¼
    Uint16 saveUserParaFlag1;           // FR-15  ÒÑ±£´æÓÃ»§²ÎÊý±êÖ¾1
    Uint16 saveUserParaFlag2;           // FR-16  ÒÑ±£´æÓÃ»§²ÎÊý±êÖ¾2
    Uint16 speedFdbDir;                 // FR-17  µç»ú·´À¡ËÙ¶È·½Ïò
    Uint16 rsvdRem[2];                  // FR-18~FR-19
    Uint16 rsvdRem1[23];                // Ô¤Áô
    Uint16 remPOffMotorCtrl[REM_P_OFF_MOTOR];  // FR-43~FR-47  ÐÔÄÜÊ¹ÓÃµÄµôµç¼ÇÒä
//======================================
// D0 ¼àÊÓ²ÎÊý--------Ô­U0
    Uint16 d0[D0NUM];               // D0 ÏÔÊ¾
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
    Uint16 df[DFNUM];               // DF, ÏÔÊ¾£¬ÐÔÄÜµ÷ÊÔÊ¹ÓÃ
//======================================
};


//=====================================================================
//
// ¹¦ÄÜÂëµÄ¶¨Òå¡£
// ÁªºÏÌå£¬³ÉÔ±·Ö±ðÎªÊý×é£¬½á¹¹Ìå£¬½á¹¹Ìå
// ÓÚÊÇ£¬Ò»¸ö¹¦ÄÜÂëµÄ·ÃÎÊ£¬ÓÐÈýÖÖ·½Ê½:
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


// Ö÷¸¨ÆµÂÊÔ´Ñ¡Ôñ
#define FUNCCODE_frqXySrc_FC                0   // ¹¦ÄÜÂëÉè¶¨£¬ôµç²»¼ÇÒ?
#define FUNCCODE_frqXySrc_FC_P_OFF_REM      1   // ¹¦ÄÜÂëÉè¶¨£¬µôµç¼ÇÒä
#define FUNCCODE_frqXySrc_AI1               2   // AI1
#define FUNCCODE_frqXySrc_AI2               3   // AI2
#define FUNCCODE_frqXySrc_AI3               4   // keyPanel rev by qtl 20131218 for Keyboard potentiometer
#define FUNCCODE_frqXySrc_PULSE             5   // PULSEÂö³åÉè¶¨(DI5)
#define FUNCCODE_frqXySrc_MULTI_SET         6   // ¶à¶ÎÖ¸Áî
#define FUNCCODE_frqXySrc_PLC               7   // PLC
#define FUNCCODE_frqXySrc_PID               8   // PID
#define FUNCCODE_frqXySrc_COMM              9   // Í¨Ñ¶Éè¶¨
//#define FUNCCODE_frqXySrc_AI3               10   // AI3,add by qtl 20131218 for Keyboard potentiometer

// ¸¨ÖúÆµÂÊÔ´Y·¶Î§Ñ¡Ôñ
#define FUNCCODE_frqYRangeBase_MAX_FRQ      0   // Ïà¶ÔÓÚ×î´óÆµÂÊ
#define FUNCCODE_frqYRangeBase_FRQ_X        1   // Ïà¶ÔÓÚÖ÷ÆµÂÊÔ´X

// ÆµÂÊÔ´(ÇÐ»»¹ØÏµ)Ñ¡Ôñ
#define FUNCCODE_frqCalcSrc_X               0   // Ö÷ÆµÂÊÔ´X
#define FUNCCODE_frqCalcSrc_COMPOSE         1   // Ö÷¸¨ÔËËã½á¹û
#define FUNCCODE_frqCalcSrc_X_OR_Y          2   // Ö÷ <--> ¸¨
#define FUNCCODE_frqCalcSrc_X_OR_COMPOSE    3   // Ö÷ <--> Ö÷¸¨ÔËËã½á¹û
#define FUNCCODE_frqCalcSrc_Y_OR_COMPOSE    4   // ¸¨ <--> Ö÷¸¨ÔËËã½á¹û

// Ö÷¸¨ÆµÂÊÔËËã¹ØÏµ
#define FUNCCODE_frqCalcSrc_ADD             0   // Ö÷ + ¸¨
#define FUNCCODE_frqCalcSrc_SUBTRATION      1   // Ö÷ - ¸¨
#define FUNCCODE_frqCalcSrc_MAX             2   // MAX(Ö÷, ¸¨)
#define FUNCCODE_frqCalcSrc_MIN             3   // MIN(Ö÷, ¸¨)
#define FUNCCODE_frqCalcSrc_4               4   //Ö÷*¸¨/×î´óÆµÂÊ rcv by qtl 20130912
#define FUNCCODE_frqCalcSrc_5               5   // 

// ÉÏÏÞÆµÂÊÔ´
#define FUNCCODE_upperFrqSrc_FC         0   // ¹¦ÄÜÂëÉè¶¨
#define FUNCCODE_upperFrqSrc_AI1        1   // AI1
#define FUNCCODE_upperFrqSrc_AI2        2   // AI2
#define FUNCCODE_upperFrqSrc_AI3        3   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_upperFrqSrc_keyPanel    3   //keyPanel,rev by qtl 20131218 for Keyboard potentiometer
#define FUNCCODE_upperFrqSrc_PULSE      4   // PULSEÂö³åÉè¶¨(DI5)
#define FUNCCODE_upperFrqSrc_COMM       5   // Í¨Ñ¶¸ø¶¨
//#define FUNCCODE_upperFrqSrc_AI3        6   // AI3,add by qtl 20131218 for Keyboard potentiometer

// ÆµÂÊÖ¸ÁîÐ¡Êýµã
#define FUNCCODE_frqPoint_1             0   // 0: 0¸öÐ¡Êýµã£¬1Hz
#define FUNCCODE_frqPoint_0_1           1   // 1: 1¸öÐ¡Êýµã£¬0.1Hz
#define FUNCCODE_frqPoint_0_01          2   // 2: 2¸öÐ¡Êýµã£¬0.01Hz

// ÔØ²¨ÆµÂÊµ÷ÕûÑ¡Ôñ
//#define FUNCCODE_autoCarrierFrq_0

// ¼Ó¼õËÙÊ±¼äµÄµ¥Î»
#define FUNCCODE_accDecTimeUnit_0POINT  0   // 0¸öÐ¡Êýµã£¬1s
#define FUNCCODE_accDecTimeUnit_1POINT  1   // 1¸öÐ¡Êýµã£¬0.1s
#define FUNCCODE_accDecTimeUnit_2POINT  2   // 2¸öÐ¡Êýµã£¬0.01s

// ÊýÖµÉè¶¨ÆµÂÊ¼ÇÒäÉè¶¨
#define FUNCCODE_frqRemMode_POWEROFF_NO     0   // µôµç²»¼ÇÒä
#define FUNCCODE_frqRemMode_POWEROFF_YES    1   // µôµç¼ÇÒä
#define FUNCCODE_frqRemMode_STOP_NO         0   // Í£»ú²»¼ÇÒä
#define FUNCCODE_frqRemMode_STOP_YES        1   // Í£»ú¼ÇÒä

// ¼Ó¼õËÙ·½Ê½
#define FUNCCODE_accDecSpdCurve_LINE        0   // Ö±Ïß¼Ó¼õËÙ
#define FUNCCODE_accDecSpdCurve_S_CURVE_A   1   // SÇúÏß1£¬ÆÕÍ¨¶þ´Î·½
#define FUNCCODE_accDecSpdCurve_S_CURVE_B   2   // SÇúÏß2£¬²Î¿¼ÈýÁâSÇúÏßB
#define ACC_DEC_LINE    FUNCCODE_accDecSpdCurve_LINE
#define ACC_DEC_SA      FUNCCODE_accDecSpdCurve_S_CURVE_A
#define ACC_DEC_SB      FUNCCODE_accDecSpdCurve_S_CURVE_B

// ×ª¾ØÉÏÏÞÔ´
#define FUNCCODE_upperTorqueSrc_FC      0   // ¹¦ÄÜÂëÉè¶¨
#define FUNCCODE_upperTorqueSrc_AI1     1   // AI1
#define FUNCCODE_upperTorqueSrc_AI2     2   // AI2
#define FUNCCODE_upperTorqueSrc_AI3     3 // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_upperTorqueSrc_keyPanel  3   // keyPanel,rev by qtl 20131218 for Keyboard potentiometer

#define FUNCCODE_upperTorqueSrc_PULSE   4   // PULSE
#define FUNCCODE_upperTorqueSrc_COMM    5   // Í¨¶
#define FUNCCODE_upperTorqueSrc_MIN_AI1_AI2 6  // min(ai1,ai2)
#define FUNCCODE_upperTorqueSrc_MAX_AI1_AI2 7  // max(ai1,ai2)
//#define FUNCCODE_upperTorqueSrc_AI3     8   // AI3,add by qtl 20131218 for Keyboard potentiometer
// FVCµÄPG¿¨Ñ¡Ôñ, 0-QEP1(±¾µØPG),1-QEP2(À©Õ¹PG)
#define FUNCCODE_fvcPgSrc_QEP1          0   // QEP1
#define FUNCCODE_fvcPgSrc_QEP2          1   // QEP2, À©Õ¹PG¿¨

#define TIME_UNIT_ACC_DEC_SPEED         100 // ¼Ó¼õËÙÊ±¼ä¥Î», ms


// VFÇúÏßÉè¶¨
#define FUNCCODE_vfCurve_Line               0   // Ö±ÏßVF
#define FUNCCODE_vfCurve_DOT                1   // ¶àµãVF
#define FUNCCODE_vfCurve_SQUARE             2   // Æ½·½VF
#define FUNCCODE_vfCurve_ALL_SEPARATE       10  // VFÍêÈ«·ÖÀëÄ£Ê½
#define FUNCCODE_vfCurve_HALF_SEPARATE      11  // VF°ë·ÖÀëÄ£Ê½

// vfVoltageSrc, VF·ÖÀëµÄµçÑ¹Ô´
#define FUNCCODE_vfVoltageSrc_FC            0   // ¹¦ÄÜÂëÉè¶¨
#define FUNCCODE_vfVoltageSrc_AI1           1   // AI1
#define FUNCCODE_vfVoltageSrc_AI2           2   // AI2
#define FUNCCODE_vfVoltageSrc_AI3           3   // AI3,del by qtl 20131218 for Keyboard potentiometer 
//#define FUNCCODE_vfVoltageSrc_keyPanel      3   //keyPane,rev by qtl 20131218 for Keyboard potentiometer 

#define FUNCCODE_vfVoltageSrc_PULSE         4   // PULSEÂö³åÉè¶¨(DI5)
#define FUNCCODE_vfVoltageSrc_MULTI_SET     5   // ¶à¶ÎÖ¸Áî
#define FUNCCODE_vfVoltageSrc_PLC           6   // PLC
#define FUNCCODE_vfVoltageSrc_PID           7   // PID
#define FUNCCODE_vfVoltageSrc_COMM          8   // Í¨Ñ¶Éè¶¨
//#define FUNCCODE_vfVoltageSrc_AI3           9   // AI3,add by qtl 20131218 for Keyboard potentiometer 

// Î»ÖÃ¿ØÖÆÑ¡Ôñ
#define FUNCCODE_posCtrl_NONE               0   // ·ÇÎ»ÖÃ¿ØÖÆ
#define FUNCCODE_posCtrl_POSITION_CTRL      1   // Î»ÖÃ¿ØÖÆ
#define FUNCCODE_posCtrl_SWITCH_TO_PC       2   // ËÙ¶È/×ª¾Ø¿ØÖÆ<->Î»ÖÃ¿ØÖÆ
#define FUNCCODE_posCtrl_SWITCH_FROM_PC     3   // Î»ÖÃ¿ØÖÆ<->ËÙ¶È/×ª¾Ø¿ØÖÆ

// Î»ÖÃ¿ØÖÆÄ£Ê½
#define FUNCCODE_pcMode_PCMD            0   // Pcmd
#define FUNCCODE_pcMode_APTP            1   // APTP
#define FUNCCODE_pcMode_SWITCH_TO_APTP  2   // Pcmd<->AP2P

// Î»ÖÃÖ¸ÁîÂö³å·½Ê½
#define FUNCCODE_pcPulseType_PULSE_AND_DIR  0   // Âö³å+·½Ïò
#define FUNCCODE_pcPulseType_QUADRATURE     1   // 2Â·Õý½»Âö³å
#define FUNCCODE_pcPulseType_CW_AND_CCW     2   // CW+CCW

// ¶¨Î»¿ØÖÆÄ£Ê½
#define FUNCCODE_aptpMode_RELATIVE      0   // Ïà¶ÔÊ½
#define FUNCCODE_aptpMode_ABSOLUTE      1   // ¾ø¶ÔÊ½
#define FUNCCODE_aptpMode_INDEX         2   // ·Ö¶ÈÅÌ

// Î»ÖÃ¿ØÖÆÁãµãÑ¡Ôñ
#define FUNCCODE_pcZeroSelect_ENCODER   0   // ±àÂëÆ÷indexÐÅºÅ
#define FUNCCODE_pcZeroSelect_DI        1   // DI¶Ë×Ó

// PG¿¨°²×°Î»ÖÃ
#define FUNCCODE_pgLocation_MOTOR       0   // µç»úÖá
#define FUNCCODE_pgLocation_AXIS        1   // »ú´²Ö÷Öá

//=====================================================================
// (µç»ú)¿ØÖÆ·½Ê½

#define FUNCCODE_motorCtrlMode_SVC  0   // SVC
#define FUNCCODE_motorCtrlMode_FVC  1   // FVC
#define FUNCCODE_motorCtrlMode_VF   2   // VF

// ÃüÁîÔ´Ñ¡Ôñ
#define FUNCCODE_runSrc_PANEL       0   // ²Ù×÷Ãæ°å¿ØÖÆÍ¨µÀ
#define FUNCCODE_runSrc_DI          1   // ¶Ë×ÓÃüÁîÍ¨µÀ
#define FUNCCODE_runSrc_COMM        2   // ´®ÐÐ¿ÚÍ¨Ñ¶¿ØÖÆÍ¨µÀ
#define FUNCCODE_runSrc_AUTO_RUN    3   // ÉÏµçÔËÐÐ

// ÔËÐÐ·½Ïò
#define FUNCCODE_runDir_NO_REVERSE      0   // ½ÏÒ»ÖÂ
#define FUNCCODE_runDir_REVERSE         1   // ·½ÏòÏà·´

// µ÷Ð³Ñ¡Ôñ
#define FUNCCODE_tuneCmd_NONE           0   // ÎÞ²Ù×÷
#define FUNCCODE_tuneCmd_ACI_STATIC     1   // Òì²½»ú¾²Ö¹µ÷Ð³
#define FUNCCODE_tuneCmd_ACI_WHOLE      2   // Òì²½»úÍêÕûµ÷Ð³
#define FUNCCODE_tuneCmd_PMSM_11        11  // Í¬²½»ú
#define FUNCCODE_tuneCmd_PMSM_12        12  // Í¬²½»ú
#define FUNCCODE_tuneCmd_PMSM_13        13  // Í¬²½»ú

// ¶Ë×ÓÃüÁî·½Ê½
#define FUNCCODE_diControlMode_2LINE1   0   // Á½ÏßÊ½1
#define FUNCCODE_diControlMode_2LINE2   1   // Á½ÏßÊ½2
#define FUNCCODE_diControlMode_3LINE1   2   // ÈýÏßÊ½1
#define FUNCCODE_diControlMode_3LINE2   3   // ÈýÏßÊ½2

// ¶à¹¦ÄÜ¶Ë×ÓÊä³öÑ¡Ôñ
#define FUNCCODE_fmOutSelect_PULSE      0   // FMPÂö³åÊä³ö
#define FUNCCODE_fmOutSelect_DO         1   // DO
#define FUNCCODE_fmOutSelect_AO         2   // AO

// Æô¶¯·½Ê½
#define FUNCCODE_startMode_DIRECT_START 0   // Ö±½ÓÆô¶¯
#define FUNCCODE_startMode_SPEED_TRACK  1   // ×ªËÙ¸ú×ÙÔÙÆô¶¯
#define FUNCCODE_startMode_FORE_MAG     2   // Òì²½»úÀø´ÅÆô¶¯

// Í£»ú·½Ê½
#define FUNCCODE_stopMode_DEC_STOP      0   // ¼õËÙÍ£»ú
#define FUNCCODE_stopMode_FREESTOP      1   // ×ÔÓÉÍ£»ú
#define FUNCCODE_stopMode_HURRY_STOP    2   // ¼±Í£Í£»ú

// ÆµÂÊµÍÓÚÏÂÏÞÆµÂÊÔËÐÐ¶¯×÷
#define FUNCCODE_lowerDeal_RUN_LOWER    0   // ÒÔÏÂÏÞÆµÂÊÔËÐÐ
#define FUNCCODE_lowerDeal_DELAY_STOP   1   // ÑÓÊ±Í£»ú
#define FUNCCODE_lowerDeal_RUN_ZERO     2   // ÁãËÙÔËÐÐ

// Éè¶¨ÔËÐÐÊ±¼äµ½´ï¶¯×÷Ñ¡Ôñ
#define FUNCCODE_runTimeOverAct_RUN     0   // ¼ÌÐøÔËÐÐ
#define FUNCCODE_runTimeOverAct_STOP    1   // Í£»ú

// Éè¶¨ÉÏµçÊ±¼äµ½´ï¶¯×÷Ñ¡Ôñ
#define FUNCCODE_powerUpTimeOverAct_RUN     0   // ¼ÌÐøÔËÐÐ
#define FUNCCODE_powerUpTimeOverAct_STOP    1   // Í£»ú

// PID¸ø¶¨Ô´
#define FUNCCODE_pidSetSrc_FC               0   // ¹¦ÄÜÂëÉè¶¨
#define FUNCCODE_pidSetSrc_AI1              1   // AI1
#define FUNCCODE_pidSetSrc_AI2              2   // AI2
#define FUNCCODE_pidSetSrc_AI3              3   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_pidSetSrc_keyPanel              3   // keyPanel,rev by qtl 20131218 for Keyboard potentiometer

#define FUNCCODE_pidSetSrc_PULSE            4   // PULSE
#define FUNCCODE_pidSetSrc_COMM             5   // Í¨?
#define FUNCCODE_pidSetSrc_MULTI_SET        6   // ¶à¶ÎÖ¸Áî
//#define FUNCCODE_pidSetSrc_AI3              7   // AI3,add by qtl 20131218 for Keyboard potentiometer

// PID·´À¡Ô´
#define FUNCCODE_pidFdbSrc_AI1              0   // AI1
#define FUNCCODE_pidFdbSrc_AI2              1   // AI2
#define FUNCCODE_pidFdbSrc_AI3              2   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_pidFdbSrc_keyPanel         2   // AI3,rev by qtl 20131218 for Keyboard potentiometer
#define FUNCCODE_pidFdbSrc_AI1_SUB_AI2      3   // AI1-AI2
#define FUNCCODE_pidFdbSrc_PULSE            4   // PULSE
#define FUNCCODE_pidFdbSrc_COMM             5   // Í¨Ñ¶
#define FUNCCODE_pidFdbSrc_AI1_ADD_AI2      6   // AI1+AI2
#define FUNCCODE_pidFdbSrc_MAX_AI           7   // MAX(|AI1|, |AI2|)
#define FUNCCODE_pidFdbSrc_MIN_AI           8   // MIN(|AI1|, |AI2|)
//#define FUNCCODE_pidFdbSrc_AI3              9   // AI3,add by qtl 20131218 for Keyboard potentiometer

// PID²ÎÊýÇÐ»»Ìõ¼þ
#define FUNCCODE_pidParaChgCondition_NO         0   // ²»ÇÐ»»
#define FUNCCODE_pidParaChgCondition_DI         1   // DI¶Ë×Ó
#define FUNCCODE_pidParaChgCondition_PID_ERROR  2   // ¸ù¾ÝÆ«²î×Ô¶¯ÇÐ»»

// PIDÔËËãÄ£Ê½
#define FUNCCODE_pidCalcMode_NO             0   // Í£»úÊ±²»ÔËËã
#define FUNCCODE_pidCalcMode_YES            1   // Í£»úÊ±ÔËËã

// °ÚÆµÉè¶¨·½Ê½
#define FUNCCODE_swingBaseMode_AGAIN_FRQSETAIM  0   // Ïà¶ÔÓÚÖÐÐÄÆµÂÊ(Éè¶¨ÆµÂÊ)
#define FUNCCODE_swingBaseMode_AGAIN_MAXFRQ     1   // Ïà¶ÔÓÚ×î´óÆµÂÊ

// MF.K¼ü¹¦ÄÜÑ¡Ôñ
#define FUNCCODE_mfkKeyFunc_NONE        0   // MF.K¼ü¹¦ÄÜÎÞÐ§
#define FUNCCODE_mfkKeyFunc_SWITCH      1   // Óë²Ù×÷Ãæ°åÍ¨µÀÇÐ»»
#define FUNCCODE_mfkKeyFunc_REVERSE     2   // Õý·´×ªÇÐ»»
#define FUNCCODE_mfkKeyFunc_FWD_JOG     3   // Õý×ªµã¶¯ÃüÁî
#define FUNCCODE_mfkKeyFunc_REV_JOG     4   // ·´×ªµã¶¯ÃüÁî

// STOP/RES¼ü¹¦ÄÜ
#define FUNCCODE_stopKeyFunc_KEYBOARD   0   // Í£»ú¹¦ÄÜ½öÔÚ¼üÅÌ¿ØÖÆ·½Ê½Ê±ÓÐÐ§
#define FUNCCODE_stopKeyFunc_ALL        1   // ¾ùÓÐÐ§

// ¶à¶ÎÖ¸Áî0¸ø¶¨·½Ê½
#define FUNCCODE_plcFrq0Src_FC          0   // ¹¦ÄÜÂëFC-00¸ø¶¨
#define FUNCCODE_plcFrq0Src_AI1         1   // AI1
#define FUNCCODE_plcFrq0Src_AI2         2   // AI2
#define FUNCCODE_plcFrq0Src_AI3         3   // AI3,del by qtl 20131218 for Keyboard potentiometer
//#define FUNCCODE_plcFrq0Src_keyPanel    3   // keyPanel,rev by qtl 20131218 for Keyboard potentiometer 
#define FUNCCODE_plcFrq0Src_PULSE       4   // PULSE
#define FUNCCODE_plcFrq0Src_PID         5   // PID¸ø¶¨
#define FUNCCODE_plcFrq0Src_PRESET_FRQ  6   // ¤ÖÃÆµÂÊ
//#define FUNCCODE_plcFrq0Src_AI3         7   // AI3,add by qtl 20131218 for Keyboard potentiometer   
// PLCÔËÐÐ·½Ê½
#define FUNCCODE_plcRunMode_ONCE_STOP   0   // µ¥´ÎÔËÐÐ½áÊøÍ£»ú
#define FUNCCODE_plcRunMode_ONCE_RUN    1   // µ¥´ÎÔËÐÐ½áÊø±£³ÖÖÕÖµ
#define FUNCCODE_plcRunMode_REPEAT      2   // Ò»Ö±Ñ­»·

// PLCµôµç¼ÇÒäÑ¡Ôñ
#define FUNCCODE_plcPowerOffRemMode_NO_REM  0   // µôµç²»¼ÇÒä
#define FUNCCODE_plcPowerOffRemMode_REM     1   // µôµç¼ÇÒä
// PLCÍ£»ú¼ÇÒäÑ¡Ôñ
#define FUNCCODE_plcStopRemMode_NO_REM  0   // µôµç²»¼ÇÒä
#define FUNCCODE_plcStopRemMode_REM     1   // µôµç¼ÇÒä

// PLCÔËÐÐÊ±¼äµ¥Î»
#define FUNCCODE_plcTimeUnit_S      0   // S(Ãë)
#define FUNCCODE_plcTimeUnit_H      1   // H(Ð¡Ê±)

// µç»úÎÂ¶È´«¸ÐÆ÷ÀàÐÍ
#define FUNCCODE_tempSenorType_NONE         0       // PTC100
#define FUNCCODE_tempSenorType_PTC100       1       // PTC100
#define FUNCCODE_tempSenorType_PTC1000      2       // PTC1000
#define FUNCCODE_tempSenorType_NTC          3       // NTC

// Êý¾Ý¸ñÊ½
#define FUNCCODE_sciParity_NONE     0   // ÎÞÐ£Ñé(8-N-2)
#define FUNCCODE_sciParity_EVEN     1   // Å¼Ð£Ñé(8-E-1)
#define FUNCCODE_sciParity_ODD      2   // ÆæÐ£Ñé(8-O-1)
#define FUNCCODE_sciParity_NONE1    3   // ÎÞÐ£Ñé(8-N-1)



// ¹¦ÄÜÂëµÄÊ±¼äµ¥Î»
// ×¢Òâ£¬³ÌÐòÖÐÎªÁË¼õÐ¡²»±ØÒªµÄ¼ÆËãºÍ¿Õ¼äÕ¼ÓÃ£¬²¿·ÖÊ¹ÓÃÁË
// X * (TIME_UNIT_WAIT_STOP_BRAKE / RUN_CTRL_PERIOD) µÄ·½Ê½
// ¶ø²»ÊÇ£¬(X * TIME_UNIT_WAIT_STOP_BRAKE) / RUN_CTRL_PERIOD
// Ö®ºóÐÞ¸ÄÕâÐ©Ê±¼äµ¥Î»£¬¿ÉÄÜÓÐ±ØÒªÐÞ¸Ä¡£
#define TIME_UNIT_SEC_PER_HOUR          3600    // 1hour = 3600sec
#define TIME_UNIT_MIN_PER_HOUR          60      // 1hour = 60min
#define TIME_UNIT_SEC_PER_MIN           60      // 1min  = 60sec
#define TIME_UNIT_MS_PER_SEC            1000    // 1s = 1000ms

#define TIME_UNIT_VF_VOL_ACC_TIME       100     // VF·ÖÀëµÄµçÑ¹ÉÏÉýÊ±¼ä

#define TIME_UNIT_AI_PULSE_IN_FILTER    10      // AI,pulseInÂË²¨Ê±¼ä, ms
#define TIME_UNIT_DI_DELAY              100     // DIÊä³öÑÓ³ÙÊ±¼ä, ms
#define TIME_UNIT_DO_DELAY              100     // DOÊä³öÑÓ³ÙÊ±¼ä, ms
#define TIME_UNIT_START_FRQ_WAIT        100      // Æô¶¯ÆµÂÊ±£³ÖÊ±¼ä£¬ms
#define TIME_UNIT_START_BRAKE           10      // Æô¶¯Ö±Á÷ÖÆ¶¯Ê±¼ä£¬ms
#define TIME_UNIT_WAIT_STOP_BRAKE       10      // Í£»úÖ±Á÷ÖÆ¶¯µÈ´ýÊ±¼ä£¬ms
#define TIME_UNIT_STOP_BRAKE            10      // Í£»úÖ±Á÷ÖÆ¶¯Ê±¼ä£¬ms
#define TIME_UNIT_ZERO_SPEED_DEAD       100     // Õý·´×ªËÀÇøÊ±¼ä
#define TIME_UNIT_LOWER_STOP_DELAY      100     // ÆµÂÊµÍÓÚÏÂÏÞÆµÂÊÊ±Í£»úµÄÑÓ³ÙÊ±¼ä
#define TIME_UNIT_PID_SET_CHANGE        10      // PID¸ø¶¨±ä»¯Ê±¼ä
#define TIME_UNIT_PID_FILTER            10      // PID·´À¡£¬Êä³öÂË²¨Ê±¼ä
#define TIME_UNIT_PID_INIT              10      // PID³õÖµ±£³ÖÊ±¼ä
#define TIME_UNIT_PID_FDB_LOSE          100     // PID·´À¡¶ªÊ§¼ì²âÊ±¼ä
#define TIME_UNIT_SWING_PERIOD          100      // °ÚÆµÖÜÆÚ
#define TIME_UNIT_sciCommOverTime       100     // Í¨Ñ¶³¬Ê±Ê±¼ä
#define TIME_UNIT_ERR_AUTO_RST_DELAY    100     // ¹ÊÕÏ×Ô¶¯¸´Î»¼ä¸ôÊ±¼ä£¬ms
#define TIME_UNIT_ERR_AUTO_RST_CLR      (TIME_UNIT_SEC_PER_HOUR*100UL) // ¹ÊÕÏ×Ô¶¯¸´Î»´ÎÊýÇå³ýÊ±¼ä, 0.1h
#define TIME_UNIT_P_OFF_VOL_BACK        10      // Ë²Í£²»Í£µçÑ¹»ØÉýÅÐ¶ÏÊ±¼ä
#define TIME_UNIT_PLC                   100     // PLCÔËÐÐÊ±¼äµ¥Î»

#define TIME_UNIT_ACC_DEC_SPEED_SERVO   10      // ËÅ·þ¼Ó¼õËÙÊ±¼äµ¥Î»
#define TIME_UNIT_WAKE_UP               100     // »½ÑÊ±¼äµÄµÎ»
#define TIME_UNIT_DORMANT               100     // ÐÝÃßÊ±¼äµÄµ¥Î»
#define TIME_UNIT_CURRENT_CHK           10      // µçÁ÷¼ì²âÊ±¼äµ¥Î»
#define TIME_UNIT_TORQUE_CTRL_ACC_DEC   10      // ×ª¾Ø¿ØÖÆÊ±¼äµ¥Î»
//=====================================================================

#if 0//F_DEBUG_RAM
#define ACC_DEC_T_INIT1  ((Uint32)2*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)    // ¼Ó¼õËÙÊ±¼ä³ö³§Öµ£¬2s£¬»úÐÍ <= 20
#define ACC_DEC_T_INIT2  ((Uint32)5*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)    // 5s£¬»úÐÍ > 20
#else
//============rev by xyw 20131216 for ACC_DEC time=================
#define ACC_DEC_T_INIT1  ((Uint32)10*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // ¼Ó¼õËÙÊ±¼ä³ö³§Öµ£¬20s£¬»úÐÍ <= 20
#define ACC_DEC_T_INIT2  ((Uint32)30*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 50s£¬»úÐÍ > 20
//#define ACC_DEC_T_INIT1  ((Uint32)10*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // ¼Ó¼õËÙÊ±¼ä³ö³§Öµ£¬10s£¬»úÐÍ <= 12---5.5KW
//#define ACC_DEC_T_INIT2  ((Uint32)20*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 20s£¬»úÐÍ 13~18----7.5~30KW
//#define ACC_DEC_T_INIT3  ((Uint32)40*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 40s£¬»úÐÍ >=19---37KW
//============rev by xyw 20131216 for ACC_DEC time=================


#endif

#define RATING_FRQ_INIT_0   50      // µç»ú¶î¶¨ÆµÂÊ£¬0¸öÐ¡Êýµã
#define RATING_FRQ_INIT_1   500     // µç»ú¶î¶¨ÆµÂÊ£¬1¸öÐ¡Êýµã
#define RATING_FRQ_INIT_2   5000    // µç»ú¶î¶¨ÆµÂÊ£¬2¸öÐ¡Êýµã
#define BAUD_NUM_MAX        12   // ²¨ÌØÂÊÑ¡Ôñ·¶Î§µÄ×î´óÖµ
//#define BAUD_NUM_MAX 10
#define PARA_INIT_MODE_MAX  501       // ²ÎÊý³õÊ¼»¯ÉÏÏÞÖµ
#define INV_TYPE_MAX   30

#define MENU_MODE_MAX       3   // ²Ëµ¥Ä£Ê½µÄ×î´óÖµ

//=====================================================================
// ÓÐÐ©¹¦ÄÜÂëµÄÉÏÏÂÏÞÊÇÆäËüÄ³¸ö¹¦ÄÜÂë£¬ÀïÊÇÔÚfuncCodeÖÐµÄindex

// ³ÌÐòÖÐÊ¹ÓÃµÄÒ»Ð©¹¦ÄÜÂëµÄindex
//= Èç¹ûÔö¼Ó/É¾³ýÁË¹¦ÜÂë£¬ÕâÀïÒªÐÞ¸?

#define FACTORY_PWD_INDEX      (GetCodeIndex(funcCode.code.factoryPassword))   // L0-00 ³§¼ÒÃÜÂë
#define INV_TYPE_INDEX         (GetCodeIndex(funcCode.code.inverterType))      // L0-01 ±äÆµÆ÷»úÐÍ
#define RATING_POWER_INVERTER_INDEX  (GetCodeIndex(funcCode.code.inverterPower))     // L0-03 ±äÆµÆ÷¹¦ÂÊ
#define FUNCCODE_FACTORY_START_INDEX     (GetCodeIndex(funcCode.group.l0[0]))            // L0×é³§¼Ò²ÎÊýµÄ¿ªÊ¼
#define FUNCCODE_FACTORY_END_INDEX       (GetCodeIndex(funcCode.group.l0[L0NUM - 1]))    // L0×é³§¼Ò²ÎÊýµÄ½áÊø

#define FC_MOTOR1_START_INDEX   (GetCodeIndex(funcCode.code.motorParaM1.all[0]))      // µÚ1µç»ú²ÎÊýµÄÆðÊ¼
#define FC_MOTOR1_END_INDEX     (GetCodeIndex(funcCode.code.pgParaM1.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // µÚ1µç»ú²ÎÊýµÄ½áÊø

#define FC_MOTOR2_START_INDEX   (GetCodeIndex(funcCode.code.motorFcM2.motorPara.all[0]))      // µÚ2µç»ú²ÎÊýµÄÆðÊ¼
#define FC_MOTOR2_END_INDEX     (GetCodeIndex(funcCode.code.motorFcM2.pgPara.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // µÚ2µç»ú²ÎÊýµÄ½áÊø

#define FC_MOTOR3_START_INDEX   (GetCodeIndex(funcCode.code.motorFcM3.motorPara.all[0]))      // µÚ3µç»ú²ÎÊýµÄÆðÊ¼
#define FC_MOTOR3_END_INDEX     (GetCodeIndex(funcCode.code.motorFcM3.pgPara.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // µÚ3µç»ú²ÎÊýµÄ½áÊø

#define FC_MOTOR4_START_INDEX   (GetCodeIndex(funcCode.code.motorFcM4.motorPara.all[0]))      // µÚ4µç»ú²ÎÊýµÄÆðÊ¼
#define FC_MOTOR4_END_INDEX     (GetCodeIndex(funcCode.code.motorFcM4.pgPara.all[sizeof(struct PG_PARA_STRUCT) - 1]))  // µÚ4µç»ú²ÎÊýµÄ½áÊø



#define PRESET_FRQ_INDEX        (GetCodeIndex(funcCode.code.presetFrq))      // F0-08   Ô¤ÖÃÆµÂÊ
#define MAX_FRQ_INDEX           (GetCodeIndex(funcCode.code.maxFrq))         // F0-10   ×î´óÆµÂÊ
#define UPPER_FRQ_INDEX         (GetCodeIndex(funcCode.code.upperFrq))       // F0-12   ÉÏÏÞÆµÂÊ
#define LOWER_FRQ_INDEX         (GetCodeIndex(funcCode.code.lowerFrq))       // F0-14   ÂÏÞÆµÂ?
#define ACC_TIME1_INDEX         (GetCodeIndex(funcCode.code.accTime1))       // F0-17   ¼ÓËÙÊ±¼ä1
#define DEC_TIME1_INDEX         (GetCodeIndex(funcCode.code.decTime1))       // F0-18   ¼õËÙÊ±¼ä1
#define CARRIER_FRQ_INDEX       (GetCodeIndex(funcCode.code.carrierFrq))     // F0-15   ÔØ²¨ÆµÂÊ

#define RATING_POWER_INDEX      (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingPower))    // µç»ú¶î¶¨¹¦ÂÊ
#define RATING_VOL_INDEX        (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingVoltage))  // µç»ú¶î¶¨µçÑ¹
#define RATING_CUR_INDEX        (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingCurrent))  // µç»ú¶î¶¨µçÁ÷
#define RATING_CUR_INDEX2       (GetCodeIndex(funcCode.code.motorFcM2.motorPara.elem.ratingCurrent))    // µÚ2µç»ú¶î¶¨µçÁ÷
#define RATING_CUR_INDEX3       (GetCodeIndex(funcCode.code.motorFcM3.motorPara.elem.ratingCurrent))   // µÚ3µç»ú¶î¶¨µçÁ÷
#define RATING_CUR_INDEX4       (GetCodeIndex(funcCode.code.motorFcM4.motorPara.elem.ratingCurrent))   // µÚ4µç»úî¶¨µçÁ÷


#define RATING_FRQ_INDEX        (GetCodeIndex(funcCode.code.motorParaM1.elem.ratingFrq))      // µç»ú¶î¶¨ÆµÂÊ

#define ZERO_LOAD_CURRENT_INDEX (GetCodeIndex(funcCode.code.motorParaM1.elem.zeroLoadCurrent))// ¿ÕÔØµçÁ÷
#define STATOR_RESISTANCE_INDEX (GetCodeIndex(funcCode.code.motorParaM1.elem.statorResistance))// ¶¨×Óµç×è

#define ANTI_VIBRATE_GAIN_INDEX (GetCodeIndex(funcCode.code.antiVibrateGain))// F3-11   Õñµ´ÒÖÖÆÔöÒæ
#define ANTI_VIBRATE_GAIN_MOTOR2_INDEX (GetCodeIndex(funcCode.code.motorFcM2.antiVibrateGain))// A4-52   Õñµ´ÒÖÖÆÔöÒæ
#define ANTI_VIBRATE_GAIN_MOTOR3_INDEX (GetCodeIndex(funcCode.code.motorFcM3.antiVibrateGain))// A5-52   Õñµ´ÒÖÖÆÔöÒæ
#define ANTI_VIBRATE_GAIN_MOTOR4_INDEX (GetCodeIndex(funcCode.code.motorFcM4.antiVibrateGain))// A6-52   Õñµ´ÒÖÖÆÔöÒæ

#define TUNE_CMD_INDEX_1  (GetCodeIndex(funcCode.code.tuneCmd))  // µ÷Ð³
#define TUNE_CMD_INDEX_2  (GetCodeIndex(funcCode.code.motorFcM2.tuneCmd))  // µ÷Ð³
#define TUNE_CMD_INDEX_3  (GetCodeIndex(funcCode.code.motorFcM3.tuneCmd))  // µ÷Ð³
#define TUNE_CMD_INDEX_4  (GetCodeIndex(funcCode.code.motorFcM4.tuneCmd))  // µ÷Ð³

#define VC_CHG_FRQ1_INDEX (GetCodeIndex(funcCode.code.vcParaM1.vcSpdLoopChgFrq1))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»µÂÊ1
#define VC_CHG_FRQ2_INDEX (GetCodeIndex(funcCode.code.vcParaM1.vcSpdLoopChgFrq2))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ2

#define VC_CHG_FRQ1_INDEX2 (GetCodeIndex(funcCode.code.motorFcM2.vcPara.vcSpdLoopChgFrq1))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ1
#define VC_CHG_FRQ2_INDEX2 (GetCodeIndex(funcCode.code.motorFcM2.vcPara.vcSpdLoopChgFrq2))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ2

#define VC_CHG_FRQ1_INDEX3 (GetCodeIndex(funcCode.code.motorFcM3.vcPara.vcSpdLoopChgFrq1))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ1
#define VC_CHG_FRQ2_INDEX3 (GetCodeIndex(funcCode.code.motorFcM3.vcPara.vcSpdLoopChgFrq2))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ2

#define VC_CHG_FRQ1_INDEX4 (GetCodeIndex(funcCode.code.motorFcM4.vcPara.vcSpdLoopChgFrq1))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ1
#define VC_CHG_FRQ2_INDEX4 (GetCodeIndex(funcCode.code.motorFcM4.vcPara.vcSpdLoopChgFrq2))  // Ê¸Á¿¿ØÖÆËÙ¶È»· ÇÐ»»ÆµÂÊ2


#define TORQUE_BOOST_INDEX      (GetCodeIndex(funcCode.code.torqueBoost))    // F1-05   ×ª¾ØÌáÉý

#define TORQUE_BOOST_MOTOR2_INDEX      (GetCodeIndex(funcCode.code.motorFcM2.torqueBoost))    // D0-52   µÚ2µç»ú×ª¾ØÌáÉý
#define TORQUE_BOOST_MOTOR3_INDEX      (GetCodeIndex(funcCode.code.motorFcM3.torqueBoost))    // D0-52   µÚ3µç»ú×ª¾ØÌáÉý
#define TORQUE_BOOST_MOTOR4_INDEX      (GetCodeIndex(funcCode.code.motorFcM4.torqueBoost))    // D0-52   ?µç»ú×ª¾ØÌáÉý

#define SVC_MODE_INDX           (GetCodeIndex(funcCode.code.svcMode))        // A5-07 SVCÄ£Ê½Ñ¡Ôñ
#define OV_POINT_SET_INDEX      (GetCodeIndex(funcCode.code.ovPointSet))     // A5-09 ¹ýÑ¹µãÉèÖÃ
#define UV_POINT_SET_INDEX      (GetCodeIndex(funcCode.code.uvPoint))        // A5-06 ¹ýÑ¹µãÉèÖÃ 20170525

#define VF_FRQ1_INDEX           (GetCodeIndex(funcCode.code.vfFrq1))         // F3-03   ¶àµãVFÆµÂÊµã1
#define VF_FRQ2_INDEX           (GetCodeIndex(funcCode.code.vfFrq2))         // F3-05   ¶àµãVFÆµÂÊµã2
#define VF_FRQ3_INDEX           (GetCodeIndex(funcCode.code.vfFrq3))         // F3-07   ¶àµãVFÆµÂÊµã3


#define CURVE1_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet2P1[0]))       // F2-08   AI1×îÐ¡ÊäÈë
#define CURVE1_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet2P1[2]))       // F2-10   AI1×î´óÊäÈë
#define CURVE2_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet2P2[0]))       // F2-14   AI2×îÐ¡ÊäÈë
#define CURVE2_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet2P2[2]))       // F2-16   AI2×î´óÊäÈë
#define CURVE3_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet2P3[0]))       // F2-14   AI3×îÐ¡ÊäÈë
#define CURVE3_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet2P3[2]))       // F2-16   AI3×î´óÊäÈë
#define PULSE_IN_MIN_INDEX      (GetCodeIndex(funcCode.code.curveSet2P4[0]))     // F2-20   PULSE×îÐ¡ÊäÈë
#define PULSE_IN_MAX_INDEX      (GetCodeIndex(funcCode.code.curveSet2P4[2]))     // F2-22   PULSE×î´óÊäÈë

#define CURVE4_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet4P1[0]))       // A8-00   AI4×îÐ¡ÊäÈë
#define CURVE4_INFLEX1_INDEX    (GetCodeIndex(funcCode.code.curveSet4P1[2]))       // A8-02   AI4¹Õµã1ÊäÈë
#define CURVE4_INFLEX2_INDEX    (GetCodeIndex(funcCode.code.curveSet4P1[4]))       // A8-04   AI4¹Õµã2ÊäÈë
#define CURVE4_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet4P1[6]))       // A8-06   AI4×î´óÊäÈë
#define CURVE5_MIN_INDEX        (GetCodeIndex(funcCode.code.curveSet4P2[0]))       // A8-08   AI5×îÐ¡ÊäÈë
#define CURVE5_INFLEX1_INDEX    (GetCodeIndex(funcCode.code.curveSet4P2[2]))       // A8-10   AI5¹Õµã1ÊäÈë
#define CURVE5_INFLEX2_INDEX    (GetCodeIndex(funcCode.code.curveSet4P2[4]))       // A8-12   AI5¹Õµã2ÊäÈë
#define CURVE5_MAX_INDEX        (GetCodeIndex(funcCode.code.curveSet4P2[6]))       // A8-14   AI5×î´óÊäÈë


#define ACC_TIME2_INDEX         (GetCodeIndex(funcCode.code.accTime2))       // F8-03 ¼ÓËÙÊ±¼ä2
#define DEC_TIME2_INDEX         (GetCodeIndex(funcCode.code.decTime2))       // F8-04 ¼õËÙÊ±¼ä2

#define ACC_TIME3_INDEX         (GetCodeIndex(funcCode.code.accTime3))       // F8-05 ¼ÓËÙÊ±¼ä3
#define DEC_TIME3_INDEX         (GetCodeIndex(funcCode.code.decTime3))       // F8-06 ¼õËÙÊ±¼ä3

#define ACC_TIME4_INDEX         (GetCodeIndex(funcCode.code.accTime4))       // F8-07 ¼ÓËÙÊ±¼ä4
#define DEC_TIME4_INDEX         (GetCodeIndex(funcCode.code.decTime4))       // F8-08 ¼õËÙÊ±¼ä4

#define RUN_TIME_ADDUP_INDEX    (GetCodeIndex(funcCode.code.runTimeAddup))     // F7-09  ÀÛ¼ÆÔËÐÐÊ±¼ä
#define POWER_TIME_ADDUP_INDEX  (GetCodeIndex(funcCode.code.powerUpTimeAddup)) // F7-13  ÀÛ¼ÆÉÏµçÊ±¼ä
#define POWER_ADDUP_INDEX       (GetCodeIndex(funcCode.code.powerAddup))       // F7-14  ÀÛ¼ÆºÄµçÁ¿


#define AI1_LIMIT               (GetCodeIndex(funcCode.code.ai1VoltageLimit)) //  F8-45  AI±£»¤ÏÂÏÞ
#define AI1_UPPER               (GetCodeIndex(funcCode.code.ai1VoltageUpper)) //  F8-46  AI±£»¤ÉÏÏÞ

#define PID_PARA_CHG_DELTA1_MAX (GetCodeIndex(funcCode.code.pidParaChgDelta2))  // FA-20  PID²ÎÊýÇÐ»»Æ«²î2
#define PID_PARA_CHG_DELTA2_MIN (GetCodeIndex(funcCode.code.pidParaChgDelta1))  // FA-19  PID²ÎÊýÇÐ»»«²?

#define DORMANT_UPPER           (GetCodeIndex(funcCode.code.wakeUpFrq))       // ÐÝÃßÆµÂÊÉÏÏÞ
#define WAKE_UP_LIMIT           (GetCodeIndex(funcCode.code.dormantFrq))      // »½ÐÑÆµÂÊÏÂÏÞ
#define RADIATOR_TEMP_INDEX     (GetCodeIndex(funcCode.code.radiatorTemp))   // FB-19   Äæ±äÆ÷Ä£¿éÉ¢ÈÈÆ÷ÎÂ¶È
#define ERROR_LATEST1_INDEX     (GetCodeIndex(funcCode.code.errorLatest1))   // FB-20   µÚÒ»´Î¹ÊÕÏÀàÐÍ
#define ERROR_LATEST2_INDEX     (GetCodeIndex(funcCode.code.errorLatest2))   // FB-21   µÚ¶þ´Î¹ÊÕÏÀàÐÍ
#define ERROR_LATEST3_INDEX     (GetCodeIndex(funcCode.code.errorLatest3))   // FB-22   (×î½üÒ»´Î)µÚÈý´Î¹ÊÕÏÀàÐÍ
#define ERROR_FRQ_INDEX         (GetCodeIndex(funcCode.code.errorScene3.elem.errorFrq))       // FB-23   ¹ÊÕÏÊ±ÆµÂÊ
#define ERROR_CURRENT_INDEX     (GetCodeIndex(funcCode.code.errorScene3.elem.errorCurrent))   // FB-24   ¹ÊÕÏÊ±µçÁ÷
#define ERROR_UDC_INDEX         (GetCodeIndex(funcCode.code.errorScene3.elem.errorGeneratrixVoltage)) // FB-25 ¹ÊÕÏÊ±Ä¸ÏßµçÑ¹
#define ERROR_DI_STATUS_INDEX   (GetCodeIndex(funcCode.code.errorScene3.elem.errorDiStatus))  // FB-26   ¹ÊÕÏÊ±ÊäÈë¶Ë×Ó×´Ì¬
#define ERROR_DO_STATUS_INDEX   (GetCodeIndex(funcCode.code.errorScene3.elem.errorDoStatus))  // FB-27   ¹ÊÕÏÊ±Êä³ö¶Ë×Ó×´Ì¬
#define LAST_ERROR_RECORD_INDEX (GetCodeIndex(funcCode.code.errorScene1.all[sizeof(struct ERROR_SCENE_STRUCT) - 1]))  // ×îºóÒ»¸ö¹ÊÕÏ¼ÇÂ¼

#define MIN_CBC_TIME_INDEX       (GetCodeIndex(funcCode.code.cbcMinTime))         // A0-14   Öð²¨ÏÞÁ÷Ê±¼äÏÂÏÞ
#define MAX_CBC_TIME_INDEX       (GetCodeIndex(funcCode.code.cbcMaxTime))         // A0-15   Öð²¨ÏÞÁ÷Ê±¼äÉÏÏÞ



#define PC_LOOP_CHG_FRQ1_I      (GetCodeIndex(funcCode.code.pcLoopChgFrq1))     //          ÇÐ»»ÆµÂÊ1
#define PC_LOOP_CHG_FRQ2_I      (GetCodeIndex(funcCode.code.pcLoopChgFrq2))     //          Ð»ÂÊ2

#define EEPROM_CHECK_INDEX      (GetCodeIndex(funcCode.code.eepromCheckWord1))  // eepromCheckWord1

#define RUN_TIME_ADDUP_SEC_INDEX    (GetCodeIndex(funcCode.code.runTimeAddupSec))   // FR-07 F209  ÀÛ¼ÆÔËÐÐÊ±¼äµÄs


#define EEPROM_CHECK_INDEX1     (GetCodeIndex(funcCode.code.eepromCheckWord1))  // eepromCheckWord1
#define EEPROM_CHECK_INDEX2     (GetCodeIndex(funcCode.code.eepromCheckWord2))  // eepromCheckWord2

#define SAVE_USER_PARA_PARA1    (GetCodeIndex(funcCode.code.saveUserParaFlag1))
#define SAVE_USER_PARA_PARA2    (GetCodeIndex(funcCode.code.saveUserParaFlag2))

#define AI_AO_CHK_FLAG          (GetCodeIndex(funcCode.code.aiaoChkWord))       // AIAOÐ£Õý±êÖ¾
#define AI_AO_CALIB_START       (GetCodeIndex(funcCode.code.aiFactoryCalibrateCurve[0].before1))  // aiao³§¼ÒÐ£Õý¿ªÊ¼
#define AI_AO_CALIB_STOP        (GetCodeIndex(funcCode.code.aoFactoryCalibrateCurve[0].after2))   // aiao³§¼ÒÐ£Õý½áÊø

#define AI1_CALB_START          (GetCodeIndex(funcCode.code.aiFactoryCalibrateCurve[0].before1))
#define AI2_CALB_STOP           (GetCodeIndex(funcCode.code.aiFactoryCalibrateCurve[1].after2))
#define AO1_CALB_START          (GetCodeIndex(funcCode.code.aoFactoryCalibrateCurve[0].before1))
#define AO1_CALB_STOP           (GetCodeIndex(funcCode.code.aoFactoryCalibrateCurve[0].after2)) 

//-------------------------------
#define FC_GROUP_FACTORY    FUNCCODE_GROUP_L0   // ³§¼Ò²ÎÊý×é
#define FC_GROUP_FC_MANAGE  FUNCCODE_GROUP_Y0   // ¹¦ÄÜÂë¹ÜÀí
#define FC_START_GROUP      FUNCCODE_GROUP_F0   // ¹¦ÄÜÂë×éÏÔÊ¾µÄµÚ1×é
#define USER_DISPLAY1_INDEX      (GetCodeIndex(funcCode.code.disPara))  // ¼àÊÓÑ¡Ôñ2
//--------------------------------


extern FUNCCODE_ALL funcCode;           // ¹¦ÄÜÂëµÄRAMÖµ
//extern FUNCCODE_ALL funcCodeEeprom;     // ¹¦ÄÜÂëµÄEEPROMÖµ

extern const Uint16 funcCodeGradeSum[];

extern Uint16 saveEepromIndex;    // DP¿¨µôµç´æ´¢
extern const Uint16 ovVoltageInitValue[];
extern const Uint16 uvVoltageInitValue[]; //20170525
extern const Uint16 funcCodeGradeAll[];
extern Uint16 funcCodeGradeCurMenuMode[];
int32 keyLineCal(void);//ÊäÈëx·¶Î§add by qtl for Keyboard potentiometer 20131218
#endif  // __F_FUNCCODE_H__




