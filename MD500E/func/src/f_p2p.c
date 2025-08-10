//======================================================================
//
// Time-stamp: <2011-2-21 Lei.Min, 0656>
//
// P2P comm
// ��Ե�ͨѶ���ݴ���
//
//======================================================================


#include "f_ui.h"
#include "f_menu.h"
#include "f_frqSrc.h"
#include "f_comm.h"
#include "f_runSrc.h"
#include "f_io.h"
#include "f_error.h"
#include "f_p2p.h"

#if DEBUG_F_MSC_CTRL

MASTER_SALVE_CONTROL_STRUCT MasterSlaveCtrl;
#if DEBUG_F_MSC_CTRL
Uint16 MscSlaveNodeId[MAX_SLAVE_NODE_NUM];             // �������ƵĴӻ��ڵ��б�
Uint16 MscSlaveCfgBuf[MAX_SLAVE_NODE_NUM];             // ��Ҫ�����������õĴӻ��б�
Uint16 MscSlaveNodeBreakTcnt[MAX_SLAVE_NODE_NUM];      // �����Դӻ���������ʱ
Uint16 MscMasterSendBeatCount;                         // ������������ʱ
extern Uint32 gCanlinkMoniBeat; 
extern Uint16 gCanlinkBeatCount;                       // CANlink�����������ʱ��ʱ���������ͼ�ʱ
extern Uint16 gCanlinkBeatTime, gCanlinkMoniAddr;      // CANlink����ʱ�䣬�������ַ
extern Uint16 gCanlinkRecDataAddr[CANLINK_RECDATA_LEN];            // CANlink���յ�Զ�����֡��ַ
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
* ˵��				: canlink������شӻ�����֡����
********************************************************************************/
void MscMastBeatFrameFun(CANlinkDataBuf *dataPi)
{
    Uint16 i, nodeId,slaveStatus;

    if (!MasterSlaveCtrl.isMaster)                    // ������,ֱ���˳� // ���ӿ�����ЧЧ
        return;
    if(dataPi->msgid.bit.srcSta == P2P_MSRSTER__ADDR) // ���˵���������������������-->�ӻ����̣�����¼�ڵ�
        return;
        
    nodeId = dataPi->msgid.bit.srcSta;                // ��������֡�ӻ�ID

    slaveStatus = dataPi->data.dByte.dByte1;           // ��������֡�ӻ�״̬    
    if((slaveStatus&0x01)) // �ӻ����ϲ��Ҵӻ���������������Ч
    {
        errorOther = ERROR_P2P_SLAVE;
        //commErrInfo = COMM_ERROR_P2P_SLAVE_ERROR;       // U0-45=22 
    }
    
    if (nodeId)
    {
        for (i = 0; i < MAX_SLAVE_NODE_NUM; i++)        // ������ѯ�ôӻ�
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
            if (!MscSlaveNodeId[i])                   // ���л���
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
                if(MasterSlaveCtrl.masterDispSlaveOut) // �ӻ���վ
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
* ˵��				: ���ӿ���������������
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

/*******************************************************************************
* ��������          : void MscClrNodeSum(void)
* ��ڲ���			: ��
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 2012-11-21
* ˵��				: ����ڵ���
********************************************************************************/
void MscClrNodeSum(void)
{
	static Uint16 i = 0;
    for(i = 0; i < MAX_SLAVE_NODE_NUM; i++)		                // ɾ���ڵ��¼
    {
        MscSlaveNodeId[i] = 0;
    } 
    MasterSlaveCtrl.slaveNodeNums = 0;
}

 
#endif

/*******************************************************************************
* ��������          : void LimitData(int16 *data, int16 max, int16 min)
* ��ڲ���			: 	*data ��Ҫ���������Ƶı���
*                       max     ��������ֵ
						min     ��������ֵ
* ����				��
* ����	            : 	
********************************************************************************/
void LimitData(int16 *data, int16 max, int16 min)
{
    if (*data > max)
    {
        *data = max;
    }
    else if (*data < min)
    {
        *data = min;
    }

}


//=====================================================================
//
// ����: ���ӿ��ƴ���
// ����: ���������������
//          1. ����������������
//          2. ����ӻ���������
// 
//=====================================================================
void MasterSlaveDeal(void)
{
    //static Uint16 p2pLooperTicker = 0; //
    Uint16 digit[5];
    
    
    MasterSlaveCtrl.MSCEnable = funcCode.code.MSCEnable;             // ���ӿ����Ƿ���Ч
    
    if((CanInitFlag == CANL_BUS_STOP)&&(funcCode.code.MSCEnable))   // canlink����ֹͣ
    {
        CanInitFlag = CANLINK_INIT_ENABLE;                          // �����ӿ���ʱ�����ܳ��ֵ�ַ��ͻ
                                                                    // �ٴμ���
        return;
    }
    else if (CanInitFlag != CANLINK_INIT_SUCC)                      // canlinkδ��ʼ���ɹ� 
    {
        return;
    }
    else if(!funcCode.code.MSCEnable)                               // ������Ч
    {
        MscClrNodeSum();                                            // ����ڵ���Ϣ
        dspSubCmd.bit.svcSlaveFlag = 0;                             
        return;
    }
    
    MasterSlaveCtrl.isMaster = (!funcCode.code.slaveEnable)&MasterSlaveCtrl.MSCEnable;  // �Ƿ�Ϊ����    
    MasterSlaveCtrl.mscCommErrTime = funcCode.code.MSCOverTime*100;  // ���ӿ�������ʱ�� _*ms
    MasterSlaveCtrl.MasterSendPeriod = funcCode.code.MasterSendPeriod + 1;   // _ms
    MasterSlaveCtrl.masterAddress = funcCode.code.commSlaveAddress; // ������ַ
    
    GetNumberDigit1(digit, funcCode.code.salveCmdFollow);
    MasterSlaveCtrl.slaveFollowMasterCmd   = digit[0];              // �����������
    MasterSlaveCtrl.slaveErrorSendToMaster = digit[1];              // �ӻ�������Ϣ�������
    MasterSlaveCtrl.masterDispSlaveOut     = digit[2];              // ������ʾ�ӻ��������
    // Ϊ�ӻ�
    if (!MasterSlaveCtrl.isMaster)                                  
    {
        if(MasterSlaveCtrl.isMasterPre != MasterSlaveCtrl.isMaster)
        {
            MscClrNodeSum();                                        // ����ڵ���Ϣ
            MasterSlaveCtrl.isMasterPre = MasterSlaveCtrl.isMaster; // ����
            return;    
        }
        CANlinkDelConfig();                                         // ɾ���������� 
        gCanlinkRecDataAddr[0] = P2P_MSRSTER__ADDR;                 // ���ý��մ��� 
        gCanlinkBeatTime = MasterSlaveCtrl.mscCommErrTime;          // ��������
        CANlinkRunMode = CANLINK_RUN_MODE;                          // RUNģʽ
    }
    // Ϊ����
    else 
    {
        if(MasterSlaveCtrl.isMasterPre != MasterSlaveCtrl.isMaster)
        {
            MscClrNodeSum();                                        // ����ڵ���Ϣ
            MasterSlaveCtrl.isMasterPre = MasterSlaveCtrl.isMaster; // ����
            return;  
        }
        CANlinkDelConfig();                                         // ɾ���������� 
        MSCMasterCfg();                                             // ���÷��ʹ��� 
        CANlinkRunMode = CANLINK_RUN_MODE;                          // RUNģʽ
        CheckSlaveNodeBeatStatus();                                 // ���ڵ�ĸ��������ڼ��
        if((!MasterSlaveCtrl.slaveNodeNums)&&(runFlag.bit.run))     // �޴�վ���б���
        {
            errorOther = ERROR_COMM;
            commErrInfo = COMM_ERROR_P2P_SLAVE_ERROR; 
        }
    }

    MasterSlaveCtrl.isMasterPre         = MasterSlaveCtrl.isMaster; // ����
//    MasterSlaveCtrl.mscCommErrTimePre   = MasterSlaveCtrl.mscCommErrTime;
//    MasterSlaveCtrl.MasterSendPeriodPre = MasterSlaveCtrl.MasterSendPeriod;
//    MasterSlaveCtrl.masterAddressPre    = MasterSlaveCtrl.masterAddress;
    MasterSlaveCtrl.MSCEnablePre        = MasterSlaveCtrl.MSCEnable;
//    p2pLooperTicker = 0xA5A5;                                        // �ϵ�����

    
    if (MasterSlaveCtrl.MSCEnable)                                   // ���ӿ�����Ч
    {
      
        if (MasterSlaveCtrl.isMaster)                                // ������������
        {
            dspSubCmd.bit.svcSlaveFlag = 0; // ��������
            
            //MasterSlaveCtrl.MasterFrq = frqRun * 10000 / maxFrq;     // -100.00%~100.00%
            switch(funcCode.code.slaveRevDataSel)
            {
                // ���� ת�� + ����Ƶ��
                case 0:
                    MasterSlaveCtrl.MasterFrq = frqDroop * 10000 / maxFrq;//�´����Ƶ�ʣ���ֹ�����´����ƣ��Ӵ����롣
                    break;
                // ����ת�� + Ŀ��Ƶ��
                case 1:
                    MasterSlaveCtrl.MasterFrq = frqCurAim * 10000 / maxFrq;
                    break;
                default:
                    break;
            }
            MasterSlaveCtrl.MasterStatus = runFlag.bit.run;
            MasterSlaveCtrl.MasterTorque = (int32)((int16)itDispP2pSlave) * 10000 / 2000;  // -100.00%~100.00%
            MasterSlaveCtrl.MasterSendPeriod = funcCode.code.MasterSendPeriod + 1;   // _ms
            SLAVE_RCV_STATUS = MasterSlaveCtrl.MasterStatus;
            SLAVE_RCV_FRQ = MasterSlaveCtrl.MasterFrq;
            SLAVE_RCV_TORQUE = MasterSlaveCtrl.MasterTorque;
            SLAVE_RCV_DIR = funcCode.code.runDir;
            
        }
        else                                                         // �ӻ���������
        {  
            dspSubCmd.bit.svcSlaveFlag = 1; // �ӻ�����
            MasterSlaveCtrl.SlaveRcvStatus = SLAVE_RCV_STATUS;
            MasterSlaveCtrl.SlaveRcvFrq = SLAVE_RCV_FRQ;       
            MasterSlaveCtrl.SlaveRcvTorque = SLAVE_RCV_TORQUE;  
            MasterSlaveCtrl.p2pRunDir = SLAVE_RCV_DIR;

            MasterSlaveCtrl.SlaveRcvTorque = ((int32)(int16)funcCode.code.slaveRevGain*(int16)MasterSlaveCtrl.SlaveRcvTorque / 100) + ((int16)funcCode.code.slaveRevOffset);
            LimitData((int16*)&MasterSlaveCtrl.SlaveRcvTorque, (int16)10000,(int16)-10000);

            MasterSlaveCtrl.SlaveRcvFrq = ((int32)(int16)funcCode.code.slaveRevGain*(int16)MasterSlaveCtrl.SlaveRcvFrq / 100) + ((int16)funcCode.code.slaveRevOffset);
            LimitData((int16*)&MasterSlaveCtrl.SlaveRcvFrq, (int16)10000, (int16)-10000);

      }
    }
    else
    {
        dspSubCmd.bit.svcSlaveFlag = 0; // ������Ч
    }
}


#else
void MasterSlaveDeal(void);
#endif


