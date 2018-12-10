//#pragma once
//
//#include "CMD_WZQ.h"
// 
//#include "Game/FV/FvSingleton.h"
//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
////�߼�����
//
//#define	MASK_COLOR					0xF0								//��ɫ����
//#define	MASK_VALUE					0x0F								//��ֵ����
//
////////////////////////////////////////////////////////////////////////////
////��������
//
////������־
//#define WIK_NULL					0x00								//û������
//#define WIK_LEFT					0x01								//�������
//#define WIK_CENTER					0x02								//�г�����
//#define WIK_RIGHT					0x04								//�ҳ�����
//#define WIK_PENG					0x08								//��������
//#define WIK_GANG					0x10								//��������
//#define WIK_LISTEN					0x20								//��������
//#define WIK_CHI_HU					0x40								//�Ժ�����
//#define WIK_ZI_MO					0x80								//����
//
////////////////////////////////////////////////////////////////////////////
////���ƶ���
//
////����
//#define CHK_NULL					0x00										//�Ǻ�����
//#define CHK_CHI_HU				    0x01										//������
//
//#define CHR_DA_DUI_ZI				0x00000001									//�����		1��
//#define CHR_QING_YI_SE				0x00000002									//��һɫ		2��
//#define CHR_QI_XIAO_DUI			    0x00000004									//�߶�		2��
//#define CHR_LONG_QI_DUI			    0x00000008									//���߶�		3��
//#define CHR_QING_DUI				0x00000010									//���		3��
//#define CHR_DAI_YAO				    0x00000020									//ȫ�۾�		3��
//#define CHR_JIANG_DUI				0x00000040									//����		3��
//
//#define CHR_QING_QI_DUI			    0x00000080									//���߶�		4��
//#define CHR_QING_LONG_QI_DUI		0x00000100									//�����߶�	5��
//#define CHR_JIANG_QI_DUI			0x00000200									//��	�߶�		5��
//#define CHR_TIAN_HU				    0x00000400									//���		5��
//#define CHR_DI_HU					0x00000800									//�غ�		5��
//#define CHR_QING_YAO_JIU			0x00001000									//���۾�		5��
//
//#define CHR_QIANG_GANG				0x00002000									//���ܺ�		1��
//#define CHR_GANG_SHANG_PAO			0x00004000									//������		1��
//#define CHR_GANG_KAI				0x00008000									//���ϻ�		1��
//#define CHR_JIN_GOU_DAO			    0x00010000									//�𹳵�		1��
//#define CHR_HAI_DI_HU				0x00020000									//������		1��
//#define CHR_HAI_DI_PAO				0x00040000									//������		1��
//	
//#define CHR_ZI_MO					0x00080000									//����		
//#define CHR_FANG_PAO				0x00100000									//����	
//#define CHR_PIN_HU			    	0x00200000									//ƽ��	
//#define CHR_CHA_DA_JIAO			    0x00400000									//����
//
//#define CHR_PENGPENGHU				0x00800000									//��	
//#define CHR_QINGYISE			    0x01000000									//��	
//#define CHR_QINGPENG			    0x02000000									//����
//#define CHR_MENQiNG			        0x04000000									//����
//
////�˿˶���
//#define HEAP_FULL_COUNT				26									//����ȫ��
//
//#define MAX_RIGHT_COUNT				1	
////////////////////////////////////////////////////////////////////////////
//
//
//namespace XZDD
//{
//
//	//��������
//	struct tagKindItem
//	{
//		byte							cbWeaveKind;						//�������
//		byte							cbCenterCard;						//�����˿�
//		byte							cbCardIndex[3];						//�˿�����
//		byte							cbValidIndex[3];					//ʵ���˿�����
//	};
//
//	//�������
//	struct tagWeaveItem
//	{
//		byte							cbWeaveKind;						//�������
//		byte							cbCenterCard;						//�����˿�
//		byte							cbPublicCard;						//������־
//		word							wProvideUser;						//��Ӧ�û�
//	};
//
//	//���ƽ��
//	struct tagGangCardResult
//	{
//		byte							cbCardCount;						//�˿���Ŀ
//		byte							cbCardData[4];						//�˿�����
//	};
//
//	//��������
//	struct tagAnalyseItem
//	{
//		byte							cbCardEye;							//�����˿�
//		bool                            bMagicEye;                          //�����Ƿ�������
//		byte							cbWeaveKind[4];						//�������
//		byte							cbCenterCard[4];					//�����˿�
//		byte                            cbCardData[4][4];                   //ʵ���˿�
//	};
//
//	//////////////////////////////////////////////////////////////////////////
//
//	/*
//	//	Ȩλ�ࡣ
//	//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
//	//  CChiHuRight chr;
//	//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
//	//  ֻ�ܵ�������:
//	//  chr |= chr_zi_mo;
//	//  chr |= chr_peng_peng;
//	*/
//	class CChiHuRight
//	{	
//		//��̬����
//	private:
//		static bool						m_bInit;
//		static dword					m_dwRightMask[MAX_RIGHT_COUNT];
//
//		//Ȩλ����
//	private:
//		dword							m_dwRight[MAX_RIGHT_COUNT];
//
//	public:
//		//���캯��
//		CChiHuRight();
//
//		//���������
//	public:
//		//��ֵ��
//		CChiHuRight & operator = ( dword dwRight );
//
//		//�����
//		CChiHuRight & operator &= ( dword dwRight );
//		//�����
//		CChiHuRight & operator |= ( dword dwRight );
//
//		//��
//		CChiHuRight operator & ( dword dwRight );
//		CChiHuRight operator & ( dword dwRight ) const;
//
//		//��
//		CChiHuRight operator | ( dword dwRight );
//		CChiHuRight operator | ( dword dwRight ) const;
//
//		//���ܺ���
//	public:
//		//�Ƿ�ȨλΪ��
//		bool IsEmpty();
//
//		//����ȨλΪ��
//		void SetEmpty();
//
//		//��ȡȨλ��ֵ
//		byte GetRightData( dword dwRight[], byte cbMaxCount );
//
//		//����Ȩλ��ֵ
//		bool SetRightData( const dword dwRight[], byte cbRightCount );
//
//	private:
//		//���Ȩλ�Ƿ���ȷ
//		bool IsValidRight( dword dwRight );
//	};
//
//
//	//////////////////////////////////////////////////////////////////////////
//
//	//��Ϸ�߼���
//	class CGameLogic:public FvSingleton<CGameLogic>
//	{
//		//��������
//	protected:
//		static const byte				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����
//		byte							m_cbMagicIndex;						//��������
//
//		//��������
//	public:
//		//���캯��
//		CGameLogic();
//		//��������
//		virtual ~CGameLogic();
//
//		//���ƺ���
//	public:
//		//�����˿�
//		void RandCardData(byte cbCardData[], byte cbMaxCount);
//		//ɾ���˿�
//		bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
//		//ɾ���˿�
//		bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);
//		//ɾ���˿�
//		bool RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount);
//		//ɾ���˿�
//		byte RemoveValueCardAll(byte cbCardData[], byte cbCardCount,byte cbRemoveCard);
//		//ɾ���˿�
//		bool RemoveValueCardOne(byte cbCardData[], byte cbCardCount,byte cbRemoveCard);
//		//��������
//		void SetMagicIndex( byte cbMagicIndex ) { m_cbMagicIndex = cbMagicIndex; }
//		//����
//		bool IsMagicCard( byte cbCardData );
//
//		//�����
//		bool AddCard(byte cbCardIndex[MAX_INDEX], const byte cbAddCard[], byte cbAddCount);
//
//		// �滻���cbAddCount ������  ��Ϻ�����RemoveValueCard ʹ��
//		bool ReplaceCardData(byte cbCardData[], byte cbCardCount, const byte cbAddCardData[], byte cbAddCount);
//
//		//��������
//	public:
//		//��Ч�ж�
//		bool IsValidCard(byte cbCardData);
//		//�˿���Ŀ
//		byte GetCardCount(const byte cbCardIndex[MAX_INDEX]);
//		//����˿�
//		byte GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4]);
//
//		//�ȼ�����
//	public:
//		//�����ȼ�
//		byte GetUserActionRank(byte cbUserAction);
//		//���Ƶȼ�
//		word GetChiHuActionRank(const CChiHuRight & ChiHuRight);
//
//		//�����ж�
//	public:
//		//�����ж�
//		byte EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
//		//�����ж�
//		byte EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
//		//�����ж�
//		byte EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
//
//		//�����ж�
//	public:
//		//���Ʒ���
//		byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);
//		//�Ժ�����
//		byte AnalyseChiHuCard(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount, byte cbCurrentCard, CChiHuRight &ChiHuRight);
//		//���Ʒ���
//		byte AnalyseTingCard( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount );
//		//�Ƿ�����
//		bool IsTingCard( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount );
//		//�Ƿ���
//		bool IsHuaZhu( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount );
//
//
//		//���Ʒ���
//		byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);
//		//ת������
//	public:
//		//�˿�ת��
//		byte SwitchToCardData(byte cbCardIndex);
//		//�˿�ת��
//		byte SwitchToCardIndex(byte cbCardData);
//		//�˿�ת��
//		byte SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_COUNT]);
//		//�˿�ת��
//		byte SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX]);
//
//
//		//����,������ֵ����
//		bool SortCardList( byte cbCardData[MAX_COUNT], byte cbCardCount );
//
//		//�ֱ��ȡ��ɫ����
//		void GetCardWithColor(byte cbCardData[MAX_COUNT],byte cbCardCount,std::vector<byte>& CardList1,std::vector<byte>& CardList2,std::vector<byte>& CardList3);
//		//��������
//	protected:
//		//�����
//		bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
//		//��һɫ��
//		bool IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const byte cbItemCount,const byte cbCurrentCard);
//		//��С����
//		bool IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  byte cbWeaveCount,const byte cbCurrentCard);
//		//����
//		bool IsDaiYao( const tagAnalyseItem *pAnalyseItem );
//		//����
//		bool IsJiangDui( const tagAnalyseItem *pAnalyseItem );
//	
//	public:
//		byte GetCardColor(byte cbCardDat);
//		byte GetCardValue(byte cbCardDat);
//
//		void GetCardInfoList(byte* pHandCardData, int nHandCardCount,std::vector<Card_Info>& kCardInfoList);
//	};
//}
//
