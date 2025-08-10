/*************** (C) COPYRIGHT 2010  Inovance Technology Co., Ltd****************
* File Name          : f_dspcan.c
* Author             : Yanyi	
* Version            : V0.0.1
* Date               : 08/09/2010
* Description        : DSP CAN���ߵײ�������
*                    : 2013/02/27
                                �޸ĵײ�Ӳ��Ϊ�жϽ��շ�ʽ
* �޸�                 1012 20130301 V3.02 �޸ĵ�Զ�����֡���ж��ڹ��ˣ���ֹ��Զ��������
                                
********************************************************************************/
//#include "DSP28x_Project.h"     							// DSP2803x Headerfile Include File	
//#include "main.h"											// ����ͷ�ļ�

#include "f_funcCode.h"
#include "f_dspcan.h"
#include "f_canlink.h"



#define DEBUG_F_CAN              1



#if DEBUG_F_CAN

#if (DSP_CLOCK == 100)                                      // CAN������ʹ����ʱ�ӣ�KHz��λ
	#define		DSPCAN_CLK		100000                      // 2808   100M
#else
	#define		DSPCAN_CLK		30000                       // 28035   30M
#endif

const	CAN_BAUD	eCanBaud[CAN_BAUD_SUM] = {
									{(DSPCAN_CLK/20/20)-1, 2, 15},	// 20Kbps	
									{(DSPCAN_CLK/20/50)-1, 2, 15},	// 50Kbps		
									{(DSPCAN_CLK/20/100)-1, 2, 15},	// 100Kbps	
									{(DSPCAN_CLK/20/125)-1, 2, 15},	// 125Kbps		3+14+ 2 +1 = 20
									{(DSPCAN_CLK/20/250)-1, 2, 15},	// 250Kbps		3+14+ 2 +1 = 20
									{(DSPCAN_CLK/20/500)-1, 2, 15},	// 500Kbps		2+15+ 2 +1 = 20  85%
								#if (DSPCAN_CLK == 100000)
                                    {(DSPCAN_CLK/20/1000)-1, 2, 15}
                                #else
									{(DSPCAN_CLK/15/1000)-1, 1, 11} //  1Mbps		1+11+ 2 +1 = 15  86.67%
					    		#endif
								};

Uint32 eCanTranEnFlag;// = 0;
Uint32 eCanReEnFlag;// = 0;

/*******************************************************************************
* ��������          : interrupt void eCanRxIsr(void)
* ��ڲ���			: ��
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 01/30/2013
* ˵��				: eCan�жϷ�ʽ�������ݵ�����
********************************************************************************/
Uint32 recCanCout;
interrupt void eCanRxIsr(void)
{
    Uint16 i, mbox, dataTy;
    Uint32 *pi;
    DspCanDataStru *data;

    mbox = ECanaRegs.CANGIF0.bit.MIV0;                      // ��ȡ�ж������

    for (i=0; i<REC_MBOX_NUM-1; i++)
    {
        if ( (CanlinkRecBuf.bufFull & (1<<i) ) == 0)        // �жϵ�ǰ����գ�����
            break;
    }
    recCanCout++;
    data = (DspCanDataStru *)(& (CanlinkRecBuf.buf[i]) );   

    pi = (Uint32 *)(&ECANMBOXES.MBOX0.MSGID);			
    data->msgid = pi[mbox<<2];							    //  ��ID��������
    data->data.mdl= pi[(mbox<<2) + 2];	
    data->data.mdh = pi[(mbox<<2) + 3];
    data->len= pi[(mbox<<2) + 1] & 0xf;                     // ��ȡ�������ݳ���

    dataTy = P2bFilte(data->msgid);
    if (dataTy)                                             // �յ���Զ�����
    {
        if (dataTy == 0xcc)
            CanlinkRecBuf.bufFull |= (1<<i);                // �ý��ջ��� 
    }
    else
        CanlinkRecBuf.bufFull |= (1<<i);                    // �ý��ջ���

    ECANREGS.CANRMP.all = 1ul<<mbox;				        // �����Ϣ����Ĵ���

    ECANREGS.CANGIF0.all = 0xffffffff;                      // ��������жϱ�־
    ECANREGS.CANGIF1.all = 0xffffffff;    

    PieCtrlRegs.PIEACK.bit.ACK9 = 1;                        // Issue PIE ACK
}

/*******************************************************************************
* ��������          : void DisableDspCAN(void)
* ��ڲ���			: ��
* ����				��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ��ֹDSP Ecan�ӿ�
********************************************************************************/
void DisableDspCAN(void)
{
	struct ECAN_REGS ECanaShadow;							// ����һ��Ӱ�ӼĴ�����ĳЩ�Ĵ���ֻ��ʹ��32λ����
    ECANREGS.CANTRR.all	= 0xFFFFFFFF;                       // ȡ�����ڽ��еķ���
    EALLOW;
	ECanaShadow.CANMC.all = ECANREGS.CANMC.all;			    // ��ȡCAN�����ƼĴ���
	ECanaShadow.CANMC.bit.CCR = 1;						    // CPU�����޸Ĳ����ʻ�ȫ�����μĴ���
	ECANREGS.CANMC.all = ECanaShadow.CANMC.all;			    // ��д���ƼĴ���
	EDIS;
}

/*******************************************************************************
* ��������          : Uint16 InitdspECan(Uint16 baud)
* ��ڲ���			: CAN�ӿڲ����ʱ�� (eCanBaud����)
* ����				��CAN_INIT_TIME	 ��ʼ��������
*					  CAN_INIT_SUCC  ��ʼ���ɹ�
*					  CAN_INIT_TIMEOUT ��ʼ����ʱ
*					  CAN_INIT_BAUD_ERR �����ʳ���
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ��ʼ��DSP Ecan�ӿ�
********************************************************************************/
#define		IINIT_CAN_TIME				3
Uint16 InitdspECan(Uint16 baud)		// Initialize eCAN-A module
{
	struct ECAN_REGS ECanaShadow;							// ����һ��Ӱ�ӼĴ�����ĳЩ�Ĵ���ֻ��ʹ��32λ����
	Uint32 *MsgCtrlPi;										// ��ʼ������ָ��
	Uint16	i;												// ѭ������
	static	Uint16 con = 0;
	static	Uint16 count = 0;								// ��ʱ������
	
	if (baud >= CAN_BAUD_SUM)
		return CAN_INIT_BAUD_ERR;							// �����ʳ���
	if (count > IINIT_CAN_TIME)								// ��ʼ����ʱ����
    {
    	con = 0;
    	count = 0;
		return  CAN_INIT_TIMEOUT;        
    }

	
	EALLOW;
	if (con == 0)
	{
		GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
		GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)

	/* Set qualification for selected CAN pins to asynch only */
	// Inputs are synchronized to SYSCLKOUT by default.  
	// This will select asynch (no qualification) for the selected pins.

		GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)   

	/* Configure eCAN-A pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be eCAN functional pins.

		GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANTXA operation
		GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANRXA operation
	
	/* Configure eCAN RX and TX pins for eCAN transmissions using eCAN regs*/  
		ECANREGS.CANTIOC.bit.TXFUNC = 1;
		ECANREGS.CANRIOC.bit.RXFUNC = 1;  

	/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
										// HECC mode also enables time-stamping feature
		ECanaShadow.CANMC.all = 0;
		ECanaShadow.CANMC.bit.SRES = 1;
		ECANREGS.CANMC.all = ECanaShadow.CANMC.all;			// �����λCANģ��
		
		ECanaShadow.CANMC.all = ECANREGS.CANMC.all;			// ��ȡCAN�����ƼĴ���
		ECanaShadow.CANMC.bit.SCB = 1;						// eCANģʽ				
		ECanaShadow.CANMC.bit.SUSP = 1;						// ���費�ܵ���Ӱ��
//		ECanaShadow.CANMC.bit.DBO = 1;						// ���� �����Чλ ���ֽ���ǰ ʹ�ô��ģʽ
		ECanaShadow.CANMC.bit.CCR = 1;						// CPU�����޸Ĳ����ʻ�ȫ�����μĴ���
		ECanaShadow.CANMC.bit.ABO = 1;						// �Զ��ָ�����ʹ��
		ECANREGS.CANMC.all = ECanaShadow.CANMC.all;			// ��д���ƼĴ���
		
	/* Initialize all bits of 'Master Control Field' to zero */
	// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
	// all bits (including reserved bits) of MSGCTRL must be initialized to zero
		MsgCtrlPi = (Uint32 *)(&ECANMBOXES.MBOX0.MSGCTRL);	// ��Ϣ������ָ��
		for (i=0; i<32; i++)
		{
			MsgCtrlPi[i<<2] = 0x00000000;					// ����������Ϣ���ƼĴ���
		}
		MsgCtrlPi = (Uint32 *)(&ECANLAMS.LAM0);				// Ϣ������ָ��
		for (i=0; i<32; i++)								// ����������μĴ���
		{
			MsgCtrlPi[i] = 0x00000000;						// 
		}
		
	/* 
		ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
		..........
		ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;
	*/    
	// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
	//	as a matter of precaution. 
		ECANREGS.CANTRR.all	= 0xFFFFFFFF;					// ��λ��������ȡ�����ڽ��еķ���
		ECANREGS.CANTA.all	= 0xFFFFFFFF;					// ���㷢����Ӧ�Ĵ���/* Clear all TAn bits */      
		ECANREGS.CANRMP.all = 0xFFFFFFFF;					// ������Ϣ����Ĵ���/* Clear all RMPn bits */      
		ECANREGS.CANGIF0.all = 0xFFFFFFFF;					// ȫ���жϱ�־/* Clear all interrupt flag bits */ 
		ECANREGS.CANGIF1.all = 0xFFFFFFFF;
		ECANREGS.CANOPC.all = 0;							// ��������ɱ�����
	/* Configure bit timing parameters for eCANA
		ECanaShadow.CANMC.all = ECANREGS.CANMC.all;
		ECanaShadow.CANMC.bit.CCR = 1 ;            			// CPU�����޸Ĳ����ʻ�ȫ�����μĴ���
		ECANREGS.CANMC.all = ECanaShadow.CANMC.all;
	*/	
		con = 1;											// ��һ�׶����
	}
    if (con == 1)
	{
		ECanaShadow.CANES.all = ECANREGS.CANES.all;
		if (ECanaShadow.CANES.bit.CCE == 0 ) 				// Wait for CCE bit to be set..
		{
			count++;
			EDIS;
			return CAN_INIT_TIME;							// ��ʼ��������
		}
		else
			con = 2;
	}
	
    if (con == 2)
	{
		ECanaShadow.CANBTC.all = 0;                         // ��ʼ��������
		ECanaShadow.CANBTC.bit.BRPREG = eCanBaud[baud].BRPREG;
		ECanaShadow.CANBTC.bit.TSEG2REG = eCanBaud[baud].TSEG2REG;
		ECanaShadow.CANBTC.bit.TSEG1REG = eCanBaud[baud].TSEG1REG; 
		ECanaShadow.CANBTC.bit.SAM = 0;                     // ��λ����
		ECANREGS.CANBTC.all = ECanaShadow.CANBTC.all;
		
		ECanaShadow.CANMC.all = ECANREGS.CANMC.all;
		ECanaShadow.CANMC.bit.CCR = 0 ;            			// ������������� Set CCR = 0
		ECANREGS.CANMC.all = ECanaShadow.CANMC.all;
		con = 3;
    }
	if (con == 3)
	{
		ECanaShadow.CANES.all = ECANREGS.CANES.all;
		if (ECanaShadow.CANES.bit.CCE != 0 ) 				// Wait for CCE bit to be  cleared..
		{
			count++;
			EDIS;
			return CAN_INIT_TIME;		
		}
	}
    ECANREGS.CANAA.all = 0xffffffff;
/* Disable all Mailboxes  */

	con = 0;
	count = 0;
 	ECANREGS.CANME.all = 0;									// Required before writing the MSGIDs

    EDIS;

// �����жϳ�ʼ��
    EALLOW;
    PieCtrlRegs.PIEIER9.bit.INTx5 = 0;                      // ��ֹ�ж�
    ECanaRegs.CANGIM.all = 0;
    ECanaRegs.CANMIM.all = 0;                               // �������������ж�
    
    ECanaRegs.CANGIF0.all = 0xffffffff;
    ECanaRegs.CANGIF1.all = 0xffffffff;                     // ����ж�

    ECanaRegs.CANMIL.all = 0;                               // ѡ��EcanA�ж�0
    ECanaRegs.CANGIM.all = 1;                               // ʹ���ж�0


    PieVectTable.ECAN0INTA = &eCanRxIsr;                    // CANA 0�����ж����
    EDIS;
    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;                      // ʹ��ECAN1�ж�
    IER |= M_INT9; 											// Enable CPU INT9
	eCanTranEnFlag = 0;                                     // ��������ʼ����־
	eCanReEnFlag = 0;
	return CAN_INIT_SUCC;									// ��ʼ���ɹ� 
}	

/*******************************************************************************
* ��������          : void ErroCountReset(void)
* ��ڲ���			: ��
* ����				��0  CAN��������״̬
*                     1  CAN����������λ��
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 05/16/2012
* ˵��				: �ֶ���λCAN���ߴ��������
********************************************************************************/
Uint16 ErroCountReset(void)
{
    static Uint16 stat = 0;
    struct ECAN_REGS ECanaShadow;							// ����һ��Ӱ�ӼĴ�����ĳЩ�Ĵ���ֻ��ʹ��32λ����
    Uint32 canIf;
    
    if (0 == stat)
    {
        canIf = ECANREGS.CANGIF0.all;                       // ��ȡ�жϱ�־�Ĵ���
        if (canIf & (7ul<<8) )                              // �������>96�����������������߹ر�
        {
            EALLOW;
            ECANREGS.CANGIF0.all = canIf;                   // ����жϱ�ʶ
            ECanaShadow.CANMC.all = ECANREGS.CANMC.all;
            ECanaShadow.CANMC.bit.CCR = 1 ;
            ECANREGS.CANMC.all = ECanaShadow.CANMC.all;
            EDIS;
            stat = 1;                                       // ������һ״̬
        }
    }
    else
    {
        EALLOW;
        ECanaShadow.CANMC.all = ECANREGS.CANMC.all;
		ECanaShadow.CANMC.bit.CCR = 0 ;            			// Set CCR = 0  ������������
		ECANREGS.CANMC.all = ECanaShadow.CANMC.all;
        EDIS;
        stat = 0;
    }
    return (stat);
}


/*******************************************************************************
* ��������          : void InitTranMbox(Uint16 mbox)
* ��ڲ���			: mbox      ������ 0~31�� 
*					  *datapi   �����ʼ�����ݽṹ
*                     msgid     bit0~bit28  29λ֡ID
*                               bit31       ��չ֡��ʶ
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ����CAN�������䣬�ɳ�ʼ��Ϊ�Զ�Ӧ������
********************************************************************************/
void InitTranMbox(Uint16 mbox, DspCanDataStru *datapi)//Uint32 msgid, Uint32 *dataPi)
{
	Uint16 id;
	Uint32 ECanaShadow, *msgIdPi;	                        //ָ�븳ֵ��ϢID��ַ

	id = mbox & 0x1f;
	eCanTranEnFlag |= 1ul <<mbox;							// �����ʼ�����ͱ�־

	msgIdPi = (Uint32 *)(&ECANMBOXES.MBOX0.MSGID);
	msgIdPi[id<<2] = datapi->msgid; 					    // д��Ϣ��־��ȷ���Ƿ�Ϊ�Զ�Ӧ������
	msgIdPi[(id<<2) +1] = 8;
	
	ECanaShadow = ECANREGS.CANMD.all;
	ECanaShadow &= ~(1ul<<id);
	ECANREGS.CANMD.all = ECanaShadow;						// ��������Ϊ��������

	ECanaShadow = ECANREGS.CANME.all;
	ECanaShadow |= 1ul<<id;
	ECANREGS.CANME.all = ECanaShadow;						// ʹ�ܶ�Ӧ����

	msgIdPi[(id<<2) + 2] = datapi->data.mdl;			    // д�Զ�Ӧ����Ϣ��
	msgIdPi[(id<<2) + 3] = datapi->data.mdl;	
	
}

/*******************************************************************************
* ��������          : void InitReMbox(Uint16 mbox, union CANMSGID_REG msgid, union CANLAM_REG lam)
* ��ڲ���			: mbox ������ 0~31��bit7 ��1�� ����Զ��֡ ��0����ͨ֡	    bit6 "1"���Ǳ���(��������)
*					  msgid	��Ϣ��ʶID
*					  lam	�������μĴ�����д"0"�������չ���λ
* ����				����
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 07/29/2010
* ˵��				: ����CAN��������
********************************************************************************/
void InitRecMbox(Uint16 mbox, Uint32 msgid, Uint32 lam)
{
	Uint16 id;
	Uint32 ECanaShadow,  *pi;								// = (Uint32 *)(&ECANMBOXES.MBOX0.MSGID);
	
	id = mbox & 0x1f;
	eCanReEnFlag |= 1ul << id;
	
	pi = (Uint32 *)(&ECANMBOXES.MBOX0.MSGID);
	pi[id<<2] = msgid | 1ul<<30;  							// ��Ϣ��ʶ�Ĵ�����ʹ�ù��˹���
	if ((mbox & 0x80) == 0x80)								// ����Զ��֡�����ʼ��
		pi[(id<<2) +1] = 1<<4 | 8;							// ��Ϣ���ƼĴ���
	else
		pi[(id<<2) +1] = 8;
		
	ECanaShadow = ECANREGS.CANOPC.all;
	if ( (mbox & 0x40) == 0x40 )							// ʹ�ܸ��Ǳ�����飬����ʼ�����Ѿ������������ֹ���Ǳ���
		ECanaShadow |= 1ul<<id;
	else
		ECanaShadow &= ~(1ul<<id);
	ECANREGS.CANOPC.all = ECanaShadow;
		
	ECanaShadow = ECANREGS.CANMD.all;						// �á�1������Ϊ��������
	ECanaShadow |= 1ul<<id;
	ECANREGS.CANMD.all = ECanaShadow;						// 
	
	ECanaShadow = ECANREGS.CANME.all;
	ECanaShadow |= 1ul<<id;
	ECANREGS.CANME.all = ECanaShadow;						// ʹ�ܶ�Ӧ����
	
	pi = (Uint32 *)(&ECANLAMS.LAM0);						// ���ý������μĴ���
	pi[id] = lam;

    EALLOW;
    ECanaShadow = ECANREGS.CANMIM.all;
	ECanaShadow |= 1ul<<id;
    ECANREGS.CANMIM.all = ECanaShadow;                      // ʹ�ܽ����ж�
    EDIS;
}


/*******************************************************************************
* ��������          : Uint16 eCanDataTran(Uint16 mbox, Uint16 len, Uint32 msgid, Uint32 *dataPi)
* ��ڲ���			: mbox      ������ 0~31��
*                     len       �������ݳ����ֽ���
*					  msgid     ��Ϣ��ʶID			ֻ������ЧIDλ
*                     dataPi    ���ݻ�����
* ����				��CAN_MBOX_NUM_ERROR		����ų���������δ����ʼ��Ϊ��������
*					  CAN_MBOX_BUSY				����æ
*					  CAN_MBOX_TRAN_SUCC		���ͳɹ�
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 08/25/2010
* ˵��				: ָ�����䷢�����ݣ�������뱻��ʼ��Ϊ��������
********************************************************************************/
Uint16 eCanDataTran(Uint16 mbox, Uint16 len, DspCanDataStru *data)
{
	Uint32 ECanaShadow, *pi;
    Uint32 msgid;
    

	if ( (eCanTranEnFlag & (1ul << mbox)) != (1ul << mbox) )
	{
		return (CAN_MBOX_NUM_ERROR);						// CAN����ų�������δ��ʼ��
	}
	
	if (ECANREGS.CANTRS.all & (1ul << mbox))				// ����ϴη����Ƿ���ɣ����������־��λ
	{
		return (CAN_MBOX_BUSY);								// CAN����æ
	}

    msgid = data->msgid;
	mbox &= 0x1f;
   
	ECANREGS.CANTA.all = 1ul << mbox;						// ��շ�����Ӧ��־
	
	pi = (Uint32 *)(&ECANMBOXES.MBOX0.MSGID);				// ��ȡ�����Ĵ�����ַ��дID��д����
	
	msgid &= ~(0x7ul<<29);									// �������λ
	msgid |= pi[mbox<<2] & (0x7ul << 29);					// ���޸�ID����λ
	
	ECanaShadow = ECANREGS.CANME.all;
	ECanaShadow &= ~(1ul<<mbox);
	ECANREGS.CANME.all = ECanaShadow;						// ��ֹ��Ӧ����
	
	pi[mbox<<2] = msgid;									// ��дID
	pi[(mbox<<2) + 1] = len & 0xf;
	pi[(mbox<<2) + 2] = data->data.mdl;			            // д����
	pi[(mbox<<2) + 3] = data->data.mdh;
	
	ECanaShadow |= 1ul<<mbox;
	ECANREGS.CANME.all = ECanaShadow;						// ʹ�ܶ�Ӧ����	

	ECANREGS.CANTRS.all = 1ul << mbox;						// ʹ�ܷ���
	return (CAN_MBOX_TRAN_SUCC);
}

/*******************************************************************************
* ��������          : Uint16 eCanDataRec(Uint16 mbox, Uint32 *dataPi)
* ��ڲ���			: mbox      ������ 0~31��
*					  *	data    ���ջ���
* ����				��CAN_MBOX_NUM_ERROR		����ų���������δ����ʼ��Ϊ��������
*					  CAN_MBOX_EMPTY			���������
*					  CAN_MBOX_REC_SUCC			�������ݳɹ�
*					  CAN_MBOX_REC_OVER			���������и���
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 08/25/2010
* ˵��				: �������ݽ��ջ�����
********************************************************************************/
/*
Uint16 eCanDataRec(Uint16 mbox, DspCanDataStru *data)
{
	Uint32 *pi;
	
	mbox &= 0x1f;
//	if ( (eCanReEnFlag & (1ul << mbox)) != (1ul << mbox))
//	{
//		return (CAN_MBOX_NUM_ERROR);						// CAN����ų�������δ��ʼ��
//	}
	if (ECANREGS.CANRMP.all & (1ul << mbox) )				// ����Ƿ��н�����Ϣ����
	{
		pi = (Uint32 *)(&ECANMBOXES.MBOX0.MSGID);			
		data->msgid = pi[mbox<<2];							//  ��ID��������
		data->data.mdl= pi[(mbox<<2) + 2];	
		data->data.mdh = pi[(mbox<<2) + 3];
        data->len= pi[(mbox<<2) + 1] & 0xf;                 // ��ȡ�������ݳ���

//		ECanaShadow = 1ul<<mbox;
		
		if (ECANREGS.CANRML.all & (1ul << mbox))			// ��������Ƿ񱻸��ǹ�
		{
			ECANREGS.CANRMP.all = 1ul<<mbox;				// �����Ϣ����Ĵ���
			return (CAN_MBOX_REC_OVER);
		}	
		else
		{
			ECANREGS.CANRMP.all = 1ul<<mbox;				// �����Ϣ����Ĵ���
			return (CAN_MBOX_REC_SUCC);		
		}
	}
	else
	{
		return (CAN_MBOX_EMPTY);							// CAN����գ��޿ɶ�ȡ����		
	}
}
*/

/*******************************************************************************
* ��������          : Uint16 CanMailBoxEmp()
* ��ڲ���			: 
* ����				: 1  �п���
*                   : 0  �޿���
* ����	            : 	
* �汾		        : V0.0.1
* ʱ��              : 08/25/2010
* ˵��				: CAN ��������ռ��
********************************************************************************/
#define TX_MAILBOX_MAST (( (1ul << TRAN_MBOX_NUM) - 1) << TRAN_BOX_N)
Uint16 CanMailBoxEmp(void)
{
    if ( (ECANREGS.CANTRS.all & TX_MAILBOX_MAST) == TX_MAILBOX_MAST)
        return 0;                                           // ��������æ
    else
        return 1;

}



#elif 1



#endif




