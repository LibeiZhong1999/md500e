//======================================================================
//
// Time-stamp: <2010-06-23  LeiMin, 0656>
//
// MODBUS Э��
//======================================================================

#include "f_comm.h"

#define RTU_MASTER_FRAME_NUM_MAX  8      // ��������֡���ַ�����
#define DP_RCV_CRC_ERR_NUMBER   5        // CRCУ��������������ô�������Ϊ����

// MODBUSЭ��
#define RTUslaveAddress rcvFrame[0]      // RTU֡�Ĵӻ���ַ
#define RTUcmd          rcvFrame[1]      // RTU֡��������
#define RTUhighAddr     rcvFrame[2]      // RTU֡�ĵ�ַ���ֽ�
#define RTUlowAddr      rcvFrame[3]      // RTU֡�ĵ�ַ���ֽ�
#define RTUhighData     rcvFrame[4]      // RTU֡�����ݸ��ֽ�
#define RTUlowData      rcvFrame[5]      // RTU֡�����ݵ��ֽ�
#define RTUlowCrc       rcvFrame[6]      // RTU֡��CRCУ����ֽ�
#define RTUhighCrc      rcvFrame[7]      // RTU֡��CRCУ����ֽ�

// DP/CANOPENЭ��
//----------------------------------------------------------------------------//
// ֡ͷ 3.5�ֽ�ʱ��
// ����   �����ַ  ����            ������ַ   ����  ������   ��������    У��
// ����   0xf8     0x47/0x48        xxxx       xxxx  xxxxx    PD1~PD11    CRC16
// Ӧ��   0xf8     0x47/0x48        xxxx       xxxx  xxxxx    PD1~PD11    CRC16
// ����   1byte    1byte            2byte      2byte 2byte    24byte      2byte
//----------------------------------------------------------------------------//
// ֡ͷ
#define DPStartAddr       rcvFrame[0]    // DP����������֡ͷ
// ������
#define DPCmd             rcvFrame[1]    // DP��������������
#define DPCmdHighFuncAddr rcvFrame[2]    // DP����������������ַ��λ
#define DPCmdLowFuncAddr  rcvFrame[3]    // DP�����������������ַ��λ
#define DPDatHighFuncAddr rcvFrame[4]    // DP�����ݵ�ַ��λ
#define DPDatLowFuncAddr  rcvFrame[5]    // DP�����ݵ�ַ��λ
// PZD
#define DPPdCtrlHighWord  rcvFrame[6]    // DP��PZD�����ָ�λ
#define DPPdCtrlLowWord   rcvFrame[7]    // DP��PZD�����ֵ�λ

// KZW
#define DP_START_ADDR       0xf8          // ֡ͷ
#define DP_DATA_MAX_NUM     34            // ��󳤶�
#define DP_FUNCCODE_READ    0x64          // ��������
#define DP_FUNCCODE_WRTITE  0x65          // д������

#define DP_SPEC_ADDR_START_INDEX         (GetCodeIndex(funcCode.code.u3[16]))       //
#define DP_SPEC_ADDR_END_INDEX           (GetCodeIndex(funcCode.code.u3[25]))       //
#define DP_COMM_FRQ_INDEX                (GetCodeIndex(funcCode.code.u3[16]))  // ��Ӧ1000  ͨѶ����Ƶ��
#define DP_COMM_CRTL_WORD_INDEX          (GetCodeIndex(funcCode.code.u3[17]))  // ��Ӧ2000  ͨѶ�趨������
#define DP_COMM_DIGIT_DO_INDEX           (GetCodeIndex(funcCode.code.u3[18]))  // ��Ӧ2001  ͨ�ſ���DO
#define DP_COMM_AO1_OUT_INDEX            (GetCodeIndex(funcCode.code.u3[19]))  // ��Ӧ2002  ͨѶ����AO1
#define DP_COMM_AO2_OUT_INDEX            (GetCodeIndex(funcCode.code.u3[20]))  // ��Ӧ2003  ͨ�ſ���AO2
#define DP_COMM_FPM_OUT_INDEX            (GetCodeIndex(funcCode.code.u3[21]))  // ��Ӧ2004  ͨѶ����FMP
#define DP_COMM_CMD_OUT_INDEX            (GetCodeIndex(funcCode.code.u3[22]))  // ��ӦCMD   ͨѶ������
#define DP_COMM_SPEED_OUT_INDEX          (GetCodeIndex(funcCode.code.u3[23]))  // ��Ӧ�ٶ�  ͨѶ����frq

Uint16 dpFrqAim; 
Uint16 saveEepromIndex = 0;
union DP_STATUS dpStatus;                 // DP��Ƶ��״̬������ 
Uint16 toDpSpeed;
Uint16 Modbus_Standard_ErrInfo[9] = {0, 0x03, 0x01, 0x04, 0x02, 0x03, 0x04, 0x04, 0x04};
//====================================================================
//
// ���ݽ��պ���Ϣ����
//
//====================================================================
void ModbusRcvDataDeal(void)
{   
    if(commRcvData.dpOrModbus == DP)
    {
    	#if 1
    	commRcvData.commAddr = (DPCmdHighFuncAddr << 8) + DPCmdLowFuncAddr;  //  �������ܲ�����ַ
    	
        commRcvData.commData = (DPDatHighFuncAddr << 8) + DPDatLowFuncAddr;  //  ���Ĺ��ܲ���ֵ
       
        if (DPCmd == DP_FUNCCODE_READ)                                       //  ��������
        {
            commRcvData.commCmd = SCI_CMD_READ;
    		commRcvData.commData = 1;                                        //  �̶���һ��
        }
        else if(DPCmd == DP_FUNCCODE_WRTITE)                                 // д�������ұ���EEPROM
        {        
            commRcvData.commCmd = SCI_CMD_WRITE;                             // д�������
            commRcvData.commCmdSaveEeprom = SCI_WRITE_NO_EEPROM;             // ��дEE
            if ((commRcvData.commAddr&0xf000) >= 0xA000)
            {
                commRcvData.commCmdSaveEeprom = SCI_WRITE_WITH_EEPROM;       // дEE
            }
        }
        else
        {
            commRcvData.commCmd = 0;                                         // ������
        }
        commRcvData.slaveAddr = DP_START_ADDR;                               // �ӻ���ַ(DP����)
                                                                        	 // ���յ���CRCУ��ֵ
    	commRcvData.crcRcv = (rcvFrame[commRcvData.rcvNumMax - 1] << 8) + rcvFrame[commRcvData.rcvNumMax - 2];  
    	
    	commRcvData.crcSize = commRcvData.rcvNumMax - 2;                     // CRCУ�鳤��
        #endif
    }
    else
    {
        if(RTUcmd == SCI_CMD_WRITE_MORE)
        {
            commRcvData.slaveAddr = RTUslaveAddress;                // �ӻ���ַ
            commRcvData.commCmd = RTUcmd;                           // ͨѶ����
            commRcvData.commAddr = (RTUhighAddr << 8) + RTUlowAddr; // ������ʼ��ַ
            commRcvData.commData = (RTUhighData << 8) + RTUlowData; // �����Ĵ���������
            
            commRcvData.moreWriteNum = rcvFrame[6];                 // �ֽ���
            
        	commRcvData.crcSize = commRcvData.moreWriteNum + 7; // CRCУ�鳤��
        	commRcvData.crcRcv = (rcvFrame[commRcvData.crcSize+1] << 8) + rcvFrame[commRcvData.crcSize];     // CRCУ��ֵ
        	commRcvData.commCmdSaveEeprom = SCI_WRITE_WITH_EEPROM;  // �洢EEPROM����
        }
        else
        {
            commRcvData.slaveAddr = RTUslaveAddress;                // �ӻ���ַ
            commRcvData.commCmd = RTUcmd;                           // ͨѶ����
            commRcvData.commAddr = (RTUhighAddr << 8) + RTUlowAddr; // ������ַ
            commRcvData.commData = (RTUhighData << 8) + RTUlowData; // ��������
            commRcvData.crcRcv = (RTUhighCrc << 8) + RTUlowCrc;     // CRCУ��ֵ    
        	commRcvData.crcSize = 6;                                // CRCУ�鳤��
        	commRcvData.commCmdSaveEeprom = SCI_WRITE_WITH_EEPROM;  // �洢EEPROM����
        }
    }
}


//====================================================================
//
// MODBUS֡ͷ�ж�
// ����: tmp-����֡����
// ����: 0-֡ͷ�жϹ�����
//       1-����Ҫ֡ͷ�жϣ�ֱ�Ӵ洢��������
//
//===================================================================
Uint16 ModbusStartDeal(Uint16 tmp)
{
    if (commTicker >= 4000)                                     // 2sʱ��
    {
        commRcvData.rcvDataJuageFlag = 0;                       // ��ʱ��û�����꣬֡ͷ�жϸ�λ
    }
    if((funcCode.code.commProtocolSec == EXTEND_COM_CAR)
        &&(commRcvData.rcvDataJuageFlag)
        )
    {
        return 1;
    }
    
    if ((commTicker > commRcvData.frameSpaceTime))              // ����3.5���ַ�ʱ�䣬�µ�һ֡�Ŀ�ʼ
    {
        RTUslaveAddress = tmp;          
        // �㲥ģʽ    
        if (RTUslaveAddress == 0)       
        {
            commRcvData.rcvNum = 1;
            commRcvData.rcvFlag = 1;                           // ���յ�֡�ĵ�һ���ֽڣ����ǹ㲥ģʽ
        }
                                                               // ������ַ
        else if (RTUslaveAddress == funcCode.code.commSlaveAddress) 
        {
            commRcvData.rcvNum = 1;
            commRcvData.rcvFlag = 2;                        
        }
        else if(RTUslaveAddress == DP_START_ADDR)              // DP/CANOPEN
        {
            commRcvData.rcvNum = 1;
            commRcvData.rcvFlag = 2;    
            commRcvData.rcvDataJuageFlag = 1;
        }                                                  
        else                                                   // ������ַ
        {
            commRcvData.rcvFlag = 0;                           // ��ַ����Ӧ�����ݲ����� 
        }
        
        return 0;
    }

    return 1;
}


//====================================================================
//
// ����ͨѶ����
// 1���������ݸ���
// 2����֡��ʼ�ж�ʱ��
// 3��Ӧ���ӳ�ʱ��
//
//====================================================================
void UpdateModbusCommFormat(Uint16 baudRate)
{
    static Uint16 i = 0;
    if(!i)
    {
        commRcvData.rcvNumMax = RTU_MASTER_FRAME_NUM_MAX;      // ��ʼ�����ճ��ȣ�ʵ�ʳ������ж��ж�
        i = 0xcc;
    }
    commRcvData.frameSpaceTime = 385 * 2 / baudRate+ 1;        // 3.5 char time=3.5*(1+8+2)/baud
    commRcvData.delay = (funcCode.code.commProtocolSec == EXTEND_COM_CAR) ? 0 : (funcCode.code.commDelay * 2);
}
//====================================================================//
// name : Uint16 specialAddrShineDeal(Uint16 index,Uint16 data)
// in   : �����±� index;���� data
// out  : ���ڱ��� 0  ,�ڱ��� 1
// modf : zhurf 1513
// Time : 2013-06-14
// ˵�� : �����ַӳ�����(����������Ч��ַ����ûʵ�ʹ�����ĵ�ַ)
//====================================================================//
Uint16 dataDisp;
Uint16 specialAddrShineDeal(Uint16 index,Uint16 data)
{
    int16 frq,speed;
    if((index >= DP_SPEC_ADDR_START_INDEX)&&(index <= DP_SPEC_ADDR_END_INDEX))
    {
        if(DP_COMM_FRQ_INDEX == index)             // FRQ
        {
            dpFrqAim = data;
        }
        else if(DP_COMM_CRTL_WORD_INDEX == index)  // CMD
        {   
            // ��8λ�����8λ������
            commRunCmd = data&0xff;
            if(data >> 8)
            {
                errorOther = ERROR_COMM;
                commErrInfo = COMM_ERROR_EXTENDCAR;
            }
        }
        else if(DP_COMM_DIGIT_DO_INDEX == index)  // DO
        {
            doComm.all = data;
        }
        else if(DP_COMM_AO1_OUT_INDEX == index)  // AO1
        {
            aoComm[1] = data;
        }
        else if(DP_COMM_AO2_OUT_INDEX == index)  // AO2 
        {
            aoComm[2] = data;
        }
        else if(DP_COMM_FPM_OUT_INDEX == index)  // FMP
        {
            aoComm[0] = data;
        }
		#if 0
        else if(DP_COMM_CMD_OUT_INDEX == index)
        {
            // ��8λ�����8λ������            // ������  
            commRunCmd = data&0xff;
            if(data >> 8)
            {
                errorOther = ERROR_COMM;
                commErrInfo = COMM_ERROR_EXTENDCAR;
            }
        }
		#endif
        else if(DP_COMM_SPEED_OUT_INDEX == index)
        {
            speed = data;                          // Ŀ��ת��
            frq = (int32)(int16)speed * polePair * 100 / 60;
            if (ABS_INT16(frq) <= maxFrq)        // ���Ƶ������
            {
                dpFrqAim = frq;
            }  
        }
                                                 // ����RAM,����16��ָU3-16����
        funcCode.code.u3[index - DP_SPEC_ADDR_START_INDEX + 16] = data;
                                                 
        return 1;
    }
	dataDisp = data;
    return 0;
}
//====================================================================
//
// ����DP��Ƶ��״̬��Ϣ
//
//===================================================================
void setDPStatusInfo(void)
{
    dpStatus.all = 0;
    
    // ͣ��/���� ״̬
    dpStatus.bit.run = runFlag.bit.run;     
    // ��ת/��ת ״̬
    dpStatus.bit.fwdRev = runFlag.bit.dir;
    
	// Ŀ��Ƶ�ʵ��� 
    if ((runFlag.bit.run) 
        && (frq == frqAim))
    {

        dpStatus.bit.frqArrive = 1;
    }

    // ����
    if (errorCode)
    {
        dpStatus.bit.error = 1; 
    }
    // ���ϴ���
    dpStatus.bit.errCode = errorCode;
   
}

//====================================================================
//
// ׼����������
// ����: err-ͨѶ����������Ϣ,Ϊ0��ʾ�����ɹ�
// ����: 1���������ݳ���
//       2������������Ϣ
//
// ת��Ϊ��׼��MODBUS��������
// 01,���������
// 02,��ַ����
// 03,���ݴ���
// 04,�����޷�����
//====================================================================
void ModbusSendDataDeal(Uint16 err)
{
//    Uint16 PZD2ReturnData;
    Uint16 cycleReadDataStartIndex;                     // ѭ�����ݿ�ʼ��ַ
	Uint16 indexRead,indexWrite, group, grade;
    Uint16 pzdSaveData;
	Uint16 pzdDataSaveCmd = 0; 
	int16 i;
    if(commRcvData.dpOrModbus == DP)
    {
        #if 1
        commSendData.sendNumMax = DP_DATA_MAX_NUM;
    	// ��������
        sendFrame[0] = DP_START_ADDR;                   // ����֡ͷ
        sendFrame[1] = DPCmd;                           // ��������
    		
    	if (!(DPCmd))                                   // û�в�������
    	{
            sendFrame[2] = 0x0;                         // 
    	    sendFrame[3] = 0x0;                         // ������ַ
    	    sendFrame[4] = 0x0;                         // ���ݸ�λ
            sendFrame[5] = 0x0;                         // ���ݵ�λ
    	}
        else if (err)                                   // �����������������ڹ���
        {
            sendFrame[1] = DPCmd + 0x80;                // �������� +��0x80
            sendFrame[2] = Modbus_Standard_ErrInfo[err]>>8;
            sendFrame[3] = Modbus_Standard_ErrInfo[err]&0x00ff;
    	    //sendFrame[2] = err>>8;                      // ��ַ
    	    //sendFrame[3] = err&0x00ff;                  // 
            sendFrame[4] = 0x0;                         // ���ݱ���
            sendFrame[5] = 0x0;                         // ���ݱ���
        }
        else if (sciFlag.bit.read)                      // ͨѶ������ȡ����
        {
            sendFrame[2] = DPCmdHighFuncAddr;           // �����������ַ��λ
    	    sendFrame[3] = DPCmdLowFuncAddr;            // �����������ַ��λ           
            sendFrame[4] = commReadData[0] >> 8;        // ���ض�ȡֵ
            sendFrame[5] = commReadData[0] & 0x00ff;
        }
        else if (sciFlag.bit.write)
        {
            sendFrame[2] = rcvFrame[2];                // �����������ַ��λ
    	    sendFrame[3] = rcvFrame[3];                // �����������ַ��λ
    	    sendFrame[4] = rcvFrame[4];                // ���ݸ�λ
            sendFrame[5] = rcvFrame[5];                // ���ݵ�λ  
        }
 

    	// PD1��PD2����
        if (!sciFlag.bit.crcChkErr)                    // CRCУ��ɹ�
        {
            commRcvData.rcvCrcErrCounter = 0;           // ���¿�ʼ�ж��Ƿ�CRCУ�����
        }
        setDPStatusInfo();                              // ״̬�� PD1��PD2����
    	if (motorFc.motorCtrlMode == FUNCCODE_motorCtrlMode_FVC)
    	{
            toDpSpeed = frqFdb;                    // �ջ�ʸ��ʱ����ʵ��ת��
    	}
        else
        {
            toDpSpeed = frqRunDisp;                // ����ʸ��ʱ����ͬ��ת��
        }
    	sendFrame[6] = rcvFrame[6];                     // ������
    	sendFrame[7] = rcvFrame[7];
                                              
    	pzdDataSaveCmd = (DPPdCtrlHighWord << 8) + DPPdCtrlLowWord; // PZD������ݱ��������־
        // PROFIBUS/CANOPENЭ���ַΪFE-00~FE-11 FE-20~FE-31
		cycleReadDataStartIndex = 8;                    // ��ʼ�ֽ�
    	                                                
    	for (i = 0; i < 12; i++)                        // �����������������(PZD1~PZD12)
    	{   
            if(pzdDataSaveCmd&(1<<i))                 // ��Ч�����ϴ�       
    	    {
                // PZD�����ַ(��վ->��Ƶ��)
        		group = funcCode.group.fe[i] / 100;
                grade = funcCode.group.fe[i] % 100;
        		indexWrite = GetGradeIndex(group, grade);

                // PZD��� CRCУ��ɹ��Ҳ�ΪF0-00
                if ((!sciFlag.bit.crcChkErr) && (indexWrite))
                {
                    pzdSaveData = (rcvFrame[cycleReadDataStartIndex] << 8) + rcvFrame[cycleReadDataStartIndex + 1];

                    if(1 == specialAddrShineDeal(indexWrite,pzdSaveData)) // �����ַ����
                    {
                        // do nothing 
                    }
                    else if (ModifyFunccodeUpDown(indexWrite, &pzdSaveData, 0) == COMM_ERR_NONE)
                    {
                        ModifyFunccodeEnter(indexWrite, pzdSaveData);
                    }
                }

    	    }
            // ��ȡ��Ԫȫ���ϴ�
            // PZD�����ַ(��Ƶ��->��վ)
    	    group = funcCode.group.fe[i + 20] / 100;
            grade = funcCode.group.fe[i + 20] % 100;
    		indexRead = GetGradeIndex(group, grade);
            if (!indexRead)
            {
                sendFrame[cycleReadDataStartIndex++] = 0;
                sendFrame[cycleReadDataStartIndex++] = 0;
            }
            else
            {
        		// PZD����
        		sendFrame[cycleReadDataStartIndex++] = (funcCode.all[indexRead] >> 8);
            	sendFrame[cycleReadDataStartIndex++] = (funcCode.all[indexRead] & 0x00ff);  
            }

    	} 
        #endif
        
    }
    else
    {
        sendFrame[0] = rcvFrame[0];   // �ظ�����֡ͷǰ2λ
        sendFrame[1] = rcvFrame[1];   // �ظ�����֡ͷǰ2λ
        commSendData.sendNumMax = 8;  // �������ݳ���

        if (err)
        {
            // ��׼��MODBUSЭ��
            if (commProtocol)
            {
                sendFrame[1] = rcvFrame[1]+0x80;   // �ظ�����֡ͷǰ2λ    
                sendFrame[2] = Modbus_Standard_ErrInfo[err];
                commSendData.sendNumMax = 5;      // �������ݳ���
            }
            // �Ǳ�׼��MODBUSЭ��
            else
            {
                sendFrame[2] = 0x80;
        	    sendFrame[3] = 0x01;
                sendFrame[4] = 0x00;
                sendFrame[5] = err;
            }
        }
        else if (sciFlag.bit.pwdPass)           // ����ͨ��������0x8888
        {
            sendFrame[2] = RTUhighAddr;
            sendFrame[3] = RTUlowAddr;
#if DEBUG_MD500_SEARIS||DEBUG_MD290_SEARIS
            sendFrame[4] = RTUhighData;//0x88;
            sendFrame[5] = RTUlowData;//0x88;
#else
            sendFrame[4] = 0x88;
            sendFrame[5] = 0x88;
#endif
    	}
        else if (sciFlag.bit.write)             // д���ݲ����������д����򱨴���������д
        {
            sendFrame[2] = RTUhighAddr;
            sendFrame[3] = RTUlowAddr;
            sendFrame[4] = RTUhighData;
            sendFrame[5] = RTUlowData;
    	}
        else if (sciFlag.bit.read)              // ͨѶ������ȡ����������Ҫ��ȡ
        {
            Uint16 sendNum;
            Uint16 readDataStartIndex;
            sendNum = commRcvData.commData << 1;
            
            // ��׼��MODEBUSͨѶЭ��
            if (commProtocol)
            {
                sendFrame[2] = sendNum;                 // ���յ����ǹ��������*2
                commSendData.sendNumMax = sendNum + 5;  // ������ַ�����
                readDataStartIndex = 3;
            }
            // �Ǳ�׼MODBUSͨѶЭ��
            else if (commProtocol == 0)
            {
                sendFrame[2] = sendNum >> 8;     // �����������λ
                sendFrame[3] = sendNum & 0x00ff; // �����������λ
                commSendData.sendNumMax = sendNum + 6;    // ������ַ�����
                readDataStartIndex = 4;
            }

            // ��ȡ������ֵ
            for (i = commRcvData.commData - 1; i >= 0; i--)
            {
                sendFrame[(i << 1) + readDataStartIndex] = commReadData[i] >> 8;
                sendFrame[(i << 1) + readDataStartIndex + 1] = commReadData[i] & 0x00ff;
            }
        }
    }
}


//====================================================================
//
// ͨѶ�����ж�
// ����: 0��ͨѶ����
//       1��ͨѶ����
//
//====================================================================
Uint16 ModbusCommErrCheck(void)
{
    if ((funcCode.code.commProtocolSec == MODBUS)
        &&(funcCode.code.commOverTime) 
        && (commTicker >= (Uint32)funcCode.code.commOverTime * 2 * TIME_UNIT_sciCommOverTime)
        )
    {
        // MODBUSͨѶ��ʱ����ʱ
        commTicker = 0;  
        errorOther = ERROR_COMM;
        commErrInfo = COMM_ERROR_MODBUS;
        return 1;
    }
    else if((funcCode.code.commProtocolSec == EXTEND_COM_CAR)
            && ((curTime.powerOnTimeSec > 10)|| (curTime.powerOnTimeM > 0)) // �ϵ�ʱ�䳬��10����ж�
            && (funcCode.code.commExtendOverTime)
            && (commTicker >= (Uint32)funcCode.code.commExtendOverTime * 2 * TIME_UNIT_sciCommOverTime)
            )
    {
        commTicker = 0;  
        errorOther = ERROR_COMM;
        commErrInfo = COMM_ERROR_PROFIBUS;
        return 1;
    }
    
    if ((commRcvData.rcvCrcErrCounter > DP_RCV_CRC_ERR_NUMBER) && (funcCode.code.commProtocolSec == EXTEND_COM_CAR))
    {
       // PROFIBUS CRCУ��������
       commRcvData.rcvCrcErrCounter = 0;           // ���¿�ʼ�ж��Ƿ�CRCУ�����
       return 1;
    }

    return 0;

}











