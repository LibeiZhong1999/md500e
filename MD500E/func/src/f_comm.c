//======================================================================
//
// ͨѶ����ModBusЭ�顣
// 
// Time-stamp: <2008-11-23 11:40:25  Shisheng.Zhi, 0354>
//
//======================================================================

#include "f_comm.h"
#include "f_p2p.h"
#include "f_ui.h"
#include "f_osc.h"
#include "f_io.h"

#if F_DEBUG_RAM                     // �����Թ��ܣ���CCS��build option�ж���ĺ�
#define DEBUG_F_MODBUS          0   // �Ƿ�ʹ��ͨѶ����
#elif 1
#define DEBUG_F_MODBUS          1
#endif

enum COMM_STATUS commStatus;    // ���ڳ�ʼ��Ϊ�ȴ�����״̬
COMM_SEND_DATA commSendData;
COMM_RCV_DATA commRcvData;
Uint16 commErrInfo;
Uint16 commBaudRate;
Uint16 commType;                 // ����ͨѶЭ������
Uint16 commProtocol;             // ͨѶЭ�����ݸ�ʽ
Uint32 commTicker;               // *_0.5ms
Uint32 commSendTicker;                  
Uint32 canLinkTicker;            // *_0.5ms
Uint16 commRunCmd;               // ͨѶ��������������
Uint16 aoComm[AO_NUMBER+1];      // ͨѶAO����. 0-HDO, ����ΪAO
union DO_SCI doComm;             // ͨѶDO����
Uint16 userPwdPass4Comm;         // 1-�û�����ͨ������ͨѶFP-01ʹ��
Uint16 companyPwdPass4Comm;      // 1-��������ͨ����FF��ʹ��
Uint16 groupHidePassComm;        // 1-��������������ͨ����AF��ʹ��
LOCALF Uint16 sendFrame[36];                           // �ӻ���Ӧ֡
LOCALF Uint16 rcvFrame[36];                            // ������������(��������֡)
LOCALF Uint16 commReadData[RTU_READ_DATA_NUM_MAX];     // ��ȡ������
LOCALF union SCI_FLAG sciFlag;                         // SCIʹ�õı�־
Uint16 FuncFeData;              // FE������
Uint16 FEHigh_group;
Uint16 FELow_grade;
Uint16 indexgroup;
Uint16 indexgrade;
/************************Һ������ʹ��****************************/
#if FC_KEY_CONTROL_EN
extern Uint16 LcdKeyFlag;  // Һ�����̲����־λ
extern Uint16 LcdReadFEflag; //Һ�����̶�ȡFE��
#endif
/*************************************************/
#if DEBUG_F_MODBUS

#if (DSP_CLOCK == 100)
// 100*10^6/4/(_*8)-1
// ʵ�ʲ������� 100*10^6/4/(_*8)
#if !DSP_2803X
LOCALF const DSP_BAUD_REGISTER_DATA dspBaudRegData[BAUD_NUM_MAX + 1] =  // LSPCLK = SYSCLKOUT/4
{
    {   3, 0x0028, 0x00af},           // 0, 300bps
    {   6, 0x0014, 0x0057},           // 1, 600bps
    {  12, 0x000a, 0x002b},           // 2, 1200bps
    {  24, 0x0005, 0x0015},           // 3, 2400bps
    {  48, 0x0002, 0x008a},           // 4, 4800bps
    {  96, 0x0001, 0x0044},           // 5, 9600bps
    { 192, 0x0000, 0x00a1},           // 6, 19200bps
    { 384, 0x0000, 0x0050},           // 7, 38400bps
    { 576, 0x0000, 0x0035},           // 8, 57600bps
    {1152, 0x0000, 0x001a},           // 9, 115200bps
    {1280, 0x0000, 0x000E},           // 10,208300bps
    {2560, 0x0000, 0x000b},           // 11,256000bps
    {5120, 0x0000, 0x0005},           // 12,512000bps
};
#endif
#elif (DSP_CLOCK == 60)                               // DSP����Ƶ��60MHz

LOCALF const DSP_BAUD_REGISTER_DATA dspBaudRegData[BAUD_NUM_MAX + 1] =  // LSPCLK = SYSCLKOUT/4
{
#if DSP_2803X
    {   3, 0x0000, 0x1869,  0},           // 0, 300bps
    {   6, 0x0000, 0x0C34,  0},           // 1, 600bps
    {  12, 0x0000, 0x0619,  8},           // 2, 1200bps
    {  24, 0x0000, 0x030c,  4},           // 3, 2400bps
    {  48, 0x0000, 0x0185, 10},           // 4, 4800bps
    {  96, 0x0000, 0x00c2,  5},           // 5, 9600bps
    { 192, 0x0000, 0x0060, 11},           // 6, 19200bps
    { 384, 0x0000, 0x002f, 13},           // 7, 38400bps
    { 576, 0x0000, 0x001f,  9},           // 8, 57600bps
    {1152, 0x0000, 0x000f,  4},           // 9, 115200bps
    {1280, 0x0000, 0x0008,  0},           // 10, 208300bps
    {2560, 0x0000, 0x0006, 15},           // 11, 256000bps
    {5120, 0x0000, 0x0002, 11},           // 12, 512000bps
#else
    {   3, 0x0018, 0x0069},           //  0, 300bps
    {   6, 0x000c, 0x0034},           //  1, 600bps
    {  12, 0x0006, 0x0019},           //  2, 1200bps
    {  24, 0x0003, 0x000c},           //  3, 2400bps
    {  48, 0x0001, 0x0085},           //  4, 4800bps
    {  96, 0x0000, 0x00c2},           //  5, 9600bps
    { 192, 0x0000, 0x0060},           //  6, 19200bps
    { 384, 0x0000, 0x002f},           //  7, 38400bps
    { 576, 0x0000, 0x001f},           //  8, 57600bps
    {1152, 0x0000, 0x000f},           //  9, 115200bps
    {1280, 0x0000, 0x0008},           // 10, 208300bps
    {2560, 0x0000, 0x0006},           // 11, 256000bps
    {5120, 0x0000, 0x0002},           // 12,512000bps
#endif
};
#endif

#if DSP_2803X
const Uint16 commParitys[4] = {0x10, 0x0c, 0x04, 0x00};  //[STOP PARITYENA PARITY] 100 011 010 000
#else
const Uint16 commParitys[4] = {0x87, 0x67, 0x27, 0x07};
#endif

// ĳЩ�����룬ͨѶ���ܽ���W
#define COMM_NO_W_FC_0  GetCodeIndex(funcCode.code.tuneCmd)              // ��г
#define COMM_NO_W_FC_1  GetCodeIndex(funcCode.code.menuMode)             // �˵�ģʽ
#define COMM_NO_W_FC_2  GetCodeIndex(funcCode.code.motorFcM2.tuneCmd)    // ��г
#define COMM_NO_W_FC_5  GetCodeIndex(funcCode.code.funcParaView)         // ���ܲ˵�ģʽ����
// ĳЩ�����룬ͨѶ���ܽ���R
#define COMM_NO_R_FC_0  GetCodeIndex(funcCode.code.userPassword)         // �û�����
// ĳЩ�����룬ͨѶ���ܽ���RW
#define COMM_NO_RW_FC_0 GetCodeIndex(funcCode.code.userPasswordReadOnly) // ֻ���û�����
#define COMM_READ_CURRENT_FC GetCodeIndex(funcCode.group.u0[4])          // ͨѶ��ȡ����


const Uint16 COMM_ERR_INDEX[8] = {10, 8, 2, 7, 6, 9, 12, 13};

LOCALD void CommRcvDataDeal(void);
LOCALD void CommSendDataDeal(void);
LOCALD void resetLinSci(void);
LOCALD void closeRTX(void);
LOCALD void setRxConfig(void);
LOCALD void setTxConfig(void);
LOCALD void commErrorDeal(void);
LOCALD void commStatusDeal(void);
LOCALD void CommDataReRcv(Uint16 tmp);
LOCALD void CommDataSend(void);
LOCALD Uint16 CommRead(Uint16, Uint16);
LOCALD Uint16 CommRwFuncCode(Uint16, Uint16, Uint16 rwMode);
LOCALD void inline CommStartSend(void);

#define COMM_READ_FC    0       // ͨѶ��������
#define COMM_WRITE_FC   1       // ͨѶд������

// �ж�
#if DSP_2803X
void SCI_RXD_isr(void);
void SCI_TXD_isr(void);
interrupt void Lina_Level0_ISR(void);    // LIN-SCI �ж�
interrupt void Lina_Level1_ISR(void);    // LIN-SCI �ж�
#else
interrupt void SCI_RXD_isr(void);       // SCI�ж�
interrupt void SCI_TXD_isr(void);       // SCI�ж�
#endif


// RS485�Ľ��շ����л�
#if DSP_2803X
#define RTS (GpioDataRegs.GPBDAT.bit.GPIO39)
#define SET_RTS_R (GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1)
#define SET_RTS_T (GpioDataRegs.GPBSET.bit.GPIO39 = 1)
#else
#define RTS (GpioDataRegs.GPADAT.bit.GPIO27)
#define SET_RTS_R (GpioDataRegs.GPACLEAR.bit.GPIO27 = 1)
#define SET_RTS_T (GpioDataRegs.GPASET.bit.GPIO27 = 1)
#endif
#define RS485_R     0
#define RS485_T     1


// ͨѶЭ�����ݲ�������
// Ŀǰ����MODBUS��PROFIBUS
// �Ƿ��ǽ��ֲֳ�������Э��ҲΪ��������
#define PROTOCOL_NUM    1
const protocolDeal protocolFunc[PROTOCOL_NUM] =
{
// MODBUS
    { ModbusRcvDataDeal,       ModbusStartDeal,      
      UpdateModbusCommFormat,  ModbusSendDataDeal,
      ModbusCommErrCheck,
    },
};


//=====================================================================
//
// ͨѶ������
//
//=====================================================================
void SciDeal(void)
{
    Uint16 commErrFlag;

    // ���´�������
    UpdateSciFormat();

    // ����ͨ����Чʱ��
    if (commTicker >= (Uint32)30 * TIME_UNIT_MS_PER_SEC * 2)    // 30s֮�󣬷���ȨʧЧ
    {
        userPwdPass4Comm = 0;
        companyPwdPass4Comm = 0;
        groupHidePassComm = 0;
    }

    // ͨѶ������
	// MODBUS��ⷽʽΪ����ͣ��ʱ�䳬��ͨѶӦ��ʱʱ������
	// PROFIBUS��ⷽʽΪCRCУ���������ﵽ10������(PROFIBUS�Ƿ��б�Ҫ��������)
    commErrFlag = protocolFunc[commType].CommErrCheck();

    // ͨѶ����
    if (commErrFlag)
    {
        // ͨѶ������
        commErrorDeal();     // �����ϲ��ô���Ϊ����
    }
    else
    {
        // ͨѶ���̴���
        commStatusDeal();
    }
}


//=====================================================================
//
// ͨѶ�����жϺ���
//
//=====================================================================
#if DSP_2803X
void SCI_RXD_isr(void)
#else
interrupt void SCI_RXD_isr(void)
#endif
{
    Uint16 tmp;

#if DSP_2803X
    tmp = LinaRegs.SCIRD;
#else
    tmp = SciaRegs.SCIRXBUF.all;
#endif
    
    // ���ݽ���֡ͷ�ж�
    if (protocolFunc[commType].StartDeal(tmp))
    {
		// Ϊ�����Ľ�������  0-��Ч   1-�㲥��ַ    2-������ַ
		if (commRcvData.rcvFlag)
		{
	        // ��֡ͷͨѶ���ݽ���
	        CommDataReRcv(tmp);
		}
    }
    
    commTicker = 0;                     // �н������ݣ����¼�ʱ
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;    // Issue PIE ACK
}


//=====================================================================
//
// ͨѶ�����жϺ���
//
// ����һ���ַ���ɣ��ͽ�����ж�
//
//=====================================================================
#if DSP_2803X
void SCI_TXD_isr(void)
#else
interrupt void SCI_TXD_isr(void)
#endif
{
	// ͨѶ��������
    CommDataSend();   
    commTicker = 0;                     // ����һ���ַ���ɣ����¼�ʱ                        
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;    // Issue PIE ACK
}


#if DSP_2803X
// �����ȼ��ж�
interrupt void Lina_Level0_ISR(void)
{
	Uint32 LinL0IntVect;  

	LinL0IntVect = LinaRegs.SCIINTVECT0.all;

	// �����ж�
	if(LinL0IntVect == 11)
	{
		SCI_RXD_isr();
	}
	//  �����ж�
	else if(LinL0IntVect == 12)
	{
		SCI_TXD_isr();
	}
    // other
    else
    {
        PieCtrlRegs.PIEACK.bit.ACK9 = 1;
    }
}

//Low priority BLIN ISR.  Just a placeholder.
interrupt void Lina_Level1_ISR(void)
{
	PieCtrlRegs.PIEACK.bit.ACK9 = 1; 
}
#endif


//=====================================================================
//
// ͨѶ���յ����ݴ�����
//
//=====================================================================  
LOCALD void CommRcvDataDeal(void)
{
    Uint16 writeErr;
    Uint16 i,j;
    Uint16 rcvFrameMore[12];
	// ��ͬЭ��Ľ���������Ϣ����
	// �������������ַ�����ݵ���Ϣ
    protocolFunc[commType].RcvDataDeal();
    // ��SCI��־
    sciFlag.all = 0;

    // CRCУ��
    if (commRcvData.crcRcv != CrcValueByteCalc(rcvFrame, commRcvData.crcSize))  // CRCУ������ж�
    {         
        sciFlag.bit.crcChkErr = 1;                      // ��λ��CRCErr��send
		commRcvData.rcvCrcErrCounter++;                 // ��¼CRCУ��������
    }
     // �㲥ģʽ
    else if (!commRcvData.slaveAddr)  
    {
		// �㲥д����
        if ((SCI_CMD_WRITE == commRcvData.commCmd)
          || (SCI_CMD_WRITE_RAM == commRcvData.commCmd)
          || (SCI_CMD_WRITE_MORE == commRcvData.commCmd)
           )
        {
			// ��д��־
            sciFlag.bit.write = 1;
        }
        else
        {   
            sciFlag.bit.cmdErr = 1;                                                 // �������
        }
    }
    else //if (RTUslaveAddress == funcCode.code.commSlaveAddress) // ������ַ�ж�
    {
		if (SCI_CMD_READ == commRcvData.commCmd)       // ���������
        {
            sciFlag.bit.read = 1;                           // ��λ��read��send
        }
        else if ((SCI_CMD_WRITE == commRcvData.commCmd) 
                || (SCI_CMD_WRITE_RAM == commRcvData.commCmd)
                || (SCI_CMD_WRITE_MORE == commRcvData.commCmd))      // д�������
        {
            sciFlag.bit.write = 1;			                        
        }
        else
        {   
            sciFlag.bit.cmdErr = 1;                                                 // �������
        }
    }

    // д���ݴ���
    if (sciFlag.bit.write)
    {

#if 0
        // дEEPROM
        if (SCI_CMD_WRITE == commRcvData.commCmd)
        {
            commRcvData.commCmdSaveEeprom = SCI_WRITE_WITH_EEPROM;
        }
#endif
        if(SCI_CMD_WRITE_MORE == commRcvData.commCmd)
        {	
            if(((commRcvData.commData*2) != commRcvData.moreWriteNum)
                ||(commRcvData.commData > RTU_WRITE_DATA_NUM_MAX))
            {
                writeErr = COMM_ERR_ADDR;
            }
            else
            {
                j=0;
                for(i=0;i<commRcvData.moreWriteNum;i++)
                {
                    rcvFrameMore[j] = (rcvFrame[7+i]<<8) + rcvFrame[8+i];
                    i++;
                	j++;
                }
                writeErr = WriteMultiReg(commRcvData.commAddr, (Uint16*) (&rcvFrameMore[0]),commRcvData.commData, 1);
                if(writeErr)
				writeErr = COMM_ERR_PARA;
                sciFlag.bit.write = 1;
            }
        }
        else
        {
            writeErr = CommWrite(commRcvData.commAddr, commRcvData.commData);  
        }
        // дʧ��
        if (writeErr)
        {
			// ��ʾдʧ�ܹ���
            sciFlag.all |= (0x0001 << COMM_ERR_INDEX[writeErr - 1]);
        }  
    }
}

//=====================================================================
//
// ���ݽ��մ�����������
//
//=====================================================================
Uint16 SciErrCheck(void)
{
    Uint16 readErr;
	Uint16 operateErr;
	
	// ��ֵ���޹���
	operateErr = COMM_ERR_NONE;
    
    // ͨѶ������
    if (sciFlag.bit.read)               // ͨѶ������ȡ����
    {
        if(commRcvData.commData > RTU_READ_DATA_NUM_MAX)    // ����ȡ12������
        {
            sciFlag.bit.paraOver = 1;   //  �����������
        }
        else
        {
            readErr = CommRead(commRcvData.commAddr, commRcvData.commData);
            if (readErr)
            {
				// ���ݶ�ȡ. ��������ù���λ������Ҫ�����Ķ�ȡ
                sciFlag.all |= (0x0001 << COMM_ERR_INDEX[readErr - 1]);                  
            }
        }
    }

	// ������Ϣ����
    if (sciFlag.bit.pwdErr)                 // �������Err01
    {
        operateErr = COMM_ERR_PWD;
    }
    else if (sciFlag.bit.cmdErr)            // ��д�������Err02
    {
        operateErr = COMM_ERR_CMD;
    }
    else if (sciFlag.bit.crcChkErr)         // CRCУ�����: Err03
    {
        operateErr = COMM_ERR_CRC;
    }
    else if (sciFlag.bit.addrOver)          // ��������Ч��ַ��Err04
    {
        operateErr = COMM_ERR_ADDR;
    }
    else if (sciFlag.bit.paraOver)          // ��������Ч������Err05
    {
        operateErr = COMM_ERR_PARA;
    }
    else if (sciFlag.bit.paraReadOnly)      // ����������Ч��Err06
    {
        operateErr = COMM_ERR_READ_ONLY;
    }
    else if (sciFlag.bit.systemLocked)      // ϵͳ����������0x0007
    {
        operateErr = COMM_ERR_SYSTEM_LOCKED;
    }
#if 1   // Ŀǰeeprom��������£������иô��󣬵�������������Ǳ���
    else if (sciFlag.bit.saveFunccodeBusy)  // ���ڴ洢����������0x0008
    {
        operateErr = COMM_ERR_SAVE_FUNCCODE_BUSY;
    }
#endif

	return operateErr;
}


//=====================================================================
//
// ͨѶ�������ݴ�����
//
//=====================================================================
LOCALF void CommSendDataDeal(void)
{
    int16 error;
    Uint16 crcSend;

    // �ж�ͨѶ��д��������
    error = SciErrCheck();

    // ׼��Э�鷢������
    protocolFunc[commType].SendDataDeal(error);

    // ׼��CRCУ������
    crcSend = CrcValueByteCalc(sendFrame, commSendData.sendNumMax - 2);
    sendFrame[commSendData.sendNumMax - 2] = crcSend & 0x00ff;    // CRC��λ��ǰ
    sendFrame[commSendData.sendNumMax - 1] = crcSend >> 8;
}


//=====================================================================
//
// ͨѶд���ݺ���
// (ͨѶҲ�����޸ĵ������)
//
//=====================================================================
Uint16 ConmmWriteAtribte(Uint16 addr, Uint16 data)
{
    Uint16 errType = COMM_ERR_NONE;   // ��ȡ�������������
    Uint16 high = (addr & 0xF000);

// ͨѶ�趨ֵ(ͨѶƵ���趨)
    if (COMM_SET_VALUE_ADDR == addr)
    {
        if ((-10000 > (int16)data) || ((int16)data > 10000))
        {
            errType = COMM_ERR_PARA;
        }
    }
// ͨѶ���������
    else if (COMM_CMD1_ADDR == addr)                 
    {
        if ((data == 0)||(data > 8))
        {
            commRunCmd = 0;
            errType = COMM_ERR_PARA;
        }
    }
// DO����
    else if ((COMM_DO_ADDR == addr) 
            ||(COMM_KEYBORD_TEST == addr)) // ��������
    {
        //  �������ж�
    }
// HDO����
    else if ((COMM_HDO_ADDR == addr)  // HDO����
            ||(COMM_AO1_ADDR == addr) // AO1����
            ||(COMM_AO2_ADDR == addr) // AO2����
            )
    {
        if (data > 0x7FFF)
        {
            errType = COMM_ERR_PARA;
        }
    }
    else if ((addr & 0xFF00) == 0x2f00)   // ͨ�� FE��ר��
    {
        Uint16 dataH,dataL,IndexTmp;
        if((addr & 0xFF00) == 0x2F00)
        {
            dataH = data & 0xf000;
            dataL = data & 0x00ff;
            if(dataH == 0xf000)                  // F
            {
                IndexTmp = (data & 0xf00)>>8;    // FX-00
            }
            //else if(dataH == 0x1000)// 0x1fxx  fp
			else if((data & 0xff00) == 0x1F00)  //by modfied yz1990 2014-06-16
            {
                IndexTmp = 16;
            }
            else if(dataH == 0xA000)             // A
            {
                IndexTmp = (data & 0xf00)>>8;    // FX-00
                IndexTmp +=17;
            }
            else if(dataH == 0xB000)             // B
            {
                IndexTmp = (data & 0xf00)>>8;    // FX-00
                IndexTmp +=33;
            }
            else if(dataH == 0xC000)             //c
            {
                IndexTmp = (data & 0xf00)>>8;    // FX-00
                IndexTmp +=49;
            }
            else if(dataH == 0x7000)             //U0
            {
                IndexTmp = (data & 0xf00)>>8;    // FX-00
                IndexTmp += 67;
            }
            else
            {
                errType = COMM_ERR_ADDR;
            }
            
            if(dataL >= funcCodeGradeAll[IndexTmp])
            {
                errType = COMM_ERR_ADDR;
            }
            return errType;
        }
    }
// д������
    else if ((high == 0x0000)      // Fx-RAM
             || (high == 0xF000)   // Fx
             || (high == 0xA000)   // Ax
             || (high == 0x4000)   // Ax-RAM
             || (high == 0xB000)   // Bx
             || (high == 0x5000)   // Bx-RAM
             || (high == 0xC000)   // Cx
             || (high == 0x6000)   // Cx-RAM
             || ((addr & 0xFF00) == 0x1F00)          // FP��1Fxx
             || ((addr & 0xFF00) == 0x7300)   // U3
        ) 
    {
        errType = CommRwFuncodeAtrrib(addr,data,COMM_WRITE_FC);
    }
    // Ϊͳһ��ַ
    else if((addr & 0xFF00) == 0x7000)
    {
        errType = COMM_ERR_READ_ONLY;
    }
// ��ַԽ��
    else
    {
        errType = COMM_ERR_ADDR;
    }
    return errType;  
}
/*******************************************************************************
// ��������         : Uint16 CommWrite(Uint16 addr, Uint16 data)
// ��ڲ���			: addr  data
// ����				��errType
// ����	            : zhurf1513	
// �汾		        : V0.71
// ʱ��             : 2012-06-28
// ˵��				: ͨѶд����,�����ж����Կ�д��д��Ĵ�����
********************************************************************************/
Uint16 CommWrite(Uint16 addr, Uint16 data)
{
   Uint16 errType = COMM_ERR_NONE;
   errType = ConmmWriteAtribte(addr, data);
   if(errType) // ���Թ��ϣ�ֱ���˳�
   {
       return errType; 
   }
   else
   {
       errType = CommRwFuncCode(addr, data, COMM_WRITE_FC); // ����д��
   }
   return errType;

}
//
// ͨѶ�����ݺ���

/*******************************************************************************
// ��������         : Uint16 CommReadAtribte(Uint16 addr, Uint16 data)
// ��ڲ���			: addr  data
// ����				��errType
// ����	            : zhurf1513	
// �汾		        : V0.71
// ʱ��             : 2012-06-28
// ˵��				: ͨѶ�����������ж�
********************************************************************************/
Uint16 CommReadAtribte(Uint16 addr, Uint16 data)
{
    //int16 i;
    Uint16 high = (addr & 0xF000);
    Uint16 low = (addr & 0x00FF);
    Uint16 errType = COMM_ERR_NONE;
	
// ͨѶ��ȡͣ����������ʾ����
// ����������ȡ�������
    if ((addr & 0xFF00) == 0x1000)      // ͣ��/���в���
    {
        if (low + data > COMM_PARA_NUM)
        {
            errType = COMM_ERR_ADDR;
        }
    }
// ��ȡ��Ƶ������״̬
    else if ((COMM_STATUS_ADDR == addr)   // ��ȡ��Ƶ������״̬
           ||(COMM_INV_ERROR == addr)     // ��ȡ����
           ||(COMM_KEYBORD_TEST == addr)) // ��ȡ��������ֵ 
    {
        if (data > 0x01)
        {
            errType = COMM_ERR_PARA;
        }
    }
#if FC_KEY_CONTROL_EN
	//��ȡLCDJ����DI����״̬
	else if(((addr & 0xFF00) == 0x7100)) //  &&(LcdKeyFlag == 1)
	{

	} 
#endif
// ��ȡ������
    else if ((high == 0xF000) ||     // Fx, ��ȡ������ֵ
             (high == 0xA000) ||     // Ax
             (high == 0xB000) ||     // Bx
             (high == 0xC000) ||     // Cx
             (high == 0x7000) ||     // U0
             ((addr & 0xFF00) == 0x1F00)    // FP
        )
    {
        errType = CommRwFuncodeAtrrib(addr, data, COMM_READ_FC);
    }
// ��ַԽ��
    else    
    {
        errType = COMM_ERR_ADDR;
    }
    return errType;
}
/*******************************************************************************
// ��������         : LOCALD Uint16 CommRead(Uint16 addr, Uint16 data)
// ��ڲ���			: addr  data
// ����				��errType
// ����	            : zhurf1513	
// �汾		        : V0.71
// ʱ��             : 2012-06-28
// ˵��				: ͨѶ�������������жϿɶ����Ŷ�ȡ�Ĵ�����
********************************************************************************/
LOCALD Uint16 CommRead(Uint16 addr, Uint16 data)
{
   Uint16 errType = COMM_ERR_NONE;
   errType = CommReadAtribte(addr, data);
   if(errType) // ���Թ��ϣ�ֱ���˳�
   {
       return errType;
   }
   else
   {
       errType = CommRwFuncCode(addr, data, COMM_READ_FC); // ��ȡ
   }
   
   return errType;
}


Uint16 comIndex;
/*******************************************************************************
// ��������         :LOCALD Uint16 CommRwFuncodeAtrrib(Uint16 addr, Uint16 data, Uint16 rwMode)
// ��ڲ���			: addr  data rwmode
// ����				��errType
// ����	            : zhurf1513	
// �汾		        : V0.71
// ʱ��             : 2012-06-28
// ˵��				: ͨѶ��д�����루�����������ַ�������������жϿɶ�д���Ŷ�д�Ĵ�����
********************************************************************************/
LOCALD Uint16 CommRwFuncodeAtrrib(Uint16 addr, Uint16 data, Uint16 rwMode)
{
    Uint16 index;
       Uint16 group;
       Uint16 grade;
       Uint16 gradeAdd;
       //int16 i;
       Uint16 tmp;
       //Uint16 highH;
       Uint16 funcCodeGradeComm[FUNCCODE_GROUP_NUM];
       Uint16 errType = COMM_ERR_NONE;
       // ��ù��������������
       CommGetFuncCodeGroupGrade(addr,&group, &grade);

   // ����ͨѶ����£���ÿһgroup���û����Բ�����grade����
       UpdataFuncCodeGrade(funcCodeGradeComm);

   // �ж�group, grade�Ƿ����
       if (COMM_READ_FC == rwMode)     // ͨѶ��������
       {
           if(!data) // ��ȡ 0 ������
           {
              errType = COMM_ERR_PARA;
              return errType;
           }
           gradeAdd = data - 1;
       }
       else        // ͨѶд������
       {
           gradeAdd = 0;
       }

       if (grade + gradeAdd >= funcCodeGradeComm[group]) // ��������
       {
           errType = COMM_ERR_ADDR;
           return errType;
       }

       index = GetGradeIndex(group, grade);    // ���㹦�������

   #if FC_KEY_CONTROL_EN
      if(LcdKeyFlag !=1)   //����1ʱ��ʾҺ�����̽���
   #endif
      {
       if ((COMM_NO_RW_FC_0 == index) ||       // ĳЩ�����룬ͨѶ���ܽ���RW
           //(COMM_NO_RW_FC_1 == index) ||
           ((COMM_WRITE_FC == rwMode) &&       // ĳЩ����룬ͨѶ���ܽ���W
            (//(COMM_NO_W_FC_0 == index) ||
             (COMM_NO_W_FC_1 == index) ||
             //(COMM_NO_W_FC_2 == index) ||
             //(COMM_NO_W_FC_3 == index) ||
             //(COMM_NO_W_FC_4 == index) ||
             (COMM_NO_W_FC_5 == index)
             )
            ) ||
           ((COMM_READ_FC == rwMode) &&
            ((COMM_NO_R_FC_0 == index)         // ĳЩ�����룬ͨѶ���ܽ���R
             )
            )
           )
       {
           errType = COMM_ERR_ADDR;            // ��Ч��ַ
           return errType;
       }
       }

   #if DEBUG_MD290_SEARIS   // ���ι����룬ͨѶ���ܽ���RW
       if (((index >= GetCodeIndex(funcCode.code.motorParaM1.elem.rsvdF11[0]))
           &&(index <= GetCodeIndex(funcCode.code.pgParaM1.elem.fvcPgLoseTime))
           )
           ||((index >= GetCodeIndex(funcCode.code.errorAction[4]))
           &&(index <= GetCodeIndex(funcCode.code.errorShow[1]))
           )
           ||((index >= GetCodeIndex(funcCode.code.osChkValue))
           &&(index <= GetCodeIndex(funcCode.code.devChkTime))
           )
           ||((index >= GetCodeIndex(funcCode.code.motorFcM2.motorPara.elem.rsvdF11[0]))
           &&(index <= GetCodeIndex(funcCode.code.motorFcM2.pgPara.elem.fvcPgLoseTime))
           )
           ||((index >= GetCodeIndex(funcCode.code.motorFcM2.vcPara.vcSpdLoopKp1))
           &&(index <= GetCodeIndex(funcCode.code.motorFcM2.vcPara.spdLoopI))
           )
           ||(index ==  GetCodeIndex(funcCode.code.svcMode))
          )
       {
           errType = COMM_ERR_ADDR;            // ��Ч��ַ
           return errType;
       }
   #endif

       // ����У��
       if (COMM_WRITE_FC == rwMode) // д
       {
           // U3��
           if (group == FUNCCODE_GROUP_U3)
           {
               // д��U3 PLC�����Ӳ�������Ϣ
               funcCode.all[index] = data;
               return errType;
           }
           else if (GetCodeIndex(funcCode.code.factoryPassword) == index) // FF-00/0F-00, ��������
           {
               if (COMPANY_PASSWORD == data)   // ����������ȷ
               {
                   companyPwdPass4Comm = 1;
               }
               else
               {
                   errType = COMM_ERR_PARA;        // ��Ч����
               }

               return errType;
           }
           else if (GetCodeIndex(funcCode.code.userPassword) == index) // �û�����
           {
              #if FC_KEY_CONTROL_EN
             if(LcdKeyFlag !=1)//����1��ʾҺ�����̽���
             #endif
             {
               if (data == funcCode.code.userPassword)
               {
                   sciFlag.bit.pwdPass = 1;
                   userPwdPass4Comm = 1;
               }
               else
               {
                   errType = COMM_ERR_PWD;
               }

               return errType;
           }
         }
       }

       if ((group == FC_GROUP_FACTORY) && (!companyPwdPass4Comm)) // FF��
       {
           errType = COMM_ERR_SYSTEM_LOCKED;   // ϵͳ(���ҹ�����)����
           return errType;
       }

       if ((group == FUNCCODE_GROUP_AF) && (!groupHidePassComm))  // AF��
       {
           errType = COMM_ERR_SYSTEM_LOCKED;   // ϵͳ(���ҹ�����)����
           return errType;
       }

       if (COMM_READ_FC == rwMode)     // ͨѶ��������
       {
           return errType;     // ���涼��ͨѶд������Ĵ���
       }

       tmp = ModifyFunccodeUpDown(index, &data, 0);
       if (COMM_ERR_PARA == tmp)
       {
           errType = COMM_ERR_PARA;
       }
       else if (COMM_ERR_READ_ONLY == tmp)
       {
           errType = COMM_ERR_READ_ONLY;
       }
       else
       {
           if (GetCodeIndex(funcCode.code.tuneCmd) == index)
           {
               errType == ValidateTuneCmd(data, MOTOR_SN_1);
               return errType;
           }
           else if (GetCodeIndex(funcCode.code.paraInitMode) == index) // �������ʼ��
           {
               if (!userPwdPass4Comm)
               {
                   errType = COMM_ERR_SYSTEM_LOCKED;
               }
               else if (FUNCCODE_RW_MODE_NO_OPERATION != funcCodeRwMode) // ���ڲ���������
               {
                   errType = COMM_ERR_SAVE_FUNCCODE_BUSY;
               }
               else if (FUNCCODE_paraInitMode_RESTORE_COMPANY_PARA == data) // �ָ���������(�������������)
               {
                   funcCodeRwModeTmp = FUNCCODE_paraInitMode_RESTORE_COMPANY_PARA;
               }
               else if (FUNCCODE_paraInitMode_SAVE_USER_PARA == data)    // �����û�����
               {
                   funcCodeRwModeTmp = FUNCCODE_paraInitMode_SAVE_USER_PARA;
               }
               else if (FUNCCODE_paraInitMode_RESTORE_USER_PARA == data) // �ָ��û�����Ĳ���
               {

                   if ((funcCode.code.saveUserParaFlag1 == USER_PARA_SAVE_FLAG1)
                       && (funcCode.code.saveUserParaFlag2 == USER_PARA_SAVE_FLAG2))
                   {
                       funcCodeRwModeTmp = FUNCCODE_paraInitMode_RESTORE_USER_PARA;
                   }
                   else
                   {
                       errType = COMM_ERR_PARA;
                   }
               }
               else if (FUNCCODE_paraInitMode_CLEAR_RECORD == data) // �����¼��Ϣ
               {
                   ClearRecordDeal();
               }

               else if (FUNCCODE_paraInitMode_RESTORE_COMPANY_PARA_ALL == data) // �ָ���������(�����������)
               {
                   //funcCodeRwModeTmp = FUNCCODE_paraInitMode_RESTORE_COMPANY_PARA_ALL;
               }
               return errType;
           }
           if (GetCodeIndex(funcCode.code.inverterType) == index) // �޸ı�Ƶ�����������ж�
           {
               // ���Ͳ��ܳ�����Χ
               errType = ValidateInvType();
           }
       // ��DI5���Զ���ΪDI_FUNC_APTP_ZERO
           else if ((GetCodeIndex(funcCode.code.diFunc[0]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[1]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[2]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[3]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[5]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[6]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[7]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[8]) == index)
           || (GetCodeIndex(funcCode.code.diFunc[9]) == index)
           )
           {
               if (data == DI_FUNC_APTP_ZERO)
               {
                   errType = COMM_ERR_PARA;
               }
           }

       }

       return errType;
}
/*******************************************************************************
// ��������         : Uint16 CommRwFuncCode(Uint16 addr, Uint16 data, Uint16 rwMode)
// ��ڲ���			: ��ַaddr  ����data  ��дģʽrwMode
// ����				����������errType 0 ��ʾ�޴���
// ����	            : zhurf1513	
// �汾		        : V0.71
// ʱ��             : 2012-06-28
// ˵��				: ��д�����룬ֱ�Ӷ�д��û�о����κ����Ե��ж�
********************************************************************************/
Uint16 CommRwFuncCode(Uint16 addr, Uint16 data, Uint16 rwMode)
{
    Uint16 highH;
    Uint16 errType = 0;
    Uint16 group = 0,grade = 0;
    int16 i;
    Uint16 Index;
    static  Uint16 canOpen = 0;
    Uint16 low = (addr & 0x00FF);
    highH = (addr & 0xF000);
    // ��ȡ���ܲ����������
    CommGetFuncCodeGroupGrade(addr,&group,&grade);
    // ��ȡ�±�
    Index = GetGradeIndex(group, grade); 
    if(COMM_WRITE_FC==rwMode)
    {
    // ��������
        if (COMM_KEYBORD_TEST == addr)
        {
            if (data == 1)
            {
                // ��ʼ�����жϲ���
                keyBordTestFlag = 1;
                keyBordValue = 0;
            }
        }
    // ͨѶ�趨ֵ(ͨѶƵ���趨)
        else if (COMM_SET_VALUE_ADDR == addr)
        {
            funcCode.code.frqComm = data;
            // ����F0-28�������ã�1000��ַ����д
            dpFrqAim = (int32)(int16)funcCode.code.frqComm * maxFrq / 10000;
        }
    // ͨѶ���������
        else if (COMM_CMD1_ADDR == addr)                 
        {
            if (data == 8)
            {
                if (canOpen == 0)
                {
                    commRunCmd = 1;
                    canOpen = 1;
                }
                else
                {
                    commRunCmd = 2;
                    canOpen = 0;                
                }
            }else if ((1 <= data) && (data <= 7)) // ��0001-0007����
            {
                commRunCmd = data;
            } 

        }
    // DO����
        else if (COMM_DO_ADDR == addr)            
        {

            doComm.all = data;
            
        }
    // HDO����
        else if (COMM_HDO_ADDR == addr)            
        {

            aoComm[0] = data;
        }
    // AO1����
        else if (COMM_AO1_ADDR == addr)            
        {
            aoComm[1] = data;
        }
    // AO2����
        else if (COMM_AO2_ADDR == addr)            
        {

            aoComm[2] = data;
        }
		else if ((Index == GetCodeIndex(funcCode.code.tuneCmd))       // F1-37����дEEPROM
		|| (Index == GetCodeIndex(funcCode.code.motorFcM2.tuneCmd))   // A2-37����дEEPROM
		|| (Index == GetCodeIndex(funcCode.code.paraInitMode))        // FP-01����дEEPROM
		|| (Index == GetCodeIndex(funcCode.code.factoryPassword))     // FF-00����дEEPROM
		|| (Index == GetCodeIndex(funcCode.code.userPassword))        // FP-00����дEEPROM
		|| (highH == 0x7300)                                           // U3�鲻��дEEPROM
		)
		{
			errType = COMM_ERR_NONE;
		} 
        else if (Index == GetCodeIndex(funcCode.code.checkDigitalTube))  //F7-00������EEPROM
        {
            funcCode.all[Index] = data;   //RAM
        }

    // д���������
        else if  ((highH == 0x0000)      // Fx-RAM
                 || (highH == 0xF000)   // Fx
                 || (highH == 0xA000)   // Ax
                 || (highH == 0x4000)   // Ax-RAM
                 || (highH == 0xB000)   // Bx
                 || (highH == 0x5000)   // Bx-RAM
                 || (highH == 0xC000)   // Cx
                 || (highH == 0x6000)   // Cx-RAM
                 || ((addr & 0xFF00) == 0x1F00)   // FP��1Fxx
                 || ((addr & 0xFF00) == 0x7300)   // U3
                 ||((addr & 0xFF00) == 0x2F00)    // ͨ��ר�� FE
            )// ͨѶд������
        {
            Uint16 dataH,dataL,IndexTmp;
            if((addr & 0xFF00) == 0x2F00)
            {
                dataH = data & 0xf000;
                dataL = data & 0x00ff;
                if(dataH == 0xf000)                  // F
                {
                    IndexTmp = (data & 0xf00)>>8;    // FX-00
                }
               // else if(dataH == 0x1000)// 0x1fxx  fp
			    else if((data & 0xff00) == 0x1F00)  //by modfied yz1990 2014-06-16
                {
                    IndexTmp = 16;
                }
                else if(dataH == 0xA000)             // A
                {
                    IndexTmp = (data & 0xf00)>>8;    // FX-00
                    IndexTmp +=17;
                }
                else if(dataH == 0xB000)             // B
                {
                    IndexTmp = (data & 0xf00)>>8;    // FX-00
                    IndexTmp +=33;
                }
                else if(dataH == 0xC000)             //c
                {
                    IndexTmp = (data & 0xf00)>>8;    // FX-00
                    IndexTmp +=49;
                }
                else if(dataH == 0x7000)             //UX-00
                {
                    IndexTmp = (data & 0xf00)>>8;    // FX-00
                    IndexTmp += 67;
                }
                else
                {
                    errType = COMM_ERR_ADDR;
                    return errType;
                }

                if(dataL >= funcCodeGradeAll[IndexTmp])
                {
                    errType = COMM_ERR_ADDR;
                }
                else
                {
                    data = IndexTmp*100+dataL;
                    ModifyFunccodeEnter(Index, data); 
                }
            }
            else if (COMM_ERR_PARA == ModifyFunccodeEnter(Index, data))
            {
                errType = COMM_ERR_PARA;
            }
            else
            {
                if ((highH >= 0xA000)                                            // Ϊ����EEPROM��ַ
                    && (commRcvData.commCmdSaveEeprom == SCI_WRITE_WITH_EEPROM)      // �б���EEPROM����
    			)
                {
                    if (FUNCCODE_RW_MODE_NO_OPERATION == funcCodeRwMode)
                    {
                        SaveOneFuncCode(Index);
                    }
                    else                            // ���ڲ���EEPROM������Ӧ
                    {
                        errType = COMM_ERR_SAVE_FUNCCODE_BUSY;
                    }
                }
            }
        }     

    }
    // ͨѶ��������
    else 
    {
    // ͨѶ��ȡͣ����������ʾ����
    // ����������ȡ�������
        if ((addr & 0xFF00) == 0x1000)      // ͣ��/���в���
        {
            for (i = 0; i < data; i++)
            {
                commReadData[i] = funcCode.group.u0[commDispIndex[i + low]];
                
                // ΪͨѶ��ȡ����
                if ((i + low) == DISP_OUT_CURRENT)
                {
                    // ͨѶ��ȡ�����ֱ���Ϊ0.1A
                    if ((funcCode.code.commReadCurrentPoint)
                        ||(funcCode.code.commProtocolSec == EXTEND_COM_CAR)  // DP || CANOPEN �̻�1��С���� 
                        )
                    {
                        if (invPara.type <= invPara.pointLimit) // С����2С����
                        commReadData[i] = commReadData[i] / 10;
                    }
                }
            }
        }
    // ��ȡ��Ƶ������״̬
        else if (COMM_STATUS_ADDR == addr)         
        {

#if FC_KEY_CONTROL_EN
           	 if(tuneCmd&&(LcdKeyFlag ==1))
    		{
    		   if (runFlag.bit.run)
    		   {
    		       commReadData[0] = 0x0005;
    		   }		   
    		   else
    		   {
    		   	   commReadData[0] = 0x0004;
    		   }
    		} 
    		else  
#endif
             if (runFlag.bit.run)
            {
                if (FORWARD_DIR == runFlag.bit.dir) // F0-12֮ǰ�ķ���
                {
                    commReadData[0] = 0x0001;
                }
                else
                {
                    commReadData[0] = 0x0002;
                }
            }
            else
            {
                commReadData[0] = 0x0003;
            }
            
        }
    // ��ȡ����
        else if (COMM_INV_ERROR == addr)     
        {
            commReadData[0] = errorCode;

        }
    // ��ȡ��������ֵ    
        else if (COMM_KEYBORD_TEST == addr)    
        {
            if (keyBordValue == 0x01FF)
            {
                commReadData[0] = 1;
            }
            else
            {
                commReadData[0] = 0;
            }

        }
#if FC_KEY_CONTROL_EN
    	//��ȡLCDJ����DI����״̬
    	else if(((addr & 0xFF00) == 0x7100)) //  &&(LcdKeyFlag == 1)
    	{
          
             switch(low)
             {
    	         case 0:
    			     commReadData[0] =(diStatus.a.all>>16) & 0x0000FFFF;   
    	             break;
    			 case 1:
    			      commReadData[0] = diStatus.a.all & 0x0000FFFF;  
    	             break;
    			 case 2:
    			      commReadData[0] = 0; 
    	             break;
    			 case 3:
    			      commReadData[0] = doStatus.a.all & 0x0000FFFF;  
    	             break;
    			case 4:
    			     commReadData[0] = (diFunc.f1.all>>16) & 0x0000FFFF;  
    	             break;
    			case 5:
    			     commReadData[0] = diFunc.f1.all & 0x0000FFFF;  
    	             break;
    			case 6:
    			     commReadData[0] = (diFunc.f2.all>>16) & 0x0000FFFF;   
    	             break;
    			 case 7:
    			      commReadData[0] = diFunc.f2.all & 0x0000FFFF;   
    	             break;
    			default:break;
             } 
            
    	} 
#endif
    // ��ȡ������
        else if ((highH == 0xF000) ||     // Fx, ��ȡ������ֵ
                 (highH == 0xA000) ||     // Ax
                 (highH == 0xB000) ||     // Bx
                 (highH == 0xC000) ||     // Cx
                 (highH == 0x7000) ||     // U0
                 ((addr & 0xFF00) == 0x1F00)    // FP
            )
        {
            for (i = 0; i < data; i++, Index++)
            {
                commReadData[i] = funcCode.all[Index];  // U0��Ҳ����

                // ΪͨѶ��ȡ����
                if (COMM_READ_CURRENT_FC == Index)
                {
                    // ͨѶ��ȡ�����ֱ���Ϊ0.1A
                    if ((funcCode.code.commReadCurrentPoint)
                        ||(funcCode.code.commProtocolSec == EXTEND_COM_CAR)  // DP || CANOPEN �̻�1��С���� 
                        )
                    {   
                        if (invPara.type <= invPara.pointLimit) // С����2С����
                        commReadData[i] = commReadData[i] / 10;
                    }
                }
            }
//��ȡFE�飬��ʵ����FE��������ȡFE����ӳ��Ĳ�������ַ by modfied yz1990 2014-07-15
			if((addr & 0xFF00) == 0xFE00)
			{ 		
                for (i = 0; i < data; i++)
                {				
                    FEHigh_group = (funcCode.group.fe[(addr & 0xFF)+i] / 100); //ȥFE��ӳ���ַ��λ��ע������FE��ӳ���ַ��Ϊʮ���� ��FE-02=uF3-04 ������FE02��ֵΪ304������0xF304;A1-05,1805,Ŀ�ĺ�funcCodeGradeSum[FUNCCODE_GROUP_NUM]��Ŷ�Ӧ
			        FELow_grade  = (funcCode.group.fe[(addr & 0xFF)+i] % 100);
           
                    commReadData[i] = funcCode.all[GetGradeIndex(FEHigh_group, FELow_grade)];  // U0��Ҳ����

                   // ΪͨѶ��ȡ����				   
				   if (COMM_READ_CURRENT_FC == GetGradeIndex(FEHigh_group, FELow_grade))
				   {
                       // ͨѶ��ȡ�����ֱ���Ϊ0.1A
                       if ((funcCode.code.commReadCurrentPoint)
                          ||(funcCode.code.commProtocolSec == EXTEND_COM_CAR)  // DP || CANOPEN �̻�1��С���� 
                          )
                       {   
                           if (invPara.type <= invPara.pointLimit) // С����2С����
                           commReadData[i] = commReadData[i] / 10;
                       }
                   }
               
             	}
			}
        }
    }	
        return errType;
        
}

/*******************************************************************************
// ��������         : Uint16 CommGetFuncCodeIndex(Uint16 addr, Uint16 data)
// ��ڲ���			: ��ַaddr  ����data  
// ����				����������errType 0 ��ʾ�޴���
// ����	            : zhurf1513	
// �汾		        : V0.71
// ʱ��             : 2012-06-28
// ˵��				: ��ȡͨѶ��������±�
********************************************************************************/
void CommGetFuncCodeGroupGrade(Uint16 addr, Uint16 *group, Uint16 *grade)
{
    Uint16 highH;
    //Uint16 Index;
    highH = (addr & 0xF000);
    // ��ȡgroup, grade
    *group = (addr >> 8) & 0x0F;
    *grade = addr & 0xFF;

    // ΪFE�鹦�ܲ���
#if DEBUG_MD500_SEARIS||DEBUG_MD290_SEARIS
    if ((((addr & 0xFF00) == 0xFE00)||((addr & 0xFF00) == 0x0E00))
        && (LcdReadFEflag!=1)
        )
#else
    if (((addr & 0xFF00) == 0xFE00)
        && (LcdReadFEflag!=1)
        )
#endif
    {
        *group = funcCode.group.fe[*grade] / 100;
        *grade = funcCode.group.fe[*grade] % 100;
    }

    LcdReadFEflag = 0;
    if((addr & 0xFF00) == 0x2F00)
    {
        *group = FUNCCODE_GROUP_FE;
    }
    
    if ((0xA000 == highH) || (0x4000 == highH))       // Ax
    {
        *group += FUNCCODE_GROUP_A0;
    }
    else if ((0xB000 == highH) || (0x5000 == highH))  // Bx
    {
        *group += FUNCCODE_GROUP_B0;
    }
    else if ((0xC000 == highH) || (0x6000 == highH))  // Cx
    {
        *group += FUNCCODE_GROUP_C0;
    }
    else if ((addr & 0xFF00) == 0x1F00)  // FP
    {
        *group = FUNCCODE_GROUP_FP;
    }
    else if (0x7000 == highH)            // U0
    {
        *group += FUNCCODE_GROUP_U0;
    }
////  ��ȡ�±����
}


//=====================================================================
//
// ͨѶ�жϷ��ʹ�������
//
//=====================================================================
LOCALF void inline CommStartSend(void)
{
#if DSP_2803X
    LinaRegs.SCITD = sendFrame[0];     // ���͵�һ������
#else
    SciaRegs.SCITXBUF = sendFrame[0];     // ���͵�һ������
#endif
}


//=====================================================================
//
// ͨѶ���ݽ�������
//
//=====================================================================
#if DSP_2803X
void resetLinSci(void)
{
    LinaRegs.SCIGCR0.bit.RESET = 0;
    LinaRegs.SCIGCR0.bit.RESET = 1;
    LinaRegs.SCIGCR1.bit.SWnRST = 0; 
}


void closeRTX(void)
{
    LinaRegs.SCIGCR1.bit.RXENA = 0;
    LinaRegs.SCIGCR1.bit.TXENA = 0;
}


void setRxConfig(void)
{
    LinaRegs.SCIGCR1.bit.RXENA = 1;
    LinaRegs.SCIGCR1.bit.TXENA = 0;
}


void setTxConfig(void)
{ 
    LinaRegs.SCIGCR1.bit.TXENA = 1;
    LinaRegs.SCIGCR1.bit.RXENA = 0;
}
#endif


//====================================================================
//
// ͨѶ������
//
//===================================================================
void commErrorDeal(void)
{
    errorOther = ERROR_COMM;
    commErrInfo = COMM_ERROR_MODBUS;
    commStatus = SCI_RECEIVE_DATA;        // ��Ϊ����״̬
    SET_RTS_R;  // RTS = RS485_R;         // RTS��Ϊ����
    
    #if DSP_2803X
    EALLOW;        
    setRxConfig();
    EDIS;
    #else
    SciaRegs.SCICTL1.all = 0x0021;  // ����
    SciaRegs.SCICTL2.all = 0x00C2;  // ���������ж�
    #endif
}


//====================================================================
//
// ͨѶ״̬����
//
//===================================================================
void commStatusDeal(void)
{
    switch (commStatus)
    {
        // ��������
        case SCI_RECEIVE_DATA:
            SET_RTS_R;             // RTS = RS485_R;   // RTS��Ϊ����,��ֹ״̬����
            break;
            
        case SCI_RECEIVE_OK:
            CommRcvDataDeal();
            // ��������
            if ((commRcvData.slaveAddr)              // �ǹ㲥
                && ((!sciFlag.bit.crcChkErr)         // CRCУ��ɹ���ΪPROFIBUSЭ��
                || (funcCode.code.commProtocolSec == EXTEND_COM_CAR))
                )
            {
                CommSendDataDeal();                 // ��������׼��
                commStatus = SCI_SEND_DATA_PREPARE; // ���մ�����ɣ�׼������
            }
            else                                    // �㲥��DSP��Ӧ֮�󲻷��ͣ���������
            {
                commStatus = SCI_RECEIVE_DATA;
                SET_RTS_R;  // RTS = RS485_R;       // RTS��Ϊ����
                #if DSP_2803X
                EALLOW;
                setRxConfig();
                EDIS;
                #else
                SciaRegs.SCICTL1.all = 0x0021;  // ����
                SciaRegs.SCICTL2.all = 0x00C2;  // ���������ж�
                #endif
                break;
            }

        // ��������׼��    
        case SCI_SEND_DATA_PREPARE:
            if ((commTicker >= commRcvData.delay)               // Ӧ���ӳ�
                && (commTicker > commRcvData.frameSpaceTime))   // MODBUSΪ3.5���ַ�ʱ��
            {
                SET_RTS_T;  // RTS = RS485_T;                          // RTS��Ϊ����
                #if DSP_2803X
                EALLOW;            
                setTxConfig();
                EDIS;
                #else                    
                SciaRegs.SCICTL1.all = 0x0022;          // ����
                SciaRegs.SCICTL2.all = 0x00C1;          // ���������ж�
                #endif
                commStatus = SCI_SEND_DATA;
                commSendData.sendNum = 1;               // ��ǰ�������ݸ�����Ϊ1
                CommStartSend();                        // ��ʼ����
                commSendTicker = 0;
            }
            break;

        // ��������
        case SCI_SEND_DATA:
            commSendTicker++;
            SET_RTS_T;  // RTS = RS485_T;     // RTS��Ϊ����,��ֹ״̬����
            // ��ʱ�����ݷ��Ͳ��ɹ�(1.5��)
            if (commSendTicker >= ((Uint32)825L*commSendData.sendNumMax/(commBaudRate*10) + 1))   // *2ms
            {
                commStatus = SCI_SEND_OK;
            }
            else
            {
                break;
            }
            
        // ��������OK
        case SCI_SEND_OK:
            #if DSP_2803X
			if (LinaRegs.SCIFLR.bit.TXEMPTY)
            #else
            if (SciaRegs.SCICTL2.bit.TXEMPTY)   // Transmitter empty flag, �����������
            #endif
            {
                commStatus = SCI_RECEIVE_DATA;  // ������Ϻ���Ϊ����״̬
                SET_RTS_R;  // RTS = RS485_R;                  // RTS��Ϊ����
                #if DSP_2803X
                EALLOW;
                setRxConfig();
                EDIS;
                #else
                SciaRegs.SCICTL1.all = 0x0021;  // ����
                SciaRegs.SCICTL2.all = 0x00C2;  // ���������ж�
                #endif
            }
            break;

        default:
            break;
    }
}


//====================================================================
//
// ͨѶ���ݽ��մ���
//
//===================================================================
void CommDataReRcv(Uint16 tmp)
{
    if (commRcvData.rcvNum < commRcvData.rcvNumMax)  // ����һ֡���ݻ�û�����
    {
        rcvFrame[commRcvData.rcvNum++] = tmp;
    }

    if(funcCode.code.commProtocolSec == EXTEND_COM_CAR)    //   �ж���DPģʽ�µ�ר��Э�黹�Ǳ�׼MODBUS
    {
        if((rcvFrame[1] ==SCI_CMD_READ)
            ||(rcvFrame[1] ==SCI_CMD_WRITE)
            ||(rcvFrame[1] ==SCI_CMD_WRITE_MORE))
        {
            commRcvData.dpOrModbus = MD; //  2:modbus
            commRcvData.rcvNumMax = 8;
            // 01 10 f0 08 00 02 04
            if((rcvFrame[1] ==SCI_CMD_WRITE_MORE)&&(commRcvData.rcvNum>=8))
            {
                commRcvData.rcvNumMax = rcvFrame[6] + 9;
            }
        }
        else
        {
            #if 1
            commRcvData.dpOrModbus = DP; // 1:dp 
            commRcvData.rcvNumMax = 34;
            #endif
        }
    }
    else
    {
        commRcvData.dpOrModbus = MD;
        commRcvData.rcvNumMax = 8;
        // 01 10 f0 08 00 02 04
        if((rcvFrame[1] ==SCI_CMD_WRITE_MORE)&&(commRcvData.rcvNum>=8))
        {
            commRcvData.rcvNumMax = rcvFrame[6] + 9;
        }
        
    }
    if (commRcvData.rcvNum >= commRcvData.rcvNumMax) // ����һ֡�������
    {
        // PROFIBUS��֡ͷ�жϱ�־
        commRcvData.rcvDataJuageFlag = 0;
        
        if (2 == commRcvData.rcvFlag)                // ������ַ�ŷ���(DSP�л��ɷ���״̬)
        {
            #if DSP_2803X
            EALLOW;
            closeRTX();                  // �رշ��ͽ���
            EDIS;
            #else
            SciaRegs.SCICTL1.all = 0x0004;      // �رշ��ͽ��գ�sleep
            SciaRegs.SCICTL2.all = 0x00C0;      // �رս��շ����ж�
            #endif
        }
        
        commStatus = SCI_RECEIVE_OK;            // ��־�����������
        commRcvData.rcvFlag = 0;
    }
}


//====================================================================
//
// ͨѶ���ݷ��ʹ���
//
//===================================================================
void CommDataSend(void)
{
	 // ����һ֡����û�����
    if (commSendData.sendNum< commSendData.sendNumMax)          
    {
#if DSP_2803X
        LinaRegs.SCITD = sendFrame[commSendData.sendNum++];
#else
        SciaRegs.SCITXBUF = sendFrame[commSendData.sendNum++];
#endif
    }
	// ����һ֡����ȫ�����
    else if (commSendData.sendNum >= commSendData.sendNumMax)     
    {
		// ��־�����������
        commStatus = SCI_SEND_OK;      
    }
}


//=====================================================================
//
// ͨѶ��ʼ��
//
//=====================================================================
void InitSetScia(void)
{
    // Ӧ�÷���ǰ��
    commStatus = SCI_RECEIVE_DATA;              // ����״̬   
    SET_RTS_R;  // RTS = RS485_R;               // RTS��Ϊ����
#if DSP_2803X
EALLOW;
    // reset
	resetLinSci();

	LinaRegs.SCIGCR1.bit.SLEEP = 0;
    LinaRegs.SCIFLR.bit.TXWAKE = 0;  
    LinaRegs.SCIFLR.bit.TXEMPTY = 1;
    LinaRegs.SCIFLR.bit.TXRDY = 1;
	// ����Ϊ���ݽ���
    setRxConfig(); 

	LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; //Asynchronous Timing
	LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; //Enable SCI Clock
	LinaRegs.SCIGCR1.bit.CONT = 1;		 //Continue on Suspend
	if (funcCode.code.commProtocolSec == EXTEND_COM_CAR)          // ��У��(8-N-1)
        LinaRegs.SCIGCR1.all = (LinaRegs.SCIGCR1.all&0xFFFFFFE3)^commParitys[3];
    else
        LinaRegs.SCIGCR1.all = (LinaRegs.SCIGCR1.all&0xFFFFFFE3)^commParitys[funcCode.code.commParity];
    LinaRegs.SCISETINT.bit.SETRXINT = 1;
    LinaRegs.SCISETINT.bit.SETTXINT = 1;
    LinaRegs.SCIFORMAT.bit.CHAR = 0x7;
    LinaRegs.SCIGCR1.bit.SWnRST = 1; 
       
EDIS;
#else
    SciaRegs.SCICTL1.all = 0x0001;              // SCI�����λ������Ч
    SciaRegs.SCICTL2.all = 0x00C2;
    SciaRegs.SCICCR.all = 0x0087;               // 2 stop bit, No loopback, No parity,8 bits,async mode,idle-line
    SciaRegs.SCIPRI.bit.FREE = 1;
    SciaRegs.SCICTL1.all = 0x0021;              // Relinquish SCI from Reset
#endif
}


//=====================================================================
//
// SCIͨѶ�����޸ĺ���
//
//=====================================================================
void UpdateSciFormat(void)
{
    Uint16 digit[5];
    Uint16 tmp;
    static Uint16 commParityBak = 0;
	// ���ͨѶ������
    GetNumberDigit1(digit, funcCode.code.commBaudRate);
    //commType = MODBUS;     // ͨѶ����
    commType = 0;     // ͨѶ���� MD DP CANOPEN ����
    // ΪPROFIBUS
    if (funcCode.code.commProtocolSec == EXTEND_COM_CAR)          // CANOPEN��Modbus����
    {
		tmp = digit[1] + 9;  // ������ 
    }
    // ΪMODBUS
    else 
    {
		tmp = digit[commType]; // ������
    }
    
    // ���ͨѶ���ݴ��͸�ʽ   
    GetNumberDigit1(digit, funcCode.code.commProtocol);
    commProtocol = digit[commType];
    
	// ����ͨѶ�����ļ�
    protocolFunc[commType].UpdateCommFormat(dspBaudRegData[tmp].baudRate);

    commBaudRate = dspBaudRegData[tmp].baudRate;
    
 // ���ֽ��չ���ʱ����    
#if DSP_2803X
	if ( LinaRegs.SCIFLR.bit.BRKDT
        || LinaRegs.SCIFLR.bit.PE 
        || LinaRegs.SCIFLR.bit.OE
        || LinaRegs.SCIFLR.bit.FE)
#else
    if (SciaRegs.SCIRXST.bit.RXERROR)      
#endif
    {
        InitSetScia();   // ��ʼ��SCI�Ĵ���
    }
#if DSP_2803X

EALLOW;
    if (funcCode.code.commProtocolSec == EXTEND_COM_CAR)          // ��У��(8-N-1)
        LinaRegs.SCIGCR1.all = (LinaRegs.SCIGCR1.all&0xFFFFFFE3)^commParitys[3];
    else
        LinaRegs.SCIGCR1.all = (LinaRegs.SCIGCR1.all&0xFFFFFFE3)^commParitys[funcCode.code.commParity];
    
    LinaRegs.BRSR.bit.SCI_LIN_PSH = dspBaudRegData[tmp].high;
    LinaRegs.BRSR.bit.SCI_LIN_PSL = dspBaudRegData[tmp].low;
    LinaRegs.BRSR.bit.M = dspBaudRegData[tmp].M;
EDIS;
    if(commParityBak != funcCode.code.commParity ) // ���ݸ�ʽ�����ı���Ҫ��ʼ�������� 8-N-1 �л���8-E/O-1������
    {
        InitSetScia();   // ��ʼ��SCI�Ĵ���
    }
    commParityBak = funcCode.code.commParity;
#else
    if (funcCode.code.commProtocolSec == EXTEND_COM_CAR)
        SciaRegs.SCICCR.all = commParitys[3];               // ��У��(8-N-1)
    else
        SciaRegs.SCICCR.all = commParitys[funcCode.code.commParity];
    
    SciaRegs.SCIHBAUD = dspBaudRegData[tmp].high;
    SciaRegs.SCILBAUD = dspBaudRegData[tmp].low;
    SciaRegs.SCICTL1.bit.SWRESET = 1;
#endif

}


#if 1//F_DEBUG_RAM    // �����Թ��ܣ���CCS��build option�ж���ĺ�
void InitSciaGpio(void)
{
#if DSP_2803X  // 2803xƽ̨
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;		// Enable pull-up for GPIO14 (LIN TX)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;		// Enable pull-up for GPIO15 (LIN RX)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (LINRXA)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 0x01;  // No qualification for all group A GPIO 0-15
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0x03;
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for LIN TX operation (2-Enable,0-Disable)
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for LIN RX operation (2-Enable,0-Disable)
    GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;    
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;        // Configure GPIO39, RTS
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;         // output
    GpioDataRegs.GPBDAT.bit.GPIO39 = RS485_R;   // Receive
    EDIS;
// ͨѶ����ʹ���жϣ���ʼ��
    EALLOW;
    PieVectTable.LIN0INTA = &Lina_Level0_ISR;
    PieVectTable.LIN1INTA = &Lina_Level1_ISR;
    EDIS;
	IER |= M_INT9;   	                 // Enable interrupts:
	PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3
	PieCtrlRegs.PIEIER9.bit.INTx4=1;     // PIE Group 9, INT4	
#else
    EALLOW;    
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // Enable pull-up for GPIO29 (SCITXDA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;    
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;        // Configure GPIO27, RTS
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;         // output
    GpioDataRegs.GPADAT.bit.GPIO27 = RS485_R;   // Receive
    EDIS;
    // ͨѶ����ʹ���жϣ���ʼ��
    EALLOW;
    PieVectTable.SCIRXINTA = SCI_RXD_isr;
	PieVectTable.SCITXINTA = SCI_TXD_isr;
    EDIS;
	IER |= M_INT9;   	            //  Enable interrupts:
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
 #endif   
	
}
#endif  // #if F_DEBUG_RAM

#else   // #if DEBUG_F_MODBUS
void InitSetScia(void){}
void InitSciaGpio(void){}
void SciDeal(void){}
void UpdateSciFormat(void){}
interrupt void SCI_RXD_isr(void){}
interrupt void SCI_TXD_isr(void){}
Uint16 CommWrite(Uint16 addr, Uint16 data){}
Uint16 CommRead(Uint16 addr, Uint16 data){}
#endif










