/*************** (C) COPYRIGHT 2010  Inovance Technology Co., Ltd****************
* File Name          : f_canlink.h
* Author             : Yanyi	
* Version            : V0.0.1
* Date               : 08/25/2010
* Description        : CANlink������
					  ����31�������Զ�Ӧ������
					  ����30~16��������������
					  15~0��������������


********************************************************************************/

#ifndef	__f_canlink__
#define	__f_canlink__


// CANlinkִ������
#define     CANlink_DEAL_PERIOD     2                       // CANlink�������� ms��λ
// ������ض���
#define     CANLINK_CFG_LEN        16                       // ֧�����ó���
#define     CANLINK_RECDATA_LEN     8                       // ���յ�Զ�����֡���ó���
#define     CANlINK_SYNC_W_LEN      8                       // ͬ��д����

#define     CANLINK_DVC_INFO_LEN    12                      // �豸���ӱ�ǩ����

// ��ⳬʱ����
#define     CANLINK_MONI_TIMEOUT    25                      // CANlink�������ʱ���ʣ�1λС��
#define     CANLINK_REQ_CFG_TIME    2000                    // CANlink��������ʱ��
#define     MSC_REQ_CFG_TIME        500                     // ���ӿ��ƴӻ���������ʱ��

// CANlink����ģʽ
#define     CANLINK_SAFE_MODE       0                       // CANlink��ʼ��ȫģʽ
#define     CANLINK_CFG_MODE        1                       // CANlink����ģʽ
#define     CANLINK_RUN_MODE        2                       // CANlink����ģʽ

// ��ʼ��״̬
#define     CANLINK_INIT_ENABLE      0                      // CANlink��ʼ��׼����
#define     CANLINK_INIT_SUCC       0xcc                    // ��ʼ���ɹ�
#define     CANL_BUS_STOP           (CANLINK_INIT_SUCC-1)   // CANlinkֹͣ


// ֡���Ͷ���
#define		CAN_REMOTE_FRAME		0xd						// Զ��֡		1101
#define		CAN_MANAG_FRAME		    0x7						// ����֡       0111
#define     CAN_COMM_FRAME          0x8                     // �������֡	1000
#define		CAN_CONFIG_FRAME		0xA						// ����֡		1010
#define		CAN_PLC_DATA_FRAME		0xB						// PLC��ʹ������֡		1011
#define     CANLINK_DATA_FRAME      0xC                     // CANlink����֡ 1100       
#define     CANLINK_BEAT_FRAME      0xE                     // CANlink����֡ 1110

// Ӧ��/���ݴ����ʶ
#define     CANlink_ASK             1                       // CAN��֡֡/��Զ�
#define     CANlink_ACK             0                       // Ӧ��֡/��Ե�

// �շ�״̬��־
#define		CANLINK_RT_SUCC			0						// ������/���ɹ�
#define		CANLINK_RT_BUSY			1						// ����æ
#define		CANLINK_RT_TIMEOUT		2						// ��ʱ
#define		CANLINK_R_EMPTY			3						// δ���յ�����
#define		CANLINK_R_OVER			4						// �������������

// ID�����α�ʶ
#define		CANLINK_ID_MASK			0x3f					// ID���α�ʶ

// ����������
#define		TRAN_MBOX_NUM			5						// ����������
#define		REC_MBOX_NUM			10						// ����������
#define          REC_MAX            7                       //����ÿ2ms���ڴ������

// ���������
#define		AAM_MBOX_N				31						// �Զ�Ӧ������ţ��Զ�Ӧ������������ȼ����
#define		REC_BOX_N				30						// ���������
#define		TRAN_BOX_N				15						// ���������

// ����֡����
#define		CANLINK_STARTUP_NODE	1						// CANlink�����ڵ�
#define		CANLINK_STOP_NODE		2						// ֹͣ�ڵ�
#define		CANLINK_ADDR_TEST		3						// ��ַ���
#define		CANLINK_SYNC_BROAD		4						// ͬ���㲥
#define		CANLINK_REQ_CFG			5						// ��������
#define     CANLINK_CLOSE           6                       // �ر�CANlink

#define     CANlink_SYNC_ENABLE     0xcc                    // ͬ���㲥��Ч
#define     CANlink_SYNC_DISABLE    0x0                     // ͬ���㲥��Ч


// ����֡����
#define		CANLINK_DEL_CFG			1						// ɾ��CANlink������Ϣ
#define		CANLINK_ADD_CFG			2						// �����豸CANlink������Ϣ
#define		CANLINK_R_CFG			3						// ������
#define		CANLINK_REC_CFG		    0x10					// ����֡��������
#define		CANLINK_DATA_REG		0x80					// ����֡�������� ���λ"1"
#define     CANLINK_CFG_ERROR       0x7F                    // ����֡����
// �����쳣����
#define		CANLINK_NO_ERROR		0						// �޴���
#define		CANLINK_CODE_ERROR		1						// �������
#define		CANLINK_INDEX_ERROR		2						// ������������
#define		CANLINK_CFGDATA_ERROR	3						// ������Ϣ����
#define     CANLINK_CFGLEN_ERROR    5                       // ������Ϣ���ȳ���

// ����֡����
#define		CAN_LINK_DEL			1						// ɾ��CANlinkK������Ϣ
#define		CAN_LINK_INC			2						// �����豸CANlink������Ϣ
#define		CAN_LINK_R_CFG			3						// ������
#define		CAN_LINK_R_REG			4						// ���Ĵ���
#define		CAN_LINK_W_REG			5						// д�Ĵ���
#define		CAN_LINK_R_INFO			6						// ��վ���豸��Ϣ
#define		CAN_LINK_R_WAR			7						// ���澯��Ϣ
#define     CAN_LINK_R_32Bit        0x14                    // ��32λ�Ĵ���
#define     CAN_LINK_W_32Bit        0x15                    // д32λ�Ĵ���
#define     CAN_LINK_SYNC_W         0x21                    // CANlinkͬ��д�Ĵ���
#define     CAN_LINK_SYNC_W_EN      0x22                    // CANlinkͬ��д��Ч

#define		CAN_LINK_W_EEP			0x0A					// дEEPROM
#define     CAN_LINK_ERROR          0xff                    // ���������Ӧ
// �����쳣����
//#define		CANLINK_NO_ERROR		0						// �޴���
#define     CANLINK_FUN_ERROR       1                       // ����ܲ�����
#define		CANLINK_ADDR_ERROR		2						// ��ַ����
#define		CANLINK_DATA_ERROR		3						// ���ݴ���
#define		CANLINK_FUN_DIS_ERROR	4						// ���ܽ�ֹ����
#define     CANLINK_DATALEN_ERROR   5                       // ����������Ч

#define     CANLINK_DATA32_ERROR    9                       // 32λд����

// ����֡����
#define     CANLINK_MASTER_BEAT     1                       // ���������
#define     CANLINK_SLAVE_BEAT      2                       // �ڵ�����

// PLC��������CANlink�޹�
#define		CAN_TRAN_TAB_CFG		0x0B					// ���ͱ�����
#define		CAN_REC_TAB_CFG			0x0C					// ���ձ�����
#define		CAN_FUN_U3_CFG			0x0D					// U3�Զ��幦��������
#define		CAN_FUN_C0_CFG			0x0E					// C0�Զ��幦��������
#define		CAN_READ_PLC_INFO		0x0F					// ��PLC���豸��Ϣ


#define		CAN_LINK_S_WAR			0x10					// ���͸澯��Ϣ
#define		CAN_LINK_Q_CFG			0x20					// ������������

#define     SERVO_DRIVER            0

// �����붨��
#define		CAN_LINK_S_ADDR			(funcCode.code.commSlaveAddress & CANLINK_ID_MASK) // ��վ��ַ
#define		CAN_LINK_BAUD_SEL		(funcCode.code.commBaudRate/1000)    // ����������



// CANlink������������

struct CANlink_32DATA                                       // CANlink 32λ�������÷�ʽ
{
	Uint32 qByte1;
	Uint32 qByte2;	
};

struct CANlink_16DATA                                       // CANlink 16λ�������÷�ʽ
{
	Uint16 dByte2;
	Uint16 dByte1;
	Uint16 dByte4;
	Uint16 dByte3;	    
};

struct DSP_CAN_8DATA                                        // CANlink 8λ�������÷�ʽ
{
	Uint16 byte4:8;
	Uint16 byte3:8;
	Uint16 byte2:8;
	Uint16 byte1:8;	    
	Uint16 byte8:8;
	Uint16 byte7:8;
	Uint16 byte6:8;
	Uint16 byte5:8;
    
};

// CANlink �����ݽṹ����
typedef union CAN_LINK_DATA 
{
    struct CANlink_32DATA qByte;
    struct CANlink_16DATA dByte;
    struct DSP_CAN_8DATA   byte;
} CANlinkDATA;

struct	CANLINK_MsgID_BITS	                        // ���ʽ ���ݷ�ʽ
{
	Uint16	srcSta:8;									// Դվ��ID/�Ĵ�����ַ���ֽ�
	Uint16	destSta:8;									// Ŀ��վID		                8
	Uint16	code:8;										// �������/�Ĵ�����ַ���ֽ�	16
	Uint16	aq:1;										// �ʴ��־/���ݴ�������		24
	Uint16	framf:4;									// ֡���ͱ�ʶ	25
//    	Uint16	aam:1;										// �Զ�Ӧ��λ	29
//    	Uint16	ame:1;										// ����ʹ��λ	30
//    	Uint16	ide:1;										// 				31
};

// CANLINK��ϢIDλ����
typedef union CANLINK_MsgID
{
    Uint32 all;
    struct	CANLINK_MsgID_BITS bit;
} CANlinkMsgID;


// CANlinkK������������
typedef struct CANLINK_DATA_BUF
{
	CANlinkMsgID msgid;                                     // CAN֡ID
    CANlinkDATA data;                                       // CANlink��������
    Uint16 len;
}CANlinkDataBuf;

// CANlink���ջ������ݽṹ����
struct CANLINK_REC_BUF
{
	Uint16 bufFull;											// ������Ч��ʶλ��bit0 "1" buf[0]������Ч
	CANlinkDataBuf buf[REC_MBOX_NUM];
};

// CANlink���ݴ������ýṹ��
typedef struct CANLINK_DATA_CFG                         // ע����������Ľṹ�仯
{                                                          
	Uint16 srcRegAddr;                                      // Դ�Ĵ����׵�ַ
	Uint16 regLen:8;                                        // �贫��Ĵ�������
    Uint16 cfgAddr:8;       							    // ����Ŀ���ַ������
	Uint16 transInterval;                                   // ������ ��msΪ��λ��0 ��ʾ�¼�����
	Uint16 disRegAddr;                                      // Ŀ��Ĵ����׵�ַ
} CanLinkDataCfgType;


// ͬ��д�Ĵ�����������
struct CANLINK_SYNC_WREG                                // ͬ��д�Ĵ������ݽṹ
{                                                          
	Uint16 index;                                           // ���� "0"��ʾ��Ч 1~CANlINK_SYNC_W_LEN
	Uint16 dWord;                                           // 32λ��ʶ��bitλ��ʾ32λ��16λ
	Uint16 addr[CANlINK_SYNC_W_LEN];                        // ��ַ����
    Uint16 data[CANlINK_SYNC_W_LEN];       					// �ȴ�д������
};

// �ⲿʹ�ñ���
extern struct CANLINK_REC_BUF	CanlinkRecBuf;				// ���ջ��� �ɽ�������������

// �����ⲿ����
extern Uint16 CanNodeRunStat;                               // �ڵ�����״̬
extern Uint16 gDeviceInformation[CANLINK_DVC_INFO_LEN];     // �豸���ӱ�ǩ


// �ⲿʹ�ú�������
extern void CanlinkFun(void);
extern Uint16 WriteMultiRegTest(Uint16 addr, Uint16 *data, Uint16 len, Uint16 daTest);
extern void WriteMultiRegFun(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom);
extern Uint16 WriteMultiReg(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom);
extern Uint16 ReadMultiReg(Uint16 addr, Uint16 *dataPi, Uint16 len);
/*
extern Uint16 CanControlWriter(Uint16 addr, Uint16 data, Uint16 eeprom);
extern Uint16 WriteMultiReg(Uint16 addr, Uint16 *data, Uint16 len, Uint16 eeprom);
extern Uint16 ReadMultiReg(Uint16 addr, Uint16 *dataPi, Uint16 len);
extern Uint16 ReadSingleReg(Uint16 addr, Uint16 *data);
extern Uint16 WriteRegFunTest(Uint16 addr, Uint16 data, Uint16 daTest);
extern Uint16 WriteRegFun(Uint16 addr, Uint16 data, Uint16 eeprom);
*/
extern Uint16 WriteSingleReg(Uint16 addr, Uint16 data, Uint16 eeprom);
extern Uint16 CanControlRead(Uint16 addr, Uint16* result);
extern Uint16 CanlinkDataTran(CANlinkDataBuf *dataPi, Uint16 len, Uint16 timeOut);
extern Uint16 P2bFilte(Uint32 msgid);

#endif


