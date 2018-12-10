//#pragma once
//
//#include "CMD_WZQ.h"
// 
//#include "Game/FV/FvSingleton.h"
//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
////逻辑掩码
//
//#define	MASK_COLOR					0xF0								//花色掩码
//#define	MASK_VALUE					0x0F								//数值掩码
//
////////////////////////////////////////////////////////////////////////////
////动作定义
//
////动作标志
//#define WIK_NULL					0x00								//没有类型
//#define WIK_LEFT					0x01								//左吃类型
//#define WIK_CENTER					0x02								//中吃类型
//#define WIK_RIGHT					0x04								//右吃类型
//#define WIK_PENG					0x08								//碰牌类型
//#define WIK_GANG					0x10								//杠牌类型
//#define WIK_LISTEN					0x20								//吃牌类型
//#define WIK_CHI_HU					0x40								//吃胡类型
//#define WIK_ZI_MO					0x80								//自摸
//
////////////////////////////////////////////////////////////////////////////
////胡牌定义
//
////胡牌
//#define CHK_NULL					0x00										//非胡类型
//#define CHK_CHI_HU				    0x01										//胡类型
//
//#define CHR_DA_DUI_ZI				0x00000001									//大对子		1番
//#define CHR_QING_YI_SE				0x00000002									//清一色		2番
//#define CHR_QI_XIAO_DUI			    0x00000004									//七对		2番
//#define CHR_LONG_QI_DUI			    0x00000008									//龙七对		3番
//#define CHR_QING_DUI				0x00000010									//清对		3番
//#define CHR_DAI_YAO				    0x00000020									//全幺九		3番
//#define CHR_JIANG_DUI				0x00000040									//将对		3番
//
//#define CHR_QING_QI_DUI			    0x00000080									//清七对		4番
//#define CHR_QING_LONG_QI_DUI		0x00000100									//清龙七对	5番
//#define CHR_JIANG_QI_DUI			0x00000200									//将	七对		5番
//#define CHR_TIAN_HU				    0x00000400									//天胡		5番
//#define CHR_DI_HU					0x00000800									//地胡		5番
//#define CHR_QING_YAO_JIU			0x00001000									//清幺九		5番
//
//#define CHR_QIANG_GANG				0x00002000									//抢杠胡		1番
//#define CHR_GANG_SHANG_PAO			0x00004000									//杠上炮		1番
//#define CHR_GANG_KAI				0x00008000									//杠上花		1番
//#define CHR_JIN_GOU_DAO			    0x00010000									//金钩钓		1番
//#define CHR_HAI_DI_HU				0x00020000									//海底捞		1番
//#define CHR_HAI_DI_PAO				0x00040000									//海底炮		1番
//	
//#define CHR_ZI_MO					0x00080000									//自摸		
//#define CHR_FANG_PAO				0x00100000									//放炮	
//#define CHR_PIN_HU			    	0x00200000									//平胡	
//#define CHR_CHA_DA_JIAO			    0x00400000									//查大叫
//
//#define CHR_PENGPENGHU				0x00800000									//碰	
//#define CHR_QINGYISE			    0x01000000									//清	
//#define CHR_QINGPENG			    0x02000000									//清碰
//#define CHR_MENQiNG			        0x04000000									//门清
//
////扑克定义
//#define HEAP_FULL_COUNT				26									//堆立全牌
//
//#define MAX_RIGHT_COUNT				1	
////////////////////////////////////////////////////////////////////////////
//
//
//namespace XZDD
//{
//
//	//类型子项
//	struct tagKindItem
//	{
//		byte							cbWeaveKind;						//组合类型
//		byte							cbCenterCard;						//中心扑克
//		byte							cbCardIndex[3];						//扑克索引
//		byte							cbValidIndex[3];					//实际扑克索引
//	};
//
//	//组合子项
//	struct tagWeaveItem
//	{
//		byte							cbWeaveKind;						//组合类型
//		byte							cbCenterCard;						//中心扑克
//		byte							cbPublicCard;						//公开标志
//		word							wProvideUser;						//供应用户
//	};
//
//	//杠牌结果
//	struct tagGangCardResult
//	{
//		byte							cbCardCount;						//扑克数目
//		byte							cbCardData[4];						//扑克数据
//	};
//
//	//分析子项
//	struct tagAnalyseItem
//	{
//		byte							cbCardEye;							//牌眼扑克
//		bool                            bMagicEye;                          //牌眼是否是王霸
//		byte							cbWeaveKind[4];						//组合类型
//		byte							cbCenterCard[4];					//中心扑克
//		byte                            cbCardData[4][4];                   //实际扑克
//	};
//
//	//////////////////////////////////////////////////////////////////////////
//
//	/*
//	//	权位类。
//	//  注意，在操作仅位时最好只操作单个权位.例如
//	//  CChiHuRight chr;
//	//  chr |= (chr_zi_mo|chr_peng_peng)，这样结果是无定义的。
//	//  只能单个操作:
//	//  chr |= chr_zi_mo;
//	//  chr |= chr_peng_peng;
//	*/
//	class CChiHuRight
//	{	
//		//静态变量
//	private:
//		static bool						m_bInit;
//		static dword					m_dwRightMask[MAX_RIGHT_COUNT];
//
//		//权位变量
//	private:
//		dword							m_dwRight[MAX_RIGHT_COUNT];
//
//	public:
//		//构造函数
//		CChiHuRight();
//
//		//运算符重载
//	public:
//		//赋值符
//		CChiHuRight & operator = ( dword dwRight );
//
//		//与等于
//		CChiHuRight & operator &= ( dword dwRight );
//		//或等于
//		CChiHuRight & operator |= ( dword dwRight );
//
//		//与
//		CChiHuRight operator & ( dword dwRight );
//		CChiHuRight operator & ( dword dwRight ) const;
//
//		//或
//		CChiHuRight operator | ( dword dwRight );
//		CChiHuRight operator | ( dword dwRight ) const;
//
//		//功能函数
//	public:
//		//是否权位为空
//		bool IsEmpty();
//
//		//设置权位为空
//		void SetEmpty();
//
//		//获取权位数值
//		byte GetRightData( dword dwRight[], byte cbMaxCount );
//
//		//设置权位数值
//		bool SetRightData( const dword dwRight[], byte cbRightCount );
//
//	private:
//		//检查权位是否正确
//		bool IsValidRight( dword dwRight );
//	};
//
//
//	//////////////////////////////////////////////////////////////////////////
//
//	//游戏逻辑类
//	class CGameLogic:public FvSingleton<CGameLogic>
//	{
//		//变量定义
//	protected:
//		static const byte				m_cbCardDataArray[MAX_REPERTORY];	//扑克数据
//		byte							m_cbMagicIndex;						//钻牌索引
//
//		//函数定义
//	public:
//		//构造函数
//		CGameLogic();
//		//析构函数
//		virtual ~CGameLogic();
//
//		//控制函数
//	public:
//		//混乱扑克
//		void RandCardData(byte cbCardData[], byte cbMaxCount);
//		//删除扑克
//		bool RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard);
//		//删除扑克
//		bool RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount);
//		//删除扑克
//		bool RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount);
//		//删除扑克
//		byte RemoveValueCardAll(byte cbCardData[], byte cbCardCount,byte cbRemoveCard);
//		//删除扑克
//		bool RemoveValueCardOne(byte cbCardData[], byte cbCardCount,byte cbRemoveCard);
//		//设置钻牌
//		void SetMagicIndex( byte cbMagicIndex ) { m_cbMagicIndex = cbMagicIndex; }
//		//钻牌
//		bool IsMagicCard( byte cbCardData );
//
//		//添加牌
//		bool AddCard(byte cbCardIndex[MAX_INDEX], const byte cbAddCard[], byte cbAddCount);
//
//		// 替换最后cbAddCount 个数据  配合函数：RemoveValueCard 使用
//		bool ReplaceCardData(byte cbCardData[], byte cbCardCount, const byte cbAddCardData[], byte cbAddCount);
//
//		//辅助函数
//	public:
//		//有效判断
//		bool IsValidCard(byte cbCardData);
//		//扑克数目
//		byte GetCardCount(const byte cbCardIndex[MAX_INDEX]);
//		//组合扑克
//		byte GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4]);
//
//		//等级函数
//	public:
//		//动作等级
//		byte GetUserActionRank(byte cbUserAction);
//		//胡牌等级
//		word GetChiHuActionRank(const CChiHuRight & ChiHuRight);
//
//		//动作判断
//	public:
//		//吃牌判断
//		byte EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
//		//碰牌判断
//		byte EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
//		//杠牌判断
//		byte EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard);
//
//		//动作判断
//	public:
//		//杠牌分析
//		byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);
//		//吃胡分析
//		byte AnalyseChiHuCard(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount, byte cbCurrentCard, CChiHuRight &ChiHuRight);
//		//听牌分析
//		byte AnalyseTingCard( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount );
//		//是否听牌
//		bool IsTingCard( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount );
//		//是否花猪
//		bool IsHuaZhu( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount );
//
//
//		//杠牌分析
//		byte AnalyseGangCard(const byte cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult);
//		//转换函数
//	public:
//		//扑克转换
//		byte SwitchToCardData(byte cbCardIndex);
//		//扑克转换
//		byte SwitchToCardIndex(byte cbCardData);
//		//扑克转换
//		byte SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_COUNT]);
//		//扑克转换
//		byte SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX]);
//
//
//		//排序,根据牌值排序
//		bool SortCardList( byte cbCardData[MAX_COUNT], byte cbCardCount );
//
//		//分别获取颜色牌组
//		void GetCardWithColor(byte cbCardData[MAX_COUNT],byte cbCardCount,std::vector<byte>& CardList1,std::vector<byte>& CardList2,std::vector<byte>& CardList3);
//		//胡法分析
//	protected:
//		//大对子
//		bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
//		//清一色牌
//		bool IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const byte cbItemCount,const byte cbCurrentCard);
//		//七小对牌
//		bool IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  byte cbWeaveCount,const byte cbCurrentCard);
//		//带幺
//		bool IsDaiYao( const tagAnalyseItem *pAnalyseItem );
//		//将对
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
