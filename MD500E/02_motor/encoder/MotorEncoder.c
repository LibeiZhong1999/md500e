 /***************************************************************
文件功能： 所有编码器测速的处理，ABZ, UVW信号，旋变
文件版本：
最新更新：
	
****************************************************************/
// 说明:
//2808的gpio20-23可以配置为QEP用作编码器测速，也可以配置为spiC用作旋变测速， 在更改时需要初始化其gpio的复用；
//28035的gpio20-23可以配置为QEP，或者gpio口，用模拟旋变测速，需要注意区分；

#include "MotorEncoder.h"
#include "MotorPmsmMain.h"
#include "MotorPmsmParEst.h"
#include "MotorVCInclude.h"
#include "SubPrgInclude.h"

// 全局变量定义
volatile struct EQEP_REGS       *EQepRegs;      // QEP 指针

//PG_DATA_STRUCT			gPGData;        // 编码器公用结构体
IPM_UVW_PG_STRUCT       gUVWPG;         // UVW编码器的数据结构
IPM_PG_DIR_STRUCT       gPGDir;         // 辨识编码器接线方式的结构体
//ROTOR_SPEED_STRUCT		gRotorSpeed;    // 编码器反馈速度数据结构
ROTOR_TRANS_STRUCT		gRotorTrans;    // 旋转变压器速度位置反馈数据
BURR_FILTER_STRUCT		gSpeedFilter;   // 速度反馈滤波结构
CUR_LINE_STRUCT_DEF		gSpeedLine;	    // ..

Uint const gUVWAngleTable[6] = {        // UVW 角度编码表格
    //001                //010               //011
    (330*65536L/360), (210*65536L/360),	(270*65536L/360), 
    //100                //101                //110
    (90 *65536L/360), (30 *65536L/360), (150*65536L/360)       
};

// // 内部函数声明
void SpeedSmoothDeal( ROTOR_SPEED_SMOOTH * pSpeedError,int iSpeed );
void RotorTransCalVel_old(void);
u16 GetRotorTransPos();
/*************************************************************
    旋变硬件接口的初始化， 2808 和28035不一样
*************************************************************/
void InitRtInterface(void)
{
    EALLOW;
#ifdef TMS320F2808                             // 2808 使用spiC与旋变芯片通讯

	SysCtrlRegs.PCLKCR0.bit.SPICENCLK = 1;      //BIT6 SPI-C
    // 读取旋转变压器位置SPI复用
    GpioCtrlRegs.GPAPUD.bit.GPIO21  = 0;        // Enable pull-up
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;        // SPIC MI 
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21= 3;        // Asynch        
    GpioCtrlRegs.GPAPUD.bit.GPIO22  = 0;        // Enable pull-up
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;        // SPIC CLK 
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22= 3;        // Asynch        
    GpioCtrlRegs.GPAPUD.bit.GPIO23  = 0;        // Enable pull-up
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;        // SPIC STE
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23= 3;        // Asynch    

    // Initialize SPI FIFO registers
    SpicRegs.SPIFFTX.all=0xE040;
    SpicRegs.SPIFFRX.all=0x405f;                // Receive FIFO reset
    SpicRegs.SPIFFRX.all=0x205f;                // Re-enable transmit FIFO operation
    SpicRegs.SPIFFCT.all=0x0000;

    // Initialize  SPI
    SpicRegs.SPICCR.all =0x000F;                // Reset on, , 16-bit char bits

    #if 1                                       // 12位PG卡spi模式
    SpicRegs.SPICCR.bit.CLKPOLARITY = 1;        // falling edge receive
    SpicRegs.SPICTL.all =0x000E;                // Enable master mode, SPICLK signal delayed by one half-cycle
                                                // enable talk, and SPI int disabled.
                                                // CLOCK PHASE = 1
    #else                                       // 16位PG卡spi模式
	SpicRegs.SPICCR.bit.CLKPOLARITY = 0;	    //16bit pg
    SpicRegs.SPICTL.all =0x0006;      		    //16bit pg
    #endif
                                       
    // SPI波特率    LSPCLK = 100MHz/4
    SpicRegs.SPIBRR = 48;                       // 100/4 * 10^6 / (49)  = 510KHz
    //SpicRegs.SPIBRR = 0x0006;                         // 100/4 * 10^6 / (6+1) = 3.571MHz
    //SpicRegs.SPIBRR = 0x0005;                         // 100/4 * 10^6 / (5+1) = 4.167MHz
    //SpicRegs.SPIBRR = 0x0004;                         // 100/4 * 10^6 / (4+1) = 5.000MHz
    
    SpicRegs.SPICCR.bit.SPISWRESET = 1;     
    SpicRegs.SPIPRI.bit.FREE = 1;          

    // 旋变的RD信号GPIO34 配置
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;         // Enable pull-up       
    GpioCtrlRegs.GPBMUX1.bit.GPIO34= 0;        
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;         // Set sample to High
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;		    // Resolver sample signal config: output
    
#else                                           // 28035 需要使用IO口模拟串口通讯

    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;        // REDVEL\  ->
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;        // SO       <-
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;        // SCLK     ->

    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0x0;              //滤波设定
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0x0;              //滤波设定
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 0x0;              //滤波设定
    GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0x00; //PULS_IN滤波时间5*5*20ns = 300ns，端口滤波会造成静差

    GpioCtrlRegs.AIOMUX1.bit.AIO10 = 0;         // SAMPLE    AIO-port ->
    GpioCtrlRegs.AIOMUX1.bit.AIO12 = 0;         // RD\       AIO-port ->

    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;         // 
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;         // 
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;         //
    GpioCtrlRegs.AIODIR.bit.AIO10 = 1;
    GpioCtrlRegs.AIODIR.bit.AIO12 = 1;

    ROTOR_TRANS_RDVEL = 1;                      // 一直选择读取位置
#endif
    EDIS;
}


/*************************************************************
	设置QEP接口
*************************************************************/
void InitSetQEP(void)
{  
	/****************新测速修改，wyk******************/
	EALLOW;
	(*EQepRegs).QEPCTL.all = 0;
	(*EQepRegs).QDECCTL.all = 0;
	(*EQepRegs).QEPCTL.bit.PCRM = 1;
	(*EQepRegs).QEPCTL.bit.QPEN = 1;
	(*EQepRegs).QEPCTL.bit.QCLM = 0;
	(*EQepRegs).QEPCTL.bit.IEL = 1;

	(*EQepRegs).QPOSCTL.all = 0;

    (*EQepRegs).QCAPCTL.all = 0;
	(*EQepRegs).QCAPCTL.bit.CEN = 0;
	(*EQepRegs).QCAPCTL.bit.CCPS = 3;		//CAP时钟周期为10*4ns
    (*EQepRegs).QCAPCTL.bit.UPPS = 0;	
	(*EQepRegs).QCAPCTL.bit.CEN = 1;
	(*EQepRegs).QPOSCNT = 0;
	(*EQepRegs).QPOSMAX = 0xFFFFFFFF;

	(*EQepRegs).QEINT.all = 0;
	(*EQepRegs).QCLR.all = 0x0FFF;			//清除中断标志

	(*EQepRegs).QCTMR = 0;
	(*EQepRegs).QEPSTS.bit.COEF = 0;
	(*EQepRegs).QEPSTS.bit.CDEF = 0;

	EDIS;

	ResetSpeedGetReg();
	/****************end***********************/
}

//**************************************************************************
// 修改PG卡类型后，需要重新初始化部分外设

//**************************************************************************
void ReInitForPG(void)
{
    switch(gPGData.PGType)
    {
        case PG_TYPE_UVW:
            gPGData.PGMode = 0;
            EALLOW;
            GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;    //eQEP1A
            GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;    //eQEP1B
            GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;    //eQEP1I
            GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0x2;              //滤波设定
   			GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0x2;              //滤波设定
            GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 0x2;              //滤波设定
			GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0x01; //PULS_IN滤波时间5*5*20ns = 300ns，端口滤波会造成静差
        #ifdef TMS320F2808
            GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;    //eQEP2A
            GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;    //eQEP2B
            GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;    //eQEP2I
        #endif   
        #ifdef TMS320F28035
            GpioCtrlRegs.AIOMUX1.bit.AIO6  = 2;     // ad uvw信号
            GpioCtrlRegs.AIOMUX1.bit.AIO10 = 2;
            GpioCtrlRegs.AIOMUX1.bit.AIO12 = 2;
        #endif
            EDIS;            
            break;
            
        case PG_TYPE_SPECIAL_UVW:
            gPGData.PGMode = 0;
            break;
            
        case PG_TYPE_RESOLVER:      // 区分2808和28035进行配置
            gPGData.PGMode = 1;
            EALLOW;
            SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;//不需要QEP模块
        #ifdef TMS320F2808
            SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;
        #endif
            EDIS;
            
            InitRtInterface();      // 初始化旋变硬件接口
            break;
            
        case PG_TYPE_SC:
            gPGData.PGMode = 0;
            break;
            
        default:        // PG_TYPE_ABZ
            gPGData.PGMode = 0;
            EALLOW;
            GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;    //eQEP1A
            GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;    //eQEP1B
            GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;    //eQEP1I
            GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0x2;              //滤波设定
   			GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0x2;              //滤波设定
            GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 0x2;              //滤波设定
			GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0x01; //PULS_IN滤波时间5*5*20ns = 300ns，端口滤波会造成静差
        #ifdef TMS320F2808
            GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;    //eQEP2A
            GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;    //eQEP2B
            GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 2;    //eQEP2I
        #endif            
            EDIS;
            break;
    }

    // 确保编码器类型修改后，能正确初始化选定的qep
    if(gPGData.PGMode == 0)
    {
        gPGData.QEPIndex = QEP_SELECT_NONE;
    }
    else        // 如果是旋变，就不用再初始化qep接口电路
    {
        gPGData.QEPIndex = (QEP_INDEX_ENUM_STRUCT)gExtendCmd.bit.QepIndex;
    }
}

/************************************************************
    控制电机机械上正转(或者反转)一圈，识别AB信号的时序。
************************************************************/
s16 JudgeABDir(void)
{
	s16 m_Deta;

	m_Deta = gIPMPos.RotorPos - gPGDir.ABAngleBak;
	if(m_Deta > 1820)                               // 1820 = 10度
	{
		gPGDir.ABDirCnt++;
		gPGDir.ABAngleBak = gIPMPos.RotorPos;
	}
	else if(m_Deta < -1820)
	{
		gPGDir.ABDirCnt--;
		gPGDir.ABAngleBak = gIPMPos.RotorPos;
	}

	if(gPGDir.ABDirCnt > 2)    		         return (DIR_FORWARD);
	else if(gPGDir.ABDirCnt < -2)           return (DIR_BACKWARD);
	else							         return (DIR_ERROR);
}

/************************************************************
    控制电机机械上正转(或者反转)一圈，识别UVW信号的时序。
************************************************************/
s16 JudgeUVWDir(void)
{
    int m_Deta;
    
    GetUvwPhase(); 
    m_Deta = (int)gUVWPG.UVWAngle - (int)gPGDir.UVWAngleBak;
    
    if(m_Deta > 1820)   // 10度，10/360  *65536(Q16格式)
    {
        if(gPGDir.UVWDirCnt < 32767)
        {
            gPGDir.UVWDirCnt ++;
        }
        gPGDir.UVWAngleBak = gUVWPG.UVWAngle;
    }
    else if(m_Deta < -1820)
    {
        if(gPGDir.UVWDirCnt > -32767)
        {
            gPGDir.UVWDirCnt --;
        }
        gPGDir.UVWAngleBak = gUVWPG.UVWAngle;
    }

	if(gPGDir.UVWDirCnt > 2)    	        return (DIR_FORWARD);
	else if(gPGDir.UVWDirCnt < -2)         return (DIR_BACKWARD);
	else							        return (DIR_ERROR);
}

/*******************************************************************************
	函数:        UVW_GetUVWState(void)
	描述:        UVW编码器获取UVW绝对位置 ---- 在AD中断中执行
	             检查UVW编码器的UVW信号电平，用1~6表示,并计算对应角度
	判断方法:    首先判断编码器的接线方式，然后查表获取对应的角度
	输入:
	输出:        gUVWPG.UVWAngle
	
	被调用:      PG_Zero_isr(void)
*******************************************************************************/
void GetUvwPhase()
{
    u16  m_UVWAngle;
    if((MOTOR_TYPE_PM != gMotorInfo.MotorType) ||
        (gPGData.PGType != PG_TYPE_UVW)   ||
        (gMainCmd.Command.bit.ControlMode != IDC_FVC_CTL))
    {
        return;
    }
        
// 根据AD采样得到UVW信号的逻辑值
#if(AIRCOMPRESSOR == 0)
    gUVWPG.UVWData   = Get_UVW_PG_U() + Get_UVW_PG_V() + Get_UVW_PG_W();
#else
    gUVWPG.UVWData   = 0;
#endif

    if((gUVWPG.UVWData > 0) && (gUVWPG.UVWData < 7))
    {
        m_UVWAngle =  gUVWAngleTable[gUVWPG.UVWData - 1];
    }
    else if((gMainCmd.Command.bit.Start)&&(gUVWPG.ErrorEnable == 1))
    {
        gError.ErrorCode.all |= ERROR_ENCODER;
        if(gUVWPG.UVWData == 7) gError.ErrorInfo[4].bit.Fault3 = 11;     // uvw信号反短线
        else                    gError.ErrorInfo[4].bit.Fault3 = 12;     // uvw逻辑错误（编码器或者pg卡损坏）
    }

    if(gUVWPG.UvwDir == 0)
    {
        gUVWPG.UVWAngle = m_UVWAngle;
    }
    else
    {
        gUVWPG.UVWAngle = 65536U - m_UVWAngle;
    }
}

/************************************************************
    获取旋转变压器位置信号，时序详见AD2S1205数据手册
* 旋变高速时需要相位补偿；
* 由于辨识零点位置的需要，模拟了一个Z信号
************************************************************/
u16 GetRotorTransPos()
{    
	Uint  mData,mBit;
	static Uint m_RtErrTimes = 0;
	//Uint  mWatie;

    mData = 0;
	DINT;
	ROTOR_TRANS_RD=0;		//begin to transmit data  	    在RD信号的下降沿，数据被传输至输出缓冲器。
// 1st Fall-edge
	ROTOR_TRANS_SCLK  = 0;	//Set SCLK   下降沿后低电平状态读取数据，见数据手册15页时序图
    asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 15;     //MSB-bit15	  数据的最高有效位(DB15)首先从SO引脚逐个输出。          输出移位寄存器为16位宽。数据以16位字格式，通过串行时钟输入(SCLK)从器件输出
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 2nd Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 14;     // bit14
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 3rd Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 13;     // bit13
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 4th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 12;     // bit12
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 5th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 11;     // bit11
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 6th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 10;     // bit10
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 7th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 9;     // bit9
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 8th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 8;     // bit8
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 9th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 7;     // bit7
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 10th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 6;     // bit6
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 11th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 5;     // bit5
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 12th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 4;     // bit4
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 12th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 3;     // bit3
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 12th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 2;     // bit2
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 12th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 1;     // bit1
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 12th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
	mBit   = (ROTOR_TRANS_SO) << 0;     // bit0
	mData  = mData | mBit;	
	ROTOR_TRANS_SCLK = 1;
    asm(" RPT #5 || NOP ");
// 12th Fall-edge
    ROTOR_TRANS_SCLK  = 0;
    //asm(" RPT #5 || NOP ");
    EINT;

	gRotorTrans.RtorBuffer = (Uint)mData;
    gRotorTrans.RtError = gRotorTrans.RtorBuffer & 0x0006;// 旋变故障（主要是未接编码器），DOS和LOS 位在位0的时候是有故障。 DB2为DOS，即信号降级标志位(请参阅“故障检测电路”部分)。位1(LOT)是跟踪丢失标志位(请参阅“故障检测电路”部分)。
    if((gRotorTrans.RtError == 0)&&(IDC_FVC_CTL == gMainCmd.Command.bit.ControlMode)&&(gPGData.DiscDetectDelayTime != 0))
    {
        m_RtErrTimes++;
        if(m_RtErrTimes > 50)
        {
            m_RtErrTimes = 500;
            gError.ErrorCode.all |= ERROR_ENCODER;
            gError.ErrorInfo[4].bit.Fault3 = 13;          
        }
    }
    else
    {
        m_RtErrTimes = 0;
    }
    
    gRotorTrans.RtorBuffer = gRotorTrans.RtorBuffer & 0xFFF0;  //DB15至DB4对应角度信息
	 if(gCtrMotorType != RUN_SYNC_TUNE)//下面是旋变安装和转动方向定义的转换，如从电机出轴端看，逆时针为正方向，及CW，顺时针为反向，及CCW。这样正好与旋变的定义方向相反。应该是定义的CW方向角度是增加的。
	{
		if( gPGData.SpeedDir )  //ABZ,UVW编码器代表AB信号，旋变代表旋变信号
	    {
			gRotorTrans.RtorBuffer = 0xFFFF - gRotorTrans.RtorBuffer;
		}
	}	 
	else
	{
	    if( gPGData.PGDir )   //辨识得到的编码器速度正反向
	    {
			gRotorTrans.RtorBuffer = 0xFFFF - gRotorTrans.RtorBuffer;
		}
	} 


    gRotorTrans.AbsRotPos = gRotorTrans.RtorBuffer >> 4;    // 供功能包含使用的旋变机械位置角度    //DB15至DB4对应角度信息，从键盘上面可以读取此数据

    return(gRotorTrans.RtorBuffer);
}
/*************************************************************
	通过UVW编码器的UVW信号获取段恢�
*************************************************************/
u16 IPMCalAbsPos(void)
{
	if(gPGData.PGType == PG_TYPE_UVW)
	{
		GetUvwPhase();
        return (gUVWPG.UVWAngle + gIPMPos.RotorZero);
	}// UVW编码器
	else if(gPGData.PGType == PG_TYPE_SPECIAL_UVW)
	{
        gUVWPG.UVWAngle = gUVWAngleTable[gUVWPG.UVWExtData - 1];
        return (gUVWPG.UVWAngle + gIPMPos.RotorZero);
	}// 省线方式UVW编码器
    else
    {
        return (gIPMPos.RotorPos);
    }// 不需要校正角度的编码器类型，直接返回当前角度。
}

/***************************************************************************************
1、 编码器断线故障，报故障所需条件:FVC、ABZ编码器(需要验证OC编码器)
2、	通过CF-20进行延时时间的设置，单位为ms，最小分辨率为2ms；考虑将功能码外置
3、	为0时，取消故障；最大延时时间10s
	z1812
***************************************************************************************/

void EncodeLineDiscProtect(void)
{
	if((PG_TYPE_ABZ == gPGData.PGType)&&(IDC_FVC_CTL == gMainCmd.Command.bit.ControlMode))
	{		
		if(Encode_DisConnect)
		{
			gPGData.Cnt ++;
			if((gPGData.DiscDetectDelayTime != 0) && (gPGData.Cnt >= 10))   // 20ms,太长的话可能保护不住
			{
				gError.ErrorCode.all |= ERROR_ENCODER;
		        gError.ErrorInfo[4].bit.Fault3 = 13;		//编码器硬件故障
		        gPGData.Cnt = 0;
			}
            
			if(gPGData.Cnt > 32766)
			{
				gPGData.Cnt = 32766;	
			}
		}
		else
		{
			gPGData.Cnt = 0;			
		}			
	}
}
