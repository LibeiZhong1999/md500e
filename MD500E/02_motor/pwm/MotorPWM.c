/****************************************************************
ÎÄ¼þ¹¦ÄÜ£ºPWM·¢ËÍ²¿·ÖµÄ³ÌÐò£¬°üÀ¨£ºËÀÇø²¹³¥¼ÆËã¡¢VFÏÂµ÷ÖÆ·½·¨¡¢VCÏÂSVPWM¼ÆËã¡¢DSPÄ£¿éBUG¹æ±Ü
ÎÄ¼þ°æ±¾£º 
×îÐÂ¸üÐÂ£º 
	
****************************************************************/
#include "MotorPwmInclude.h"
//#include "MotorInfoCollectInclude.h"

// // È«¾Ö±äÁ¿¶¨Òå 
OUT_VOLT_STRUCT			gOutVolt;
PWM_OUT_STRUCT			gPWM;
Uint 					gRatio;			//µ÷ÖÆÏµÊý
//ALPHABETA_STRUCT		gUAlphBeta1;	

Uint 	EPWMFlagU;						//ÓÃÓÚ½â¾öEPWMÄ£¿éBUGµÄ±äÁ¿
Uint	EPWMFlagV;
Uint	EPWMFlagW;

Uint 	iTrig1_Cpwm[12] = {0,0,1,1,1,1,2,2,2,2,0,0};	// U,V,W ÄÄÒ»ÏàÂö¿í×î¿í
Uint 	iTrig1_Dpwm[12] = {0,2,2,1,1,0,0,2,2,1,1,0};
Uint 	iTrig2_Pwm[12]  = {1,1,0,0,2,2,1,1,0,0,2,2};	// U,V,W ´¦ÓÚÖÐ¼ä¿í¶ÈµÄÏà
Uint 	iTrig3_Cpwm[12] = {2,2,2,2,0,0,0,0,1,1,1,1}; 	// U,V,W ÄÄÒ»ÏàÂö¿í×îÕ­
Uint 	iTrig3_Dpwm[12] = {2,0,1,2,0,1,2,0,1,2,0,1};

/************************************************************
º¯ÊýÊäÈë:ÎÞ
º¯ÊýÊä³ö:ÎÞ
µ÷ÓÃÎ»ÖÃ:Ö÷Ñ­»·Ö®Ç°
µ÷ÓÃÌõ¼þ:ÎÞ
º¯Êý¹¦ÄÜ:³õÊ¼PWMÄ£¿é
************************************************************/
void InitSetPWM(void)
{
    gCBCProtect.EnableFlag = 1;				//Ä¬ÈÏÆô¶¯,·ÀÖ¹²ÎÊý±æÊ¶ÔÙ´Î¿ªÆôÖð²¨ÏÞÁ÷Ê¹¹Ø±ÕÎÞÐ§zbz1812
	EALLOW;
	/////////////PWM1//////////////
//Set the Time-Base (TB) Module
	EPwm1Regs.TBPRD = C_INIT_PRD; 
	EPwm1Regs.CMPB = EPwm1Regs.TBPRD - gADC.DelayApply;
	EPwm1Regs.TBPHS.all = 0;
	EPwm1Regs.TBCTL.all = 0xE012;
    //EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;
	//EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
	//EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;	//¶¨Ê±Æ÷ÏÂÒç²úÉúÍ¬²½ÐÅºÅ
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;
//Set the Counter-compare (CC) Module
	EPwm1Regs.CMPCTL.all = 0x010A; //0x0100;
	//EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPA.half.CMPA = C_INIT_PRD/2;
//Set the Action-qualifier (AQ) Module
	EPwm1Regs.AQCTLA.all = 0x0090;
	//EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
	//EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//Set the Dead-Band Generator (DB) Module
	EPwm1Regs.AQSFRC.all  = 0x0;
	EPwm1Regs.AQCSFRC.all = 0x0;	
	EPwm1Regs.DBCTL.all = 0x0007;
	//EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; 
	EPwm1Regs.DBFED = gDeadBand.DeadBand;//C_MAX_DB;
	EPwm1Regs.DBRED = gDeadBand.DeadBand;//C_MAX_DB;
//Set the PWM-chopper (PC) Module
//Set the Trip-zone (TZ) Module
#ifdef TMS320F2808
	EPwm1Regs.TZCLR.all = 0x07;
	//EPwm1Regs.TZSEL.all = 0x0104;
	EPwm1Regs.TZSEL.all = 0x010E;
	//EPwm1Regs.TZSEL.bit.OSHT1 = TZ_ENABLE;	//¹ýÁ÷ÐÅºÅ¶ÔPWM1µÄ·âËø
	//EPwm1Regs.TZSEL.bit.CBC3 = TZ_ENABLE;		//EPWM1µÄÖð²¨ÏÞÁ÷
	EPwm1Regs.TZCTL.all = 0x0005;
	//EPwm1Regs.TZCTL.bit.TZA = 1;
	//EPwm1Regs.TZCTL.bit.TZB = 1;
	EPwm1Regs.TZEINT.all = 0x0004;
	//EPwm1Regs.TZEINT.bit.OST = TZ_ENABLE;		//ÆôÓÃ¹ýÁ÷ÖÐ¶Ï
#else
    EPwm1Regs.TZCLR.all = 0x07;
    EPwm1Regs.TZSEL.all = 0x0102;       // OHS1:TZ1;
                                        // CBC2:TZ2;
    EPwm1Regs.TZCTL.all = 0x0005;
    EPwm1Regs.TZEINT.all = 0x0004;
#endif

//Set the Event-trigger (ET) Module	
	EPwm1Regs.ETCLR.bit.INT = 1;				//Ê×ÏÈÇå³ýÖÐ¶Ï±êÖ¾
	EPwm1Regs.ETSEL.all = 0x0B00;//0x0F09;
	//EPwm1Regs.ETSEL.bit.INTEN = 1;
	//EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
	//EPwm1Regs.ETSEL.bit.SOCAEN = 1;
	//EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTRD_CMPB;	//COMPAR_BµÄÏÂ½µÑØÆô¶¯ADC
	EPwm1Regs.ETPS.all = 0x0100;//0x0101;
	//EPwm1Regs.ETPS.bit.INTPRD = 1;
	//EPwm1Regs.ETPS.bit.SOCAPRD = 1;				//Ã¿Ò»ÊÂ¼þÆô¶¯Ò»´ÎAD

	/////////////PWM2//////////////
//Set the Time-Base (TB) Module
	EPwm2Regs.TBPRD = C_INIT_PRD; 
	EPwm2Regs.TBPHS.all = 0;
	EPwm2Regs.TBCTL.all = 0xE006;
    //EPwm2Regs.TBCTL.bit.FREE_SOFT = 3;
	//EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	//EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;	//ÒÔPWM1Í¬²½ÐÅºÅÎªÊä³öÍ¬²½ÐÅºÅ
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;
//Set the Counter-compare (CC) Module
	EPwm2Regs.CMPCTL.all = 0x010A;//0x0100;
	//EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPA.half.CMPA = C_INIT_PRD/2;
//Set the Action-qualifier (AQ) Module
	EPwm2Regs.AQCTLA.all = 0x0090;
	//EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
	//EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//Set the Dead-Band Generator (DB) Module
	EPwm2Regs.AQSFRC.all  = 0x0;
	EPwm2Regs.AQCSFRC.all = 0x0;	
	EPwm2Regs.DBCTL.all = 0x0007;
	//EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; 
	EPwm2Regs.DBFED = gDeadBand.DeadBand;//C_MAX_DB;
	EPwm2Regs.DBRED = gDeadBand.DeadBand;//C_MAX_DB;
//Set the PWM-chopper (PC) Module
//Set the Trip-zone (TZ) Module
#ifdef TMS320F2808
	//EPwm2Regs.TZSEL.all = 0x0102;
	EPwm2Regs.TZSEL.all = 0x010E;
	//EPwm2Regs.TZSEL.bit.OSHT1 = TZ_ENABLE;
	//EPwm2Regs.TZSEL.bit.CBC2 = TZ_ENABLE;
	EPwm2Regs.TZCTL.all = 0x0005;
	//EPwm2Regs.TZCTL.bit.TZA = 1;
	//EPwm2Regs.TZCTL.bit.TZB = 1;
	EPwm2Regs.TZEINT.all = 0x0002;//CBCÖÐ¶Ï
#else
    EPwm2Regs.TZCLR.all = 0x07;
    EPwm2Regs.TZSEL.all = 0x0102;       // OHS1:TZ1;
                                        // CBC2:TZ2;
    EPwm2Regs.TZCTL.all = 0x0005;
    EPwm2Regs.TZEINT.all = 0x0002;    //LJH1917
#endif

//Set the Event-trigger (ET) Module	
    EPwm2Regs.ETSEL.all = 0;
    EPwm2Regs.ETPS.all  = 0;
    
	/////////////PWM3//////////////
//Set the Time-Base (TB) Module
	EPwm3Regs.TBPRD = C_INIT_PRD; 
	EPwm3Regs.TBPHS.all = 0;
	EPwm3Regs.TBCTL.all = 0xE036;
    //EPwm3Regs.TBCTL.bit.FREE_SOFT = 3;
	//EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
	//EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
	//EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
	//EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;	//²»²úÉúÍ¬²½ÐÅºÅ
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = PWM_CLK_DIV;
	//EPwm3Regs.TBCTL.bit.PHSDIR = TB_UP;
//Set the Counter-compare (CC) Module
	EPwm3Regs.CMPCTL.all = 0x010A;//0x0100;
	//EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	//EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPA.half.CMPA = C_INIT_PRD/2;
//Set the Action-qualifier (AQ) Module
	EPwm3Regs.AQCTLA.all = 0x0090;
	//EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;
	//EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//Set the Dead-Band Generator (DB) Module
	EPwm3Regs.AQSFRC.all  = 0x0;
	EPwm3Regs.AQCSFRC.all = 0x00;	
	EPwm3Regs.DBCTL.all = 0x0007;
	//EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	//EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC; 
	EPwm3Regs.DBFED = gDeadBand.DeadBand;//C_MAX_DB;
	EPwm3Regs.DBRED = gDeadBand.DeadBand;//C_MAX_DB;
//Set the PWM-chopper (PC) Module
//Set the Trip-zone (TZ) Module
#ifdef TMS320F2808
	//EPwm3Regs.TZSEL.all = 0x0108;
	EPwm3Regs.TZSEL.all = 0x010E;
	//EPwm3Regs.TZSEL.bit.OSHT1 = TZ_ENABLE;
	//EPwm3Regs.TZSEL.bit.CBC4 = TZ_ENABLE;
	EPwm3Regs.TZCTL.all = 0x0005;
	//EPwm3Regs.TZCTL.bit.TZA = 1;
	//EPwm3Regs.TZCTL.bit.TZB = 1;
#else
    EPwm1Regs.TZCLR.all = 0x07;
    EPwm3Regs.TZSEL.all = 0x0102;       // OHS1:TZ1;
                                        // CBC2:TZ2;
    EPwm3Regs.TZCTL.all = 0x0005;
    EPwm3Regs.TZEINT.all = 0x0004;
#endif

//Set the Event-trigger (ET) Module	
	EDIS;
	gCBCProtect.EnableFlag = 1;  //±ÜÃâ²ÎÊý±æÊ¶ºó£¬CBCÉèÖÃÓë¹¦ÄÜÂëÅäÖÃ²»·û
 
}

/*************************************************************
ËµÃ÷£ºÊä³öµçÑ¹ÏàÎ»gPhase.OutPhaseÎª0µÄÊ±ºò£¬±íÊ¾UÏàµçÑ¹×î´óµÄÊ±ºò£¬
Òò´Ë£¬ÐèÒªÓÃÓàÏÒ·½Ê½¼ÆËãÊä³öµçÑ¹¡£
gPWM.U×îÐ¡£¬±íÊ¾UÏàµçÑ¹×î´ó¡£
*************************************************************/
/************************************************************
	·½²¨·¢ËÍ³ÌÐò£¨µ±Ç°ÆµÂÊ±È½ÏµÍ£¬Ö±½Óµ÷ÓÃ¬²½µ÷ÖÆ³ÌÐò£©
************************************************************/
/*
void SquarePWMAngleCal(void)
{

}
*/

/*************************************************************
	VCÔËÐÐÊ±ºò£¬·¢ËÍPWM²¨(Ö±½Ó°´ÕÕÓàÏÒ·½Ê½µ÷ÖÆµÃµ½)
*************************************************************/
/*void OutPutPWMVC(void)
{
	int   	SinU,SinV,SinW,Comp3;
	int   	phase,phase3;
	int     m_Ratio;
    
	phase = gPhase.OutPhase;
	phase3= 16384 - phase;
	phase3= phase3 + (phase3<<1);
	Comp3 = qsin(phase3)>>2;
    //Comp3 = 0;

	SinU  = qsin(16384 - phase) + Comp3;
	phase = phase - 21845;
	SinV  = qsin(16384 - phase) + Comp3;
	phase = phase - 21845;
	SinW  = qsin(16384 - phase) + Comp3;

	m_Ratio = ((long)gRatio * 4710)>>12;			//³Ë1.15ÏµÊý
	SinU = 4096 - (((long)m_Ratio * (long)SinU) >> 15);
	SinV = 4096 - (((long)m_Ratio * (long)SinV) >> 15);
	SinW = 4096 - (((long)m_Ratio * (long)SinW) >> 15);

	gPWM.U = (((long)SinU * (long)gPWM.gPWMPrdApply)>>13);
	gPWM.V = (((long)SinV * (long)gPWM.gPWMPrdApply)>>13);
	gPWM.W = (((long)SinW * (long)gPWM.gPWMPrdApply)>>13);

	DeadBandComp();
	SendPWM();
}
*/
/************************************************************
	VFÔËÐÐÊ±ºò£¬·¢ËÍPWM²¨(°´ÕÕ¿Õ¼äÊ¸Á¿·¨µÃµ½)

ÏÖÓÐ·¢²¨·½Ê½£ºCPWM£¬DPWM£¬·½²¨£»
Ñ¡Ôñ·½Ê½£º
1. Èç¹ûÊÖ¶¯Ñ¡Ôñ£¬Ôò×ñÑ­ÊÖ¶¯Ñ¡Ôñ£»
2. Ñ¡Ôñ×Ô¶¯ÇÐ»»£¬Ôò×ñÑ­280µÄÇÐ»»·½·¨£»
×Ô¶¯ÇÐ»»Ô­Ôò£º
      1. Èç¹ûÎªÍ¬²½µ÷ÖÆ£¬»òÕßÔØ²¨Ð¡ÓÚ1.5kHz£¬¹Ì¶¨ÎªCPWM£»
      2. Èç¹ûÎªÒì²½µ÷ÖÆ£¬²¢ÇÒÔØ²¨´óÓÚ1.5kHz£¬¹Ì¶¨ÎªDPWM£»
      3. ×Ô¶¯ÇÐ»»´æÔÚ12.5~15HzµÄÖÍ»·£»
************************************************************/
/*void OutPutPWMVF(void)
{
	Uint 	m_iSec30;		
	Uint 	m_iSec60;
	Uint 	m_iRamainPhase;
	Uint 	m_FlagDPWMInverse;
	Ulong*	m_pUVW;

	Uint 	SinAlfa,SinAlfa60;
	Uint	m_iT1Length,m_iT2Length,m_iZeroLength,m_TotalLength;
	Uint 	m_UVWSel1,m_UVWSel2;
	Uint	phase;

	phase = gPhase.OutPhase;// + 32768;		//È·±£gPhase.OutPhase=0¶ÔÓ¦UÏàµçÑ¹×î´óÖµ
	phase = (phase > 65531)?65531:phase;	//±ÜÃâºóÃæ¼ÆËã³öÏÖÒç³ö
	//phase = __IQsat((Uint)gPhase.OutPhase, 65531, 0);
    
//...¼ÆËã30¶È·¶Î§µÄÉÈÇøµÈ
	m_iSec30 = phase/5461;
	if (m_iSec30 > 11)
    {
        m_iSec30 = 0;
    }
	m_iSec60 = m_iSec30 >> 1;

	m_iRamainPhase = phase - (m_iSec30 * 5461);	
	if((m_iSec30 & 0x01)  == 1) //judge it's odd or even
	{
		if(gPWM.PWMModle == MODLE_CPWM)  //CPWM
		{
			m_iRamainPhase = 5461 + m_iRamainPhase;
		}
		else
		{
			m_iRamainPhase = 5461 - m_iRamainPhase;
		}
	}

	m_FlagDPWMInverse = 0;
	if(gPWM.PWMModle == MODLE_DPWM)
	{
		if((((m_iSec30+1)>>1) & 0x01) == 1) m_FlagDPWMInverse = 1;
	}
	else
	{
		if((m_iSec60 & 0x01) == 1)	m_iRamainPhase = 10922 - m_iRamainPhase;	
	}

	SinAlfa60 = qsin(10922 - m_iRamainPhase);
	SinAlfa = qsin(m_iRamainPhase);

	m_iT1Length = ((long)SinAlfa60 * (long)gRatio)>>15;
	m_iT1Length = ((long)m_iT1Length * (long)gPWM.gPWMPrdApply)>>12;

	m_iT2Length = ((long)SinAlfa * (long)gRatio)>>15;
	m_iT2Length = ((long)m_iT2Length * (long)gPWM.gPWMPrdApply)>>12;

	m_TotalLength = m_iT1Length + m_iT2Length;
	if(m_TotalLength > gPWM.gPWMPrdApply)		
	{
		m_iT1Length = ((Ulong)m_iT1Length * (Ulong)gPWM.gPWMPrdApply)/m_TotalLength;
		m_iT2Length = gPWM.gPWMPrdApply - m_iT1Length;
		m_iZeroLength = 0;
	}										//È·±£T1+T2<=T
	gPWM.gZeroLengthPhase = ZERO_VECTOR_NONE;
	if(gPWM.PWMModle == MODLE_CPWM)
	{
		m_iZeroLength = (Uint)((gPWM.gPWMPrdApply - m_iT1Length - m_iT2Length)>>1);
		m_UVWSel1 = iTrig1_Cpwm[m_iSec30];	//Ô¤ÏÈÉèÖÃ²é±íÎ»ÖÃ
		m_UVWSel2 = iTrig3_Cpwm[m_iSec30];
	}
	else
	{
		m_iZeroLength = 0;
		m_UVWSel1 = iTrig1_Dpwm[m_iSec30];	//Ô¤ÏÈÉèÖÃ²é±íÎ»ÖÃ
		gPWM.gZeroLengthPhase = (ZERO_LENGTH_PHASE_SELECT_ENUM)m_UVWSel1;
		m_UVWSel2 = iTrig3_Dpwm[m_iSec30];
	}

	m_iT1Length = m_iZeroLength + m_iT1Length;
	m_iT2Length = m_iT1Length + m_iT2Length;

	if(m_FlagDPWMInverse == 1)
	{
		m_iZeroLength = gPWM.gPWMPrdApply - m_iZeroLength;
		m_iT1Length   = gPWM.gPWMPrdApply - m_iT1Length;
		m_iT2Length   = gPWM.gPWMPrdApply - m_iT2Length;
	}

// ¿ªÊ¼ÉèÖÃÊä³öUVWµÄ±È½ÏÖµ
	m_pUVW = (Ulong *)&gPWM.USet;
	*(m_pUVW + m_UVWSel1) = m_iZeroLength;
	*(m_pUVW + iTrig2_Pwm[m_iSec30]) = m_iT1Length;
	*(m_pUVW + m_UVWSel2) = m_iT2Length;	
}*/
/*******************************************************************************
* Function Name  : PWMÊä³öº¯Êý
* Description    : Ö´ÐÐPWM¼ÆËã/ËÀÇø²¹³¥/ÉèÖÃPWM/DSPµÄBUG¹æ±Ü
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void OutPutVolt(void)
{
    gPWM.U = gPWM.USet;                                 
    gPWM.V = gPWM.VSet;
    gPWM.W = gPWM.WSet;
    if(DEADBAND_COMP_280 == gExtendCmd.bit.DeadCompMode)  
    {
         CalDeadBandComp();
    }
    else if(DEADBAND_COMP_380== gExtendCmd.bit.DeadCompMode)
    {
         HVfDeadBandComp();
    }
    else  
    {
	     gDeadBand.CompU = 0;
	     gDeadBand.CompV = 0;
	     gDeadBand.CompW = 0;  /*ËÀÇø²¹³¥Ä£Ê½Îª0Ê±¡£²»½øÐÐËÀÇø²¹³¥ 2011.5.7 L1082*/       
    }
	DeadBandComp();   
	SendPWM();

}
/*************************************************************
	ËÀÇø²¹³¥ºÍ×î´óÂö¿í¡¢×îÐ¡Âö¿íÏÞÖÆ¡£
*************************************************************/
void DeadBandComp(void)
{
	//long m_Max,m_Min,m_Deta;
    if(gRatio != 0)
    {
        if(ZERO_VECTOR_U != gPWM.gZeroLengthPhase )
        {
            gPWM.U += (long)gDeadBand.CompU;
        }
        if(ZERO_VECTOR_V != gPWM.gZeroLengthPhase )
        {
            gPWM.V += (long)gDeadBand.CompV;
        }
        if(ZERO_VECTOR_W != gPWM.gZeroLengthPhase )
        {
            gPWM.W += (long)gDeadBand.CompW;
        }
    }

    /* È¡ÏûÕ­Âö³å2011.05.07 L1082*/
    gPWM.U = __IQsat(gPWM.U, gPWM.gPWMPrdApply, 0);
    gPWM.V = __IQsat(gPWM.V, gPWM.gPWMPrdApply, 0);
    gPWM.W = __IQsat(gPWM.W, gPWM.gPWMPrdApply, 0);
    
#if 0               
    if(gSubCommand.bit.CancelNarrorP == 0)      //²»¼ÓÕ­Âö³å¿ØÖÆ
    {
        gPWM.U = __IQsat(gPWM.U, gPWM.gPWMPrdApply, 0);
        gPWM.V = __IQsat(gPWM.V, gPWM.gPWMPrdApply, 0);
        gPWM.W = __IQsat(gPWM.W, gPWM.gPWMPrdApply, 0);
    }
    else                                        // È¥µôÕ­Âö³å
    {                                            
        m_Min = (long)gDeadBand.DeadBand + 1;
        m_Max = (long)(gPWM.gPWMPrdApply - m_Min);
// UÏàÕ­Âö³å¿ØÖÆ
        m_Deta = gPWM.U - m_Max;
        if(gPWM.U < (m_Min>>1))
        {
            gPWM.U = 0; 
        }
        else if(gPWM.U < m_Min)	
        {
            gPWM.U = m_Min;
        }
        else if(m_Deta < 0)				
        {
            ;   // ²»´¦Àí
        }
        else if(m_Deta < (m_Min>>1))	
        {
            gPWM.U = m_Max;
        }
        else    // m_Deta > (m_Min>>1)
        {
            gPWM.U = gPWM.gPWMPrdApply;
        }
// VÏàÕ­Âö³å¿ØÖÆ
        m_Deta = gPWM.V - m_Max;
        if(gPWM.V < (m_Min>>1))	
        {
            gPWM.V = 0; 
        }
        else if(gPWM.V < m_Min)	
        {
            gPWM.V = m_Min;
        }
        else if(m_Deta < 0)	
        {
            ;
        }
        else if(m_Deta < (m_Min>>1))
        {
            gPWM.V = m_Max;
        }
        else
        {
            gPWM.V = gPWM.gPWMPrdApply;
        }
// WÏàÕ­Âö³å¿ØÖÆ
        m_Deta = gPWM.W - m_Max;
        if(gPWM.W < (m_Min>>1))
        {
            gPWM.W = 0; 
        }
        else if(gPWM.W < m_Min)
        {
            gPWM.W = m_Min;
        }
        else if(m_Deta < 0)
        {
            ;
        }
        else if(m_Deta < (m_Min>>1))
        {
            gPWM.W = m_Max;
        }
        else
        {
            gPWM.W = gPWM.gPWMPrdApply;
        }
    }
#endif

}

/*************************************************************
	ÉèÖÃEPWMÄ£¿éµÄ¼Ä´æÆ÷£¬Êµ¼Ê·¢ËÍPWM²¨ÐÎ£¬°üÀ¨¶ÔEPWMÄ£¿éBUGµÄ¹æ±Ü
*************************************************************/
void SendPWM()
{
    static Uint DirectActFlagU = 0;
    static Uint DirectActFlagV = 0;
    static Uint DirectActFlagW = 0;
//#ifdef TMS320F2808                  // ePWMÄ£¿ébug¹æ±Ü, 28035ÎÞÐè¹æ±Ü
	//...»Ö¸´¼Ä´æÆ÷ÖÃ
	EALLOW;

	if(EPWMFlagU != 0)
	{
		EPWMFlagU = 0;
		EPwm1Regs.AQCTLA.all = 0x90;
        if(DirectActFlagU != 0)
        {
            EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0;
            DirectActFlagU = 0;
        }
	}
	if(EPWMFlagV != 0)
	{
		EPWMFlagV = 0;
		EPwm2Regs.AQCTLA.all = 0x90;
        if(DirectActFlagV != 0)
        {
            EPwm2Regs.CMPCTL.bit.SHDWAMODE = 0;
            DirectActFlagV = 0;
        }        
	}
	if(EPWMFlagW != 0)
	{
		EPWMFlagW = 0;
		EPwm3Regs.AQCTLA.all = 0x90;
        if(DirectActFlagW != 0)
        {
            EPwm3Regs.CMPCTL.bit.SHDWAMODE = 0;
            DirectActFlagW = 0;
        }
	}

	//...(CMPR=!0)=>(CMPR=0)¡¢(CMPR=!0)=>(CMPR=0)µÚÒ»ÅÄÐÞ¸Ä¼Ä´æÆ÷
	if(EPwm1Regs.TBSTS.bit.CTRDIR == 0)    //ÏÂ½µÑØÖ´ÐÐµÄÖÐ¶Ï
    {
        if(gPWM.U == 0)
    	{
    		if(EPwm1Regs.CMPA.half.CMPA != 0)
    		{
    			EPwm1Regs.AQCTLA.all = 0x91;
    			EPWMFlagU = 1;
    		}
    	}
    	else
    	{
    		if(EPwm1Regs.CMPA.half.CMPA == 0)
    		{
    			EPwm1Regs.AQCTLA.all = 0x12;
                EPwm1Regs.CMPCTL.bit.SHDWAMODE = 1;
                EPWMFlagU = 1;
                DirectActFlagU = 1;
    		}
    	}

    	if(gPWM.V == 0)
    	{
    		if(EPwm2Regs.CMPA.half.CMPA != 0)
    		{
    			EPwm2Regs.AQCTLA.all = 0x91;
    			EPWMFlagV = 1;
    		}
    	}
    	else
    	{
    		if(EPwm2Regs.CMPA.half.CMPA == 0)
    		{
    			EPwm2Regs.AQCTLA.all = 0x12;
                EPwm2Regs.CMPCTL.bit.SHDWAMODE = 1;
    			EPWMFlagV = 1;
                DirectActFlagV = 1;
    		}
    	}

    	if(gPWM.W == 0)
    	{
    		if(EPwm3Regs.CMPA.half.CMPA != 0)
    		{
    			EPwm3Regs.AQCTLA.all = 0x91;
    			EPWMFlagW = 1;
    		}
    	}
    	else
    	{
    		if(EPwm3Regs.CMPA.half.CMPA == 0)
    		{
    			EPwm3Regs.AQCTLA.all = 0x12;
                EPwm3Regs.CMPCTL.bit.SHDWAMODE = 1;
    			EPWMFlagW = 1;
                DirectActFlagW = 1;
    		}
    	}

        //EPwm1Regs.CMPB = gADC.DelayApply;
    }
    else     //ÉÏÉýÑØÖ´ÐÐµÄÖÐ¶Ï   
    {
        if(gPWM.U == gPWM.gPWMPrdApply)
        {
            if(EPwm1Regs.CMPA.half.CMPA != gPWM.gPWMPrdApply)
            {
                EPwm1Regs.AQCTLA.all = 0x98;
                EPWMFlagU = 1;
            }
        }
        else
        {
            if(EPwm1Regs.CMPA.half.CMPA == gPWM.gPWMPrdApply)
            {
                EPwm1Regs.CMPCTL.bit.SHDWAMODE = 1;
                EPwm1Regs.AQCTLA.all = 0x84; 
                EPWMFlagU = 1;
                DirectActFlagU = 1;
            }
        }

        if(gPWM.V == gPWM.gPWMPrdApply)
        {
            if(EPwm2Regs.CMPA.half.CMPA != gPWM.gPWMPrdApply)
            {
                EPwm2Regs.AQCTLA.all = 0x98;
                EPWMFlagV = 1;
            }
        }
        else
        {
            if(EPwm2Regs.CMPA.half.CMPA == gPWM.gPWMPrdApply)
            {
                EPwm2Regs.CMPCTL.bit.SHDWAMODE = 1;                
                EPwm2Regs.AQCTLA.all = 0x84;                
                EPWMFlagV = 1;
                DirectActFlagV = 1;
            }
        }


        if(gPWM.W == gPWM.gPWMPrdApply)
        {
            if(EPwm3Regs.CMPA.half.CMPA != gPWM.gPWMPrdApply)
            {
                EPwm3Regs.AQCTLA.all = 0x98;
                EPWMFlagW = 1;
            }
        }
        else
        {
            if(EPwm3Regs.CMPA.half.CMPA == gPWM.gPWMPrdApply)
            {
                EPwm3Regs.CMPCTL.bit.SHDWAMODE = 1;
                EPwm3Regs.AQCTLA.all = 0x84;
                EPWMFlagW = 1;
                DirectActFlagW = 1;
            }
        }

        //6EPwm1Regs.CMPB = EPwm1Regs.TBPRD - gADC.DelayApply;
    }    

	EPwm1Regs.TBPRD = gPWM.gPWMPrdApply; 
	//EPwm1Regs.CMPB  = EPwm1Regs.TBPRD - gADC.DelayApply;
	EPwm2Regs.TBPRD = gPWM.gPWMPrdApply;
	EPwm3Regs.TBPRD = gPWM.gPWMPrdApply;    

    EPwm1Regs.CMPA.half.CMPA = gPWM.U;
    EPwm2Regs.CMPA.half.CMPA = gPWM.V;
    EPwm3Regs.CMPA.half.CMPA = gPWM.W;   
	EDIS;        
}

