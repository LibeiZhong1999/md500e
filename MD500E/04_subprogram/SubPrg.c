/****************************************************************
�ļ����ܣ��������е��õĴ��ģ�麯��
�ļ��汾��
���¸��£�
	
****************************************************************/
#include "MotorDefine.h"
#include "SubPrgInclude.h"

/****************************************************************
����˵���������к������ú�������x��y����õķ����нǶ��Լ�4���޵ĽǶ�
****************************************************************/
int atan(int x, int y)
{
	int  result;
	long m_Input;

	if(x == 0)
	{
		if(y < 0)			
		{
			return(-16384);
		}
		else
		{
			return(16384);
		}
	}
	m_Input = (((long)y)<<16)/x;
	result = qatan(m_Input);
	if(x < 0)
	{
		result += 32768;
	}
	return result;
}

/****************************************************************
����˵����PID����(��ʱ������D���������)
����ƫ��Ϊint�ͱ���
������Ϊlong�ͱ���������16λ�õ���Ҫ�Ľ��
����������pid�ڲ������� 4λ
****************************************************************/
void PID(PID_STRUCT * pid)
{
	long m_Max,m_Min,m_Out,m_OutKp,m_OutKi;
    long vMax = 0x7FFFFFFF;
    
    SETOVM;
 	m_Max = ((long)pid->Max)<<16;						// ���ֵ
	m_Min = ((long)pid->Min)<<16;						// ��Сֵ
	
	m_OutKp = (long)pid->KP * (long)pid->Deta;			// ����
	m_OutKp = __IQsat(m_OutKp, (vMax>> (4+ pid->QP)), -(vMax>> (4+ pid->QP))); //��֤ʹ����������
    m_OutKp = m_OutKp << (4+ pid->QP);
    
 	m_OutKi = (long)pid->KI * (long)pid->Deta;// ����
    m_OutKi = __IQsat(m_OutKi, (vMax >> pid->QI), -(vMax >> pid->QI));
    m_OutKi = m_OutKi << pid->QI;
    
    // ��������µ�ȥ���ʹ���
    if((m_OutKp > m_Max) && (pid->Total > 0))
    {
        pid->Total -= (pid->Total>>8) + 1;  //��1ȥ���˲�����
        m_OutKi = 0;
    }
    else if((m_OutKp < m_Min) && (pid->Total < 0))
    {
        pid->Total -= (pid->Total>>8) - 1; 
        m_OutKi = 0;
    }
	pid->Total += m_OutKi;	 
	m_Out       = pid->Total + m_OutKp;
 	pid->Out    = __IQsat(m_Out,m_Max,m_Min);
    pid->Total  = __IQsat(pid->Total,m_Max,m_Min);
    CLROVM;
}
/****************************************************************
����˵����
PID����(��ʱ������D���������)
����ƫ��Ϊlong�ͱ���;��������Ҫ��ƫ���Qֵ��ͬ

���Ϊllong�ͱ���������16λ�õ���Ҫ�Ľ��
*�������ӱ�������µ�ȥ���ʹ���*

����������pid�ڲ������� 4λ
****************************************************************/
/*void PIDLongRegulate(PID_STRUCT_LONG * pid)
{
	llong  m_Max,m_Min,m_Out,m_OutKp,m_OutKi;
 	m_Max = ((llong)pid->Max)<<16;						//���ֵ
	m_Min = ((llong)pid->Min)<<16;						//��Сֵ
    m_OutKp = (llong)pid->KP * (llong)pid->Deta << 4;		//����
	
	m_OutKi = (llong)pid->KI * (llong)pid->Deta;

// ��������µ�ȥ���ʹ���
    if(((m_OutKp > m_Max) && (pid->Total > 0)) ||
       ((m_OutKp < m_Min) && (pid->Total < 0))) 
    {
        pid->Total -= (pid->Total>>8);
        m_OutKi = 0;
    }

	pid->Total += m_OutKi;		
	m_Out      = pid->Total + m_OutKp;
    if(m_Out < m_Min)               pid->Out = m_Min;
    else if(pid->Out > m_Max)       pid->Out = m_Max;
    else                            pid->Out = m_Out;

    if(pid->Total < m_Min)          pid->Total = m_Min;
    else if(pid->Total > m_Max)     pid->Total = m_Max;      
}*/

/****************************************************************
����˵���������͵�PI��������Q24��ʽ��ʾ
	long  	Deta;			//ƫ��ֵ(Q24.7,���Ϊ2.0)
	long 	Out;			//���ֵ(Q24.7)
	long 	Total;			//�����ۼ�ֵ(Q24.7)
	long  	Max;			//���ֵ����(Q24.7,��Ҫ����64)
	long  	Min;			//��Сֵ����(Q24.7,��Ҫ����-64)
	long  	KP;				//KP����(Q12.19) �൱����Q12��ʽ
	long  	KI;				//KI����(Q16.15) �൱����Q16��ʽ
ע�⣺PID������KP�����ñ�KI�����÷Ŵ���16����
****************************************************************/
void PID32(PID32_STRUCT * pid)
{
	long  m_OutKp,m_OutKi;
	long  mTotalMax,mTotalMin;

	//if(pid->Deta == 0)		return;

	//����������ã���������������
	m_OutKp = ((llong)pid->KP * (llong)pid->Deta) >> (16-4);	
	//�������ֵ�������
	if(pid->Deta > 0)		
	{
		mTotalMax = pid->Max - m_OutKp;
		if(mTotalMax < 0)	mTotalMax  = 0;

		mTotalMin = pid->Min;
	}
	else
	{
		mTotalMin = pid->Min - m_OutKp;
		if(mTotalMin > 0)	mTotalMin  = 0;

		mTotalMax = pid->Max;
	}

    //�����������
	m_OutKi = ((llong)pid->KI * (llong)pid->Deta)>>16;
	pid->Total = pid->Total + m_OutKi;
	pid->Total = __IQsat(pid->Total, mTotalMax, mTotalMin);

	//����PID�����
	pid->Out = pid->Total + m_OutKp;
	pid->Out = __IQsat(pid->Out, pid->Max, pid->Min);
}

/****************************************************************
	ͨ���˲�����
	LastOne     ��һ���˲��������
	Input       ���Ĳ�������
	FilterTime  �˲�ʱ�䣨ms��λ��
�㷨��OutPut = LastOne + (Input - LastOne) * Coff
      Coff��Q15��ʽ�����ݣ����ݱ���С��Q15ֵ��
*****************************************************************/
int Filter(int LastOne, int Input, int Coff)
{
	long   m_Deta;
    int   m_Add;
	
	m_Deta = (long)Input - (long)LastOne;
	if(m_Deta == 0)	
	{
		return (LastOne);
	}

	m_Add = ((long)m_Deta * (long)Coff)>>15;
	if(m_Add == 0)
	{
		m_Add = (m_Deta > 0)?1:-1;
	}

	return (LastOne + m_Add);	
}

/****************************************************************
	�޳�ë���˲��������
*****************************************************************/
/*void BurrFilter(BURR_FILTER_STRUCT * filter)
{
	int 	m_Deta;

	m_Deta = abs((filter->Input) - (filter->Output));
	if(m_Deta > filter->Err)
	{
		filter->Err = filter->Err << 1;
	}
	else
	{
		filter->Output = filter->Input;
	  	if(m_Deta < (filter->Err >> 1))
		{
			filter->Err = filter->Err >> 1;
		}
	}
	filter->Err = (filter->Err > filter->Max)?filter->Max:filter->Err;
	filter->Err = (filter->Err < filter->Min)?filter->Min:filter->Err;
}*/

/****************************************************************
	�����˲�����(�����˲�ʱ�䳣���趨��ȫƽ�������ݸ���)
*****************************************************************/
/*void SlipFilter(CUR_LINE_STRUCT_DEF * pCur)
{
	int  m_Index,m_IndexStart,m_Coff,m_TotalCoff;
	long m_Total;
	CUR_LINE_STRUCT_DEF * m_pCur = pCur;

	for(m_Index = 0;m_Index<(16-1);m_Index++)
	{
		m_pCur->Data[m_Index] = m_pCur->Data[m_Index+1];
	}
	m_pCur->Data[15] = m_pCur->Input;

	m_Total = 0;
	m_TotalCoff = 0;
	m_IndexStart = 16 - m_pCur->FilterTime;
	for(m_Coff = 0;m_Coff<(m_pCur->FilterTime);m_Coff++)
	{
		m_Total += (long)m_pCur->Data[m_IndexStart + m_Coff] * (m_Coff +1);
		m_TotalCoff += (m_Coff+1);
	}
	m_pCur->Output = m_Total/m_TotalCoff;
}*/
#if (SOFTSERIES == MD380SOFT)
int Filter_1st(int x, int y0, FILTER_1ST *pFilStr)  
{
    long g1 ;
    long g2 ;
    long tempL ;
    int  out ;

    g1 = (1L << 15) / (1 + 2* pFilStr->taoVsTs) ;     // Q15
    g2 = ((1L - 2* pFilStr->taoVsTs) << 15) / (1 + 2* pFilStr->taoVsTs) ;   // Q15

    tempL = g1 * ((long)x + pFilStr->x0) - g2 * y0 + pFilStr->resFilt;
    out = tempL >> 15 ;
    
    pFilStr->resFilt = tempL - ((long)out << 15) ;
    pFilStr->x0 = x ;
    
    return(out);
}
#endif
/*s16 VFFilter1st(s16 Input,VFFILTER1ST_STRUCT *pFilter1st)		// ʹ��˫���Ա任������ɢ�� Y(s)=X(s)/(tao*s+1)
{  
   s32  mCoff,mLong;
   s16  mOut,mAdd;
	
   if(pFilter1st->FiltimeVsSamptime == 0)
   {
   		return(Input);
   }
   
   mCoff = (1L << 15) / (1L + 2L * pFilter1st->FiltimeVsSamptime); 
   mLong = mCoff * ((s32)Input - (s32)pFilter1st->OutLast);
   mLong = mLong + mCoff * ((s32)pFilter1st->InputLast - (s32)pFilter1st->OutLast); 
   mLong = mLong + pFilter1st->Total;
   mAdd  = (mLong>>15); 

   if(Input == pFilter1st->OutLast)		// �ﵽ��̬�Ĵ���
   {
		pFilter1st->Total = 0;

		return (pFilter1st->OutLast);
   }

   pFilter1st->Total = mLong - ((s32)mAdd<<15);

   mOut = pFilter1st->OutLast + mAdd;

   pFilter1st->OutLast   = mOut;
   pFilter1st->InputLast = Input;

   return(mOut);
}
*/
