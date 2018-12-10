#include "TLJGameLogic.h"
#include "TLJPlayer.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////

FV_SINGLETON_STORAGE(TLJ::TLJGameLogic);
namespace TLJ
{
	//扑克数据
	byte TLJGameLogic::m_cbCardListData[52]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D	//黑桃 A - K
	};
	//构造函数
	TLJGameLogic::TLJGameLogic()
	{
	}

	//析构函数
	TLJGameLogic::~TLJGameLogic()
	{
	}
	//获取数值
	byte TLJGameLogic::GetCardValue(byte cbCardData){ 
		ASSERT(IsValidCard(cbCardData));
		return cbCardData&LOGIC_MASK_VALUE;
	}
	//获取花色
	byte TLJGameLogic::GetCardColor(byte cbCardData){
		ASSERT(IsValidCard(cbCardData));
		return cbCardData&LOGIC_MASK_COLOR; 
	}
	//获取显示资源名
	std::string TLJGameLogic::GetShowCardValue(byte cbCardData)
	{
		ASSERT(IsValidCard(cbCardData));
		byte cbCardType = GetCardColor(cbCardData);
		byte cbCardValue = GetCardValue(cbCardData);
		std::string name = "";
		if(cbCardType == TYPE_BLOCK){
			name = "Block";
		}else if(cbCardType == TYPE_PLUM){
			name = "Plum";
		}else if(cbCardType == TYPE_HEART){
			name = "Heart";
		}else if(cbCardType == TYPE_SPADE){
			name = "Spade";
		}
		name = utility::toString(name,"_",(int)cbCardValue);
		return name;
	}
	//有效判断
	bool TLJGameLogic::IsValidCard(byte cbCardData)
	{
		if (cbCardData<=0)
		{
			return false;
		}
		if (cbCardData>0x0D&&cbCardData<0x11)
		{
			return false;
		}
		if (cbCardData>0x1D&&cbCardData<0x21)
		{
			return false;
		}
		if (cbCardData>0x2D&&cbCardData<0x31)
		{
			return false;
		}
		if (cbCardData>0x3D)
		{
			return false;
		}
		return true;
	}

	word TLJGameLogic::switchToScore(byte cbLevel)
	{
		word wScore = 0;
		switch (cbLevel)
		{
		case 1:
			wScore = SCORE_CELL_ONE;
			break;
		case 2:
			wScore = SCORE_CELL_TWO;
			break;
		case 3:
			wScore = SCORE_CELL_THREE;
			break;
		case 4:
			wScore = SCORE_CELL_FOUR;
			break;
		default:
			break;
		}
		return wScore;
	}

	byte TLJGameLogic::switchToLevel(word wScore)
	{
		byte cbLevel = 0;
		switch (wScore)
		{
		case SCORE_CELL_ONE:
			cbLevel = 1;
			break;
		case SCORE_CELL_TWO:
			cbLevel = 2;
			break;
		case SCORE_CELL_THREE:
			cbLevel = 3;
			break;
		case SCORE_CELL_FOUR:
			cbLevel = 4;
			break;
		default:
			break;
		}
		return cbLevel;
	}

	//计算投注分数
	dword TLJGameLogic::GetBettingScore(byte cbScoreLevel, bool bMingPai, word wTimes, dword dwBaseScore)
	{
		dword lScore = 0;
		lScore = dwBaseScore * cbScoreLevel;
		if (bMingPai) {
			lScore *= wTimes;
		}
		return lScore;
	}
}

//////////////////////////////////////////////////////////////////////////
