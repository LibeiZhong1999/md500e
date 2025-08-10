/*************** (C) COPYRIGHT 2012  Inovance Technology Co., Ltd****************
* File Name          : f_CANlinkEx.c
* Author             : Yanyi	
* Version            : V0.0.1
* Date               : 06/15/2012
* Description        : CANlinkʵ���ⲿ���ܣ��û��Զ���ʵ��

********************************************************************************/

#include "f_funcCode.h"
#include "f_comm.h"
#include "f_p2p.h"
#include "f_main.h"
#define DEBUG_F_CANLINKEX   1
#if DEBUG_F_CANLINKEX 
Uint16 Canlink_ErrInfo[9] = {0, 0x03, 0x01, 0x04, 0x02, 0x03, 0x04, 0x04, 0x04};
Uint16 CanNodeRunStat;                                      // �ڵ�����״̬

#if DEBUG_MD500_SEARIS
Uint16 gDeviceInformation[12] = {
                                1,                          // �㴨����
                                11,                         // ��Ƶ����Ʒ
                                273,                        // 0x111 MD500E��Ƶ��
                                SOFTWARE_VERSION*100+SOFTWARE_VERSION_TEMP,//��Ʒ�汾��
								
                                
                                300,                        // CANlink3.0 V3.00
                                0,
                                0,
                                0
                                };
#elif DEBUG_MD290_SEARIS
Uint16 gDeviceInformation[12] = {
                                1,                          // �㴨����
                                11,                         // ��Ƶ����Ʒ
                                20,                         // MD290��Ƶ��
                                //2900,                       // 0xB54 ��Ʒ�汾��
                                SOFTWARE_VERSION*100+SOFTWARE_VERSION_TEMP,//��Ʒ�汾��
                                
                                300,                        // CANlink3.0 V3.00
                                0,
                                0,
                                0
                                };
#elif DEBUG_MD380E_SEARIS
Uint16 gDeviceInformation[12] = {
                                1,                          // �㴨����
                                11,                         // ��Ƶ����Ʒ
                                271,                         // MD380E��Ƶ��
                                SOFTWARE_VERSION*100+SOFTWARE_VERSION_TEMP,//��Ʒ�汾��
                                
                                300,                        // CANlink3.0 V3.00
                                0,
                                0,
                                0
                                };
#else
Uint16 gDeviceInformation[12] = {
                                1,                          // �㴨����
                                11,                         // ��Ƶ����Ʒ
                                263,                        // 0X107 MD380��Ƶ��
                                //7500,                       // 0X1D4C��Ʒ�汾��
                                SOFTWARE_VERSION*100+SOFTWARE_VERSION_TEMP,//��Ʒ�汾��
                                
                                300,                        // CANlink3.0 V3.00
                                0,
                                0,
                                0
                                };
#endif

/*******************************************************************************
* ��������          :   Uint16 ReadSingleReg(Uint16 addr, Uint16 *data)
* ��ڲ���			: 	addr   ��ַ
						*data  ���ݻ���
* ����				:   CANLINK_ADDR_ERROR		2						// ��ַ����
*                       CANLINK_DATA_ERROR		3						// ���ݴ���
*                       CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   �������Ĵ�����������
********************************************************************************/
Uint16 ReadSingleReg(Uint16 addr, Uint16 *data)
{
    Uint16 oscReturn;
    // ���ݶ�����
    sciFlag.all = 0;                                        // ����ͨѶ״̬��־
    oscReturn = CommRead(addr, 1);
    oscReturn = Canlink_ErrInfo[oscReturn];
    *data = commReadData[0];
    return oscReturn;    

}

/*******************************************************************************
* ��������          :   Uint16 WriteRegFunTest(Uint16 addr, Uint16 data, Uint16 daTest)
* ��ڲ���			: 	addr   ��ַ
						data   ����
						daTest "0"�����Ե�ַ    "1" ��ַ���ݶ�����
* ����				:   CANLINK_NO_ERROR		0						// �޴���
*                       CANLINK_ADDR_ERROR		2						// ��ַ����
*                       CANLINK_DATA_ERROR		3						// ���ݴ���
*                       CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   д�Ĵ������Ժ�������ַ���Խ����ڲ��Ե�ַ,ĳЩ�������жϵ�ַ�Ƿ���Ч��
*                                         ���ݲ��Ժ�ɵ���"WriteRegFun"�������д����
********************************************************************************/
Uint16 WriteRegFunTest(Uint16 addr, Uint16 data, Uint16 daTest)
{
    Uint16 errType = 0;
    errType = ConmmWriteAtribte(addr, data); // дģʽ
    errType = Canlink_ErrInfo[errType];
    return errType;
}

/*******************************************************************************
* ��������          :   Uint16 WriteRegFun(Uint16 addr, Uint16 data, Uint16 eeprom)
* ��ڲ���			: 	addr   ��ַ
						*data  ���ݻ���
						eeprom "1" ���籣��
* ����				:   CANLINK_NO_ERROR		0						// �޴���
*                       CANLINK_ADDR_ERROR		2						// ��ַ����
*                       CANLINK_DATA_ERROR		3						// ���ݴ���
*                       CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   д�Ĵ���������ֱ��д�������κ��ж�
********************************************************************************/
Uint16 WriteRegFun(Uint16 addr, Uint16 data, Uint16 eeprom)
{
    Uint16 oscReturn;
    extern Uint16 CommRwFuncCode(Uint16, Uint16, Uint16);
    
	// ����д����
    if (eeprom)
        commRcvData.commCmdSaveEeprom = SCI_WRITE_WITH_EEPROM;
    else
        commRcvData.commCmdSaveEeprom = SCI_WRITE_NO_EEPROM;    // ���ؽ�����RAM
    sciFlag.all = 0;                                        // ����ͨѶ״̬��־

    oscReturn = CommRwFuncCode(addr, data, 1); // COMM_WRITE_FC
    oscReturn = Canlink_ErrInfo[oscReturn];
    //-------------------------//
    commRcvData.commCmdSaveEeprom = SCI_WRITE_NO_EEPROM;    // ���ؽ�����RAM

    return oscReturn;

}

/*******************************************************************************
* ��������          :   Uint16 WriteSingleReg(Uint16 addr, Uint16 data, Uint16 eeprom)
* ��ڲ���			: 	addr   ��ַ
						*data  ���ݻ���
						eeprom "1" ���籣��
* ����				:   CANLINK_NO_ERROR		0						// �޴���
*                       CANLINK_ADDR_ERROR		2						// ��ַ����
*                       CANLINK_DATA_ERROR		3						// ���ݴ���
*                       CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   д�Ĵ������Ժ���
********************************************************************************/
Uint16 WriteSingleReg(Uint16 addr, Uint16 data, Uint16 eeprom)
{
    Uint16 reStat;
    reStat= WriteRegFunTest(addr, data, 1);
    if (reStat)
        return reStat;
    WriteRegFun(addr, data, eeprom);    
    return reStat;
}

/*******************************************************************************
* ��������          :   Uint16 WriteMultiReg(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom)
* ��ڲ���			: 	addr   ��ַ
						*data  ���ݻ���
						eeprom "1" ���籣��
* ����				:   CANLINK_NO_ERROR		0						// �޴���
*                       CANLINK_ADDR_ERROR		2						// ��ַ����
*                       CANLINK_DATA_ERROR		3						// ���ݴ���
*                       CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   д�Ĵ���������ֱ��д�������κ��ж�
*                       �����ж��� WriteRegFunTest ��ɣ��������ϳ�����д����
********************************************************************************/
Uint16 WriteMultiReg(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom)
{
    Uint16 i, reStat = 0;
    for (i=0; i<len; i++)
    {
        reStat = WriteRegFunTest(addr+i, data[i], 1);
        if (reStat)                                         // д�Ĵ�������
        {
            return reStat;
        }
    }
    for (i=0; i<len; i++)                                   // ��ʵд�Ĵ���
    {
        WriteRegFun(addr+i, data[i], eeprom);
    }
    return reStat;                                          // ����ִ��״̬
}

/*******************************************************************************
* ��������          :   Uint16 WriteRegFunTest(Uint16 addr, Uint16 data, Uint16 daTest)
* ��ڲ���			: 	addr   ��ַ
						data   ����
						daTest "0"�����Ե�ַ    "1" ��ַ���ݶ�����
* ����				:   CANLINK_NO_ERROR		0						// �޴���
*                       CANLINK_ADDR_ERROR		2						// ��ַ����
*                       CANLINK_DATA_ERROR		3						// ���ݴ���
*                       CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   д�Ĵ������Ժ��������ڲ��Ե�ַ��ĳЩ�������жϵ�ַ�Ƿ���Ч��
*                                         ���ݲ��Ժ�ɵ���"WriteMultiReg"�������д����
********************************************************************************/
Uint16 WriteMultiRegTest(Uint16 addr, Uint16 *data, Uint16 len, Uint16 daTest)
{
    Uint16 i, err = 0;
    for (i=0; i<len; i++)
    {
        err = WriteRegFunTest(addr, data[i], daTest);
        if (err)
            break;            
    }
    return err;
}

/*******************************************************************************
* ��������          :   void WriteMultiRegFun(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom)
* ��ڲ���			: 	addr   ��ַ
						*data  ���ݻ���
						eeprom "1" ���籣��
* ����				:   ��
* ����	            : 	
* �汾		        :   V0.0.1
* ʱ��              :   06/15/2012
* ˵��				:   д�Ĵ���������������κ��ж?
*                       �����ж��� WriteRegFunTest ��ɣ��������ϳ�����д����
********************************************************************************/
void WriteMultiRegFun(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom)
{
    Uint16 i;
    for (i=0; i<len; i++)
    {
        WriteRegFun(addr, data[i], eeprom);
    }
}

/*******************************************************************************
* ��������          : Uint16 ReadMultiReg(Uint16 addr, Uint16 *data, Uint16 len)
* ��ڲ���			: addr  д������ʼ��ַ
*                     data  ���ݻ���
*                     len   д����
* ����				��CANLINK_NO_ERROR		    0						// �޴���
*                     CANLINK_ADDR_ERROR		2						// ��ַ����
*                     CANLINK_DATA_ERROR		3						// ���ݴ���
*                     CANLINK_FUN_DIS_ERROR 	4						// ���ܽ�ֹ����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink����֡��Ͷ�Ĵ��������?********************************************************************************/
Uint16 ReadMultiReg(Uint16 addr, Uint16 *dataPi, Uint16 len)
{
    Uint16 i, reStat;
    for (i=0; i<len; i++)
    {
        reStat = ReadSingleReg(addr++, dataPi++);          // �����Ĵ�������
        if (reStat)                                         // д�Ĵ�������
        {
            return reStat;
        }
    }
    return reStat;                                          // ����ִ��״̬
}

/*******************************************************************************
* ��������          : void CanlinkOnline(void)
* ��ڲ���			: 
* ����				:
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 08/03/2012
* ˵��				: CANlink�������ӣ������ѱ�����
********************************************************************************/
void CanlinkOnline(void)
{

}

/*******************************************************************************
* ��������          : void CanlinkMoniTimeout(void)
* ��ڲ���			: 
* ����				:
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink�������ʱ����CANlink���ߵ���
********************************************************************************/
void CanlinkMoniTimeout(void)
{
    // ͨѶ���ʱ�䲻Ϊ0
	// ������ʱ��ֱ�ӹ��ϡ�add 1513 12-11-19
    //if (funcCode.code.canOverTime)
    {
        errorOther = ERROR_COMM;
        commErrInfo = COMM_ERROR_CANLINK; 
    }
}

/*******************************************************************************
* ��������          : void CanlinkSafeFun(void)
* ��ڲ���			: 
* ����				:
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink�豸��ȫ������CANlink�豸���밲ȫģʽ�����ú���
********************************************************************************/
void CanlinkSafeFun(void)
{
    
    WriteSingleReg(0x2000, 6, 0);                           // дͨѶ����ͣ��
    
}

/*******************************************************************************
* ��������          : void CanlinkAddrErrro(void)
* ��ڲ���			: 
* ����				:
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink��ַ��ͻ����
********************************************************************************/
void CanlinkAddrErrro(void)
{
    errorOther = ERROR_COMM;
    commErrInfo = COMM_ERROR_CANLINK_ADDR;     
}

/*******************************************************************************
* ��������          : void CANlinkDeviceStat(void)
* ��ڲ���			: 
* ����				:
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink�豸״̬������
********************************************************************************/
void CANlinkDeviceStat(void)
{
    CanNodeRunStat = 0;
    if (errorCode)       // ���� BIT0:1,û����BIT0:0
    {
        CanNodeRunStat |= 0x01;
    }
    if (runFlag.bit.run) // ���� BIT1:1,ͣ��  BIT1:0
    {
        CanNodeRunStat |= 0x01<<1;
    }
}
#endif
#if 0 //DEBUG_F_MSC_CTRL
Uint16 MscSlaveNodeId[MAX_SLAVE_NODE_NUM];             // �������ƵĴӻ��ڵ��б�
Uint16 MscSlaveCfgBuf[MAX_SLAVE_NODE_NUM];             // ��Ҫ�����������õĴӻ��б�
Uint16 MscSlaveNodeBreakTcnt[MAX_SLAVE_NODE_NUM];      // �����Դӻ���������ʱ
Uint16 MscMasterSendBeatCount;                         // ������������ʱ
extern Uint32 gCanlinkMoniBeat; 
extern Uint16 gCanlinkBeatCount;                       // CANlink�����������ʱ��ʱ���������ͼ�ʱ
extern Uint16 gCanlinkBeatTime, gCanlinkMoniAddr;      // CANlink����ʱ�䣬�������ַ

#if 0
/*******************************************************************************
* ��������          : void sendRemotFrame(CANlinkDataBuf *dataPi)
* ��ڲ���			: *dataPi
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: ��Զ��֡��
********************************************************************************/

Uint16 sendRemotFrame(CANlinkDataBuf *dataPi,Uint16 hurtTime, Uint16 nodeId)
{
 	dataPi->msgid.bit.srcSta = 0xff	;					    // ����
    dataPi->msgid.bit.destSta = nodeId;                     // Ŀ���ַ
 	dataPi->msgid.bit.code = 0xff;                          // ����
    dataPi->msgid.bit.aq = CANlink_ASK;						// Զ��֡ʹ�ø�λ��Ӧ
	dataPi->msgid.bit.framf = CAN_REMOTE_FRAME;				// Զ��֡
	dataPi->data.dByte.dByte2 = CAN_LINK_S_ADDR;            // ������ַ
    dataPi->data.dByte.dByte1 = hurtTime;                   // ����ʱ��

	return CanlinkDataTran(dataPi, 4, 1000);			    // ����Զ��֡��Ӧ
}

/*******************************************************************************
* ��������          : void revieceCfgFrame(CANlinkDataBuf *dataPi)
* ��ڲ���			: *dataPi
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: ������֡��
********************************************************************************/
Uint16 revieceCfgFrame(CANlinkDataBuf *dataPi,Uint16 cfgNum, Uint16 nodeId)
{
 	dataPi->msgid.bit.srcSta = CAN_LINK_S_ADDR;			    // ��������ַ
    dataPi->msgid.bit.destSta = nodeId;                     // Ŀ���ַ
 	dataPi->msgid.bit.code = cfgNum|0x10;                   // ��һ������ 0x11
    dataPi->msgid.bit.aq = CANlink_ASK;						// Զ��֡ʹ�ø�λ��Ӧ
	dataPi->msgid.bit.framf = CAN_CONFIG_FRAME;				// ����֡
	dataPi->data.byte.byte1 = CAN_LINK_S_ADDR;              // ���ڵ��ַ

	dataPi->data.byte.byte2 = 0;
	dataPi->data.byte.byte3 = 0;
	dataPi->data.byte.byte4 = 0;
    
	dataPi->data.byte.byte5 = 0;
	dataPi->data.byte.byte6 = 0;
	dataPi->data.byte.byte7 = 0;
	dataPi->data.byte.byte8 = 0;
	return CanlinkDataTran(dataPi, 8, 1000);			            // ����Զ��֡��Ӧ
}
#endif
/*******************************************************************************
* ��������          : Uint16 BroadFrame(CANlinkDataBuf *dataPi, Uint16 framf, Uint16 mode, Uint16 aq)
* ��ڲ���			: *dataPi, framf, mode, aq
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: canlink�㲥֡����
********************************************************************************/
Uint16 BroadFrame(CANlinkDataBuf *dataPi, Uint16 framf, Uint16 mode, Uint16 aq)
{
    dataPi->msgid.bit.srcSta = P2P_MSRSTER__ADDR;			// ��������ַ
    dataPi->msgid.bit.destSta = 0;                          // Ŀ���ַ
 	dataPi->msgid.bit.code = mode;                          // ��һ������ 0x11
    dataPi->msgid.bit.aq = aq;						        // Զ��֡ʹ�ø�λ��Ӧ
	dataPi->msgid.bit.framf = framf;				        
	return CanlinkDataTran(dataPi, 0, 1000);			           
}



/*******************************************************************************
* ��������          : void MscMastBeatFrameFun(CANlinkDataBuf *dataPi)
* ��ڲ���			: *dataPi
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: canlink������شӻ���������?
********************************************************************************/
void MscMastBeatFrameFun(CANlinkDataBuf *dataPi)
{
    Uint16 i, nodeId,slaveStatus;
    if (!MasterSlaveCtrl.isMaster)                    // ������,ֱ���˳�
        return;

    nodeId = dataPi->msgid.bit.srcSta;                // ��������֡�ӻ�ID
    // �ӻ�����������ѡ���Ƿ���Ҫͣ�� u0-45 = 22
    slaveStatus = dataPi->data.dByte.dByte1;           // ��������֡�ӻ�״̬    

    if((slaveStatus&0x01)&&((funcCode.code.salveCmdFollow%10)>=10)) // �ӻ����ϲ��Ҵӻ���������������Ч
    {
        errorOther = ERROR_COMM;
        commErrInfo = COMM_ERROR_P2P_SLAVE_ERROR;  // U0-45=22 
    }
    
    if (nodeId)
    {
        for (i = 0; i < MAX_SLAVE_NODE_NUM; i++)      // ������ѯ�ôӻ�
        {
            if (MscSlaveNodeId[i] == nodeId)
            {
                MscSlaveNodeBreakTcnt[i] = 0;         // �������ʱ����0
                return;
            }
        }
        // �ڵ㲻���ڱ��Ѱ�ҿ���λ�ô��
        for(i = 0; i < MAX_SLAVE_NODE_NUM; i++)
        {
            if (!MscSlaveNodeId[i]) // ���л���
            {
                MscSlaveNodeId[i] = nodeId;
				MscSlaveNodeBreakTcnt[i] = 0;
                return;
            }
        }
    }
    
}


/*******************************************************************************
* ��������          : void CheckSlaveNodeBeatStatus(void)
* ��ڲ���			: ��
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: ���ӿ���������������֡�����ڵ������Ƿ�����
********************************************************************************/
void CheckSlaveNodeBeatStatus()
{
    Uint16 i ,j;
    CANlinkDataBuf dataBuf; 

    if (!MasterSlaveCtrl.isMaster)                    // ������,ֱ���˳�
    {
        MasterSlaveCtrl.slaveNodeNums = 0;
        return;
    }
    
    if (++MscMasterSendBeatCount >= ((Uint32)MasterSlaveCtrl.mscCommErrTime/CANlink_DEAL_PERIOD) )
    {                                                 // ������������֡
        if (CANLINK_RT_SUCC == BroadFrame(&dataBuf,CANLINK_BEAT_FRAME,CANLINK_MASTER_BEAT,CANlink_ASK))
            MscMasterSendBeatCount = 0;               // �ɹ������ʱ
        else
            --MscMasterSendBeatCount;                 // ʧ����ʱһ�ķ���
    }    
    
    for (i = 0,j = 0; i < MAX_SLAVE_NODE_NUM; i++)    // �����жϽڵ������Ƿ�����
    {
        
        if (MscSlaveNodeId[i] > 0)                    // ��Ч�ڵ�
        {
            MscSlaveNodeBreakTcnt[i]++;               // ��������ʱ
            
            // ����ʱ�䳬��2����������ʱ�䰴ʱ,ɾ���ýڵ�
//            if (MscSlaveNodeBreakTcnt[i] > ((Uint32)2*MasterSlaveCtrl.mscCommErrTime/ CANlink_DEAL_PERIOD))
            if(++MscSlaveNodeBreakTcnt[i]  > ((Uint32)3 * MasterSlaveCtrl.mscCommErrTime / CANlink_DEAL_PERIOD) )
            {
                // �ӻ������ߵ����ߣ������ͻᱨ�� U0-45=21
                // Ŀ��:��ֹ�ӻ��ѻ�֮�󲻿ɿأ��ᵼ�²��ɿ��Ƶ���������ӻ����ɿأ�����ҲҪֹͣ�Ų�ԭ���ٴ����С�
                if(funcCode.code.salveCmdFollow/100) // �ӻ�����,�����Ƿ�Ҫ����ͣ��
                {
                    errorOther = ERROR_COMM;
                    commErrInfo = COMM_ERROR_P2P_SLAVE_OUT; 
                }
                MscSlaveNodeId[i] = 0;
                MscSlaveNodeBreakTcnt[i] = 0;
            }
            else
            {
                j++;                                  // ��Ч�ڵ���
            }
        }
    }

    MasterSlaveCtrl.slaveNodeNums = j;                // �������ƵĽڵ���
}




/*******************************************************************************
* ��������          : void MSCMasterCfg(void)
* ��ڲ���			: ��
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: ���ӿ���������������,2ms����

********************************************************************************/
void MSCMasterCfg(void)
{
    gCANlinkDataCFg[0].cfgAddr = P2P_MSRSTER__ADDR;
    gCANlinkDataCFg[0].srcRegAddr = P2P_STATER_SRC_ADDR;
    gCANlinkDataCFg[0].disRegAddr = P2P_STATER_DES_ADDR;
    gCANlinkDataCFg[0].regLen = 4;
    gCANlinkDataCFg[0].transInterval = MasterSlaveCtrl.MasterSendPeriod;
    //CANlinkRunMode = CANLINK_RUN_MODE;    // RUNģʽ
	gCanlinkBeatTime = 0;
	gCanlinkMoniAddr = 0;
    gCanlinkMoniBeat = 0;
    gCanlinkBeatCount = 0;
}
 
#endif

