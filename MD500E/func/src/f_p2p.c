//======================================================================
//
// Time-stamp: <2011-2-21 Lei.Min, 0656>
//
// P2P comm
// µã¶ÔµãÍ¨Ñ¶Êý¾Ý´¦Àí
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
Uint16 MscSlaveNodeId[MAX_SLAVE_NODE_NUM];             // Ö÷»ú¿ØÖÆµÄ´Ó»ú½ÚµãÁÐ±í
Uint16 MscSlaveCfgBuf[MAX_SLAVE_NODE_NUM];             // ÐèÒªÖ÷»ú½øÐÐÅäÖÃµÄ´Ó»úÁÐ±í
Uint16 MscSlaveNodeBreakTcnt[MAX_SLAVE_NODE_NUM];      // Ö÷»ú¶Ô´Ó»úÐÄÌø¼à²â¼ÆÊ±
Uint16 MscMasterSendBeatCount;                         // Ö÷»úÐÄÌø¼à²â¼ÆÊ±
extern Uint32 gCanlinkMoniBeat; 
extern Uint16 gCanlinkBeatCount;                       // CANlink¼à²âÆ÷ÐÄÌø³¬Ê±¼ÆÊ±£¬ÐÄÌø·¢ËÍ¼ÆÊ±
extern Uint16 gCanlinkBeatTime, gCanlinkMoniAddr;      // CANlinkÐÄÌøÊ±¼ä£¬¼à²âÆ÷µØÖ·
extern Uint16 gCanlinkRecDataAddr[CANLINK_RECDATA_LEN];            // CANlink½ÓÊÕµã¶Ô¶àÊý¾ÝÖ¡µØÖ·
/*******************************************************************************
* º¯ÊýÃû³Æ          : Uint16 BroadFrame(CANlinkDataBuf *dataPi, Uint16 framf, Uint16 mode, Uint16 aq)
* Èë¿Ú²ÎÊý			: *dataPi, framf, mode, aq
* ³ö¿Ú				£ºÎÞ
* ´´½¨	            : 	
* °æ±¾		        : V0.0.1
* Ê±¼ä              : 2012-11-21
* ËµÃ÷				: canlink¹ã²¥Ö¡·¢ËÍ
********************************************************************************/
Uint16 BroadFrame(CANlinkDataBuf *dataPi, Uint16 framf, Uint16 mode, Uint16 aq)
{
    dataPi->msgid.bit.srcSta = P2P_MSRSTER__ADDR;			// ÅäÖÃÆ÷µØÖ·
    dataPi->msgid.bit.destSta = 0;                          // Ä¿±êµØÖ·
 	dataPi->msgid.bit.code = mode;                          // µÚÒ»ÌõÅäÖÃ 0x11
    dataPi->msgid.bit.aq = aq;						        // Ô¶³ÌÖ¡Ê¹ÓÃ¸ÃÎ»ÏìÓ¦
	dataPi->msgid.bit.framf = framf;				        
	return CanlinkDataTran(dataPi, 0, 1000);			           
}



/*******************************************************************************
* º¯ÊýÃû³Æ          : void MscMastBeatFrameFun(CANlinkDataBuf *dataPi)
* Èë¿Ú²ÎÊý			: *dataPi
* ³ö¿Ú				£ºÎÞ
* ´´½¨	            : 	
* °æ±¾		        : V0.0.1
* Ê±¼ä              : 2012-11-21
* ËµÃ÷				: canlinkÖ÷»ú¼à¿Ø´Ó»úÐÄÌøÖ¡´¦Àí
********************************************************************************/
void MscMastBeatFrameFun(CANlinkDataBuf *dataPi)
{
    Uint16 i, nodeId,slaveStatus;

    if (!MasterSlaveCtrl.isMaster)                    // ·ÇÖ÷»ú,Ö±½ÓÍË³ö // Ö÷´Ó¿ØÖÆÎÞÐ§Ð§
        return;
    if(dataPi->msgid.bit.srcSta == P2P_MSRSTER__ADDR) // ¹ýÂËµôÖ÷»úÐÄÌø£¬ÔÚÉèÖÃÖ÷»ú-->´Ó»ú¹ý³Ì£¬²»¼ÇÂ¼½Úµã
        return;
        
    nodeId = dataPi->msgid.bit.srcSta;                // ·¢ËÍÐÄÌøÖ¡´Ó»úID

    slaveStatus = dataPi->data.dByte.dByte1;           // ·¢ËÍÐÄÌøÖ¡´Ó»ú×´Ì¬    
    if((slaveStatus&0x01)) // ´Ó»ú¹ÊÕÏ²¢ÇÒ´Ó»ú¹ÊÕÏÖ÷»ú¹ÊÕÏÉúÐ§
    {
        errorOther = ERROR_P2P_SLAVE;
        //commErrInfo = COMM_ERROR_P2P_SLAVE_ERROR;       // U0-45=22 
    }
    
    if (nodeId)
    {
        for (i = 0; i < MAX_SLAVE_NODE_NUM; i++)        // ±éÀú²éÑ¯¸Ã´Ó»ú
        {
            if (MscSlaveNodeId[i] == nodeId)
            {
                MscSlaveNodeBreakTcnt[i] = 0;         // ¼ì²âÐÄÌøÊ±¼äÇå0
                return;
            }
        }
                                                      // ½Úµã²»´æÔÚ±íÀï£¬Ñ°ÕÒ¿ÕÏÐÎ»ÖÃ´æ·Å
        for(i = 0; i < MAX_SLAVE_NODE_NUM; i++)
        {
            if (!MscSlaveNodeId[i])                   // ¿ÕÏÐ»º´æ
            {
                MscSlaveNodeId[i] = nodeId;
				MscSlaveNodeBreakTcnt[i] = 0;
                return;
            }
        }
    }
    
}


/*******************************************************************************
* º¯ÊýÃû³Æ          : void CheckSlaveNodeBeatStatus(void)
* Èë¿Ú²ÎÊý			: ÎÞ
* ³ö¿Ú				£ºÎÞ
* ´´½¨	            : 	
* °æ±¾		        : V0.0.1
* Ê±¼ä              : 2012-11-21
* ËµÃ÷				: Ö÷´Ó¿ØÖÆÖ÷»ú·¢ËÍÐÄÌøÖ¡¡¢¼ì²â½ÚµãÐÄÌøÊÇ·ñÕý³£
********************************************************************************/
void CheckSlaveNodeBeatStatus()
{
    Uint16 i ,j;
    CANlinkDataBuf dataBuf; 

    if (!MasterSlaveCtrl.isMaster)                    // ·ÇÖ÷»ú,Ö±½ÓÍË³ö
    {
        MasterSlaveCtrl.slaveNodeNums = 0;
        return;
    }
    
    if (++MscMasterSendBeatCount >= ((Uint32)MasterSlaveCtrl.mscCommErrTime/CANlink_DEAL_PERIOD) )
    {                                                 // Ö÷»ú·¢ËÍÐÄÌøÖ¡
        if (CANLINK_RT_SUCC == BroadFrame(&dataBuf,CANLINK_BEAT_FRAME,CANLINK_MASTER_BEAT,CANlink_ASK))
            MscMasterSendBeatCount = 0;               // ³É¹¦ÇåÁã¼ÆÊ±
        else
            --MscMasterSendBeatCount;                 // Ê§°ÜÑÓÊ±Ò»ÅÄ·¢ËÍ
    }    
    
    for (i = 0,j = 0; i < MAX_SLAVE_NODE_NUM; i++)    // ±éÀúÅÐ¶Ï½ÚµãÐÄÌøÊÇ·ñÕý³£
    {
        
        if (MscSlaveNodeId[i] > 0)                    // ÓÐÐ§½Úµã
        {
            MscSlaveNodeBreakTcnt[i]++;               // ÐÄÌø¼ì²â¼ÆÊ±
            
            // ÐÄÌøÊ±¼ä³¬¹ý2±¶ÐÄÌø·¢ËÍÊ±¼ä°´Ê±,É¾³ý¸Ã½Úµã
//            if (MscSlaveNodeBreakTcnt[i] > ((Uint32)2*MasterSlaveCtrl.mscCommErrTime/ CANlink_DEAL_PERIOD))
            if(++MscSlaveNodeBreakTcnt[i]  > ((Uint32)3 * MasterSlaveCtrl.mscCommErrTime / CANlink_DEAL_PERIOD) )
            {
                // ´Ó»ú´ÓÔÚÏßµ½ÏÂÏß£¬Ö÷»ú¾Í»á±¨¾¯ U0-45=21
                // Ä¿µÄ:·ÀÖ¹´Ó»úÍÑ»úÖ®ºó²»¿É¿Ø£¬»áµ¼ÖÂ²»¿É¿ØÖÆµÄÏÖÏó¡£Èç¹û´Ó»ú²»¿É¿Ø£¬Ö÷»úÒ²ÒªÍ£Ö¹ÅÅ²éÔ­ÒòÔÙ´ÎÔËÐÐ¡£
                if(MasterSlaveCtrl.masterDispSlaveOut) // ´Ó»úµôÕ¾
                {
                    errorOther = ERROR_COMM;
                    commErrInfo = COMM_ERROR_P2P_SLAVE_OUT; 
                }
                MscSlaveNodeId[i] = 0;
                MscSlaveNodeBreakTcnt[i] = 0;
            }
            else
            {
                j++;                                  // ÓÐÐ§½ÚµãÊý
            }
        }
    }

    MasterSlaveCtrl.slaveNodeNums = j;                // Ö÷»ú¿ØÖÆµÄ½ÚµãÊý
}




/*******************************************************************************
* º¯ÊýÃû³Æ          : void MSCMasterCfg(void)
* Èë¿Ú²ÎÊý			: ÎÞ
* ³ö¿Ú				£ºÎÞ
* ´´½¨	            : 	
* °æ±¾		        : V0.0.1
* Ê±¼ä              : 2012-11-21
* ËµÃ÷				: Ö÷´Ó¿ØÖÆÖ÷»ú²ÎÊýÅäÖÃ
********************************************************************************/
void MSCMasterCfg(void)
{
    gCANlinkDataCFg[0].cfgAddr = P2P_MSRSTER__ADDR;
    gCANlinkDataCFg[0].srcRegAddr = P2P_STATER_SRC_ADDR;
    gCANlinkDataCFg[0].disRegAddr = P2P_STATER_DES_ADDR;
    gCANlinkDataCFg[0].regLen = 4;
    gCANlinkDataCFg[0].transInterval = MasterSlaveCtrl.MasterSendPeriod;
    //CANlinkRunMode = CANLINK_RUN_MODE;    // RUNÄ£Ê½
	gCanlinkBeatTime = 0;
	gCanlinkMoniAddr = 0;
    gCanlinkMoniBeat = 0;
    gCanlinkBeatCount = 0;
}

/*******************************************************************************
* º¯ÊýÃû³Æ          : void MscClrNodeSum(void)
* Èë¿Ú²ÎÊý			: ÎÞ
* ³ö¿Ú				£ºÎÞ
* ´´½¨	            : 	
* °æ±¾		        : V0.0.1
* Ê±¼ä              : 2012-11-21
* ËµÃ÷				: Çå³ý½ÚµãÊý
********************************************************************************/
void MscClrNodeSum(void)
{
	static Uint16 i = 0;
    for(i = 0; i < MAX_SLAVE_NODE_NUM; i++)		                // É¾³ý½Úµã¼ÇÂ¼
    {
        MscSlaveNodeId[i] = 0;
    } 
    MasterSlaveCtrl.slaveNodeNums = 0;
}

 
#endif

/*******************************************************************************
* º¯ÊýÃû³Æ          : void LimitData(int16 *data, int16 max, int16 min)
* Èë¿Ú²ÎÊý			: 	*data ÐèÒªÉÏÏÂÏÞÏÞÖÆµÄ±äÁ¿
*                       max     ±äÁ¿ÉÏÏÞÖµ
						min     ±äÁ¿ÏÂÏÞÖµ
* ³ö¿Ú				£º
* ´´½¨	            : 	
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
// º¯Êý: Ö÷´Ó¿ØÖÆ´¦Àí
// ÃèÊö: ¸üÐÂÖ÷´ÓØÖÆÊý¾Ý
//          1. ¸üÐÂÖ÷»ú·¢ËÍÊý¾Ý
//          2. ´¦Àí´Ó»ú½ÓÊÕÊý¾Ý
// 
//=====================================================================
void MasterSlaveDeal(void)
{
    //static Uint16 p2pLooperTicker = 0; //
    Uint16 digit[5];
    
    
    MasterSlaveCtrl.MSCEnable = funcCode.code.MSCEnable;             // Ö÷´Ó¿ØÖÆÊÇ·ñÓÐÐ§
    
    if((CanInitFlag == CANL_BUS_STOP)&&(funcCode.code.MSCEnable))   // canlink×ÜÏßÍ£Ö¹
    {
        CanInitFlag = CANLINK_INIT_ENABLE;                          // ·ÇÖ÷´Ó¿ØÖÆÊ±£¬¿ÉÄÜ³öÏÖµØÖ·³åÍ»
                                                                    // ÔÙ´Î¼¤»î
        return;
    }
    else if (CanInitFlag != CANLINK_INIT_SUCC)                      // canlinkÎ´³õÊ¼»¯³É¹¦ 
    {
        return;
    }
    else if(!funcCode.code.MSCEnable)                               // Ö÷´ÓÎÞÐ§
    {
        MscClrNodeSum();                                            // ÇåÀí½ÚµãÐÅÏ¢
        dspSubCmd.bit.svcSlaveFlag = 0;                             
        return;
    }
    
    MasterSlaveCtrl.isMaster = (!funcCode.code.slaveEnable)&MasterSlaveCtrl.MSCEnable;  // ÊÇ·ñÎªÖ÷»ú    
    MasterSlaveCtrl.mscCommErrTime = funcCode.code.MSCOverTime*100;  // Ö÷´Ó¿ØÖÆÐÄÌøÊ±¼ä _*ms
    MasterSlaveCtrl.MasterSendPeriod = funcCode.code.MasterSendPeriod + 1;   // _ms
    MasterSlaveCtrl.masterAddress = funcCode.code.commSlaveAddress; // Ö÷»úµØÖ·
    
    GetNumberDigit1(digit, funcCode.code.salveCmdFollow);
    MasterSlaveCtrl.slaveFollowMasterCmd   = digit[0];              // ¸úËæÃüÁîÓë·ñ
    MasterSlaveCtrl.slaveErrorSendToMaster = digit[1];              // ´Ó»ú¹ÊÕÏÐÅÏ¢´«ÊäÓë·ñ
    MasterSlaveCtrl.masterDispSlaveOut     = digit[2];              // Ö÷»úÏÔÊ¾´Ó»úµôÏßÓë·ñ
    // Îª´Ó»ú
    if (!MasterSlaveCtrl.isMaster)                                  
    {
        if(MasterSlaveCtrl.isMasterPre != MasterSlaveCtrl.isMaster)
        {
            MscClrNodeSum();                                        // ÇåÀí½ÚµãÐÅÏ¢
            MasterSlaveCtrl.isMasterPre = MasterSlaveCtrl.isMaster; // ±¸·Ý
            return;    
        }
        CANlinkDelConfig();                                         // É¾³ý·¢ËÍÅäÖÃ 
        gCanlinkRecDataAddr[0] = P2P_MSRSTER__ADDR;                 // ÅäÖÃ½ÓÊÕ´«Êä 
        gCanlinkBeatTime = MasterSlaveCtrl.mscCommErrTime;          // ÅäÖÃÐÄÌø
        CANlinkRunMode = CANLINK_RUN_MODE;                          // RUNÄ£Ê½
    }
    // ÎªÖ÷»ú
    else 
    {
        if(MasterSlaveCtrl.isMasterPre != MasterSlaveCtrl.isMaster)
        {
            MscClrNodeSum();                                        // ÇåÀí½ÚµãÐÅÏ¢
            MasterSlaveCtrl.isMasterPre = MasterSlaveCtrl.isMaster; // ±¸·Ý
            return;  
        }
        CANlinkDelConfig();                                         // É¾³ý½ÓÊÕÅäÖÃ 
        MSCMasterCfg();                                             // ÅäÖÃ·¢ËÍ´«Êä 
        CANlinkRunMode = CANLINK_RUN_MODE;                          // RUNÄ£Ê½
        CheckSlaveNodeBeatStatus();                                 // ¼ì²é½ÚµãµÄ¸öÊý£¬ÓÃÓÚ¼à¿Ø
        if((!MasterSlaveCtrl.slaveNodeNums)&&(runFlag.bit.run))     // ÎÞ´ÓÕ¾ÔËÐÐ±¨¾¯
        {
            errorOther = ERROR_COMM;
            commErrInfo = COMM_ERROR_P2P_SLAVE_ERROR; 
        }
    }

    MasterSlaveCtrl.isMasterPre         = MasterSlaveCtrl.isMaster; // ±¸·Ý
//    MasterSlaveCtrl.mscCommErrTimePre   = MasterSlaveCtrl.mscCommErrTime;
//    MasterSlaveCtrl.MasterSendPeriodPre = MasterSlaveCtrl.MasterSendPeriod;
//    MasterSlaveCtrl.masterAddressPre    = MasterSlaveCtrl.masterAddress;
    MasterSlaveCtrl.MSCEnablePre        = MasterSlaveCtrl.MSCEnable;
//    p2pLooperTicker = 0xA5A5;                                        // ÉÏµçËø¶¨

    
    if (MasterSlaveCtrl.MSCEnable)                                   // Ö÷´Ó¿ØÖÆÓÐÐ§
    {
      
        if (MasterSlaveCtrl.isMaster)                                // Ö÷»ú´«ËÍÊý¾Ý
        {
            dspSubCmd.bit.svcSlaveFlag = 0; // Ö÷»úÇåÁã
            
            //MasterSlaveCtrl.MasterFrq = frqRun * 10000 / maxFrq;     // -100.00%~100.00%
            switch(funcCode.code.slaveRevDataSel)
            {
                // ·¢ËÍ ×ª¾Ø + ÔËÐÐÆµÂÊ
                case 0:
                    MasterSlaveCtrl.MasterFrq = frqDroop * 10000 / maxFrq;//ÏÂ´¹ºóµÄÆµÂÊ£¬·ÀÖ¹Ö÷»úÏÂ´¹¿ØÖÆ£¬ÊÓ´°ÎóÈë¡£
                    break;
                // ·¢ËÍ×ª¾Ø + Ä¿±êÆµÂÊ
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
        else                                                         // ´Ó»ú½ÓÊÕÊý¾Ý
        {  
            dspSubCmd.bit.svcSlaveFlag = 1; // ´Ó»úÁ¢ÅÆ
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
        dspSubCmd.bit.svcSlaveFlag = 0; // Ö÷´ÓÎÞÐ§
    }
}


#else
void MasterSlaveDeal(void);
#endif


