#include "MotorDefine.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"

#pragma DATA_SECTION(gDebugBuffer, "debug_buffer");

//#define MOTOR_SYSTEM_DEBUG      //���Դ��뿪��
#define CPU_TIME_DEBUG          // CPUʱ��Ƭ����

#ifdef MOTOR_SYSTEM_DEBUG
#ifdef TMS320F2808
#define  C_DEBUG_BUFF_SIZE	    1000		//���Ի�������С
#else   // 28035
#define C_DEBUG_BUFF_SIZE       1
#endif
#else   

#define C_DEBUG_BUFF_SIZE   1

#endif

Uint gDebugBuffer[C_DEBUG_BUFF_SIZE];
Uint gDebugPoint = 0;
int  gDebugFlag;



