#ifndef _COM_PAYMENT_SERVER_H_
#define _COM_PAYMENT_SERVER_H_


#pragma pack(push, 1)



//////////////////////////////////////////////////////////////////////////

#define  MDM_PM_REGIST                  1         //ע����Ϸ

#define  SUB_PM_REGIST                  100       //ע����Ϸ������
#define  SUB_PM_REGIST_RET           101      //ע����Ϸ���������

struct CMD_PM_Regist
{
    WORD wServerID;//��Ϸ������ID
    WORD wServerKindID;//��Ϸ����ID
    char csPlatformID[32];//ƽ̨��ʶ
};

enum RegistResult {

    RR_Mismatch = 101,//
    RR_ServerIDErr = 102,

};

struct CMD_PM_Regist_Result
{
    WORD wRegistResult;
};

//////////////////////////////////////////////////////////////////////////

#define MDM_PM_BESINESS                                  2           //ҵ����Ϣ

#define SUB_PM_PAY_GENORDER                         100        //���󶩵�
#define SUB_PM_PAY_GENORDER_RET                  101        //���󶩵����
#define SUB_PM_PAY_GEN_WEIXINORDER            102        //����΢�ź�̨����
#define SUB_PM_PAY_WEIXINORDER_RET             103        //����΢�ź�̨�������
#define SUB_PM_PAY_SUCCESS                              104        //֧���ɹ�

struct SUB_PM_PAY_SUC
{
	DWORD dwUserID;//���ID
    DWORD dwUserSocketID;//���ID
    WORD wMoneyType;//��������
    DWORD wMoney;//��ֵ�Ľ��
    DWORD wServerID;//��Ϸ������ID
    DWORD wKindID;//��Ϸ����ID
    DWORD wTableID;//����ID
};

#define IP_LEN                     20
#define ATTACH_LEN          128
struct SUB_PM_GEN_WEIXIN_OD
{
    DWORD dwUserID;//���ID
    DWORD dwSocketID;//���ID
    DWORD dwTableID;//���Ӻ�
    WORD wPoductID;//��ƷID
    char cstrIP[IP_LEN];//IP
    char cstrAttach[ATTACH_LEN];//�Զ�������
};

#define NONCE_LEN           64
#define PREPARE_LEN         64
#define ORDER_LEN            64
struct SUB_PM_GEN_WEIXIN_OD_RET
{
    DWORD dwUserID;//���ID
    DWORD dwSocketID;//���ID
    DWORD dwTableID;//���Ӻ�
    WORD wPoductID;
    WORD nonce_str_len;
    WORD prepare_id_len;
    WORD order_len;
    char nonce_str[NONCE_LEN];
    char prepare_id[PREPARE_LEN];
    char order[ORDER_LEN];
};

//////////////////////////////////////////////////////////////////////////


#pragma pack(pop)

#endif
