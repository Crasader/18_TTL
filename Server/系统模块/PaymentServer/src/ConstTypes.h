#ifndef _CONST_TYPES_H_
#define _CONST_TYPES_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef char TCHAR, *PTCHAR;
typedef unsigned long DWORD;
typedef double DATE;

//////////////////////////////////////////////////////////////////////////

#define  pConect boost::shared_ptr<tcp_con::tcp_connection>
#define  ConectMap std::map<unsigned long, boost::shared_ptr<tcp_con::tcp_connection>>
#define  ConectPair std::pair<unsigned long, boost::shared_ptr<tcp_con::tcp_connection>>
#define  ConItrator ConectMap::iterator 

#define ProductPair std::pair<int, Product>
#define ProductsMap std::map<int, Product>

#define WKPR std::pair<PlatformID, payment::PaymentWorker*>
#define WKMP std::map <PlatformID, payment::PaymentWorker*>

//////////////////////////////////////////////////////////////////////////

//ÿһ��socket��Ϣ�����������
#define MAX_PACKAGE_DEQUE 16
//ÿ����Ϣ����󳤶�
//#define MAX_PACKAGE_BUFFER_SIZE 65536
//���������
#define HEART_TICK_TIME 5000
//����curlURL��buffer����
#define UrlBufferLength 1024*1024

enum PlatformID
{
    PD_NULL,
    PD_WeiXin,//΢��֧��
    PD_CongCong, //�ϴ�ƽ̨֧��
    PD_GanSu, //�¸�ƽ̨֧��



    PD_Count,
};

enum IAPPurchaseResultType
{
    IAPPRT_Unknown = 0,
    IAPPRT_Ok,                      // ֧���ɹ�
    IAPPRT_DatabaseFailed,          // д�����ݿ�ʧ��
    IAPPRT_NotPurchased,            // ֧��ʧ�ܣ����������ҷ�����
    IAPPRT_AlreadyPurchased,        // �Ѿ�֧�����ˣ���֧�����ݿ����Ѿ���TransactionID������
    IAPPRT_Processing,              // �����У�����Ƿ������Ǳ߻�û�������
    IAPPRT_NoOrderSerial,           // û�ж���
};

enum Produt_Type
{
	PT_Gold = 0,//��ֵ���
	PT_RoomCard,//����
};

#endif // 