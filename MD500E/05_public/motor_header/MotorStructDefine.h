/****************************************************************

****************************************************************/

#ifndef MOTOR_STRUCT_DEFINE_H
#define  MOTOR_STRUCT_DEFINE_H
#ifdef __cplusplus
extern "C"[
#endif

#include "MotorDefine.h"
// // 鍏辩敤缁撴瀯浣撶殑瀹氫箟  中文
struct MAIN_COMMAND_STRUCT_DEF{
   Uint    Start:1;                 // 1 璧峰姩锛� 0 鍋滄満
   Uint    StartDC:1;               // 1 璧峰姩鐩存祦鍒跺姩
   Uint    StopDC:1;                // 1 鍋滄満鐩存祦鍒跺姩鍛戒护
   Uint    ControlMode:2;           // 0锛峉VC锛�1锛岶VC锛�2锛峍F
   Uint    PreExcFlux:1;            // 1 棰勫姳纾佸惎鍔�
   Uint    TorqueCtl:1;             // 1 杞煩鎺у埗鏍囧織
   Uint    SpeedFlag:2;             // 0锛嶆亽閫燂紝1锛嶅姞閫燂紝2锛嶅噺閫�
   Uint    IntegralDiscrete:1;      // 1锛嶇Н鍒嗗垎绂�
};

union MAIN_COMMAND_UNION_DEF {
   Uint   all;
   struct MAIN_COMMAND_STRUCT_DEF   bit;
};

struct MAIN_COMMAND_EXTEND_STRUCT_DEF{
   Uint    QepIndex:2;              // 0锛嶄娇鐢‥QP2娴嬮��,1锛嶄娇鐢≦EP1娴嬮�燂紝2锛峆ULSE鑴夊啿杈撳叆
   Uint    SpeedRev:1;              // 1 鐮佺洏鏂瑰悜鍙嶅悜鏍囧織(//rt 娆″瀛楁殏鏃朵笉鐢�)
   Uint    DeadCompMode:3;           //0锛嶆棤姝诲尯琛ュ伩锛�1锛岮D姝诲尯琛ュ伩 2锛嶄紭鍖栨鍖鸿ˉ鍋� 2011.5.7 L1082
   Uint    ModulateType:1;          //0锛嶅紓姝ヨ皟鍒讹紱1 锛嶅悓姝ヨ皟鍒�
   Uint    Reserved:1;             //0锛嶄繚鐣�
   Uint    Reserved1:1;             //淇濈暀
   Uint    FreqUint:2;              //棰戠巼鎸囦护鍗曚綅銆�0锛�1Hz锛�1锛�0.1Hz锛�2锛�0.01Hz
   Uint    SpeedSearch:2;           // 0锛嶈浆閫熻窡韪棤鏁堬紝1锛嶈浆閫熻窡韪ā寮�1锛�2锛嶈浆閫熻窡韪ā寮�2
   Uint    ShortGnd:1;              // 1锛嶄笂鐢靛鍦扮煭璺娴嬫爣蹇�
   Uint    ShortGndBeforeRun:1;     // 1-杩愯鍓嶅鍦扮煭璺爣蹇�
};

typedef union MAIN_COMMAND_EXTEND_UNION_DEF{
    Uint all;
    struct MAIN_COMMAND_EXTEND_STRUCT_DEF bit;
}MAIN_COMMAND_EXTEND_UNION;

struct SUB_COMMAND_STRUCT_DEF{
   Uint    ErrorOK:1;               // 1 鏁呴殰澶勭悊瀹屾瘯鏍囧織
   Uint    OutputLost:1;            // 1 杈撳嚭缂虹浉妫�娴嬩娇鑳�
   Uint    OutputLostBeforeRun:1;   // 1 杩愯鍓嶈緭鍑虹己鐩告娴嬩娇鑳�
#if (SOFTSERIES == MD500SOFT)
   Uint    InputLost:2;             // 1 杈撳叆缂虹浉淇濇姢浣胯兘
#else
   Uint    InputLost:1;             // 1 杈撳叆缂虹浉淇濇姢浣胯兘
#endif
   Uint    MotorOvLoad:1;           // 1 鐢垫満杩囪浇淇濇姢浣胯兘
   Uint    LoadLose:1;              // 1 杈撳嚭鎺夎浇淇濇姢浣胯兘鏍囧織
   Uint    NoStop:1;                // 1 鐬仠涓嶅仠浣胯兘
   Uint    CBCEnable:1;             // 1 閫愭尝闄愭祦鍔熻兘浣胯兘鏍囧織
   Uint    VarFcByTem:1;            // 1 杞芥尝棰戠巼闅忔俯搴﹁皟鏁�(鍥哄畾涓�1)
   Uint    FanNoStop:1;             // 1 鍋滄満鐩存祦鍒跺姩绛夊緟鏃堕棿鍐呴鎵囪繍琛屾爣蹇�
   Uint    RelayEnalbe:1;           // 1 缂撳啿鐢甸樆鍚稿悎浣胯兘
   Uint    VCFolFlag:1;             // 1 涓讳粠妯″紡浣胯兘
};
typedef union SUB_COMMAND_UNION_DEF {
   Uint   all;
   struct SUB_COMMAND_STRUCT_DEF    bit;
}SUB_COMMAND_UNION;

struct SEND_STATUS_STRUCT_DEF{
   Uint    RunEnable:1;             // 1 鍒濆鍖栧畬鎴愶紝鍙互杩愯鏍囧織
   Uint    LowUDC:1;                // 0 姣嶇嚎鐢靛帇娆犲帇鏁呴殰鏍囧織
   Uint    StartStop:1;             // 1 杩愯/鍋滄満鐘舵�佹爣蹇� (鐜板湪璇ユ爣蹇楁病鏈夌敤)//rt
   Uint    ShortGndOver:1;          // 1 瀵瑰湴鐭矾妫�娴嬪畬姣曟爣蹇�
   Uint    SpeedSearchOver:1;       // 1 杞�熻窡韪粨鏉熸爣蹇�
   Uint    PerOvLoadInv:1;          // 1 鍙橀鍣ㄨ繃杞介鎶ヨ鏍囧織
   Uint    PerOvLoadMotor:1;        // 1 鐢垫満杩囪浇棰勬姤璀︽爣蹇�
   Uint    FanControl:1;            // 1 椋庢墖杩愯
   Uint    OutOff:1;                // 1 鍙橀鍣ㄨ緭鍑虹┖寮�鏂紑鏍囧織
   Uint    OverLoadPreventState:2;   // 1 闃茶繃杞界姸鎬�
   Uint    RunStart:1;
};

typedef union SEND_STATUS_UNION_DEF {
   Uint   all;
   struct SEND_STATUS_STRUCT_DEF    bit;
}SEND_STATUS_UNION;

// // 浠ヤ笅涓哄熀鏈俊鎭暟鎹粨鏋�
typedef struct INV_STRUCT_DEF {
    Uint    InvTypeApply;               //瀹為檯浣跨敤鐨勬満鍨嬶紙P鍨嬫満 = G鍨嬫満-1锛�
    Uint    InvTypeSet;                 //鍔熻兘浼犻�掔殑鏈哄瀷锛屽寘鍚數鍘嬬瓑绾т俊鎭�
    Uint    InvVoltageType;             //鍙橀鍣ㄧ數鍘嬬瓑绾т俊鎭�
    Uint    InvType;                    //鐢ㄦ埛璁剧疆鐨勬満鍨嬶紝鍘婚櫎鐢靛帇绛夌骇淇℃伅锛岃寖鍥村湪00锛�99
    Uint    GpTypeSet;                  // 鍔熻兘鐮佽瀹氱殑GP
    Uint    GPType;                     // 1 G鍨嬫満锛�2 P鍨嬫満
    Uint    InvCurrent;                 //鏌ヨ〃寰楀埌鐨勫彉棰戝櫒鐢垫祦锛�   鍗曚綅鐢辨満鍨嬬‘瀹�
    Uint    InvCurrentOvload;           //鍙橀鍣ㄨ繃杞界數娴侊紝  鍗曚綅0.01A
    Uint    InvCurrForP;                //P鍨嬫満浣跨敤鐨勯瀹氱數娴�       杩囪浇淇濇姢鐢ㄧ數娴�
    Uint    InvVolt;                    //鏌ヨ〃寰楀埌鐨勫彉棰戝櫒鐢靛帇锛�   鍗曚綅1V
    Uint    CurrentCoff;                //鍙橀鍣ㄧ數娴佺煫姝ｇ郴鏁�  锛�  鍗曚綅0.1%
    Uint    UDCCoff;                    //鍙橀鍣ㄦ瘝绾跨數鍘嬬煫姝ｇ郴鏁帮紝  鍗曚綅0.1%
    Uint    TempType;                   //鍙橀鍣ㄦ俯搴︽洸绾块�夋嫨
    Uint    InvUpUDC;                   //姣嶇嚎杩囧帇鐐�             鍗曚綅0.1V
    Uint    InvLowUdcStad;              // 鏈牎姝ｇ殑娆犲帇鐐�
    Uint    InvLowUDC;                  //娆犲帇鐐�
    Uint    LowUdcCoff;                 //娆犲帇鐐规牎姝ｇ郴鏁�
    Uint    BaseUdc;                    //姣嶇嚎鐢靛帇鍩哄噯 380V鏈哄櫒涓�537.4V
    Uint    InvTypeGaiZhi;              //220V鏈哄瀷鏀瑰埗鍓嶅搴旂殑380V鏈哄瀷
#if (SOFTSERIES == MD500SOFT)
    Uint    InvBreakMaxCurrent;
    Uint    InvBreakCurrent;
#endif
}INV_STRUCT;                         //鍙橀鍣ㄧ‖浠朵俊鎭粨鏋�


// // 浣跨敤鏋氫妇鍙橀噺鍖哄垎鐢垫満绫诲瀷鍜屾帶鍒舵ā寮忕殑缁勫悎
typedef enum CONTROL_MOTOR_TYPE_ENUM_DEF{
    ASYNC_SVC,                          //
    ASYNC_FVC,                          //
    ASYNC_VF,                           // 寮傛鏈篠VC鎺у埗涓�0锛孎VC涓�1锛孷F涓�2

    SYNC_SVC = 10,                      //
    SYNC_FVC,
    SYNC_VF,                            // 鍚屾鏈篠VC涓�0锛孎VC涓�1锛孷F涓�2

    DC_CONTROL = 20,                    // 鐩存祦鍒跺姩涓�20
    RUN_SYNC_TUNE                       // 鍙傛暟杈ㄨ瘑鏃堕渶瑕佺數鏈鸿繍琛岀殑鐘舵�侊紝涓昏鏄悓姝ユ満浣跨敤
}CONTROL_MOTOR_TYPE_ENUM;

typedef struct MOTOR_STRUCT_DEF {
    Uint    MotorType;              //0锛嶆櫘閫氬紓姝ョ數鏈猴紱1锛嶅彉棰戝紓姝ョ數鏈猴紱2锛嶆案纾佸悓姝ョ數鏈�
    Uint    LastMotorType;          //瀛樺偍涓婁竴鎷嶅姛鑳戒紶閫掔殑鐢垫満鏈哄瀷锛岀敤浜庢煇浜涘湪鐢垫満绫诲瀷鏀瑰彉鏃讹紝闇�瑕佷慨姝ｇ殑鍙橀噺
    Uint    Power;                  //鐢垫満鍔熺巼                  鍗曚綅0.1KW
    Uint    Votage;                 //鐢垫満鐢靛帇                  鍗曚綅1V
    Uint    CurrentGet;             //鍔熻兘浼犻�掔殑鐢垫満鐢垫祦         鍗曚綅鐢辨満鍨嬬‘瀹�
    Uint    Frequency;              //鐢垫満棰戠巼
    Uint    FreqPer;                //鏍囦箞鍊肩數鏈洪鐜�
    Uint    Current;                //椹卞姩绋嬪簭閫夌敤鐨勭數娴佸熀鍊�(鍙兘鍜屽疄闄呯數鏈虹數娴佷笉绛�)
    Uint    CurBaseCoff;            //鐢垫祦鍩哄�肩殑鏀惧ぇ鍊嶆暟
    Uint    Motor_HFreq;            // 鐢垫満棰濆畾棰戠巼鐨�60%
    Uint    Motor_LFreq;            // 鐢垫満棰濆畾棰戠巼鐨�40%
}MOTOR_STRUCT;                   //鐢垫満鍩烘湰淇℃伅缁撴瀯

typedef struct MOTOR_EXTERN_STRUCT_DEF {
    Uint    UnitCoff;                           //鐢垫満鍙傛暟杞崲绯绘暟 鏈哄瀷灏忎簬22锛屼负1锛涘ぇ浜�22锛屼负10
    Uint    R1;                                 //瀹氬瓙鐩哥數闃�      鏈哄瀷灏忎簬22锛屽崟浣�0.001娆у锛屽ぇ浜�22锛�0.0001娆у
    Uint    R2;                                 //杞瓙鐩哥數闃�      鏈哄瀷灏忎簬22锛屽崟浣�0.001娆у锛屽ぇ浜�22锛�0.0001娆у
    Uint    L0;                                 //婕忔劅             鏈哄瀷灏忎簬22锛屽崟浣�0.01mH锛屽ぇ浜�22锛屽崟浣�0.001mH
    Uint    LM;                                 //浜掓劅             鏈哄瀷灏忎簬22锛屽崟浣�0.1mH锛屽ぇ浜�22锛屽崟浣�0.01mH
    Uint    I0;                                 //绌鸿浇鐢垫祦       鏈哄瀷灏忎簬22锛屽崟浣�0.01A锛屽ぇ浜�22锛屽崟浣�0.1A
    Uint    IoVsFreq;                           //寮辩鍖哄弽姣旈�熷害鍙樺寲鐨勭┖杞界數娴侊紝鍙绠楀畠鐨勬爣涔堝��
    Uint    Rpm;                                //鐢垫満杞��       鍗曚綅1rpm
    Uint    RatedComp;                          //棰濆畾杞樊鐜�       0.01Hz, pu
    Uint    Poles;                              //鐢垫満鏋佹暟
    Uint    L1;                                 //瀹氬瓙鐩哥數鎰�      鏈哄瀷灏忎簬22锛屽崟浣�0.1mH锛屽ぇ浜�22锛屽崟浣�0.01mH
    Uint    L2;                                 //杞瓙鐩哥數鎰�      鏈哄瀷灏忎簬22锛屽崟浣�0.1mH锛屽ぇ浜�22锛屽崟浣�0.01mH
    Uint    RsPm;                               // 鍚屾鏈哄畾瀛愮數闃�
    Uint    LD;                                 //鍚屾鏈篋杞寸數鎰�     鏈哄瀷 <22锛屽崟浣�0.01mH锛屾満鍨� >22锛屽崟浣�0.001mH
    Uint    LQ;                                 //鍚屾鏈篞杞寸數鎰�     鏈哄瀷 <22锛屽崟浣�0.01mH锛屾満鍨� >22锛屽崟浣�0.001mH
    Uint    BemfVolt;                               // 鍚屾鏈哄弽鐢靛姩鍔跨數鍘嬶紝 鍙互璁＄畻鍑鸿浆瀛愮閾�
    Uint    BemfVoltApply;
    Uint    FluxRotor;                              // 鍚屾鏈鸿浆瀛愮閾�  Q12
    Uint    ItRated;                                // 棰濆畾鍔涚煩鐢垫祦锛宲m im鍏辩敤
    Uint    FluxLeakCurveGain;
}MOTOR_EXTERN_STRUCT;   //鐢垫満鎵╁睍淇℃伅缁撴瀯

struct ERROR_FLAG_SIGNAL {
    Uint16  OvCurFlag:1;                            // bit0=1琛ㄧず鍙戠敓浜嗚繃娴佷腑鏂�
    Uint16  OvUdcFlag:1;                            // bit1=1琛ㄧず鍙戠敓浜嗚繃鍘嬩腑鏂�
    Uint16  Res:14;                                 // 淇濈暀
};

union ERROR_FLAG_SIGNAL_DEF {
   Uint16                   all;
   struct ERROR_FLAG_SIGNAL  bit;
};

typedef struct PRG_STATUS_STRUCT_DEF{
   Uint    PWMDisable:1;                                    //BIT0=1 琛ㄧず涓嶅彂閫丳WM娉紱
   Uint    ACRDisable:1;                                    //BIT1=1 琛ㄧず涓嶈绠楃數娴佺幆锛�
   Uint    ASRDisable:1;                                    //BIT2=1 琛ㄧず涓嶈绠楅�熺劧缃氾拷
}PRG_STATUS_STRUCT;                                     //绋嬪簭杩愯鎺у埗鍒侯儾鍊艰叿it瀹夋帓

typedef union PRG_STATUS_UNION_DEF {                // 绋嬪簭鐘舵�佸瓧
   Uint   all;
   PRG_STATUS_STRUCT bit;
}PRG_STATUS_UNION;

typedef struct RUN_STATUS_STRUCT_DEF {
    Uint    RunStep;                                        //涓绘楠�
    Uint    SubStep;                                        //杈呮?
    Uint    ParaCalTimes;                                   //鐢ㄤ簬鎺у埗涓婄數鍚庡彧璁＄畻涓�娆＄殑鍙傛暟杞崲銆�
    PRG_STATUS_UNION    PrgStatus;                          //绋嬪簭鎺у埗鐘舵�佸瓧
    union   ERROR_FLAG_SIGNAL_DEF   ErrFlag;
    SEND_STATUS_UNION       StatusWord;
}RUN_STATUS_STRUCT;                                     //鍙橀鍣ㄨ繍琛岀姸鎬佺粨鏋�


typedef struct BASE_COMMAND_STRUCT_DEF {
    union MAIN_COMMAND_UNION_DEF Command;               //涓诲懡浠ゅ瓧缁撴瀯
    void    (*pADCIsr)();           /*ADC缁撴潫涓柇鐨勬湇鍔＄▼搴忔寚閽堬紝鍦ㄤ笉鍚岀殑鍦哄悎鏈変笉鍚岀殑涓柇鏈嶅姟绋嬪簭*/
    int     FreqSet;                                    // 鍔熻兘浼犻�掔殑璁惧畾棰戠巼
//  int     FreqSet0;
    int     FreqSetApply;                               // 椹卞姩瀹為檯浣跨敤鐨勮瀹氶鐜�
//  int     FreqSetBak;                                 //鏈�鏂扮殑闈�0缁欏畾閫熷害
    int     FreqSyn;                                    //瀹為檯閫熷害锛堝悓姝ラ�熷害锛�
    int     FreqSynFilter;                              //瀹為檯杈撳嚭鍚屾棰戠巼鐨勬护娉㈠�硷紝鐢ㄤ簬寮辩鍖鸿绠�
    int     FreqWs;                                   // 鐭㈤噺鏃惰绠楃殑杞樊
    int     FreqDesired;                                //鐩爣閫熷害
    int     VCTorqueLim;                                //VC杞煩闄愬畾
    int     FreqToFunc;                                 //鍙嶉缁欏姛鑳芥ā鍧楃殑閫熷害锛屾爣涔堝�艰〃绀�
                                                        // 鐜板湪鍙嶉閫熷害鍒嗕负鍙橀鍣ㄨ繍琛岄鐜囧拰缂栫爜鍣ㄦ祴閫�
    int     FreqFeed;
    s16     FreqFeedReal;
    long    FreqReal;                                   // 瀹為檯杈撳嚭鍚屾棰戠巼(闈炴爣涔堝�艰〃绀�),鍗曚綅0.01Hz
    long    FreqDesiredReal;                            // 瀹為檯鐩爣棰戠巼(闈炴爣涔堝�艰〃绀�),鍗曚綅0.01Hz锛岃繖涓氨鏄潰鏉胯缃殑F0-08鐨勮缃�间箻浠�100锛屽湪鍋滄満鍑忛�熺殑鏃跺�欎細鍏堝彉涓�0锛岀瓑鐢垫満鍋滅ǔ鍚庢湁鍙樹负璁剧疆鍊�
    long    FreqSetReal;                                // 瀹為檯璁惧畾棰戠巼(闈炴爣涔堝�艰〃绀�),鍗曚綅0.01Hz
                                                        // 鍔熻兘浼犻�掔殑棰戠巼鍊艰浆鎹负0.01Hz鏃剁殑绯绘暟
    long    FreqRealFilt;

    Uint    pu2siCoeff;                          //鏍囦箞鍊奸鐜囧熀鍊煎崟浣嶄笌绋嬪簭涓娇鐢ㄧ殑瀹為檯棰戠巼鍗曚綅涓嶅悓锛屽瓨鍦ㄤ竴涓浆鎹㈢郴鏁�
                                                        // 1Hz 杞寲涓哄姛鑳藉皬鏁扮偣鐨勭郴鏁�
    Uint    si2puCoeff;                          //绋嬪簭涓殑瀹為檯棰戠巼杞崲涓烘爣涔堝�奸鐜囷紝闇�瑕佺殑鏍℃绯绘暟銆�
    s32     FreqSetStep;                         //璁惧畾浣嶇疆姝ラ暱
    u16     StepCoeff;                           // 姝ラ暱绯绘暟锛岀敤浜庝袱娆″彂娉㈠垏鎹�
    s16     DetaFreq;               /*璁惧畾棰戠巼鍜屽弽棣堥鐜囩殑鍋忓樊鍊� s16 Q15鏍煎紡*/
//  s32     DuZhuanFreq;                 // 鍫佃浆棰戠巼
    s32     XiuMianFreq;                 // 浼戠湢棰戠巼
    u16     RestarCnt;              //鍔犲噺閫熸爣蹇�
}BASE_COMMAND_STRUCT;                              //瀹炴椂淇敼鐨勫懡浠ょ粨鏋�

// // 浠ヤ笅涓哄拰鐢垫満鎺у埗鐩稿叧璁惧畾鍙傛暟瀹氫箟鏁版嵁缁撴瀯
typedef struct BASE_PAR_STRUCT_DEF {
    Ulong   FullFreq01;                                 // Full freq, SI, 0.01Hz;杩欎釜鍊煎叾瀹炲氨鏄疢axFreq 涔樹互涓�涓猵u2siCoeff(鍊嶇巼锛岄粯璁や负1)锛屽啀鍔�2000.
    Uint    FullFreq;                                   // SI, 灏忔暟鐐逛笌鍔熻兘鍩哄�间竴鑷达紝鐢ㄤ簬灏嗗姛鑳戒紶閫掔殑瀹為檯鍊艰浆鎹负鏍囦箞鍊�
    Uint    MaxFreq;                                    //鏈�澶ч鐜� format: point锛岃繖涓�煎氨鏄疐0-10鐨勮缃�间箻浠�100锛屽強鍗曚綅涓�0.01HZ
    Uint    FcSet;                                      //璁惧畾杞芥尝棰戠巼 鏈� FcSetToMotor*1.5寰楀埌
    Uint    FcSetToMotor;        //杩欎釜鍊兼槸鏈塅0-15 璁惧畾鐨勫�间箻浠�10
    Uint    FcSetApply;                                 //瀹為檯杞芥尝棰戠巼    鍙婂綋鍓嶅疄闄呬娇鐢ㄧ殑杞芥尝棰戠巼       榛樿鍦ㄥ惎鍔ㄧ殑鏃跺�欎负1.5KHZ锛岃繖涓�煎氨涓�15锛屽垏鎹㈠埌瑙掑害闂幆鍚庡氨涓�4KHZ锛岃繖涓�煎氨涓�40
}BASE_PAR_STRUCT;                                   //鍩烘湰杩愯淇℃伅缁撴瀯                                                           F2-37 璁剧疆鏈�浣庤浇娉㈤鐜�

typedef struct COM_PAR_INFO_STRUCT_DEF {
    Uint    StartDCBrakeCur;                            //璧峰姩鐩存祦鍒跺姩鐢垫祦
    Uint    StopDCBrakeCur;                             //鍋滄満鐩存祦鍒跺姩鐢垫祦
    Uint    BrakeCoff;                                  //鍒跺姩鐢甸樆浣跨敤鐜�
    Uint    MotorOvLoad;                                //鐢垫満杩囪浇淇濇姢澧炵泭
    Uint    PerMotorOvLoad;                             //鐢垫満杩囪浇淇濇姢棰勮绯绘暟
    Uint    SpdSearchMethod;                            //閫熷害鎼滅储鏂瑰紡
    Uint    SpdSearchTimeSet;                           //鍔熻兘浼犻�掗�熷害鎼滅储蹇參
    Uint    SpdSearchTime;                              //瀹為檯閫熷害鎼滅储蹇參
}COM_PAR_INFO_STRUCT;                               //鍜岃繍琛屾柟寮忔棤鍏崇殑鍙傛暟璁剧疆缁撴瀯

typedef struct MOTOR_POWER_TORQUE_DEF{
    long    InvPowerPU;                             // 鍙橀鍣ㄨ緭鍑哄姛鐜囨爣涔堝��
    int     InvPower_si;                            // 鍙橀鍣ㄨ緭鍑哄姛鐜�
    long    InvPower_E;                             // 鐢ㄤ簬鍙嶇數鍔ㄥ娍璁＄畻鐨勮緭鍑哄姛鐜�
    int     TrqOut_pu;                              // 杈撳嚭杞煩 0.1%
    int     TrqOutHoAndFo_pu;                       // 鐢ㄤ簬涓讳粠鎺у埗鐨勮浆鐭╃數娴�

    Uint    rpItRated;                              // 棰濆畾鍔涚煩鐢垫祦鐨勫�掓暟 Q12
    int     anglePF;
    int     angleFilter;                            // 鍔熺巼鍥犳暟瑙掓护娉�
    int     Cur_Ft4;
    s16     Cur_T;
    int     Volt_Ft4;
}MOTOR_POWER_TORQUE;

// // 鍏叡妯″潡浣跨敤鐨勬暟鎹粨鏋�
typedef struct CPU_TIME_STRUCT_DEF {
    Ulong   MFA2msBase;
    Ulong   MFB2msBase;
    Ulong   MFC2msBase;
    Ulong   MFD2msBase;
    Ulong   Motor05MsBase;
    Ulong   ADCIntBase;
    Ulong   PWMIntBase;
    Uint    MFA2ms;
    Uint    MFB2ms;
    Uint    MFC2ms;
    Uint    MFD2ms;
    Uint    Motor05Ms;
    Uint    ADCInt;                                 //ADC涓柇鎵ц鏃堕棿
    Uint    PWMInt;                                 //PWM涓柇鎵ц鏃堕棿
    Uint    tmpBase;
    Uint    tmpTime;
    Uint    CpuCoff0Ms;
    Uint    tmp0Ms;
    Uint    Det05msClk;
    Uint    CpuBusyCoff;                            // cpu鏃堕棿鐗囨墽琛屽害绯绘暟
}CPU_TIME_STRUCT;       //缁熻妯″潡鎵ц鏃堕棿鐨勬暟鎹粨鏋�

// // 椹卞姩閮ㄥ垎璋冭瘯鐢ㄦ暟鎹粨鏋�
typedef struct MOTOR_DEBUG_DATA_RECEIVE_STRUCT_DEF{
    int     TestData0;
    int     TestData1;
    int     TestData2;
    int     TestData3;
    int     TestData4;
    int     TestData5;
    int     TestData6;
    int     TestData7;
    int     TestData8;
    int     TestData9;
    int     TestData10;
    int     TestData11;
    int     TestData12;
    int     TestData13;
    int     TestData14;
    int     TestData15;
    int     TestData16;
    int     TestData17;
    int     TestData18;
    int     TestData19;
    int     TestData20;
    int     TestData21;
    int     TestData22;
    int     TestData23;
    int     TestData24;
    int     TestData25;
    int     TestData26;
    int     TestData27;
    int     TestData28;
    int     TestData29;
    int     TestData30;
    int     TestData31;
    int     TestData32;
    int     TestData33;
    int     TestData34;
    int     TestData35;
    int     TestData36;
    int     TestData37;
    int     TestData38;
    int     TestData39;
}MOTOR_DEBUG_DATA_RECEIVE_STRUCT;   //鎬ц兘璋冭瘯鏁版嵁鎺ユ敹

typedef struct MOTOR_DEBUG_DATA_DISPLAY_STRUCT_DEF{
    int     DisplayData1;
    int     DisplayData2;
    int     DisplayData3;
    int     DisplayData4;
    int     DisplayData5;
    int     DisplayData6;
    int     DisplayData7;
    int     DisplayData8;
    int     DisplayData9;
    int     DisplayData10;
    int     DisplayData11;
    int     DisplayData12;
    int     DisplayData13;
    int     DisplayData14;
    int     DisplayData15;
    int     DisplayData16;
    int     DisplayData17;
    int     DisplayData18;
    int     DisplayData19;
    int     DisplayData20;
    int     DisplayData21;
    int     DisplayData22;
    int     DisplayData23;
    int     DisplayData24;
    int     DisplayData25;
    int     DisplayData26;
    int     DisplayData27;
    int     DisplayData28;
    int     DisplayData29;
    int     DisplayData30;
}MOTOR_DEBUG_DATA_DISPLAY_STRUCT;   //鎬ц兘璋冭瘯鏁版嵁鏄剧ず

// // 缁撴瀯浣撳畾涔夌粨鏉�

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

