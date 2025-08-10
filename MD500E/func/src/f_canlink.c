/*************** (C) COPYRIGHT 2010  Inovance Technology Co., Ltd****************
* File Name          : f_canlink.c
* Author             : Yanyi	
* Version            : V0.0.1
* Date               : 08/25/2010
* Description        : CANlink������
* �޸�               : 2013/01/18   
*                                   ������ʱ���ʱ���޸�Ϊ����ʱ��2.5��
* �޸�               : 2013/02/27 
                                    �޸�ʹ���жϽ������ݷ�ʽ��2ms�����6֡
* �޸�                 1012 20130301 V3.02 �޸ĵ�Զ�����֡���ж��ڹ��ˣ���ֹ��Զ��������                                    
********************************************************************************/
//#include "DSP28x_Project.h"     							// DSP2803x Headerfile Include File	
//#include "main.h"											// ����ͷ�ļ�

#include "f_funcCode.h"
#include "f_dspcan.h"
#include "f_canlink.h"
#include "f_comm.h"
#include "f_plc.h"
#include "f_p2p.h"



#define DEBUG_F_CANLINK              1


#if DEBUG_F_CANLINK

// CANlink״̬�Ĵ���

Uint16 CANlinkRunMode;                                      // CANlink����ģʽ

Uint16 CanInitFlag;                                         // ��ʼ��״̬��־

// ͬ��֡����־
Uint16 CANlinkSyncFlag;                                     // ͬ���㲥��־ 

// ����CANlink���ջ���
struct CANLINK_REC_BUF	CanlinkRecBuf;						// ���ջ��� �ɽ�������������

// CANlink���ݴ�������
CanLinkDataCfgType  gCANlinkDataCFg[CANLINK_CFG_LEN]       // CANlink���ݴ�������
        = {
        {0x0,    0x0, 0x0, 0x0,   0x0}
/*        
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300}
*/
/*        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
        {0x3000, 0x4, 0x3, 0x100, 0x300},
*/        
                                                        };       // CANlink���ݴ�������



Uint16 gSendEn[CANLINK_CFG_LEN];                            // ���ݷ���ʹ�ܱ�ʶ
Uint16 sDaTemp[CANLINK_CFG_LEN*4];                          // �¼�����������ʷ
Uint16 gCanlinkRecDataAddr[CANLINK_RECDATA_LEN];            // CANlink���յ�Զ�����֡��ַ
Uint16      gtransInterval[CANLINK_CFG_LEN];                // ��ʱ�����ʱ��
Uint16 gRequCount;                                          // �������ü�ʱ
Uint32 gCanlinkMoniBeat; 
Uint16 gCanlinkBeatCount;                                   // CANlink�����������ʱ��ʱ���������ͼ�ʱ
Uint16 gCanlinkBeatTime, gCanlinkMoniAddr;                  // CANlink����ʱ�䣬�������ַ
// ͬ��д�Ĵ���������
struct CANLINK_SYNC_WREG gCanlinkSyncWriteBuf;              // CANlinkͬ��д����

// �ڲ�ʹ�ú�������
void CanLlinkDataDeal(CANlinkDataBuf *dataPi);		        // CANlink�������ݴ���
// Զ��֡
void CanLinkRemoteDeal(CANlinkDataBuf *dataPi);             // CANʹ��Ӳ��Զ��֡
// ����֡
void CanLinkCommDeal(CANlinkDataBuf *dataPi);		        // ����֡����
Uint16 CANlinkReadDeviceInfo(CANlinkDataBuf *dataPi);       // ��ȡ�豸���ӱ�ǩ
Uint16 SyncWriteAddrTest(Uint16 addr);                      // ͬ��д�ظ��ж�
void CANlinkSyncWriteReg(Uint16 addr, Uint16 data, Uint16 dWord);         // ͬ��д�Ĵ�������
// ����֡����
void CanLinkConfigDeal(CANlinkDataBuf *dataPi);             // ����֡����
Uint16 CANlinkDataFrameConfig(CANlinkDataBuf *dataPi);      // ��������֡����
void CANlinkDelConfig(void);                                // ɾ������
Uint16 CANlinkReadConfig(CANlinkDATA *data);                // ��������Ϣ
void CANlinkP2MConfig(CANlinkDataBuf *dataPi);              // ��Զ�����֡����
// ����֡����
void CanLinkManaDeal(CANlinkDataBuf *dataPi);               // CANlink����֡ 
void CANlinkStopUpNode(Uint16);                             // ����/ֹͣ�ڵ�
void CANlinkAddrConflictStop(void);                         // ��ַ��ͻֹͣ
void CANlinkSyncWriteRegFun(void);                          // ͬ��д�Ĵ���ִ��
void CANlinkSyncBroadFun(void);                             // ͬ���㲥ִ��
void CANlinkRequestCfg(void);                               // CANlink��������
void CANlinkCloseFun(void);                                 // �ر�CANlink
void CANlinkAddrClash(void);                                // ��ַ��ͻ��ⷢ��
// ����֡���
void CANlinkDataCfgRun(void);                               // CANlink��������
void CANlinkDataFrameFun(CANlinkDataBuf *dataPi);           // ����֡����
// ����֡���
void CANlinkBeatFrameFun(void);                             // ��������֡����
void CANlinkBeatRun(void);                                  // ��������
// �ⲿ�ӿں�������
void CanlinkOnline(void);                                   // CANlink ����������
void CanlinkMoniTimeout(void);                                  // �������ʱ����
void CanlinkSafeFun(void);                                   // �豸��ȫģʽ����
void CanlinkAddrErrro(void);                                // ��ַ��ͻ����
void CANlinkDeviceStat(void);                               // �豸״̬�ȴ���

void CANlinkReset(void);                                    // ��λCANlink

static Uint16 sBaud = 0, sSourID = 0;						// ��������Դ��ַ							

/*******************************************************************************
* ��������          : void InitCanlinkTran(Uint16	addr)
* ��ڲ���			: 	����ID
						�Զ�Ӧ������
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ��ʼ��CANlink����
********************************************************************************/
void InitCanlinkTran(Uint16	addr)
{
	CANlinkMsgID msgId;
	Uint16	i;
    DspCanDataStru intData;                                 // ��ʼ������
    
	addr &= CANLINK_ID_MASK;

	// 		    Դվ     ����		�ʴ�	      ��չλ()��չ֡
	msgId.all = 0xff | (0xfful<<16) | (1ul<<24)  | (1ul<<31);
//    msgId.bit.srcSta = 0xff;								// Դ��ַ
    msgId.bit.destSta = addr;						        // ������ַ
//    msgId.bit.code = 0xff;									// �������
//    msgId.bit.aq = 1;										// �ʴ��־
//    msgId.bit.framf = CAN_REMOTE_FRAME;				        // ֡���ͱ�־��Զ��֡

    intData.msgid = msgId.all;
    
	for (i=0; i<TRAN_MBOX_NUM; i++)
	{
		InitTranMbox(TRAN_BOX_N-i, &intData);		        // ��ʼ����������
	}
}

/*******************************************************************************
* ��������          : Uint16 CanlinkDataTran(CANlinkDataBuf *dataPi, Uint16 len, Uint16 timeOut)
* ��ڲ���			: 	*dataPi CANlink���ݻ���ָ��
*                       len     �������ݳ���
						timeOut ��ʱ����
* ����				��	CANLINK_RT_SUCC		���ݷ��ͳɹ�
*						CANLINK_RT_BUSY		����æ
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: CANlink֡����
********************************************************************************/
Uint32 canTxSucc;
Uint16 CanlinkDataTran(CANlinkDataBuf *dataPi, Uint16 len, Uint16 timeOut)
{
	Uint16 stat, i;
    timeOut = timeOut;
    
	for (i=0; i<TRAN_MBOX_NUM; i++)
	{
		stat = eCanDataTran(TRAN_BOX_N-i, len, (DspCanDataStru *)dataPi);
		if (CAN_MBOX_TRAN_SUCC == stat)
		{
            canTxSucc++;
			return (CANLINK_RT_SUCC);						// ���ͳɹ�		
		} 
	}

	return (CANLINK_RT_BUSY);							    // ��������æ
}

/*******************************************************************************
* ��������          : void InitCanlinkRec(Uint16 addr)
* ��ڲ���			: ����ID
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ��ʼ��CANlink��������
********************************************************************************/
void InitCanlinkRec(Uint16 addr)
{
	union CANLINK_MsgID msgId;
	Uint32 lam;
	Uint16	i;
    
	addr &= CANLINK_ID_MASK;
	// 			  ��չλ				
	msgId.all = (1ul<<31);

// ����Ŀ��Ϊ���ڵ������֡
    msgId.bit.destSta = addr;								// ����Ŀ���ַ�� �ڵ��ַ
	lam = (~(0xfful<<8)) & ( ~(7ul<<29));               	// ֻ�ܽ�����չ֡ "0"�˲�ƥ��
// 			Ŀ���ַ	  ֻ������չ֡		    			// �������ݲ����ʡ���֡������PLC������֡����
	for (i=0; i<REC_MBOX_NUM-3; i++)
	{
		InitRecMbox((REC_BOX_N-i) | 0x40, msgId.all, lam);	// ��ʼ����������
	}
// ���չ㲥֡
    msgId.bit.destSta = 0;                                  // �㲥��ַ
    msgId.bit.framf = CANLINK_BEAT_FRAME;					// �����չ㲥����֡	
    lam = (~(0xful<<25)) & (~(0xfful<<8)) & ( ~(7ul<<29));
    InitRecMbox((REC_BOX_N-i) | 0x40, msgId.all, lam);	    // �������������㲥
    i++;
	lam = (~(0xfful<<8)) & ( ~(7ul<<29));				    // 
	InitRecMbox((REC_BOX_N-i) | 0x40, msgId.all, lam);	    // ����������㲥
    i++;
    
// ���յ�Զ�����
    msgId.bit.framf = CANLINK_DATA_FRAME;					// ����֡
    msgId.bit.aq = CANlink_ASK;                             // ��Զ�����
	lam = (~(0xful<<25)) & (~(0x1ul<<24)) & ( ~(7ul<<29));	// ֻ�ܽ�����չ֡ "0"�˲�ƥ��
	InitRecMbox((REC_BOX_N-i), msgId.all, lam);				// ����������������	

}

/*******************************************************************************
* ��������          : Uint16 CanlinkDataRec(Uint32 msgid, Uint32 *dataPi, Uint16 timeOut)
* ��ڲ���			: 	��ϢID
						��������
* ����				��CANLINK_R_EMPTY	���������
*					  CANLINK_R_OVER    ���ճɹ������ݻ��������
*					  CANLINK_RT_SUCC	���ճɹ�
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: CANlink���ݽ���
********************************************************************************/
/*
Uint16 CanlinkDataRec(struct CANLINK_REC_BUF *dataPi)
{
	Uint16 i, stat=0;
    Uint16 index = 0;                                       // ��������

	dataPi->bufFull = 0;
	for (i=0; i<(REC_MBOX_NUM); i++)
	{
		stat = eCanDataRec(REC_BOX_N-i, (DspCanDataStru *)(&(dataPi->buf[index])) );
		if (CAN_MBOX_EMPTY != stat)//(CAN_MBOX_REC_SUCC == stat) || (CAN_MBOX_REC_OVER == stat) )	// ���յ�����
        {
            dataPi->bufFull |= 1<<index;				    // ���ջ�����Ч
            index ++;
        }
	}
	if ( 0 == dataPi->bufFull)								// δ������ 
		return (CANLINK_R_EMPTY);							// ��������գ�����
	if (CAN_MBOX_REC_OVER == stat)
		return (CANLINK_R_OVER);
	else
		return (CANLINK_RT_SUCC);
}
*/

/*******************************************************************************
* ��������          : Uint16 InitCanlink(Uint16 addr, Uint16 baud)
* ��ڲ���			: 	����ID
*						������ѡ�� 
* ����				��CAN_INIT_TIME	 ��ʼ��������
*					  CAN_INIT_SUCC  ��ʼ���ɹ�
*					  CAN_INIT_TIMEOUT ��ʼ����ʱ
*					  CAN_INIT_BAUD_ERR �����ʳ���
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ��ʼ��CANlink����ģ��
********************************************************************************/
Uint16 InitCanlink(Uint16 addr, Uint16 baud)
{
	Uint16 stat;
	addr &= CANLINK_ID_MASK;								// ���˲���
//	baud &= 0x7;
	
	stat = InitdspECan(baud);								// eCANģ���ʼ��
	if (stat != CAN_INIT_SUCC)
		return (stat);
		
	InitCanlinkTran(addr);							        // ��ʼ��CANlink���ͣ�
	InitCanlinkRec(addr);									// ����	
	return (CAN_INIT_SUCC);
}

/*******************************************************************************
* ��������          : void CANlinkBeatRun(void)
* ��ڲ���			: 	
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink�������У���ȫģʽ������ʱ��Ϊ�㣬������ִ��
********************************************************************************/
void CANlinkBeatRun(void)
{
    CANlinkDataBuf dataBuf;                                 // ���ݷ��ͻ���

    if ( (0 == gCanlinkBeatTime)                            // ����ʱ��Ϊ"0" ��ȫģʽ, ��ִ������
    || (CANLINK_SAFE_MODE == CANlinkRunMode) ) 
    {
        return;        
    }

// �����������ʱ���    
    
    if (++gCanlinkMoniBeat > ((Uint32)gCanlinkBeatTime* CANLINK_MONI_TIMEOUT / CANlink_DEAL_PERIOD /10) )
    {                                                       // �����������ʱ���
        CanlinkMoniTimeout();
        CanlinkSafeFun();                                   // �������ʱ����ִ�а�ȫ����
        CANlinkReset();                                     // ��λCANlinkģʽ
        return;                                             // ��ʱ�󲻱��ٴ�������
    }
// �ڵ���������
    if (++gCanlinkBeatCount >= (gCanlinkBeatTime/CANlink_DEAL_PERIOD) )
    {                                                       // �ڵ���������
        CANlinkDeviceStat();                                // �豸״̬����

        dataBuf.msgid.bit.srcSta = CAN_LINK_S_ADDR;         // ��ַ
        dataBuf.msgid.bit.destSta = gCanlinkMoniAddr;       // �������ַ
#if DEBUG_F_MSC_CTRL
        if((funcCode.code.MSCEnable) && (!MasterSlaveCtrl.isMaster))
        {
            dataBuf.msgid.bit.srcSta = P2P_MSRSTER__ADDR+funcCode.code.commSlaveAddress;

            if(!MasterSlaveCtrl.slaveErrorSendToMaster) // �ӻ�����ʱ���Ƿ�����������
            {
                CanNodeRunStat &= 0xfffe;
            }
            dataBuf.msgid.bit.destSta = P2P_MSRSTER__ADDR;
        }
#endif

        
        dataBuf.msgid.bit.code = CANLINK_SLAVE_BEAT;        // �ڵ�������    
        dataBuf.msgid.bit.aq = CANlink_ASK;                 // ��
        dataBuf.msgid.bit.framf = CANLINK_BEAT_FRAME;       // ����֡
        dataBuf.data.dByte.dByte1 = CanNodeRunStat;         // �豸״̬����
        if (CANLINK_RT_SUCC == CanlinkDataTran(&dataBuf, 2, 1000))// ��������֡
            gCanlinkBeatCount =0;                           // �ɹ����㶨ʱ
        else
            --gCanlinkBeatCount;                            // ʧ����ʱһ�ķ���
    }
}

/*******************************************************************************
* ��������          : void CANlinkDataCfgRun(void)
* ��ڲ���			: 	
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink����֡�����������д���
********************************************************************************/
void CANlinkDataCfgRun(void)
{
    CANlinkDataBuf dataBuf;                                 // ���ݷ��ͻ���
//    Uint16 data[4];
    Uint16 i, j;                                            // ����ѭ������
    Uint16 time, daTemp[4];                                 // ��ʱʱ�������ݻ���
    
    if (CANLINK_RUN_MODE != CANlinkRunMode)                 // �豸������ģʽ����ִ������֡
    {
        return;
    }
    
    for (i=0; i<CANLINK_CFG_LEN; i++)
    {                                                       // 
        if (0 == gCANlinkDataCFg[i].cfgAddr)                // �ж������Ƿ�Ϊ��
            return;

        time = gCANlinkDataCFg[i].transInterval & 0x7fff;   
                                                            // ��ȡʱ�䣬����ʱʱ����ʼʱ��
        if (0xffff == gCANlinkDataCFg[i].transInterval)
        {
            if (CANlinkSyncFlag == CANlink_SYNC_ENABLE)
                gSendEn[i] = 1;                             // ͬ��������������
        }
        else if (0 == (gCANlinkDataCFg[i].transInterval & 0x8000))
        {
            if (++gtransInterval[i]*CANlink_DEAL_PERIOD >= time)
            {
                gSendEn[i] = 1;                             // ʱ�䴥��
                gtransInterval[i] = 0;                      // ��λ��ʱ��
            }

        }
        else if (0x8000 == (gCANlinkDataCFg[i].transInterval & 0x8000))          
        {                                                   // ��������
            if (++gtransInterval[i]*CANlink_DEAL_PERIOD >= time)
            {
//                ReadSingleReg(gCANlinkDataCFg[i].srcRegAddr, &daTemp);
                ReadMultiReg(gCANlinkDataCFg[i].srcRegAddr, daTemp, gCANlinkDataCFg[i].regLen);
                for (j=0; j<gCANlinkDataCFg[i].regLen; j++)
                {
                    if (daTemp[j] != sDaTemp[(i<<2) +j])    // �ȴ��ı䣬����Ƿ������ݸı�
                    {   
                        gSendEn[i] = 2;                     // ����&ʱ�䴥��
                        gtransInterval[i] = 0;              // ��λ��ʱ��   
                        break;
                    }
                }
                if (0 == gSendEn[i])
                    gtransInterval[i]--;
            }
        }
        else
        {
            continue;                                       // �������ô���
        }
        
        if (gSendEn[i])                                     // �֡��ͱ�־��ʹ��
        {
            ReadMultiReg(gCANlinkDataCFg[i].srcRegAddr, daTemp, gCANlinkDataCFg[i].regLen);
            dataBuf.data.dByte.dByte1 = daTemp[0];            // ���ݸ�ʽ��
            dataBuf.data.dByte.dByte2 = daTemp[1];
            dataBuf.data.dByte.dByte3 = daTemp[2];
            dataBuf.data.dByte.dByte4 = daTemp[3];        
            
            dataBuf.msgid.bit.srcSta = gCANlinkDataCFg[i].disRegAddr;      
                                                            // �Ĵ�����ַ���ֽ�
            dataBuf.msgid.bit.code = gCANlinkDataCFg[i].disRegAddr>>8;              
                                                            // �Ĵ�����ַ���ֽ�
            if (CAN_LINK_S_ADDR == gCANlinkDataCFg[i].cfgAddr)
            {
                dataBuf.msgid.bit.aq = CANlink_ASK;         // ��Զ�����֡
            }
            else
            {
                dataBuf.msgid.bit.aq = CANlink_ACK;         // ��Ե�
            }
#if DEBUG_F_MSC_CTRL
            if(funcCode.code.MSCEnable && MasterSlaveCtrl.isMaster)
            dataBuf.msgid.bit.aq = CANlink_ASK;             // ��Զ�����֡
#endif
            dataBuf.msgid.bit.destSta = gCANlinkDataCFg[i].cfgAddr;           
                                                            // Ŀ��ڵ��ַ����Ե�౾վ��ַ
            dataBuf.msgid.bit.framf = CANLINK_DATA_FRAME;   // ֡����

            if (CANLINK_RT_SUCC == CanlinkDataTran(&dataBuf, gCANlinkDataCFg[i].regLen<<1, 1000))
            {                                               // ���ͳɹ������־

                if (2 == gSendEn[i])                        // ���������ɹ����ͱ�������
                {
                    for (j=0; j<gCANlinkDataCFg[i].regLen; j++)
                        sDaTemp[(i<<2) +j] = daTemp[j];
                }
                    
                gSendEn[i] = 0;                             // ���ͱ�־����
                gtransInterval[i] = 0;                      // �����ͳ�ͻ
            }
        }
        
    }
}


/*******************************************************************************
* ��������          : void CANlinkDataFrameFun(CANlinkDataBuf *dataPi)
* ��ڲ���			: 	
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/10/2012
* ˵��				: CANlink����֡���ղ������ܣ����մ���CANlink����֡
********************************************************************************/
void CANlinkDataFrameFun(CANlinkDataBuf *dataPi)
{
    Uint16 addr;//, i;
    Uint16 data[4];
    
    if (CANLINK_RUN_MODE != CANlinkRunMode)                 // �豸������ģʽ����ִ������֡
    {
        return;
    }
    /*                                                      // �����Ѿ����ж�����ɣ��ô��������
    if (CANlink_ASK == dataPi->msgid.bit.aq)                // �յ���Զ�����
    {
        for (i=0; i<CANLINK_RECDATA_LEN; i++)               // ����Ƿ��ڽ����б�
        {
            if (gCanlinkRecDataAddr[i] == dataPi->msgid.bit.destSta)
            {
                i = 0xcc;
                break;
            }
        }
    }
    else
    {
        i = 0xcc;
    }
    if (i != 0xcc)                                          // ���ݵ�Զ�����֡����������
        return;
    */
    addr = dataPi->msgid.bit.code;                          // �ϳɵ�ַ
    addr <<= 8;
    addr += dataPi->msgid.bit.srcSta;
    
    data[0] = dataPi->data.dByte.dByte1;                    // ���ݸ�ʽ��
    data[1] = dataPi->data.dByte.dByte2;
    data[2] = dataPi->data.dByte.dByte3;
    data[3] = dataPi->data.dByte.dByte4;
                                                            // ִ������֡���մ���
    WriteMultiReg(addr, data, (dataPi->len >> 1), 0);       // �����浽RAM

}

/*******************************************************************************
* ��������          : void CanlinkFun(void)
* ��ڲ���			: 
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: CANlink����ģ�飬ʹ��2ms������ò���
*					  ��Ҫ������   վID  1~63
*					  ����������	     1~2������0�ر�CAN����
********************************************************************************/

Uint32 CanDataDeal;
void CanlinkFun(void)
{
	Uint16 stat;
    Uint16 regCfgTime;
    CANlinkDataBuf rxDataBuf;                               // ��Ϊ�жϽ��շ�ʽ��������������
    Uint16 mboxNum;
	static Uint16 recCanCountTime;

#if DEBUG_F_PLC_CTRL
    static Uint16 timeOutErr = 0;
#endif

#if 0   //���θú����������ڶ��ߺ��������׼(����)�������ᵼ�¶��߼�ⱨERR-16ʱ����� modfied by yz1990 2014-08-06
	if (ErroCountReset())
        return;
#endif
	
#if DEBUG_F_PLC_CTRL
    if (funcCode.code.plcEnable)                            // ʹ��PLC��
    {
		if ( (sBaud != CAN_BAUD_1M) || (sSourID != INV_PLC_ID) )
		{
			sBaud = CAN_BAUD_1M;
			sSourID = INV_PLC_ID;		
			CanInitFlag = CANLINK_INIT_ENABLE;			
		}
    }
    else
#endif
#if DEBUG_F_MSC_CTRL
    if(MasterSlaveCtrl.MSCEnable)
    {
        if(!funcCode.code.slaveEnable)
        {
            if ( (sBaud != CAN_LINK_BAUD_SEL) 
                || (sSourID != P2P_MSRSTER__ADDR)
                )
            {														// ʹ��CANlinkЭ��	
                sBaud = CAN_LINK_BAUD_SEL;
                //sBaud = P2P_BAUD;
                sSourID = P2P_MSRSTER__ADDR;
                CanInitFlag = CANLINK_INIT_ENABLE;
            }
        }
        else
        {
            if ( (sBaud != CAN_LINK_BAUD_SEL) 
                || (sSourID != (P2P_MSRSTER__ADDR+funcCode.code.slaveEnable))
                )
            {														// ʹ��CANlinkЭ��	
                sBaud = CAN_LINK_BAUD_SEL;
                //sBaud = P2P_BAUD;
                sSourID = P2P_MSRSTER__ADDR+funcCode.code.slaveEnable;
                CanInitFlag = CANLINK_INIT_ENABLE;
            }
        }
    }
    else
#endif
    if ( (sBaud != CAN_LINK_BAUD_SEL) || (sSourID != CAN_LINK_S_ADDR) )
    {														// ʹ��CANlinkЭ��	
        sBaud = CAN_LINK_BAUD_SEL;
        sSourID = CAN_LINK_S_ADDR;
        CanInitFlag = CANLINK_INIT_ENABLE;
    }

	if (CanInitFlag == CANLINK_INIT_ENABLE)					// ��ʼ��CANlink
	{
        if (CAN_INIT_SUCC != InitCanlink(sSourID, sBaud) )
			return ;
		else
        {
#if DEBUG_F_PLC_CTRL
        funcCode.code.u3[10] = 0;
        funcCode.code.u3[11] = 0;
#endif
			CanInitFlag = CANLINK_INIT_SUCC;                // ��ʼ���ɹ�
            CANlinkReset();                                 // ��λCANlinkģʽ
#if DEBUG_F_MSC_CTRL
            if(!MasterSlaveCtrl.MSCEnable)
            CANlinkAddrClash();                             // ��ַ��ͻ���
#endif
        }			
		return;		
	}

    if (CanInitFlag != CANLINK_INIT_SUCC)                   // ��ʼ�����ɹ���CANlinkģ�鲻ִ��
        return;

    CANlinkSyncFlag = CANlink_SYNC_DISABLE;                 // ���ͬ����Ч��־
    
//	stat = CanlinkDataRec(&CanlinkRecBuf);					// ����CAN���ݣ�ʹ���жϷ�ʽ��ú�������ʹ��

	if (CanMailBoxEmp() )                                   // ���ڷ��Ϳ�����	
	{														// ��������ݳɹ�
        mboxNum = 0;
		for (stat=0; stat<REC_MBOX_NUM; stat++)		        // �����������
		{
            DINT;                                           // ��            
			if ( CanlinkRecBuf.bufFull & (1u<<stat) )
			{
															// ��������
                rxDataBuf.msgid.all= CanlinkRecBuf.buf[stat].msgid.all;
                rxDataBuf.len = CanlinkRecBuf.buf[stat].len;
                rxDataBuf.data.qByte.qByte1= CanlinkRecBuf.buf[stat].data.qByte.qByte1;
                rxDataBuf.data.qByte.qByte2= CanlinkRecBuf.buf[stat].data.qByte.qByte2;
                                                            // ����CANlink���ݴ�����

                CanlinkRecBuf.bufFull &=  ~(1u<<stat);      // �����־
                
                EINT;                                       // ���ж�
                CanDataDeal++;
				CanLlinkDataDeal((CANlinkDataBuf *)(&rxDataBuf));
				
//����ÿ2ms�����ڴ���REC_MAX��(����7����Ϊ����)��ȷ��2msִ�������ڰ����ݶ������꣬  modfied by yz1990 2014-08-05
//�������ÿ����ִ�����ݷ������(������վPLC�ʹ�վ��Ƶ��ʱ��һ�㲻��ͬ�������ܵ���һ��2ms����һ��2ms���ݲ�һ��				
//(���һ��2ms 2������һ��2ms����4+6=10��))������ȷ����3��2ms������ƽ�������ݴ������һ�㲻�ᶪ֡��
				if (++mboxNum >= REC_MAX) 
					break;
			}
            else
                EINT;
		}

#if DEBUG_F_PLC_CTRL
        timeOutErr = 0;
        funcCode.code.u3[10] = 0;
#endif
	}
#if DEBUG_F_PLC_CTRL
    else                                                    // δ�ӵ�����
    {

        if (++timeOutErr > 4)                               // 0~4������10ms
        {
            if (funcCode.code.u3[10] < 65535)
                funcCode.code.u3[10]++;                     // ��ʱ����
            timeOutErr = 0;
        }

        // �ɱ�̹�����Чʱ����
        if ((funcCode.code.plcEnable)
            && (funcCode.code.u3[10] > 0)
            && ((curTime.powerOnTimeSec > 10) // �ϵ�ʱ�䳬��10����ж�
            || (curTime.powerOnTimeM > 0))  
            )
        {
            errorOther = ERROR_COMM;
            commErrInfo = COMM_ERROR_PLC;
        }


    }
#endif
    // ����״̬ÿn����������
    if (CANLINK_SAFE_MODE == CANlinkRunMode)
    {
        regCfgTime = CANLINK_REQ_CFG_TIME;                         // canlink��׼2000ms
#if DEBUG_F_MSC_CTRL 
        if (MasterSlaveCtrl.MSCEnable)
        {
            regCfgTime = 0;                                        // ���ӿ��Ʋ�����
        }
#endif
#if DEBUG_F_PLC_CTRL
        if(funcCode.code.plcEnable)
        {
            regCfgTime = 0;                                         // ���Уʽ�����ٷ����?
        }
#endif
        if ((gRequCount++ >= (regCfgTime/CANlink_DEAL_PERIOD))&&(regCfgTime))      // ��
        {
            CANlinkRequestCfg();
            gRequCount = 0;
        }

    }
    CANlinkBeatRun();                                       // CANlink��������    
    CANlinkDataCfgRun();                                    // �������д�������CANlink���������

#if 1      
    if (++recCanCountTime >=  (1000/2)) // 1s
   	{
        funcCode.group.u3[0] = recCanCout;
		funcCode.group.u3[1] = ECanaRegs.CANTEC.bit.TEC; //���ʹ��������
		funcCode.group.u3[2] = ECanaRegs.CANREC.bit.REC; //���մ��������
		recCanCout = 0;
		recCanCountTime = 0;
	}
#endif
}

/*******************************************************************************
* ��������          : void CanLlinkDataDeal(struct CANLINK_DATA_BUF *dataPi)
* ��ڲ���			: 
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: CANlinkЭ��������ݴ���
********************************************************************************/
void CanLlinkDataDeal(CANlinkDataBuf *dataPi)
{
	switch (dataPi->msgid.bit.framf)						// ֡���ʹ���
	{

        case CAN_REMOTE_FRAME:								// CANlinkԶ��֡�������
			CanLinkRemoteDeal(dataPi);
			break;

        case CAN_MANAG_FRAME:								// CANlink�������֡
			CanLinkManaDeal(dataPi);
			break;
            
        case CAN_COMM_FRAME:								// ����֡����
			CanLinkCommDeal(dataPi);
			break;
            
		case CAN_CONFIG_FRAME:								// ����֡����
            CanLinkConfigDeal(dataPi);
			break;
            
		case CAN_PLC_DATA_FRAME:						    // PLC������֡����
			PlcDataFramDeal(dataPi);
			break;
            
		case CANLINK_DATA_FRAME:						    // CANlink����֡����
			CANlinkDataFrameFun(dataPi);
			break;			

		case CANLINK_BEAT_FRAME:						    // CANlink����֡����
			CANlinkBeatFrameFun();                          // �ӻ���������֡����
#if DEBUG_F_MSC_CTRL
            if(MasterSlaveCtrl.MSCEnable)
            MscMastBeatFrameFun(dataPi);                    // ������������֡����
#endif
			break;			

		default:

			break;				
			
	}		
}


/*******************************************************************************
* ��������          : void CanLinkRemoteDeal(struct CANLINK_DATA_BUF *dataPi)
* ��ڲ���			: ���ݻ���ָ��
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 10/18/2010
* ˵��				: CANlinkԶ��֡�������
********************************************************************************/
void CanLinkRemoteDeal(CANlinkDataBuf *dataPi)
{
    Uint16 i;
#if DEBUG_F_PLC_CTRL
    if(funcCode.code.plcEnable)
    {
    	if (dataPi->msgid.bit.aq == CANlink_ACK)                // �յ�Ӧ��֡������
            return;
    //	dataPi->msgid.bit.framf = CAN_REMOTE_FRAME;			    //
        dataPi->msgid.bit.destSta = sSourID;					// ������ַ
    //	dataPi->msgid.bit.srcSta = 0xff	;						// 
        dataPi->msgid.bit.aq = CANlink_ACK;						// Զ��֡ʹ�ø�λ��Ӧ
    //	dataPi->msgid.bit.code = 0xff;							//
    //	dataPi->mdl.all= CanLinkChara[0];
    //	dataPi->mdh.all= CanLinkChara[1];
        
        funcCode.code.u3[10] = 0;
        funcCode.code.u3[11] = 0;
        
	    CanlinkDataTran(dataPi, 8, 1000);			            // ����Զ��֡��Ӧ
        return;
    }
#endif
	if ( (dataPi->msgid.bit.aq == CANlink_ACK)              // �յ�Ӧ��֡������
	|| (CANLINK_RUN_MODE == CANlinkRunMode) )               // ����ģʽ����ӦԶ��֡
        return;
    if ( (4 != dataPi->len)                                  // ���ݳ��Ȳ��ԣ��������ַ����
        || (0 == dataPi->data.dByte.dByte2)
        || (dataPi->data.dByte.dByte1 > 20000)              // ����ʱ�����20'000ms
    )
        return;

#if DEBUG_F_MSC_CTRL
    if (MasterSlaveCtrl.MSCEnable)
        return;                                             // ��������ӦԶ��֡
#endif
    
    
    gCanlinkMoniAddr = dataPi->data.dByte.dByte2;           // �������ַ
    gCanlinkBeatTime = dataPi->data.dByte.dByte1;           // ����ʱ��
    gCanlinkMoniBeat = 0;
    gCanlinkBeatCount = 0;                                  // CANlink�����������ʱ��ʱ���������ͼ�ʱ


    gCanlinkSyncWriteBuf.index = 0;                         // ͬ��д����Ϊ��Ч
    gCanlinkSyncWriteBuf.dWord = 0;                         // 32λд���
    
    for (i=0; i<CANLINK_CFG_LEN; i++)                       // ���ö�ʱ����
    {  
        gtransInterval[i] = 0;                              // ���������ʹ��
        gSendEn[i] = 0;
    }
    
    CANlinkRunMode = CANLINK_CFG_MODE;                      // ��������ģʽ
    CanlinkOnline();                                        // CANlink������
    
    dataPi->msgid.bit.destSta = gCanlinkMoniAddr;           //�������ַ
 	dataPi->msgid.bit.srcSta = sSourID	;					// 
    dataPi->msgid.bit.aq = CANlink_ACK;						// Զ��֡ʹ�ø�λ��Ӧ
	dataPi->msgid.bit.code = CANLINK_CFG_LEN;				// ֧��������
	
	dataPi->data.dByte.dByte1= gDeviceInformation[0];       // ��ȡ�����ӱ�ǩ
    dataPi->data.dByte.dByte2= gDeviceInformation[1];
    dataPi->data.dByte.dByte3= gDeviceInformation[2];
    dataPi->data.dByte.dByte4= gDeviceInformation[3];
        
	CanlinkDataTran(dataPi, 8, 1000);			            // ����Զ��֡��Ӧ

}

/*******************************************************************************
* ��������          : void CanLinkManaDeal(CANlinkDataBuf *dataPi)
* ��ڲ���			: ���ݻ���ָ��
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: CANlinkЭ�����֡����
********************************************************************************/
void CanLinkManaDeal(CANlinkDataBuf *dataPi)
{
#if DEBUG_F_MSC_CTRL
    if (funcCode.code.MSCEnable)  // ���Ӳ���Ӧ�κ�Զ��Զ�̹���֡������Ҫ���ͻ֮�������¸Ĳ���
        return;                                             
#endif 
	if ( (dataPi->msgid.bit.aq == CANlink_ACK)                
	   && (CANLINK_ADDR_TEST == dataPi->msgid.bit.code))    // ��ַ��ͻ��Ӧ
    {
		CANlinkAddrConflictStop();
        return;            
    }
    if (dataPi->msgid.bit.aq == CANlink_ACK)                // �յ�Ӧ��֡������
        return;
    
    if ( (CANLINK_SAFE_MODE == CANlinkRunMode)             // ��ȫģʽ����Ӧ����֡
    && (CANLINK_ADDR_TEST != dataPi->msgid.bit.code))       // ��ַ�������
        return;

#if 0//DEBUG_F_MSC_CTRL
    if ((MasterSlaveCtrl.isMaster)                          // ��������ӦԶ��֡
        && (CANLINK_ADDR_TEST != dataPi->msgid.bit.code)    // ��ַ�������
        && (CANLINK_REQ_CFG != dataPi->msgid.bit.code)      // �������ó���
        )
        return;                                             
#endif
    
	switch (dataPi->msgid.bit.code)							// �������
	{
		case CANLINK_STARTUP_NODE:							// �����ڵ�
            CANlinkStopUpNode(1);
			break;
            
		case CANLINK_STOP_NODE:								// ֹͣ�ڵ�
			CANlinkStopUpNode(0);
			break;
            
		case CANLINK_ADDR_TEST:								// ��ַ��ͻ��⣬ֱ����Ӧ�������κδ���

			break;
            
		case CANLINK_SYNC_BROAD:                            // �յ�ͬ���㲥
		    CANlinkSyncBroadFun();                          // ͬ���㲥����
           break;
            
		case CANLINK_REQ_CFG:                               // ��������
            break;            
		case CANLINK_CLOSE:                                 // �ر�CANlink
            CANlinkCloseFun();
            break;            
		default:
            return;                                         // ������Ч����Ӧ
            
	}
    if (0 == dataPi->msgid.bit.destSta)                     // CANlink�㲥֡��Ӧ��
        return; 
    
	dataPi->msgid.bit.destSta = dataPi->msgid.bit.srcSta;	// дĿ���ַ
	dataPi->msgid.bit.srcSta = sSourID;						// дԴ��ַ
	dataPi->msgid.bit.aq = CANlink_ACK;						// ���־
	
    CanlinkDataTran(dataPi, 0, 1000);			            // ������Ӧ֡����������
    
}

/*******************************************************************************
* ��������          : void CANlinkBeatFrameFun(void)
* ��ڲ���			: 
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlink�������մ���
********************************************************************************/
void CANlinkBeatFrameFun(void)
{
    gCanlinkMoniBeat = 0;                                   // ���������ʱ��������
}



/*******************************************************************************
* ��������          : void CanLinkConfigDeal(CANlinkDataBuf *dataPi)
* ��ڲ���			: ���ݻ���ָ��
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlinkЭ������֡����
********************************************************************************/
void CanLinkConfigDeal(CANlinkDataBuf *dataPi)
{
	Uint16  len= 0;
    Uint16  err = CANLINK_NO_ERROR;

	if ( (dataPi->msgid.bit.aq == CANlink_ACK)              // �յ�Ӧ��֡������
	|| ((0 == dataPi->msgid.bit.destSta) && (CANLINK_DEL_CFG != dataPi->msgid.bit.code))
	    )                                                   // ��ɾ����������㲥
        return;
    
    if (CANLINK_CFG_MODE != CANlinkRunMode)                 // ������ģʽ����Ӧ����֡
        return;

    if (dataPi->msgid.bit.code >= CANLINK_DATA_REG)         // ���ݴ�������
    {
        err = CANlinkDataFrameConfig(dataPi);
    }
    else
    {
    	switch (dataPi->msgid.bit.code)					    // ���ñ���
    	{
    		case CANLINK_DEL_CFG:						    // ɾ������
                CANlinkDelConfig();
    			break;

    		case CANLINK_R_CFG:							    // ������
                err =  CANlinkReadConfig(&dataPi->data);
                len = 8;
    			break;
    		case CANLINK_REC_CFG:						    // ����֡��������
                CANlinkP2MConfig(dataPi);
    			break;

    		default:
                err = CANLINK_CODE_ERROR;
    			break;				
    			
    	}
    }
    if (0 == dataPi->msgid.bit.destSta)                     // �㲥����Ӧ
        return;
    
	dataPi->msgid.bit.destSta = dataPi->msgid.bit.srcSta;	// дĿ���ַ
	dataPi->msgid.bit.srcSta = sSourID;						// дԴ��ַ
	dataPi->msgid.bit.aq = CANlink_ACK;						// ���־
	if (err)
    {
        dataPi->msgid.bit.code = CANLINK_CFG_ERROR;         // ���ó���
        dataPi->data.dByte.dByte1 = err;                    // ������
        len = 2;
    }
    CanlinkDataTran(dataPi, len, 1000);                     // ����֡��Ӧ
}

/*******************************************************************************
* ��������          : void CanLinkCommDeal(CANlinkDataBuf *dataPi)
* ��ڲ���			: ���ݻ���ָ��
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: CANlinkЭ������֡����
********************************************************************************/
void CanLinkCommDeal(CANlinkDataBuf *dataPi)
{
	Uint16 err = 0, len = 4;
	#if SERVO_DRIVER
			Uint16 qByte[2];
            static Uint16 dWord[2];                         // ����ͬ��д32λ
            static Uint16 dWordEn;
	#endif
    
	if (dataPi->msgid.bit.aq == CANlink_ACK)                // �յ�Ӧ��֡������
        return;
    if ( (dataPi->len >= 4) || (dataPi->len == 8)           // �������ж�
        || ( (dataPi->msgid.bit.code == CAN_LINK_SYNC_W_EN) && (dataPi->len == 0))
    )
    {
    	switch (dataPi->msgid.bit.code)						// �������
    	{
    		case CAN_LINK_R_REG:							// ���Ĵ���
                if (0 == dataPi->msgid.bit.destSta)         // �㲥֡��ȡ������ִ��
                    return;
    			err = ReadMultiReg(dataPi->data.dByte.dByte2, (Uint16*) (&dataPi->data.dByte.dByte1), 1 );
                if ((0 == err) & (dataPi->len == 8))        // �Ƿ����2���Ĵ���
                {
                    err = ReadMultiReg(dataPi->data.dByte.dByte4, (Uint16*) (&dataPi->data.dByte.dByte3), 1);
                    if (0 == err)
                        len = 8;
                }                
    			break;
    		case CAN_LINK_W_REG:							// д�Ĵ���
    		    err = WriteMultiRegTest(dataPi->data.dByte.dByte2, (Uint16*)(&dataPi->data.dByte.dByte1), 1, 1);
                if (err)
                    break;
                if (dataPi->len == 8)                       // �Ƿ�д��2���Ĵ���
                {
                    err = WriteMultiRegTest(dataPi->data.dByte.dByte4, (Uint16*)(&dataPi->data.dByte.dByte3), 1, 1);
                    if (err)
                        break;
                    WriteMultiRegFun(dataPi->data.dByte.dByte4, (Uint16*)(&dataPi->data.dByte.dByte3), 1, 1);
                }
    			WriteMultiRegFun(dataPi->data.dByte.dByte2, (Uint16*)(&dataPi->data.dByte.dByte1), 1, 1);
                len = 0;
    			break;
    		case CAN_LINK_R_INFO:						    // ��վ���豸��Ϣ
                if (0 == dataPi->msgid.bit.destSta)         // �㲥֡��ȡ������ִ��
                    return;
                err = CANlinkReadDeviceInfo(dataPi);
                len = 8;
    			break;
/*                
		case CAN_LINK_R_WAR:							// ���澯��Ϣ

    			break;
*/
    // PLC��CAN�������

    		case CAN_TRAN_TAB_CFG:								// PLC�����ͱ�����
    			err = InvTranTabCfg(dataPi);
    //			len = 4;
    			break;
    		case CAN_REC_TAB_CFG:								// PLC�����ձ�����
    			err = InvRecTabCfg(dataPi);
    //			len = 4;
    			break;
    /*
    		case CAN_LINK_R_32Bit:								// ��32λ�Ĵ���

    			break;
    		case CAN_LINK_W_32Bit:								// д32λ�Ĵ���

    			break;
    */
            case CAN_LINK_SYNC_W:                           // ͬ��д�Ĵ���
		    err = WriteMultiRegTest(dataPi->data.dByte.dByte2, (Uint16*)(&dataPi->data.dByte.dByte1), 1, 1);
            if (err == 0)                                   // �޴���16λ��׼ͬ��д
            {
                CANlinkSyncWriteReg(dataPi->data.dByte.dByte2, dataPi->data.dByte.dByte1, 0);
            }
#if SERVO_DRIVER
            else if (err == CANLINK_DATA32_ERROR)           // 32λд����
            {
                if (dWordEn == 0)                           // δ��¼32λд
                {
                    dWord[0] = dataPi->data.dByte.dByte2;   // ��ַ
                    dWord[1] = dataPi->data.dByte.dByte1;   // ��������
                    dWordEn = 1;                            // ��¼���32λ����
                    err = 0;
                }
                else
                {
                    if ((dataPi->data.dByte.dByte2 - dWord[0]) == 1 )
                    {
                        CANlinkSyncWriteReg(dWord[0], dWord[1], 1);
                                                            // 32λ����ǰ���ñ�ʶ
                        CANlinkSyncWriteReg(dataPi->data.dByte.dByte2, dataPi->data.dByte.dByte1, 0);
                        err = 0;
                        dWordEn = 0;
                    } 
                    else if ((dWord[0] - dataPi->data.dByte.dByte2) == 1)
                    {
                        CANlinkSyncWriteReg(dataPi->data.dByte.dByte2, dataPi->data.dByte.dByte1, 1);
                                                            // 32λ����ǰ���ñ�ʶ
                        CANlinkSyncWriteReg(dWord[0], dWord[1], 0);
                        err = 0;
                        dWordEn = 0;
                    }
                    else
                    {
                        err = CANLINK_ADDR_ERROR;           // ��ַ����
                        dWordEn = 0;
                    }
                }
            }
#endif            
            else
                break;

            len = 0;
            
            break;

            case CAN_LINK_SYNC_W_EN:                        // ͬ��д�Ĵ�����Ч
                CANlinkSyncWriteRegFun();
                break; 
                
    		default:
    			err = CANLINK_FUN_ERROR;					// ��д������Ч
    			break;				
    			
    	}
    }    
    else                                                    // CAN���ݸ�ʽ����
        err = CANLINK_DATALEN_ERROR;
    
    if (0 == dataPi->msgid.bit.destSta)                     // CANlink�㲥֡����Ӧ
        return; 
    
	dataPi->msgid.bit.destSta = dataPi->msgid.bit.srcSta;	// дĿ���ַ
	dataPi->msgid.bit.srcSta = sSourID;						// дԴ��ַ
	dataPi->msgid.bit.aq = CANlink_ACK;						// ���־

    if (err)
    {
        dataPi->msgid.bit.code = CAN_LINK_ERROR;            // �������������뷵��0xFF
        dataPi->data.dByte.dByte1 = err;
        len = 2;
    }
	
	CanlinkDataTran(dataPi, len, 1000);			            // ������Ӧ֡
}

/*******************************************************************************
* ��������          : Uint16 CANlinkReadDeviceInfo(CANlinkDataBuf *dataPi)
* ��ڲ���			: *dataPi CANlink���ݽṹ
* ����				��ִ��״̬
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/27/2012
* ˵��				: CANlink��ȡ�豸��Ϣ����
********************************************************************************/
Uint16 CANlinkReadDeviceInfo(CANlinkDataBuf *dataPi)
{
    Uint16 index;
    index = dataPi->data.dByte.dByte1 - 1;
    
    if (index >= (CANLINK_DVC_INFO_LEN>>2) )
    {                                                       // �豸��Ϣ������������
        return CANLINK_ADDR_ERROR;                          // ��ַ����
    }
    index <<= 2;
    dataPi->data.dByte.dByte1 = gDeviceInformation[index + 0];
    dataPi->data.dByte.dByte2 = gDeviceInformation[index + 1];
    dataPi->data.dByte.dByte3 = gDeviceInformation[index + 2];
    dataPi->data.dByte.dByte4 = gDeviceInformation[index + 3];
                                                            // ��ȡ���ӱ�ǩ������
    return CANLINK_NO_ERROR;
}


/*******************************************************************************
* ��������          : Uint16 CANlinkDataFrameConfig(CANlinkDataBuf *dataPi)
* ��ڲ���			: *dataPi CANlink���ݽṹ
* ����				��ִ��״̬
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/13/2012
* ˵��				: CANlink����֡�������������
********************************************************************************/
Uint16 CANlinkDataFrameConfig(CANlinkDataBuf *dataPi)
{
    Uint16 index, err = CANLINK_NO_ERROR;
    Uint16 data[4];
    Uint16 i;
    
    index = dataPi->msgid.bit.code & 0x7f;
    if (index < CANLINK_CFG_LEN)                            // С��֧�ֵ��������ֵ
    {
        if ( (dataPi->data.byte.byte2 <= 4)                // ����<=4��
            && (dataPi->data.byte.byte1 <= 245)            // ��������<=245
            && (dataPi->data.dByte.dByte4 > 0)              // ��ʱʱ��������0
            && ( (dataPi->data.dByte.dByte4 <= 30000)      // ��ʱ����
            || ( (0x8000 == (dataPi->data.dByte.dByte4 & 0x8000)) && (dataPi->data.dByte.dByte4 <= (0x8000 + 30000)) )
                                                            // ��������
            || (dataPi->data.dByte.dByte4 <= 0xffff) )      // ͬ������
            && (8 == dataPi->len)                           // ���ݳ��ȱ�����8
            )                                               
        {
            if ( ReadMultiReg(dataPi->data.dByte.dByte2, data, dataPi->data.byte.byte2) )
            {
                err = CANLINK_CFGDATA_ERROR;
            }
            else                                            // ��ʼ��Ϊ���ʼֵ��ͬ
            {
                for (i=0; i<4; i++)
                    sDaTemp[(index<<2) + i] = data[i]-1;
            } 
/*            
            for (i=0; i<dataPi->data.byte.byte2; i++)       // ����Ƿ�ɱ���ȡ
            {
                if (ReadSingleReg(dataPi->data.dByte.dByte2+i, &data) )
                {
                    err = CANLINK_CFGDATA_ERROR;
                    break;
                }
                else if (0 == i)                            // ��ʼ��Ϊ���ʼֵ��ͬ
                {
                    sDaTemp[i] = data-1;
                }                    
            }
*/            
            if (CANLINK_NO_ERROR == err)
            {
                gCANlinkDataCFg[index].cfgAddr = dataPi->data.byte.byte1;
                gCANlinkDataCFg[index].regLen = dataPi->data.byte.byte2;
                gCANlinkDataCFg[index].srcRegAddr = dataPi->data.dByte.dByte2;
                gCANlinkDataCFg[index].disRegAddr = dataPi->data.dByte.dByte3;
                gCANlinkDataCFg[index].transInterval = dataPi->data.dByte.dByte4;
                                                            // ��CANlink�������ڳ�ʼ��
            }
		}
        else
        {
            if (8 == dataPi->len)                           // �ǳ��ȳ���
                err = CANLINK_CFGDATA_ERROR;
            else
                err = CANLINK_CFGLEN_ERROR;
        }

    }
    else
    {
        err = CANLINK_INDEX_ERROR;                          // ��������������Χ
    }
    return err;
}

/*******************************************************************************
* ��������          : void CANlinkDelConfig(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/13/2012
* ˵��				: CANlinkɾ������
********************************************************************************/
void CANlinkDelConfig(void)
{
    Uint16 i;
    for (i=0; i<CANLINK_CFG_LEN; i++)
    {
        gCANlinkDataCFg[i].cfgAddr = 0;                     // ���õ�ַ/����"0"��ʾ����Ϊ��

    }
    for (i=0; i<CANLINK_RECDATA_LEN; i++)
    {
        gCanlinkRecDataAddr[i] = 0;                         // ��Զ���յ�ַ"0"��ʾ����Ϊ��

    }
}

/*******************************************************************************
* ��������          : Uint16 CANlinkReadConfig(CANlinkDATA *data)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlink��ȡ������Ϣ
********************************************************************************/
Uint16 CANlinkReadConfig(CANlinkDATA *data)
{
    Uint16 err, index;
    index = data->dByte.dByte1;
    
    if (data->dByte.dByte1 < CANLINK_CFG_LEN)               // ��������Χ��
    {
        data->byte.byte1 = gCANlinkDataCFg[index].cfgAddr;
        data->byte.byte2 = gCANlinkDataCFg[index].regLen;        
        data->dByte.dByte2 = gCANlinkDataCFg[index].srcRegAddr;
        data->dByte.dByte3 = gCANlinkDataCFg[index].disRegAddr;
        data->dByte.dByte4 = gCANlinkDataCFg[index].transInterval;
        err = CANLINK_NO_ERROR;
    }
    else
    {
        err = CANLINK_INDEX_ERROR;                          // ������������
    }  
    return err;
}


/*******************************************************************************
* ��������          : Uint16 CANlinkP2MConfig(CANlinkDATA *data)
* ��ڲ���			: ֡���ݽṹ
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlink��Զ�����֡��������
********************************************************************************/
void CANlinkP2MConfig(CANlinkDataBuf *dataPi)
{
    #if CANLINK_RECDATA_LEN >0 
    gCanlinkRecDataAddr[0] = dataPi->data.byte.byte1;
    #endif
    #if CANLINK_RECDATA_LEN >1 
    gCanlinkRecDataAddr[1] = dataPi->data.byte.byte2;
    #endif
    #if CANLINK_RECDATA_LEN >2 
    gCanlinkRecDataAddr[2] = dataPi->data.byte.byte3;
    #endif
    #if CANLINK_RECDATA_LEN >3
    gCanlinkRecDataAddr[3] = dataPi->data.byte.byte4;
    #endif
    #if CANLINK_RECDATA_LEN >4 
    gCanlinkRecDataAddr[4] = dataPi->data.byte.byte5;
    #endif
    #if CANLINK_RECDATA_LEN >5 
    gCanlinkRecDataAddr[5] = dataPi->data.byte.byte6;
    #endif
    #if CANLINK_RECDATA_LEN >6
    gCanlinkRecDataAddr[6] = dataPi->data.byte.byte7;
    #endif
    #if CANLINK_RECDATA_LEN >7 
    gCanlinkRecDataAddr[7] = dataPi->data.byte.byte8;
    #endif
}


/*******************************************************************************
* ��������          : void CANlinkStartUpNode(void)
*                     void CANlinkStopNode(void)
* ��ڲ���			: "0"   ֹͣ�ڵ�
*                     "1"   �����ڵ�
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlink����֡����/ֹͣ�豸
********************************************************************************/
void CANlinkStopUpNode(Uint16 con)
{
    Uint16 i;
    if (con)
        CANlinkRunMode = CANLINK_RUN_MODE;                  // �����ڵ�
    else
        CANlinkRunMode = CANLINK_CFG_MODE;                  // ֹͣ   
        
    for (i=0; i<CANLINK_CFG_LEN; i++)                       // ���������
    {  
        gtransInterval[i] = 0;                              // ���������ʹ��
        gSendEn[i] = 0;
    }    
}

/*******************************************************************************
* ��������          : void CANlinkAddrConflictStop(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlinkֹͣһ�лֹͣ
********************************************************************************/
void CANlinkAddrConflictStop(void)
{
    CanInitFlag = CANL_BUS_STOP;                            // CANlink����ֹͣ�����޸Ĳ����������³�ʼ��
    CANlinkReset();                                         // ��λCANlinkģʽ
    DisableDspCAN();                                        // ��ֹ CANlinkʹ�õ�CAN������
    CanlinkAddrErrro();                                     // ��ַ���󴥷��ⲿ�¼�
}

/*******************************************************************************
* ��������          : void CANlinkSyncBroadFun(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/27/2012
* ˵��				: CANlink���յ�ͬ���㲥����
********************************************************************************/
void CANlinkSyncBroadFun(void)
{
    gCanlinkMoniBeat = 0;                                   // ���������ʱ��������
    CANlinkSyncFlag = CANlink_SYNC_ENABLE;                  // ͬ���㲥��Ч����ʶ
}

/*******************************************************************************
* ��������          :void CANlinkRequestCfg(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/27/2012
* ˵��				: CANlink�������ù㲥֡
********************************************************************************/
void CANlinkRequestCfg(void)
{
    CANlinkDataBuf data;

    data.msgid.bit.destSta = 0;                             // �㲥��ַ
 	data.msgid.bit.srcSta = sSourID	;					    // Դ��ַ
    data.msgid.bit.aq = CANlink_ASK;						// ��
	data.msgid.bit.code = CANLINK_REQ_CFG;				    // ��������֡
    data.msgid.bit.framf = CAN_MANAG_FRAME;

    CanlinkDataTran(&data, 0, 1000);			            // ����֡
}

/*******************************************************************************
* ��������          : void CANlinkAddrClash(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/27/2012
* ˵��				: CANlink��ַ��ͻ���
********************************************************************************/
void CANlinkAddrClash(void)
{
    CANlinkDataBuf data;

    data.msgid.bit.destSta = sSourID;                       // ������ַ
 	data.msgid.bit.srcSta = sSourID	;					    // Դ��ַ
    data.msgid.bit.aq = CANlink_ASK;						// ��
	data.msgid.bit.code = CANLINK_ADDR_TEST;				// ��ַ��ͻ���
    data.msgid.bit.framf = CAN_MANAG_FRAME;

    CanlinkDataTran(&data, 0, 1000);			            // ����֡

}


/*******************************************************************************
* ��������          : void CANlinkCloseFun(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/27/2012
* ˵��				: CANlink�رգ�ʹ��ص���ʼ��ģʽ����ȫ״̬
********************************************************************************/
void CANlinkCloseFun(void)
{
    CANlinkReset();                                         // ��λCANlinkģʽ
    CanlinkSafeFun();                                       // ִ�а�ȫ����
}

/*******************************************************************************
* ��������          : void CANlinkSyncWriteRegFun(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlinkͬ��д�Ĵ���������ִ�и�������й���ͬ��дִ��
********************************************************************************/
void CANlinkSyncWriteRegFun(void)
{
    Uint16 i; 

    for (i=0; i<gCanlinkSyncWriteBuf.index; i++)            // �������ִ��д
    {
        if (gCanlinkSyncWriteBuf.dWord & (1<<i) )
        {
            WriteMultiReg(gCanlinkSyncWriteBuf.addr[i], (Uint16*) (&gCanlinkSyncWriteBuf.data[i]), 2, 0);
            i++;                                            // д����16λ
        }
        else
            WriteMultiReg(gCanlinkSyncWriteBuf.addr[i], (Uint16*) (&gCanlinkSyncWriteBuf.data[i]), 1, 0);
    }
    gCanlinkSyncWriteBuf.index = 0;                         // ����Ϊ��Ч
    gCanlinkSyncWriteBuf.dWord = 0;                         // 32λд���
}


/*******************************************************************************
* ��������          : void CANlinkSyncWriteReg(Uint16 addr, Uint16 data)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2012
* ˵��				: CANlinkͬ��д�Ĵ�����������ͬ��д�������䣬�������������������һ��
********************************************************************************/
void CANlinkSyncWriteReg(Uint16 addr, Uint16 data, Uint16 dWord)
{
    if (SyncWriteAddrTest(addr) == 0)                       // ��ַ�����ظ�
        return;
    
    if (gCanlinkSyncWriteBuf.index < CANlINK_SYNC_W_LEN)    // С�����ֵ, ++
    {
        gCanlinkSyncWriteBuf.index++;
    }
    if (dWord)
        gCanlinkSyncWriteBuf.dWord |= 1 << (gCanlinkSyncWriteBuf.index -1);
                                                            // 32λд��ʶ
        
    gCanlinkSyncWriteBuf.addr[gCanlinkSyncWriteBuf.index-1] = addr;
    gCanlinkSyncWriteBuf.data[gCanlinkSyncWriteBuf.index-1] = data;    
}

/*******************************************************************************
* ��������          : Uint16 SyncWriteAddrTest(Uint16 addr)
* ��ڲ���			: dWord 32λͬ��д��ʶ
* ����				��1 ��ַ��Ч
*                     0 ��ַ��Ч
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 06/11/2013
* ˵��				: ͬ��д���������ַ�ж�
********************************************************************************/
Uint16 SyncWriteAddrTest(Uint16 addr)
{
    Uint16 i;

    for (i=0; i<gCanlinkSyncWriteBuf.index; i++)            // ���ڵ�ַ�ظ�������
    {
        if (addr == gCanlinkSyncWriteBuf.addr[i])
            return 0;
    }
    return 1;
}


/*******************************************************************************
* ��������          : void CANlinkResetReg(void)
* ��ڲ���			: 
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 08/02/2012
* ˵��				: ��λCANlink
********************************************************************************/
void CANlinkReset(void)
{
    gRequCount = 0;                                         // ��λ�������ü���
    gCanlinkMoniBeat = 0;                                   // �����������ʱ������������
    gCanlinkBeatCount = 0;
    ClrCanSendBuf();                                        // ��շ��ͻ���
#if DEBUG_F_MSC_CTRL
    if(!MasterSlaveCtrl.MSCEnable)
    CANlinkRunMode = CANLINK_SAFE_MODE;                     // ��λ����ģʽ
#endif
}


/*******************************************************************************
* ��������          : Uint16 P2bFilte(CANlinkMsgID *msgid)
* ��ڲ���			: 
* ����				: 0xcc  
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 08/25/2010
* ˵��				: �ж��ڹ��˵�Զ�����֡
********************************************************************************/
Uint16 P2bFilte(Uint32 msgid)
{
    Uint16 i;
    CANlinkMsgID id;

    id.all = msgid;

    #define CANLINK_DATA_ASK    ((1ul + (CANLINK_DATA_FRAME<<1))<<24)
    
    if ((msgid & (0x1ful <<24) ) == CANLINK_DATA_ASK)           // �յ���Զ�����
    {
        for (i=0; i<CANLINK_RECDATA_LEN; i++)                   // ����Ƿ��ڽ����б�
        {
            if (gCanlinkRecDataAddr[i] == id.bit.destSta)
            {
                i = 0xcc;
                break;
            }
        }    
    }
    else
         i = 0;
    
    return i;
}



//***********************���º�����CANlinkЭ�����ݣ��������Զ���************






#elif 1

void CanlinkFun(void){}

#endif










