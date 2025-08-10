/***********************************Inovance***********************************
����������Function Description��:�����������ļ�������λ�÷������ٶȷ����Ĵ���
    1) M������
    2) T������
    3) ��ת��ѹ������
����޸����ڣ�Date����
�޸���־��History��:�����¼�¼Ϊ��һ��ת���Ժ󣬿�ʼ��¼��
	���� 		ʱ�� 		����˵��
1 	xx 		xxxxx 		xxxxxxx
2 	yy 		yyyyy 		yyyyyyy
************************************Inovance***********************************/

/* Includes ------------------------------------------------------------------*/
#include "MotorInclude.h"
#include "MotorEncoder.h"

/* Private variables ---------------------------------------------------------*/
PG_DATA_STRUCT			gPGData;
ROTOR_SPEED_STRUCT		gRotorSpeed;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
//void    ResetPGType(void);
void    ResetSpeedGetReg(void);
void    GetFeedRotorSpeed(void);
void    GetFeedSpeedQEP(void);
s32     CalFeedSpeed(s16 DetaPos, u32 DetaTime);
void    QEPGetRealPos(void);              
void    ReadRTPos(void);
void    CalRTSpeed(void);
extern  u16 GetRotorTransPos();
/****************************************************************
	ͨ���˲�����(Long��ʽ)
	LastOne     ��һ���˲��������
	Input       ���Ĳ�������
	FilterTime  �˲�ʱ�䣨ms��λ��
�㷨��OutPut = LastOne + (Input - LastOne) * Coff
      Coff�Ǳ���С��32��ֵ��
*****************************************************************/
s32 FilterL(s32 LastOne, s32 Input, s32 Coff)
{
	s32   m_Deta,m_Add;
	
	m_Deta = Input - LastOne;
	m_Add = ((s64)m_Deta * (s64)Coff)>>15;
	if((m_Add == 0) && (m_Deta != 0))
	{
		m_Add = (m_Deta > 0)?1:-1;
	}
	return (LastOne + m_Add);	
}

/*******************************************************************************
* Function Name  : ��ȡQEP�������������ֵ
* Description    :  ��д�����׿���1918��
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
u16 GetQepTimLat(void)
{
	return((*EQepRegs).QCTMRLAT);
}

/*******************************************************************************
* Function Name  : ��ȡQEP�����������������ֵ
* Description    :  ��д�����׿���1918��
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
u16 GetQepTimPrdLat(void)
{
	return((*EQepRegs).QCPRDLAT);
}
/*******************************************************************************
* Function Name  : ��ʼ�������õļĴ���
* Description    : 
* Input          : SetpTime ����������Ϊ�����Ĳ��ټ������2ms�����ϵͳʱ��100MHz������Ϊ200000
                   MaxQepCnt QEP���������ֵ��32λ������Ϊ(1UL<<32),16λ������Ϊ(1UL<<16)
* Output         : None
* Return         : None
*******************************************************************************/
void ResetSpeedGetReg(void)
{
    gRotorSpeed.Pos              = GetQepCnt();
    gRotorSpeed.Time             = GetTime();
    gRotorSpeed.DetaPos          = 0;
	gRotorSpeed.CapTimeLast      = 0;			//�Բ���Ԫǰһ������ֵ���г�ʼ��
    gRotorSpeed.DetaTimeAdd      = C_TIME_4MS;
    gRotorSpeed.SpeedFeed        = 0;
    gRotorSpeed.PrescaFlag       = 0;
    gRotorSpeed.ChangeFlag       = 0;
}

/*******************************************************************************
* Function Name  : �������ٶȷ������������ֲ�ͬ����������
* Description    : 
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
void GetFeedRotorSpeed(void)
{
    s32 m_tempLong,m_Data,m_SpeedFeed;

    if(gPGData.PGType == PG_TYPE_RESOLVER)
    {	    
	    // ģ��һ��Z�źţ� ���ڴ��ر�ʶ������������: �������ĸ��Ƕȣ���ǰ�����Ƕ�, ͬʱ����4ms ���ʱ��
	    if(gMainStatus.RunStep == STATUS_GET_PAR)
	    {        
	        if(((int)gRotorTrans.RtorBuffer > 0) &&
	            ((int)gRotorTrans.SimuZBack <= 0) &&
	            ((int)gRotorTrans.SimuZBack2 <= 0) &&
	            (gIPMZero.zFilterCnt == 4))
	        {
				gIPMPos.ZSigNumSet++;
			    gIPMZero.zFilterCnt = 0;
	        }
	        gRotorTrans.SimuZBack2 = gRotorTrans.SimuZBack;
	        gRotorTrans.SimuZBack = gRotorTrans.RtorBuffer;
	    } 
	       
	    if(gVCPar.VCSpeedFilter <= 10)    // �����˲�ʱ��Ƚϳ�
		{
		    m_Data =  163840L / (20 + 3*gVCPar.VCSpeedFilter);  
		}
		else
		{
		    m_Data =  163840L / (40 + gVCPar.VCSpeedFilter);
		}	
		gAsr.KPLowCoff = 10;	                                     	
    }
    else
    {    	
    	GetFeedSpeedQEP();                                /*ABZ���ٳ���*/
        m_Data =  163840L / (10 + 3*gVCPar.VCSpeedFilter);
		gAsr.KPLowCoff = 15;
    } 
	gRotorSpeed.SpeedFeedFilter = FilterL(gRotorSpeed.SpeedFeedFilter,gRotorSpeed.SpeedFeed,m_Data);  
    m_SpeedFeed = gRotorSpeed.SpeedFeedFilter>>9;
	gRotorSpeed.SpeedFeedQ12 = (s16)__IQsat(m_SpeedFeed,32767,-32767);
	m_tempLong = (long)gRotorSpeed.SpeedFeedQ12 * gRotorSpeed.TransRatio;
    m_SpeedFeed= (llong)m_tempLong * 16777 >> 24;
	gRotorSpeed.SpeedEncoder = (s16)__IQsat(m_SpeedFeed,32767,-32767); 	  
}
/*******************************************************************************
* Function Name  : ����ʽ���٣�������������������׼ȷʱ��
* Description    : ��Ȼ���ڵ�����:
                    1)�޷�����4�������׼ȷʱ��
                    2)�����£������������޷���׽�����壬��μ����ٶ�
                    3)����ʱ�̺��ٶȱջ�����Чʱ����ʱ����Ҫ�����ٶȱ仯
* Input          : 
* Output         : gRotorSpeed.MSpeed
* Return         : None
*******************************************************************************/
void GetFeedSpeedQEP(void)
{
    s32 m_Speed;
    u32 m_QEPCnt;              //��ǰλ��
    u32 m_Time;                //��ǰʱ��
    u16 m_CaptureTime;         //����Ԫ������������ֵ
	u16 m_CapturePeriod;       //����Ԫ����ֵ
	u16 m_Cnt;	

    m_Cnt = 0;
    do
    {
        DINT;
        m_QEPCnt        = GetQepCnt();
        m_Time          = GetTime();
        m_CaptureTime   = GetQepTimLat(); 
        m_CapturePeriod = GetQepTimPrdLat();
        EINT;
        if((m_Cnt++) > 4)   break;                          /*������ѭ��*/
    }while((0 == m_CaptureTime) || (m_CaptureTime == m_CapturePeriod));

    gRotorSpeed.DetaPos   = (s16)(m_QEPCnt - gRotorSpeed.Pos); 
    gRotorSpeed.DetaTime  = ((gRotorSpeed.Time - m_Time) & 0xFFFFFFUL); 
    gRotorSpeed.DetaTime += gRotorSpeed.DetaTimeAdd;

    /*���ټ���*/
    if(gRotorSpeed.DetaPos != 0) 
    {
        if(gRotorSpeed.DetaTimeAdd == 0)
        {
	        gRotorSpeed.DetaTime = gRotorSpeed.DetaTime + ((u32)gRotorSpeed.CapTimeLast<<3) - ((u32)m_CaptureTime<<3);
        }
        gRotorSpeed.DetaTimeAdd = 0;
        gRotorSpeed.Pos         = m_QEPCnt;
        gRotorSpeed.Time        = m_Time;
        gRotorSpeed.CapTimeLast = m_CaptureTime;            //�����ϴεĲ���ֵ
        gRotorSpeed.PulseNum    = gPGData.PulseNum;
        m_Speed = CalFeedSpeed(gRotorSpeed.DetaPos,gRotorSpeed.DetaTime);/*�����ٶȼ��㺯�������ٶ�*/
    }
    else if(gRotorSpeed.DetaTime >= C_TIME_4MS)		        //���ٵĴ���                                                                  
    {
        gRotorSpeed.DetaTimeAdd     = C_TIME_4MS;
        gRotorSpeed.Pos             = m_QEPCnt;
        gRotorSpeed.Time            = m_Time;
        gRotorSpeed.CapTimeLast     = 0;
        m_Speed = 0;
    }
    else
    {
        m_Speed = gRotorSpeed.SpeedFeedFilter;   // �������û�м�⵽���壬������һ�ĵ��ٶ�
    } 
    gRotorSpeed.SpeedFeed = m_Speed;
}

/*******************************************************************************
* Function Name  : ͨ��λ��ƫ���ʱ��ƫ������ٶȣ�����ٶȼ��㺯�����ڱ����������䣬���� ���ٶȼ��㣬��������һ�����������ٶȵ�
* Description    : ʹ��4��ȫ�ֱ���:
                    gMotorExtInfo.Poles    ���������
                    gPGData.PulseNum    ����������
                    gBasePar.FullFreq   32768��Ӧ�ĵ�?0.1Hz��λ)
                    gSysClock           ϵͳʱ�ӣ�(1MHz��λ)
* Input          : DetaTime ʱ����ڼ�����DetaPos QEP������(4��Ƶ���ֵ)
* Output         : Ƶ�ʣ�����ֵs32��ʽ���ᳬ��32767��Ӧó���ʶ�?
* Return         : None
*******************************************************************************/
s32 CalFeedSpeed(s16 DetaPos, u32 DetaTime)//Q24
{
    s16 m_DetaPos;
    s32 m_Speed;
    u32 m_DetaTime;
    u32 m_Long,m_Long1,m_Long2;
    u64 m_Llong;
    //����ʵ�ʿ�����γ��������
    m_DetaPos  = abs(DetaPos);
    m_DetaTime = DetaTime;
    
	m_Long1 = (u32)gMotorExtInfo.Poles * (u32)m_DetaPos;//�����������Ǽ����˵���������ĽǶȱ仯��
	m_Long2 = (25000000UL * (u32)DSP_CLOCK)/m_DetaTime;//��������������һ��Ƶ�ʵ�λHZ//25000000UL ������ֵ����ɼ�AD2S1205�����ֲ�,����SPI�����Ƶ��Ϊ25MHZ(��λ�úͽ��ٶ��ö��������ݱ�ʾ������ͨ��һ��12λ���нӿڻ���һ�����ʱ������Ϊ25 MHz�����ߴ��нӿ�����ȡ)
	m_Llong = ((u64)m_Long1 * (u64)m_Long2);
    if(m_Llong > ((u64)1 << (63 - 24)))
    {
        m_Llong = ((u64)1 << 63);
    }
    else
    {
        m_Llong = m_Llong<<24;                              /*�����ٶ��޸�ΪQ24��ʽ*/
    }
	m_Long  = gRotorSpeed.PulseNum * (u32)gBasePar.FullFreq01;//gBasePar.FullFreq01Ϊ0.01Hz��λ

	m_Speed = (s32)(m_Llong/m_Long);
    m_Speed = Min(m_Speed,(1UL<<24));
	if(DetaPos < 0)
	{
		m_Speed = -m_Speed;
	}
    return m_Speed;

}

/*******************************************************************************
* Function Name  : ��������������λ��
* Description    : ͬ����������gPGData.RefPos��gPGData.RotorPos���ϵ硢���������λ�ý�
                   ��г�����������
                   ͬ����������gPGData.RotorPos�ڻ�׼Z�źŵ���������
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void QEPGetRealPos(void)              
{
    u32 m_QepCnt;
    s32 m_s32;
	s16	m_DetaQep,m_Add;
    static s16 m_Remain = 0;
    static s16 m_RemainAbs = 0;	

	m_QepCnt             = GetQepCnt();									//��ȡQEP����ֵ
    gIPMPos.QepCntPosCal = m_QepCnt;
	m_DetaQep       = (s16)(m_QepCnt - gIPMPos.QepBak);
	m_s32 = (((s32)m_DetaQep<<14) * gMotorExtInfo.Poles + m_RemainAbs);
    m_Add = m_s32/gPGData.PulseNum;       	
	gIPMPos.RotorPos += m_Add; 	
	gIPMPos.QepBak  = m_QepCnt;
	m_RemainAbs = m_s32 - ((s32)m_Add * (s32)gPGData.PulseNum);/*����*/ 
		
    // �������λ��gPGData.PgPos����λ���ڻ�׼Z�źŴﵽ�󲻻����,�����ٶȻ��Ͷ�ABZ�����������ɳ�
    m_s32 = (((s32)m_DetaQep<<14) * gMotorExtInfo.Poles + m_Remain);/*��Ϊ�������*/
    m_Add = m_s32/gPGData.PulseNum; 
    gPGData.RefPos += ((s32)m_Add<<8);
	//gPGData.RefPosLast = gPGData.RefPos;                                  /*Q24��ʽ*/                           
    m_Remain = m_s32 - ((s32)m_Add * (s32)gPGData.PulseNum);/*����*/
    // �������λ��gPGData.PgPosAbs����λ���ڻ�׼Z�źŴﵽ������¸�λ������ͬ������ת�Ӵų�����
    	                          	
    gIPMPos.RotorPos += gIPMPos.ZDetaPos;
    gIPMPos.ZDetaPos  = 0;
}
/*************************************************************
	��ת��ѹ����ȡλ�ú�����ͨ��SPI��ȡλ���ź�,�����ж���ִ��
ע��:Ҫ���������������伫�����������������伫����С�ڵ��ڵ��������
*************************************************************/
void ReadRTPos(void)
{
	s16     m_DetaPos;
    u16     m_Pos;
    s32     m_s32,m_Mod;
	u32     m_DetaTime;

 	gRotorTrans.ReadPos = GetRotorTransPos();                  // ��ȡSPI����(����λ��)�������ȡ����ֻ������Ļ�е�����ľ���λ�ã�û�м������������Ļ��㣬������������ļ�����
    RT_SAMPLE_START; //SAMPLE�øߣ�׼����ȡ��һ������
                                         // Ϊ��һ�λ�ȡλ������SAMPLE�ź���Ч
   	m_Pos = gRotorTrans.ReadPos;                                           
	m_DetaPos           = (s16)(m_Pos - gRotorTrans.PosBak);//����᲻���и�BUG��������Ƕ���360λ�õ����ұ任����ű������Ĵ���ʽ
	gRotorTrans.PosBak  = m_Pos;       	
    m_s32 = ((s32)m_DetaPos * gMotorExtInfo.Poles + gRotorTrans.Remain);//gRotorTrans.Remain��һ������������С�����֣��������кܶ������Ĵ����൱���ϴε�С����������ÿ�ζ��ۼ�����һ�Ρ�
    m_Mod = (s16)(m_s32/gRotorTrans.Poles);//m_Mod ��ʵ���Ǽ�������ۼ����ϴμ�������������С����Ļ�е�Ƕ�һ�������ڵı仯��ֵ
	gRotorTrans.PosComp   = m_Mod;//PosComp  ���ڲ�����
    gRotorTrans.AddPos   += (m_Mod<<8);                                       // �ۼӵ�Ƕ�(�ٶȻ���)��Q24��ʽ   ��0λ�õ�ʱ�򲻸�λ�����ٶȻ���ʱ����
    gRotorTrans.Remain    = m_s32 - (m_Mod * gRotorTrans.Poles);                  // ���� 

    // �����е�Ƕ�ת��Ϊ��Ƕȴ���(ͬ����ֱ��ʹ�õĽǶ�)��ͬʱ�����˲�����.
    gRotorTrans.RTPos = ((u32)m_Pos * (u32)gMotorExtInfo.Poles)/gRotorTrans.Poles;   //��ʶ�ż���ʼλ��ʱʹ��  ���������/���伫����
 
    gRotorTrans.SampleTime = GetTime();    //��ȡ��ǰʱ��
    m_DetaTime = abs((s32)(gRotorTrans.TimeBak - gRotorTrans.SampleTime));	
		
    if(m_DetaTime >= C_TIME_045MS)//0.45ms��ʱ�����һ���ٶ�
    {   
        gRotorTrans.TimeBak  = gRotorTrans.SampleTime; 
        gRotorTrans.DetaTime = m_DetaTime;
    	m_DetaPos = (s16)(m_Pos - gRotorTrans.PosRTBak);  
        gRotorTrans.PosRTBak = m_Pos;   //�����ٶȼ���
        gRotorTrans.DetaPos  = (m_DetaPos>>2);//�����ٶȼ���                              // Ϊ��ƥ��CalFeedSpeed�������ã�������ʧ���ȡ�
        CalRTSpeed();
    }
}

/*************************************************************
	��ת��ѹ������£������ٶȺ���,���֧��32�Լ������䡣
*************************************************************/
void CalRTSpeed(void)//��ʵ����
{
    s32     m_Speed;
    s16     m_DetaPos;
    u32     m_DetaTime;
    
    DINT;
    m_DetaTime = gRotorTrans.DetaTime;
    m_DetaPos  = gRotorTrans.DetaPos;
    EINT;
    // ���㷴��Ƶ�ʣ������ٶ�
    gRotorSpeed.PulseNum = (u32)gRotorTrans.Poles<<12;                         // ����̶�Ϊ4096��   gRotorTrans.PolesΪ���伫����������12λ��������4096�����伫��������4096����λ������ʵ��תһȦ���������ı�������������1�Լ�����תһȦ��λ4096�����壬������ʵ�����ñ�����������ȥ����������
    m_Speed              = CalFeedSpeed(m_DetaPos,m_DetaTime);
    gRotorSpeed.SpeedFeed = m_Speed;     //�������������ٶȾ���һ����Ƕȶ�Ӧ���ٶ�   Q24��ʽ
}

