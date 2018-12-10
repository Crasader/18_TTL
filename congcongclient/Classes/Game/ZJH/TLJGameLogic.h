#pragma once
#include "TLJHeaders.h"
#include "CMD_TLJ.h"

using namespace TLJ;

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//花色类型
#define TYPE_BLOCK					0x00								//方块
#define TYPE_PLUM					0x10								//梅花
#define TYPE_HEART					0x20								//红桃
#define TYPE_SPADE					0x30								//黑桃

//扑克类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对子类型
#define	CT_SHUN_ZI					3									//顺子类型
#define CT_JIN_HUA					4									//金花类型
#define	CT_SHUN_JIN					5									//顺金类型
#define	CT_BAO_ZI					6									//豹子类型
#define CT_SPECIAL					7									//特殊类型

namespace TLJ
{
	class TLJGameLogic: public FvSingleton<TLJGameLogic>
	{		
		//变量定义
	private:
		static byte						m_cbCardListData[52];				//扑克定义
		//函数定义
	public:
		//构造函数
		TLJGameLogic();
		//析构函数
		virtual ~TLJGameLogic();
	public:
		//获取数值
		byte GetCardValue(byte cbCardData);
		//获取花色
		byte GetCardColor(byte cbCardData);
		//获取显示资源名
		std::string GetShowCardValue(byte cbCardData);
		//有效判断
		bool IsValidCard(byte cbCardData);
	public:
		word switchToScore(byte cbLevel);
		byte switchToLevel(word wScore);

		//计算投注分数
		dword GetBettingScore(byte cbScoreLevel,bool bMingPai, word wTimes, dword dwBaseScore);
	};
}

