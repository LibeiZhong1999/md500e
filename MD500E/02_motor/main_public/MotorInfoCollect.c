/****************************************************************
�ļ�����: ADģ���ʼ����ģ����������ת��
�ļ��汾��
���¸��£�
	
****************************************************************/
#include "MotorInfoCollectInclude.h"

// // ȫ�ֱ�������
ADC_STRUCT				gADC;		    //ADC���ݲɼ��ṹ
UDC_STRUCT				gUDC;		    //ĸ�ߵ�ѹ����
IUVW_SAMPLING_STRUCT	gCurSamp;

UVW_STRUCT				gIUVWQ12;		//�����������������
UVW_STRUCT_Q24          gIUVWQ24;       //Q24��ʽ�����ඨ�ӵ���
MT_STRUCT				gIMTQ12;		//MT��ϵ�µĵ���,Q12��ʾ
MT_STRUCT_Q24           gIMTQ24;        //MT��ϵ�µĵ���,Q24��ʾ
MT_STRUCT               gIMTSetQ12;
ALPHABETA_STRUCT		gIAlphBeta;	    //�����������������
ALPHABETA_STRUCT		gIAlphBetaQ12;	    //�����������������
AMPTHETA_STRUCT			gIAmpTheta;	    //�������ʾ�ĵ���
LINE_CURRENT_STRUCT		gLineCur;	
CUR_EXCURSION_STRUCT	gExcursionInfo; //�����Ưʹ�õĽṹ
TEMPLETURE_STRUCT		gTemperature;
AI_STRUCT				gAI;
ADC_ADJ_STRUCT          gADCAdjust;
#if (SOFTSERIES == MD500SOFT)
/************************************************************
	MD500������Ư����
	��W����Ư���㼰�ƶ�����������Ư
************************************************************/
void GetCurExcursion(void)
{
	int m_ErrIu,m_ErrIv,m_ErrIw;
	Uint m_ErrIb;
	
	if((gMainStatus.RunStep != STATUS_LOW_POWER) && 
	   (gMainStatus.RunStep != STATUS_STOP))
	{
		gExcursionInfo.EnableCount = 0;
		return;
	}

	gExcursionInfo.EnableCount++;
	gExcursionInfo.EnableCount = (gExcursionInfo.EnableCount>200)?200:gExcursionInfo.EnableCount;
	if((gExcursionInfo.EnableCount < 200))
	{
		gExcursionInfo.TotalIu = 0;
		gExcursionInfo.TotalIv = 0;
		gExcursionInfo.TotalIw = 0;
		gExcursionInfo.Count = 0;
		gExcursionInfo.IbCount = 0;
		gExcursionInfo.IbOkFlag = 0;
		return;		
	}
	gExcursionInfo.TotalIu += gExcursionInfo.Iu;
	gExcursionInfo.TotalIv += gExcursionInfo.Iv;
	gExcursionInfo.TotalIw += gExcursionInfo.Iw;
	gExcursionInfo.Count++;
	gExcursionInfo.TotalIb += gExcursionInfo.Ib;
	gExcursionInfo.IbCount++;
	
	if(gExcursionInfo.Count >= 32)					//ÿ32�ļ��һ����Ư
	{
		m_ErrIu = gExcursionInfo.TotalIu >> 5;
		m_ErrIv = gExcursionInfo.TotalIv >> 5;
		m_ErrIw = gExcursionInfo.TotalIw >> 5;
        if(-32768 == m_ErrIu)                       //��ֹȡ����ֵʱ���
            m_ErrIu = -32767;
        if(-32768 == m_ErrIv)
            m_ErrIv = -32767;	
        if(-32768 == m_ErrIw)
            m_ErrIw = -32767;	
		gExcursionInfo.TotalIu = 0;
		gExcursionInfo.TotalIv = 0;
		gExcursionInfo.TotalIw = 0;
		gExcursionInfo.Count = 0;
		
		if( (abs(m_ErrIu) < 5120) && (abs(m_ErrIv) < 5120) && (abs(m_ErrIw) < 5120))
		{
			gExcursionInfo.ErrIu = m_ErrIu;
			gExcursionInfo.ErrIv = m_ErrIv;
			gExcursionInfo.ErrIw = m_ErrIw;
			gExcursionInfo.ErrCnt = 0;
			gMainStatus.StatusWord.bit.RunEnable = 1;
		}
		else if((gExcursionInfo.ErrCnt++) > 5)		//����5����Ư������ű�18����
		{
			gError.ErrorCode.all |= ERROR_CURRENT_CHECK;
			gExcursionInfo.ErrCnt = 0;
			gExcursionInfo.EnableCount = 0;
			gMainStatus.StatusWord.bit.RunEnable = 0;
		}
	}

	if(gExcursionInfo.IbCount >= 512) 				//ÿ512�ļ��һ����Ư
	{	    		
		if(gExcursionInfo.IbExcursionOkFlag == 0)   //ֻ�ϵ���һ��
		{
			m_ErrIb = gExcursionInfo.TotalIb >> 9;
			gExcursionInfo.TotalIb = 0;
			gExcursionInfo.IbCount = 0;
			//gExcursionInfo.Test = ((Ulong)m_ErrIb * 330L) >> 16;
			//	�����⵽�ڷ�Χ�ڵ�ֵ��Ϊ����Ư�������ڴ˴��ж��Ƿ�ֱͨ
			//	��ר�ŵ��ж�ֱͨ�������жϣ��Ǳ��õ�ֵ��ȥ����Ư��
			if (m_ErrIb < 3971)  //200mV
			{
				gExcursionInfo.ErrIb = m_ErrIb;						
				gExcursionInfo.IbOkFlag = 1;
				gExcursionInfo.IbExcursionOkFlag = 1;
			}
			else if (gExcursionInfo.IbOkFlag == 0)
			{
				gError.ErrorCode.all |= ERROR_IGBT_SHORT_BRAKE;
	        	gError.ErrorInfo[4].bit.Fault1 = 1;	
				gExcursionInfo.EnableCount = 0;
				gExcursionInfo.IbOkFlag = 0;
			}
		}
		else
		{
		    gExcursionInfo.IbOkFlag = 1;
			gExcursionInfo.TotalIb = 0;
			gExcursionInfo.IbCount = 0;
		}
	}
}

#else  //MD380

/************************************************************
	������Ư����
************************************************************/
void GetCurExcursion(void)
{
	int m_ErrIu,m_ErrIv;
	
	if((gMainStatus.RunStep != STATUS_LOW_POWER) && 
	   (gMainStatus.RunStep != STATUS_STOP))
	{
		gExcursionInfo.EnableCount = 0;
		return;
	}

	gExcursionInfo.EnableCount++;
	gExcursionInfo.EnableCount = (gExcursionInfo.EnableCount>200)?200:gExcursionInfo.EnableCount;
	if((gExcursionInfo.EnableCount < 200))
	{
		gExcursionInfo.TotalIu = 0;
		gExcursionInfo.TotalIv = 0;
		gExcursionInfo.Count = 0;
		return;		
	}
	gExcursionInfo.TotalIu += gExcursionInfo.Iu;
	gExcursionInfo.TotalIv += gExcursionInfo.Iv;
	gExcursionInfo.Count++;

	if(gExcursionInfo.Count >= 32)					//ÿ32�ļ��һ����Ư
	{
		m_ErrIu = gExcursionInfo.TotalIu >> 5;
		m_ErrIv = gExcursionInfo.TotalIv >> 5;
        if(-32768 == m_ErrIu)                       //��ֹȡ����ֵʱ���
            m_ErrIu = -32767;
        if(-32768 == m_ErrIv)
            m_ErrIv = -32767;		
		gExcursionInfo.TotalIu = 0;
		gExcursionInfo.TotalIv = 0;
		gExcursionInfo.Count = 0;
		
		gMainStatus.StatusWord.bit.RunEnable = 1;
		if( (abs(m_ErrIu) < 5120) && (abs(m_ErrIv) < 5120) )
		{
			gExcursionInfo.ErrIu = m_ErrIu;
			gExcursionInfo.ErrIv = m_ErrIv;
			gExcursionInfo.ErrCnt = 0;
		}
		else if((gExcursionInfo.ErrCnt++) > 5)		//����5����Ư������ű�18����
		{
			gError.ErrorCode.all |= ERROR_CURRENT_CHECK;
			gExcursionInfo.ErrCnt = 0;
			gExcursionInfo.EnableCount = 0;
		}
	}
}
#endif
/****************************************************************
	��ȡĸ�ߵ�ѹ���ݣ����gUDC
*****************************************************************/
void GetUDCInfo(void)
{
	Uint m_uDC;
	//int	 m_DetaUdc;

   	m_uDC = ((Uint32)ADC_UDC * gUDC.Coff)>>16;                  //9
   	gUDC.uDC = (gUDC.uDC + m_uDC)>>1;
   	gUDC.uDCFilter = gUDC.uDCFilter - (gUDC.uDCFilter>>3) + (gUDC.uDC>>3);
    gUDC.uDCShow = Filter32(gUDC.uDC,gUDC.uDCShow);  // ���ڼ�����ʾ��ĸ�ߵ�ѹ
	gUDC.uDCBigFilter = Filter128(gUDC.uDC,gUDC.uDCBigFilter);   // Wc = 1Hz; trise > 1ms
#if (SOFT_INPUT_DETECT == STLEN)
    gSoftInLose.MaxUdc_5s = Max(gSoftInLose.MaxUdc_5s ,gUDC.uDC);
    gSoftInLose.MinUdc_5s = Min(gSoftInLose.MinUdc_5s ,gUDC.uDC);
	gSoftInLose.MaxUdcTemper = Max(gSoftInLose.MaxUdcTemper ,gUDC.uDC);
	gSoftInLose.MinUdcTemper = Min(gSoftInLose.MinUdcTemper ,gUDC.uDC);
#endif 
}

/****************************************************************
	��ȡ�����������ݣ����gCurSamp
*****************************************************************/
void GetCurrentInfo(void)
{
	long  m_Iu,m_Iv,m_TempT;
#if (SOFTSERIES == MD500SOFT)
	long m_Iw;
//	Uint m_AvgU,m_AvgV,m_AvgW;
#endif	

#if (SOFTSERIES == MD500SOFT)
/*#ifdef CURRENT_ADJUST		// ���й�����AD��Ư��������MD500��Ŀ���Բ�����
	long  m_TempU,m_TempV,m_TempW;

    // ��֪б�ʺ�ƫ�ã�ͨ��ADֵ�ȷ����ѹ��Ȼ���������ѹ��Ӧ65536���������ADֵ
    m_TempU = (long)ISamp.iuAver - (long)gADCAdjust.Offset;
    m_TempU = (m_TempU * (long)gADCAdjust.SlopePU) >> 14;   
	m_TempU = (m_TempU < 0) ? 0: m_TempU;
	m_AvgU = (Uint)((m_TempU > 65535) ? 65535:m_TempU);

	m_TempV= (long)ISamp.ivAver - (long)gADCAdjust.Offset;
    m_TempV = (m_TempV * (long)gADCAdjust.SlopePU) >> 14;   
	m_TempV = (m_TempV < 0) ? 0: m_TempV;
	m_AvgV= (Uint)((m_TempV > 65535) ? 65535: m_TempV);

	m_TempW = (long)ISamp.iwAver - (long)gADCAdjust.Offset;
    m_TempW = (m_TempW * (long)gADCAdjust.SlopePU) >> 14;   
	m_TempW = (m_TempW < 0) ? 0: m_TempW;
	m_AvgW = (Uint)((m_TempW > 65535) ? 65535: m_TempW); 
#endif*/
#endif

	gExcursionInfo.Iu = (int)(ADC_IU - (Uint)32768);		
	m_Iu = (long)gExcursionInfo.Iu - (long)gExcursionInfo.ErrIu;	//ȥ����Ư
    gExcursionInfo.IuValue = m_Iu;                                  //���ڲ�����ʶ��������ʶ�Ż��󽫲���ʹ�øñ���                       
	gShortGnd.ShortCur = Filter32(m_Iu, gShortGnd.ShortCur);
	m_Iu = (m_Iu * gCurSamp.Coff) >> 3;
	m_Iu = __IQsat(m_Iu, C_MAX_PER, -C_MAX_PER);   

    
	gExcursionInfo.Iv = (int)(ADC_IV - (Uint)32768);		
	m_Iv = (long)gExcursionInfo.Iv - (long)gExcursionInfo.ErrIv;	//ȥ����Ư
    gExcursionInfo.IvValue = m_Iv;    
	//m_Iv = (m_Iv * gUVCoff.UDivV) >> 12;						    //��������ƫ��
	m_Iv = (m_Iv * gCurSamp.Coff) >> 3;	
	m_Iv = __IQsat(m_Iv, C_MAX_PER, -C_MAX_PER);

    gIUVWQ24.U = m_Iu;                      /*��ʹ���޳�ë���˲�����2011.05.07 L1082*/
    gIUVWQ24.V = m_Iv;
	
#if (SOFTSERIES == MD500SOFT)
	gExcursionInfo.Iw = (int)(ADC_IW - (Uint)32768);		
	m_Iw = (long)gExcursionInfo.Iw - (long)gExcursionInfo.ErrIw;	//ȥ����Ư
	m_Iw = (m_Iw * gCurSamp.Coff) >> 3;
	m_Iw = __IQsat(m_Iw, C_MAX_PER, -C_MAX_PER); 
	gIUVWQ24.W = m_Iw;
	
	//Q16->2*InvBreakMaxCurrent
	gExcursionInfo.Ib = ADC_IBREAK;
	if (gExcursionInfo.Ib > gExcursionInfo.ErrIb)
	{
		//Q12->InvBreakMaxCurrent
		gBrake.IBreakQ12 = ((Ulong)(gExcursionInfo.Ib - gExcursionInfo.ErrIb)*282L) >> 11;
	}
	else
	{
		gBrake.IBreakQ12 = 0;
	}
	//	�ƶ������������
	if(0 == EPwm1Regs.TBSTS.bit.CTRDIR && gExcursionInfo.IbOkFlag == 1)
	{			
		gBrake.IBreak = ((Uint32)gBrake.IBreakQ12 * gInvInfo.InvBreakMaxCurrent) >> 12;
		if((gBrake.Flag & 0x02) == 0x02)
		{
		    gBrake.IBreakTotal = gBrake.IBreak;
		}
	}
#else
	gIUVWQ24.W = - (gIUVWQ24.U + gIUVWQ24.V);
	gIUVWQ24.W = __IQsat(gIUVWQ24.W,C_MAX_PER,-C_MAX_PER);  
#endif	

#if (SOFTSERIES == MD500SOFT)
    m_TempT = (long)ADC_TEMP - (long)gADCAdjust.Offset;
    m_TempT = (m_TempT * (long)gADCAdjust.SlopePU) >> 14;   
	m_TempT = (m_TempT < 0) ? 0: m_TempT;
	m_TempT = (Uint)((m_TempT > 65535) ? 65535:m_TempT);
#else           
    m_TempT = ADC_TEMP;
#endif        
    gTemperature.TempAD = Filter16((m_TempT &0xFFF0), gTemperature.TempAD);

    gAI.ai1Total += (ADC_AI1);
    gAI.ai2Total += (ADC_AI2);
    gAI.ai3Total += (ADC_AI3);
#if(AIRCOMPRESSOR == 1)
    gAI.ai4Total += (ADC_AI4);
	gAI.ai5Total += (ADC_AI5);
#endif
    gAI.aiCounter ++;
}

#if (SOFTSERIES == MD500SOFT)
//#ifdef CURRENT_ADJUST
/*******************************************************************************
* Function Name  : 
* Description    : ��ȡ��������У��б����ƫ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetADCAdjustInfo(void)
{
	Uint m_Dleta;
	int m_Offset;
	Uint m_Slope;
	Uint m_YL;
	Uint m_YH;
    int  m_AdjEn;
	
	gADCAdjust.XL = 1247;         // 1.25,��0.001VΪ��λ
	gADCAdjust.XH = 2494;         // 2.5V,��0.001VΪ��λ
	m_YL = ADC_1250;  // 1.25V��Ӧ��ADC����ֵ��Q16
	m_YH = PL_VOE_PROTECT; // 2.5V��Ӧ��ADC����ֵ��Q16

    m_AdjEn = 0;
    if((m_YL > 23824) && (m_YL < 25817))
    {
        m_AdjEn = 1;
    }
//	�ж��Ƿ��Ǹߵ�ƽ�����������ܽ���У��
    if(m_AdjEn == 1)
    {
    	if (PL_VOE_PROTECT > ADC_VOLTAGE_20)
    	{
    		gADCAdjust.Ticker ++;
    		if (gADCAdjust.Ticker <= 1024)
    		{
    			gADCAdjust.YLTotal += m_YL;
    			gADCAdjust.YHTotal += m_YH;		
    		}
    		else
    		{
    			gADCAdjust.Ticker = 0;
    			gADCAdjust.YL = gADCAdjust.YLTotal >> 10;
    			gADCAdjust.YH = gADCAdjust.YHTotal >> 10;
    			gADCAdjust.YLTotal = 0;
    			gADCAdjust.YHTotal = 0;
    		}	
    		m_Dleta = gADCAdjust.YH - gADCAdjust.YL;
    		m_Slope = 1000UL * m_Dleta/(gADCAdjust.XH - gADCAdjust.XL); // б�� Q16
    		m_Offset = (long)gADCAdjust.YL - ((long)gADCAdjust.XL * m_Slope / 1000L);
    		m_Offset = Min(m_Offset, 256);
    		m_Offset = Max(m_Offset, -256);
    		gADCAdjust.Offset = m_Offset;// ƫ�� Q16 
    	}
    	else
    	{
    		m_Slope = gADCAdjust.Slope;
    	}
    }
    else
    {
        gADCAdjust.Offset = 0;
        m_Slope = 19859L;
    }
	gADCAdjust.Slope = Min(m_Slope, 20000);//19859
	gADCAdjust.Slope = Max(gADCAdjust.Slope, 19700);
	gADCAdjust.SlopePU = (19859L << 14) / gADCAdjust.Slope; 
}
//#endif
#endif
