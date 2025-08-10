/***********************************Inovance***********************************
����������Function Description��:ͬ�������ٶȴ���������
����޸����ڣ�Date����
�޸���־��History��:�����¼�¼Ϊ��һ��ת���Ժ󣬿�ʼ��¼��
	���� 		ʱ�� 		����˵��
1 	xx 		xxxxx 		xxxxxxx
2 	yy 		yyyyy 		yyyyyyy
************************************Inovance***********************************/

/* Includes ------------------------------------------------------------------*/
#include "MotorInclude.h"
#include "MotorIPMSVC.h"

/* Private variables ---------------------------------------------------------*/
PMSM_ROTOR_POS_EST_STRUCT   gPmsmRotorPosEst;
extern MT_STRUCT_Q24           gUMTQ24; 
/* Private function prototypes -----------------------------------------------*/
void PrepareParForPmsmSvc(void);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : PmsmSvc������ʼ������
* Description    : ��ʼ��ͬ����SVC��Ҫ�õ��ı�����ͬʱ���������ʼֵ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ResetParForPmsmSvc(void)
{       
    /*��λͬ����SVC�õ��ı���*/
    gPmsmRotorPosEst.Kc = 768;	        //��Ӧ0.75

	gPmsmRotorPosEst.IdLast = 0;
	gPmsmRotorPosEst.IqLast = 0;

	gPmsmRotorPosEst.DetaId = 0;
	gPmsmRotorPosEst.DetaIq = 0;

    gPmsmRotorPosEst.FcSetlpf = gPmsmRotorPosEst.FcLow * 100;
    gPmsmRotorPosEst.SvcIdSetForLowSpeed = (((s32)gPmsmRotorPosEst.SvcIdMaxForLowSpeed)<<12)/100;
    gPmsmRotorPosEst.SvcSpeedLpfCoef = 30;

    gPmsmRotorPosEst.CurFreqSetReal = 0;
	gPmsmRotorPosEst.FCMode = 0;

	gPmsmRotorPosEst.UdLast = 0;
	gPmsmRotorPosEst.UqLast = 0;
	gPmsmRotorPosEst.SpeedEstValueLpf =0;
	gPmsmRotorPosEst.SpeedEstValue =0;
	gPmsmRotorPosEst.SvcRotorSpeed = 0;
    gPmsmRotorPosEst.SpeedEstValueLast = 0;

	gPmsmRotorPosEst.BmfEstValueLast = 0;
	gPmsmRotorPosEst.BmfEstValue = 0;	
   /**********************��ʼ�����綯����ر���******************/
    if((gUVCoff.OnlineTuneBemfVoltEnable == 1)&&(INV_VOLTAGE_380V == gInvInfo.InvVoltageType))
	{
	    gEstBemf.BemfVoltDisPlay = gEstBemf.BemfVoltFilter;
		gEstBemf.BemfVoltOnline = gEstBemf.BemfVoltFilter;
		gEstBemf.BemfVoltDisPlayLast = gEstBemf.BemfVoltFilter;		
	}
	/***************************END********************************/
    /*��ʼ��ת�ٹ����õ���ϵ��*/
    PrepareParForPmsmSvc();
}

/*************************************************************
    ��ʼ��ת�ٺ�λ�ù�������Ҫ�õ���ϵ����Ĭ����Ƶ2.0K
    ʹ�õ��ⲿ����:
    ggBasePar.FullFreq01:Ƶ�ʻ�ֵ����λ0.01HZ
    gBasePar.FullFreq��gMotorInfo.FrequencyС����λ����ͬ
    gBasePar.FcSetApply:�ز�Ƶ�ʣ���λ100HZ
    gMotorExtPer.LQ:Q13
    gMotorExtInfo.EMF:�߷��綯����Чֵ����λ0.1V
*************************************************************/
void PrepareParForPmsmSvc(void)
{
    s32 m_s32;
//	static u16 m_Flag = 0;
      
    //�ز�Ƶ�� 
    gPmsmRotorPosEst.FcSetPer  = 65189865L/gBasePar.FullFreq01;  

    //����ϵ��T/L 
    m_s32 = 33554432L/gPmsmRotorPosEst.FcSetPer;
//    gPmsmRotorPosEst.TDivLqCoef = ((m_s32<<8)/gMotorExtPer.LQ);
    gPmsmRotorPosEst.TDivLqCoef = ((m_s32<<8)/gMotorExtPer.LD);
    m_s32 = 33554432L/gPmsmRotorPosEst.FcSetPer;
    gPmsmRotorPosEst.TDivLdCoef = ((m_s32<<8)/gMotorExtPer.LD);

    //����ϵ��KiForEmf=Ka*L/T 
//	m_s32 = (((s32)gMotorExtPer.LQ * (s32)gPmsmRotorPosEst.FcSetPer)>>8);
    m_s32 = (((s32)gMotorExtPer.LD * (s32)gPmsmRotorPosEst.FcSetPer)>>8); 
    gPmsmRotorPosEst.KiForEmf = (((s32)gPmsmRotorPosEst.Ka * m_s32)>>5);
	  
    //�����ͨ��ôֵ 1/Kf 
	if((gUVCoff.OnlineTuneBemfVoltEnable == 1)&&(INV_VOLTAGE_380V == gInvInfo.InvVoltageType))
	{
	    m_s32 = ((s32)gEstBemf.BemfVoltFilter<<10)/gMotorInfo.Votage;
	}
	else
	{
        m_s32 = ((s32)gMotorExtInfo.BemfVolt<<10)/gMotorInfo.Votage;
	}
    m_s32 = (m_s32 * (s32)gBasePar.FullFreq)/((s32)gMotorInfo.Frequency * 10);
	gPmsmRotorPosEst.InvOfKf = 1048576L/m_s32; 
 
    //����ϵ��KiForSpeed=Kb*L/(T*Kf)
	m_s32 = ((s32)gMotorExtPer.LD * (s32)gPmsmRotorPosEst.FcSetPer)>>10;
	gPmsmRotorPosEst.KiForSpeed = ((m_s32 * (s32)gPmsmRotorPosEst.Kb)>>13) * gPmsmRotorPosEst.InvOfKf;  // kb��Сһ��

    //����Ƕȱ任ϵ��
	gPmsmRotorPosEst.TsPer = ((s32)gBasePar.FullFreq01 * 524L)/200;   
}


/*******************************************************************************
* Function Name  : ͬ����SVCת�ٺ�λ�ù��㣬���жϳ�����ִ��
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PmsmSvcCtrl(void)
{
    s16 m_Id,m_Iq,m_Ud,m_Uq;
	u16 m_FcSet;
    s32 m_Long,m_Long1;
    s32 m_TDivLqCoef,m_TDivLdCoef,m_TsPer,m_KiForSpeed,m_KiForEmf;


    if((1 == EPwm1Regs.TBSTS.bit.CTRDIR)||(gBasePar.FcSetApply > C_DOUBLE_ACR_MAX_FC))
	{
        m_FcSet = 3000000ul / (u32)gPWM.gPWMPrdApplyLast;    //��ʵ��ֵ�Ŵ���10��������gPmsmRotorPosEst.FcCoffҲ���10��
    }
	else 
	{
	    m_FcSet = 3000000ul / (u32)gPWM.gPWMPrdApply; 
	}

    m_TDivLdCoef = (gPmsmRotorPosEst.TDivLdCoef * (s32)gPmsmRotorPosEst.FcCoff)/m_FcSet;
    //m_TDivLdCoef = (((gPmsmRotorPosEst.TDivLdCoef * (s32)gPmsmRotorPosEst.FcCoff)/100L) * (s32)gPWM.gPWMPrdApplyLast)/3000L;
    m_TDivLqCoef = (gPmsmRotorPosEst.TDivLqCoef * (s32)gPmsmRotorPosEst.FcCoff)/m_FcSet; 
    //m_TDivLqCoef = (((gPmsmRotorPosEst.TDivLqCoef * (s32)gPmsmRotorPosEst.FcCoff)/100L) * (s32)gPWM.gPWMPrdApplyLast)/3000L; 
    m_TsPer      = (gPmsmRotorPosEst.TsPer * (s32)gPmsmRotorPosEst.FcCoff)/m_FcSet;
    //m_TsPer      = (((gPmsmRotorPosEst.TsPer * (s32)gPmsmRotorPosEst.FcCoff)/100L) * (s32)gPWM.gPWMPrdApplyLast)/3000L;
    m_KiForEmf   = (gPmsmRotorPosEst.KiForEmf * (s32)m_FcSet)/(s32)gPmsmRotorPosEst.FcCoff;
    //m_KiForEmf   = (gPmsmRotorPosEst.KiForEmf * 3000L)/(((s32)gPmsmRotorPosEst.FcCoff * (s32)gPWM.gPWMPrdApplyLast) / 100L);
    m_KiForSpeed = (gPmsmRotorPosEst.KiForSpeed * (s32)m_FcSet)/(s32)gPmsmRotorPosEst.FcCoff;
    //m_KiForSpeed = (gPmsmRotorPosEst.KiForSpeed * 3000L)/(((s32)gPmsmRotorPosEst.FcCoff * (s32)gPWM.gPWMPrdApplyLast) / 100L);
    //�õ���ǰ��������ѹ����Чֵ	
    m_Id = gIMTQ12.M;
    m_Iq = gIMTQ12.T;

   // m_Ud = (s32)gOutVolt.VoltApply * (s32)qsin(16384 - gOutVolt.VoltPhaseApply)>>15;
   // m_Uq = (s32)gOutVolt.VoltApply * (s32)qsin(gOutVolt.VoltPhaseApply)>>15;

    m_Ud = gPmsmRotorPosEst.Ud;
    m_Uq = gPmsmRotorPosEst.Uq;

    //����D��Q���������ֵ
    //m_Long  = ((s32)gMotorExtPer.LQ * (s32)gPmsmRotorPosEst.IqLast)>>10;  
    m_Long  = ((s32)gMotorExtPer.LD * (s32)gPmsmRotorPosEst.IqLast)>>10;             
    m_Long  = (m_Long * (s32)gPmsmRotorPosEst.SpeedEstValueLast)>>15;              
    m_Long1 = ((s32)gPmsmRotorPosEst.UdLast)<<3; 
    m_Long  = m_Long + m_Long1;                                                    
    m_Long1 = ((s32)gMotorExtPer.Rpm * (s32)gPmsmRotorPosEst.IdLast)>>13;            
    m_Long  = m_Long - m_Long1;             
    m_Long  = (m_Long * (s32)m_TDivLdCoef)>>13;                                      
    gPmsmRotorPosEst.IdEstValue = m_Long + gPmsmRotorPosEst.IdLast;               
    
    m_Long  = ((s32)gMotorExtPer.LD * (s32)gPmsmRotorPosEst.IdLast)>>10;                    
    m_Long  = (m_Long * (s32)gPmsmRotorPosEst.SpeedEstValueLast)>>15;                                
    m_Long1 = ((s32)gPmsmRotorPosEst.UqLast - gPmsmRotorPosEst.BmfEstValueLast)<<3;     
    m_Long  = m_Long1 - m_Long;                                                           
    m_Long1 = ((s32)gMotorExtPer.Rpm * (s32)gPmsmRotorPosEst.IqLast)>>13;             
    m_Long  = m_Long - m_Long1;      
    m_Long  = (m_Long * (s32)m_TDivLqCoef)>>13;       
    gPmsmRotorPosEst.IqEstValue = m_Long + gPmsmRotorPosEst.IqLast;

    //�������ƫ��ͷ��綯�ƹ���ֵ
    gPmsmRotorPosEst.DetaId = gPmsmRotorPosEst.DetaId + ((gPmsmRotorPosEst.IdEstValue - m_Id)>>3) - (gPmsmRotorPosEst.DetaId>>3);
    gPmsmRotorPosEst.DetaIq = gPmsmRotorPosEst.DetaIq + ((gPmsmRotorPosEst.IqEstValue - m_Iq)>>1)-(gPmsmRotorPosEst.DetaIq>>1);
    gPmsmRotorPosEst.BmfEstValue = gPmsmRotorPosEst.BmfEstValue + ((s32)gPmsmRotorPosEst.DetaIq * (s32)m_KiForEmf>>17);

    m_Long = gPmsmRotorPosEst.Kc;

    if(gPmsmRotorPosEst.DetaId > 0)
    {
        m_Long = -m_Long;
    }
    m_Long  = 1024 + m_Long;

    m_Long1 = m_KiForSpeed;
	if(gPmsmRotorPosEst.BmfEstValueLast >0)
    {
        m_Long1 = -m_Long1;
    }
    m_Long = (m_Long * m_Long1)>>10;                      

    m_Long = (m_Long * gPmsmRotorPosEst.DetaId)>>12;   

    //����תٺ�λ��
    gPmsmRotorPosEst.SpeedEstValue    = m_Long + (((s32)gPmsmRotorPosEst.InvOfKf * (s32)gPmsmRotorPosEst.BmfEstValue)>>7);
    gPmsmRotorPosEst.SpeedEstValueLpf = gPmsmRotorPosEst.SpeedEstValueLpf + (gPmsmRotorPosEst.SpeedEstValue/gPmsmRotorPosEst.SvcSpeedLpfCoef) 
                                        -(gPmsmRotorPosEst.SpeedEstValueLpf /gPmsmRotorPosEst.SvcSpeedLpfCoef);
    
    gPmsmRotorPosEst.SvcRotorSpeed = __IQsat(gPmsmRotorPosEst.SpeedEstValueLpf,32767,-32767);      // ���ٶ����������޷�
    //gPmsmRotorPosEst.SvcRotorSpeed  = gPmsmRotorPosEst.SpeedEstValueLpf;
    
    gPmsmRotorPosEst.SvcRotorPos    = gPmsmRotorPosEst.SvcRotorPos + (((s64)gPmsmRotorPosEst.SpeedEstValue * (s64)m_TsPer)>>3);

    gPmsmRotorPosEst.BmfEstValueLast   = gPmsmRotorPosEst.BmfEstValue;
    gPmsmRotorPosEst.SpeedEstValueLast = gPmsmRotorPosEst.SpeedEstValue;
    
    gPmsmRotorPosEst.IdLast = m_Id;
    gPmsmRotorPosEst.IqLast = m_Iq;
    gPmsmRotorPosEst.UdLast = m_Ud;
    gPmsmRotorPosEst.UqLast = m_Uq;   
}


/*******************************************************************************
* Function Name  : ����ʱ����Ƶ�Ĵ���
* Description    : ��Ҫ���ڼ�С������Ӱ?
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PmsmSvcCalFc(void)
{
/*	s16 m_Freq,m_FreqLow,m_FreqHigh;
    u16 m_FcLow,m_FcHigh,m_FcSet,m_FcSetlpf;
    s32 m_Long;
*/
    s16 m_Freq;
	s32 m_Long,m_FcLow,Fc;

	m_FcLow = gPmsmRotorPosEst.FcLow;
	m_Freq = abs(gRotorSpeed.SpeedBigFilter);

	if(gPmsmRotorPosEst.FCMode == 0)
	{
		if(m_Freq > (gMotorInfo.FreqPer>>2))
		{
			gPmsmRotorPosEst.FCMode = 1;
		}
	}
	else
	{
		if(m_Freq < (gMotorInfo.FreqPer>>3))
		{
			gPmsmRotorPosEst.FCMode = 0;
		}
	}
	if(gPmsmRotorPosEst.FCMode == 0)
	{
		Fc = m_FcLow;
   //     gFcCal.MinFc = gPmsmRotorPosEst.FcLow;
	}
	else
	{
		Fc = gFcCal.FcBak;
	}
	gFcCal.FcBak = __IQsat(Fc,60,10);
/*
#if (SOFTSERIES == MD500SOFT)
	m_FcLow = gPmsmRotorPosEst.FcLow * 100;
#else
    m_FcLow = gPmsmRotorPosEst.FcLow * 150;
#endif
	//m_FcHigh = gBasePar.FcSet * 100;
	if(gBasePar.FcSet < 20)
	{
	    m_FcHigh = 2000;
	}
	else
	{
	    m_FcHigh = gBasePar.FcSet * 100;
	}

	if((m_FcHigh <= m_FcLow)||(gPmsmRotorPosEst.FcLowPoint == 0))
	{
		m_FcSet = m_FcHigh;
		gPmsmRotorPosEst.FcSetlpf = m_FcSet;
	}
	else
	{
		if(1 == gMainCmd.Command.bit.TorqueCtl)     // ת��ģʽ����ʵ���ٶ��ж�
		{
		    m_Freq = abs(gRotorSpeed.SpeedBigFilter);
		}
		else
		{
		    m_Freq = abs(gMainCmd.FreqSet);
		}
		m_FreqLow  = (Ulong)gMotorInfo.FreqPer*gPmsmRotorPosEst.FcLowPoint/250UL;
		m_FreqHigh = (Ulong)gMotorInfo.FreqPer*gPmsmRotorPosEst.FcLowPoint/100UL;

		if(m_Freq <= m_FreqLow)
		{
		    m_FcSet = m_FcLow;
		}
		else if(m_Freq > m_FreqHigh)
		{
			m_FcSet = m_FcHigh;
		}
		else
		{
			m_Long  = (s32)(m_Freq - m_FreqLow) * (m_FcHigh - m_FcLow);
			m_FcSet = m_FcLow + m_Long/(m_FreqHigh - m_FreqLow);	
		}
		gPmsmRotorPosEst.FcSetlpf = m_FcSet;//gPmsmRotorPosEst.FcSetlpf + (m_FcSet>>3) - (gPmsmRotorPosEst.FcSetlpf>>3);
	}
    //gPmsmRotorPosEst.FcSetlpf = gPmsmRotorPosEst.FcSetlpf + (m_FcSet>>3) - (gPmsmRotorPosEst.FcSetlpf>>3);
    m_FcSetlpf = gPmsmRotorPosEst.FcSetlpf/100;
    gFcCal.FcBak = Min(gFcCal.FcBak,m_FcSetlpf);
*/
    m_Long = ((s32)(gBasePar.FcSetApply * 100) * (s32)gPmsmRotorPosEst.SvcSpeedLpfTs)>>13;
    m_Long = m_Long * 200 / (s32)gPmsmRotorPosEst.FcCoff;  //����һ���ز����ڵ���һ�κ����Σ����˲�ʱ�䳣����Ӱ��
	if(m_Freq < (gMotorInfo.FreqPer / 5))   // �����������˲�ϵ����wyk
	{
	    m_Long = 2*m_Long;
	}
    m_Long = Min(m_Long,512);
    m_Long = Max(m_Long,8);
	gPmsmRotorPosEst.SvcSpeedLpfCoef = m_Long;         
}

/*******************************************************************************
* Function Name  : ����ʱ�����ŵ����Ĵ���
* Description    : �������ѹ�ϵ�ʱ��ͨ��ע�����ţ����������ѹ����С������Ӱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PmsmSvcCalImForLowSpeed()
{
    s16 m_Freq,m_FreqLow,m_FreqHigh,m_IdSet,m_IdMax;
    s32 m_Long;

	m_Freq     = abs(gRotorSpeed.SpeedBigFilter);
	m_FreqLow  = gMotorInfo.FreqPer/10;
	m_FreqHigh = gMotorInfo.FreqPer/5;
    m_IdMax    = (((s32)gPmsmRotorPosEst.SvcIdMaxForLowSpeed)<<12)/100;

    if(m_Freq >= m_FreqLow)
    {
        if(m_Freq >= m_FreqHigh)
    	{
    		m_IdSet = 0;
    	}
    	else
    	{
    		m_Long  = (s32)(m_FreqHigh - m_Freq) * (s32)m_IdMax;
            m_IdSet = m_Long/(m_FreqHigh - m_FreqLow);
    	}
    }
    else  //����Ƶ�ʺ�Сʱ��m_FreqZero��m_FreqLow���
    {
        m_IdSet =  m_IdMax;
    }

    gPmsmRotorPosEst.SvcIdSetForLowSpeed = m_IdSet;
    gPmsmRotorPosEst.SvcIdSetForLowSpeed = Max(gPmsmRotorPosEst.SvcIdSetForLowSpeed,0);
    gPmsmRotorPosEst.SvcIdSetForLowSpeed = Min(gPmsmRotorPosEst.SvcIdSetForLowSpeed,4000);

    if(gMotorInfo.Current > gInvInfo.InvCurrent)
    {
        gPmsmRotorPosEst.SvcIdSetForLowSpeed = (long)gPmsmRotorPosEst.SvcIdSetForLowSpeed * gInvInfo.InvCurrent / gMotorInfo.Current;
    }
    
}

/******************************* END OF FILE***********************************/
