//======================================================================
//
// Time-stamp: <2011-2-21 Lei.Min, 0656>
//
// P2P comm
// ��Ե�ͨѶ���ݴ���
//
//======================================================================

#ifndef __F_P2P_H__
#define __F_P2P_H__

#include "f_canlink.h"


#define MSC_RCV_TORQUE_SET      0   // ת�ظ���
#define MSC_P2P_RCV_FRQ_SET     1   // Ƶ�ʸ���

#if DEBUG_F_MSC_CTRL

extern CanLinkDataCfgType  gCANlinkDataCFg[CANLINK_CFG_LEN];       // CANlink���ݴ�������

extern Uint16 CANlinkRunMode;
extern Uint16 CanInitFlag;

#define MAX_SLAVE_NODE_NUM        5                    // ���ӿ��ƴ�վ����ӽڵ���
// �̶������ӻ���ַ
#define P2P_MSRSTER__ADDR         20       // �̶�������ַ
#define P2P_BAUD                  6        // �̶�������
// ���ø��ɷ��䣬��Ҫ�޸����º�
#define P2P_STATER_SRC_ADDR       0x730c   // Դ�׵�ַ
#define P2P_STATER_DES_ADDR       0x730c   // Ŀ���׵�ַ
#define P2P_TRANSINTERVAL_TIME    5        // ����ʱ�� ��λ ms 
#define P2P_REG_LENGHT            1        // �Ĵ������� ���4��
// �������ݸ��ӻ����ݴ�ŵ�ַ
#define SLAVE_RCV_STATUS          funcCode.code.u3[12]  // 0x730c  ����״̬
#define SLAVE_RCV_FRQ             funcCode.code.u3[13]  // 0x730d  ����Ƶ��
#define SLAVE_RCV_TORQUE          funcCode.code.u3[14]  // 0x730e  ����ת��
#define SLAVE_RCV_DIR             funcCode.code.u3[15]  // 0x730f  ��������

typedef struct
{
    Uint16 MSCEnable;               // ���ӿ����Ƿ���Ч
    Uint16 MSCEnablePre;            // ǰһ�����ӿ����Ƿ���Ч    
    Uint16 isMaster;                // �Ƿ�Ϊ����
    Uint16 isMasterPre;             // ǰһ���Ƿ�Ϊ����
    // ��������
    Uint16 MasterStatus;            // ����״̬
    Uint16 MasterFrq;               // ����Ƶ��
    Uint16 MasterTorque;            // ����ת��
    // �ӻ���������
    Uint16 SlaveRcvStatus;          // ͨѶ��������
    Uint16 SlaveRcvFrq;             // �������ݽ������Դ����
    Uint16 SlaveRcvTorque;          // ͨѶ��������
    // ���ͽ��ճ����жϼ�ʱ
    Uint16 MasterSendPeriod;        // ���ӿ��Ʒ��ͼ�ʱ
    Uint16 mscCommErrTime;          // ��Ե�ͨѶ�쳣��ʱ     _ms
    Uint16 MasterSendPeriodPre;     // ǰһ�����ӿ��Ʒ�������   *_ms
    Uint16 mscCommErrTimePre;       // ǰһ�����ӿ�������ʱ��   *_ms
    Uint16 slaveNodeIds[4];         // ���ӿ��ƴӻ���ַ�б�
    Uint16 slaveNodeNums;           // ���ӿ��ƴӻ��ڵ���
    // ������ַ
    Uint16 masterAddress;           // ������ַ
    Uint16 masterAddressPre;        // ǰһ��������ַ
    Uint16 p2pRunDir;               // ���ӷ���
    // ���ӻ����桢������Ϣ����
    Uint16 slaveFollowMasterCmd;    // �ӻ������������� 0: no  1:yes
    Uint16 slaveErrorSendToMaster;  // �ӻ����ϴ�����   0: no  1:yes
    Uint16 masterDispSlaveOut;      // ������ʾ�ӻ����� 0: no  1:yes

} MASTER_SALVE_CONTROL_STRUCT;

// ���ӿ���
extern MASTER_SALVE_CONTROL_STRUCT MasterSlaveCtrl;
void MasterSlaveDeal(void);

#elif 1
#endif
#endif



