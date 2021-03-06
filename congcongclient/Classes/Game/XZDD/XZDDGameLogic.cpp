#include "XZDDGameLogic.h"

//////////////////////////////////////////////////////////////////////////
FV_SINGLETON_STORAGE(XZDD::CGameLogic);
namespace XZDD
{
	//静态变量
	bool		CChiHuRight::m_bInit = false;
	dword		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

	//构造函数
	CChiHuRight::CChiHuRight()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );

		if( !m_bInit )
		{
			m_bInit = true;
			for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
			{
				if( 0 == i )
					m_dwRightMask[i] = 0;
				else
					m_dwRightMask[i] = (dword(pow((float)2,(float)(i-1))))<<28;
			}
		}
	}

	//赋值符重载
	CChiHuRight & CChiHuRight::operator = ( dword dwRight )
	{
		dword dwOtherRight = 0;
		//验证权位
		if( !IsValidRight( dwRight ) )
		{
			//验证取反权位
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			dwRight = ~dwRight;
			dwOtherRight = MASK_CHI_HU_RIGHT;
		}

		for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
			else m_dwRight[i] = dwOtherRight;
		}

		return *this;
	}

	//与等于
	CChiHuRight & CChiHuRight::operator &= ( dword dwRight )
	{
		bool bNavigate = false;
		//验证权位
		if( !IsValidRight( dwRight ) )
		{
			//验证取反权位
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			//调整权位
			dword dwHeadRight = (~dwRight)&0xF0000000;
			dword dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
			dwRight = dwHeadRight|dwTailRight;
			bNavigate = true;
		}

		for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			{
				m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
			}
			else if( !bNavigate )
				m_dwRight[i] = 0;
		}

		return *this;
	}

	//或等于
	CChiHuRight & CChiHuRight::operator |= ( dword dwRight )
	{
		//验证权位
		if( !IsValidRight( dwRight ) ) return *this;

		for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
		}

		return *this;
	}

	//与
	CChiHuRight CChiHuRight::operator & ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//与
	CChiHuRight CChiHuRight::operator & ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//或
	CChiHuRight CChiHuRight::operator | ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//或
	CChiHuRight CChiHuRight::operator | ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//是否权位为空
	bool CChiHuRight::IsEmpty()
	{
		for( byte i = 0; i < CountArray(m_dwRight); i++ )
			if( m_dwRight[i] ) return false;
		return true;
	}

	//设置权位为空
	void CChiHuRight::SetEmpty()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );
		return;
	}

	//获取权位数值
	byte CChiHuRight::GetRightData( dword dwRight[], byte cbMaxCount )
	{
		ASSERT( cbMaxCount >= CountArray(m_dwRight) );
		if( cbMaxCount < CountArray(m_dwRight) ) return 0;

		memcpy( dwRight,m_dwRight,sizeof(dword)*CountArray(m_dwRight) );
		return CountArray(m_dwRight);
	}

	//设置权位数值
	bool CChiHuRight::SetRightData( const dword dwRight[], byte cbRightCount )
	{
		ASSERT( cbRightCount <= CountArray(m_dwRight) );
		if( cbRightCount > CountArray(m_dwRight) ) return false;

		zeromemory( m_dwRight,sizeof(m_dwRight) );
		memcpy( m_dwRight,dwRight,sizeof(dword)*cbRightCount );

		return true;
	}

	//检查仅位是否正确
	bool CChiHuRight::IsValidRight( dword dwRight )
	{
		dword dwRightHead = dwRight & 0xF0000000;
		for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
			if( m_dwRightMask[i] == dwRightHead ) return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	//静态变量

	//扑克数据
	const byte CGameLogic::m_cbCardDataArray[MAX_REPERTORY_XZDD]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	};

	//////////////////////////////////////////////////////////////////////////

	//构造函数
	CGameLogic::CGameLogic()
	{
		m_cbMagicIndex = MAX_INDEX;
		
	}

	//析构函数
	CGameLogic::~CGameLogic()
	{
	}

	//混乱扑克
	void CGameLogic::RandCardData(byte cbCardData[], byte cbMaxCount)
	{
		//混乱准备
		byte cbCardDataTemp[CountArray(m_cbCardDataArray)];
		memcpy(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

		//混乱扑克
		byte cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=rand()%(cbMaxCount-cbRandCount);
			cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);

		return;
	}

	//删除扑克
	bool CGameLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

		//删除扑克
		byte cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
		if (cbCardIndex[cbRemoveIndex]>0)
		{
			cbCardIndex[cbRemoveIndex]--;
			return true;
		}

		//失败效验
		ASSERT(FALSE);

		return false;
	}

	//删除扑克
	bool CGameLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount)
	{
		//删除扑克
		for (byte i=0;i<cbRemoveCount;i++)
		{
			//效验扑克
			ASSERT(IsValidCard(cbRemoveCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

			//删除扑克
			byte cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
			if (cbCardIndex[cbRemoveIndex]==0)
			{
				//错误断言
				ASSERT(FALSE);

				//还原删除
				for (byte j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbRemoveCard[j]));
					cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
				}

				return false;
			}
			else 
			{
				//删除扑克
				--cbCardIndex[cbRemoveIndex];
			}
		}

		return true;
	}

	byte CGameLogic::RemoveValueCardAll(byte cbCardData[], byte cbCardCount,byte cbRemoveCard)
	{
		byte cbCardIndex[MAX_INDEX];			//手中扑克
		SwitchToCardIndex(cbCardData,cbCardCount,cbCardIndex);
		byte cbRemoveCardArray[MAX_INDEX];
		byte cbRemoveCout = cbCardIndex[SwitchToCardIndex(cbRemoveCard)];
		for (int i = 0;i<cbRemoveCout;i++)
		{
			cbRemoveCardArray[i] = cbRemoveCard;
		}
		RemoveValueCard(cbCardData,cbCardCount,cbRemoveCardArray,cbRemoveCout);
		return cbCardCount - cbRemoveCout;
	}
	bool CGameLogic::RemoveValueCardOne(byte cbCardData[], byte cbCardCount,byte cbRemoveCard)
	{
		byte cbRemoveCardArray[MAX_INDEX];
		cbRemoveCardArray[0] = cbRemoveCard;
		return RemoveValueCard(cbCardData,cbCardCount,cbRemoveCardArray,1);
	}
	//删除扑克
	bool CGameLogic::RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount)
	{
		//检验数据
		ASSERT(cbCardCount<=14);
		ASSERT(cbRemoveCount<=cbCardCount);

		//定义变量
		byte cbDeleteCount=0,cbTempCardData[14];
		if (cbCardCount>CountArray(cbTempCardData))
			return false;
		memcpy(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

		//置零扑克
		for (byte i=0;i<cbRemoveCount;i++)
		{
			for (byte j=0;j<cbCardCount;j++)
			{
				if (cbRemoveCard[i]==cbTempCardData[j])
				{
					cbDeleteCount++;
					cbTempCardData[j]=0;
					break;
				}
			}
		}

		//成功判断
		if (cbDeleteCount!=cbRemoveCount) 
		{
			//ASSERT(FALSE);
			return false;
		}

		//清理扑克
		byte cbCardPos=0;
		for (byte i=0;i<cbCardCount;i++)
		{
			if (cbTempCardData[i]!=0) 
				cbCardData[cbCardPos++]=cbTempCardData[i];
		}

		return true;
	}

	bool CGameLogic::AddCard(byte cbCardIndex[MAX_INDEX], const byte cbAddCard[], byte cbAddCount)
	{

		//添加扑克
		for (byte i=0;i<cbAddCount;i++)
		{
			//效验扑克
			ASSERT(IsValidCard(cbAddCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbAddCard[i])]<4);

			//删除扑克
			byte cbAddIndex=SwitchToCardIndex(cbAddCard[i]);
			if (cbCardIndex[cbAddIndex]<4)
			{
				//删除扑克
				++cbCardIndex[cbAddIndex];
			}
			else 
			{
				//错误断言
				ASSERT(FALSE);
				//还原添加
				for (byte j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbAddCard[j]));
					cbCardIndex[SwitchToCardIndex(cbAddCard[j])]--;
				}
				return false;
			}
		}
		return true;
	}

	bool CGameLogic::ReplaceCardData(byte cbCardData[], byte cbCardCount, const byte cbAddCardData[], byte cbAddCount)
	{
		byte cbCardPos=cbCardCount-cbAddCount;
		for (byte i=0;i<cbAddCount;i++)
		{
			cbCardData[cbCardPos]=cbAddCardData[i];
			cbCardPos++;
		}
		return true;
	}

	//有效判断
	bool CGameLogic::IsValidCard(byte cbCardData)
	{
		byte cbValue=(cbCardData&MASK_VALUE);
		byte cbColor=(cbCardData&MASK_COLOR)>>4;
		return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
	}

	//扑克数目
	byte CGameLogic::GetCardCount(const byte cbCardIndex[MAX_INDEX])
	{
		//数目统计
		byte cbCardCount=0;
		for (byte i=0;i<MAX_INDEX;i++) 
			cbCardCount+=cbCardIndex[i];

		return cbCardCount;
	}

	//获取组合
	byte CGameLogic::GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4])
	{
		//组合扑克
		switch (cbWeaveKind)
		{
		case WIK_LEFT:		//上牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard+1;
				cbCardBuffer[2]=cbCenterCard+2;

				return 3;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard-2;

				return 3;
			}
		case WIK_CENTER:	//上牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard+1;

				return 3;
			}
		case WIK_PENG:		//碰牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;

				return 3;
			}
		case WIK_GANG:		//杠牌操作
			{
				//设置变量
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;
				cbCardBuffer[3]=cbCenterCard;

				return 4;
			}
		default:
			{
				ASSERT(FALSE);
			}
		}

		return 0;
	}

	//动作等级
	byte CGameLogic::GetUserActionRank(byte cbUserAction)
	{
		//胡牌等级
		if (cbUserAction&WIK_CHI_HU) { return 4; }

		//杠牌等级
		if (cbUserAction&WIK_GANG) { return 3; }

		//碰牌等级
		if (cbUserAction&WIK_PENG) { return 2; }

		//上牌等级
		if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

		return 0;
	}

	//胡牌等级
	word CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
	{
		word wFanShu = 0;
		return wFanShu;
	}

	//吃牌判断
	byte CGameLogic::EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
	{
		//参数效验
		ASSERT(IsValidCard(cbCurrentCard));

		//过滤判断
		if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//变量定义
		byte cbExcursion[3]={0,1,2};
		byte cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

		//吃牌判断
		byte cbEatKind=0,cbFirstIndex=0;
		byte cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
		for (byte i=0;i<CountArray(cbItemKind);i++)
		{
			byte cbValueIndex=cbCurrentIndex%9;
			if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
			{
				//吃牌判断
				cbFirstIndex=cbCurrentIndex-cbExcursion[i];

				//吃牌不能包含有王霸
				if( m_cbMagicIndex != MAX_INDEX &&
					m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

				if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
					continue;

				//设置类型
				cbEatKind|=cbItemKind[i];
			}
		}

		return cbEatKind;
	}

	//碰牌判断
	byte CGameLogic::EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
	{
		//参数效验
		ASSERT(IsValidCard(cbCurrentCard));

		//过滤判断
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//碰牌判断
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
	}

	//杠牌判断
	byte CGameLogic::EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
	{
		//参数效验
		ASSERT(IsValidCard(cbCurrentCard));

		//过滤判断
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//杠牌判断
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
	}

	//杠牌分析
	byte CGameLogic::AnalyseGangCard(const  byte cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult,byte cbColor )
	{
		//设置变量
		byte cbActionMask=WIK_NULL;
		zeromemory(&GangCardResult,sizeof(GangCardResult));

		//手上杠牌
		for (byte i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]==4 && GetCardColor(SwitchToCardData(i)) != cbColor)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[0]=SwitchToCardData(i);
				GangCardResult.cbCardData[1]=SwitchToCardData(i);
				GangCardResult.cbCardData[2]=SwitchToCardData(i);
				GangCardResult.cbCardData[3]=SwitchToCardData(i);
				GangCardResult.cbCardCount++;
			}
		}

		//组合杠牌
		for (byte i=0;i<MAX_WEAVE;i++)
		{
			if (WeaveItem[i].cbWeaveKind==WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1 && GetCardColor(WeaveItem[i].cbCenterCard)!= cbColor)
				{
					cbActionMask|=WIK_GANG;
					GangCardResult.cbCardData[0]=WeaveItem[i].cbCenterCard;
					GangCardResult.cbCardCount++;
				}
			}
		}

		return cbActionMask;
	}


	//扑克转换
	byte CGameLogic::SwitchToCardData(byte cbCardIndex)//数值转换
	{
		ASSERT(cbCardIndex<34);
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	}

	//扑克转换
	byte CGameLogic::SwitchToCardIndex(byte cbCardData) //排序牌
	{
		ASSERT(IsValidCard(cbCardData));
		return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
	}

	//扑克转换
	byte CGameLogic::SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_COUNT])
	{
		//转换扑克
		byte cbPosition=0;
		//钻牌
		if( m_cbMagicIndex != MAX_INDEX )
		{
			for( byte i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
				cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
		}
		for (byte i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]!=0)
			{
				for (byte j=0;j<cbCardIndex[i];j++)
				{
					ASSERT(cbPosition<MAX_COUNT);
					cbCardData[cbPosition++]=SwitchToCardData(i);
				}
			}
		}

		return cbPosition;
	}

	//扑克转换
	byte CGameLogic::SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX])
	{
		//设置变量
		zeromemory(cbCardIndex,sizeof(byte)*MAX_INDEX);

		//转换扑克
		for (byte i=0;i<cbCardCount;i++)
		{
			ASSERT(IsValidCard(cbCardData[i]));
			cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
		}

		return cbCardCount;
	}

	//钻牌
	bool CGameLogic::IsMagicCard( byte cbCardData )
	{
		if( m_cbMagicIndex != MAX_INDEX )
			return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
		return false;
	}

	//排序,根据牌值排序
	bool CGameLogic::SortCardList( byte cbCardData[MAX_COUNT], byte cbCardCount )
	{
		//数目过虑
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

		//排序操作
		bool bSorted=true;
		byte cbSwitchData=0,cbLast=cbCardCount-1;
		do
		{
			bSorted=true;
			for (byte i=0;i<cbLast;i++)
			{
				if (cbCardData[i]>cbCardData[i+1])
				{
					//设置标志
					bSorted=false;

					//扑克数据
					cbSwitchData=cbCardData[i];
					cbCardData[i]=cbCardData[i+1];
					cbCardData[i+1]=cbSwitchData;
				}	
			}
			cbLast--;
		} while(bSorted==false);

		return true;
	}

	void CGameLogic::GetCardWithColor(byte cbCardData[MAX_COUNT],byte cbCardCount,std::vector<byte>& CardList1,std::vector<byte>& CardList2,std::vector<byte>& CardList3)
	{
		//数目过虑
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return;
		for(int i=0;i<cbCardCount;i++)
		{
			byte cbColor=(((byte)cbCardData[i]&MASK_COLOR)>>4)+1;
			if (cbColor== 1 )
			{
				CardList1.push_back(cbCardData[i]);
			}
			else if (cbColor == 2 )
			{
				CardList2.push_back(cbCardData[i]);
			}
			else if (cbColor == 3 )
			{
				CardList3.push_back(cbCardData[i]);
			}
		}
	}

	/*
	// 胡法分析函数
	*/

	//大对子
	bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
	{
		for( byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
				return false;
		}
		return true;
	}

	//清一色牌
	bool CGameLogic::IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const byte cbItemCount,const byte cbCurrentCard)
	{
		//胡牌判断
		byte cbCardColor=0xFF;

		for (byte i=0;i<MAX_INDEX;i++)
		{
			if(i==m_cbMagicIndex) continue;
			if (cbCardIndex[i]!=0)
			{
				//花色判断
				if (cbCardColor!=0xFF)
					return false;

				//设置花色
				cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

				//设置索引
				i=(i/9+1)*9-1;
			}
		}

		//如果手上只有王霸
		if( cbCardColor == 0xFF )
		{
			ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0 );
			//检查组合
			ASSERT( cbItemCount > 0 );
			cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
		}

		if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

		//组合判断
		for (byte i=0;i<cbItemCount;i++)
		{
			byte cbCenterCard=WeaveItem[i].cbCenterCard;
			if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
		}

		return true;
	}

	//七小对牌
	bool CGameLogic::IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const byte cbWeaveCount,const byte cbCurrentCard,int& nGenCount)
	{
		//组合判断
		if (cbWeaveCount!=0) return false;

		//单牌数目
		byte cbReplaceCount = 0;
		nGenCount = 0;
		//临时数据
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//插入数据
		byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[cbCurrentIndex]++;

		//计算单牌
		for (byte i=0;i<MAX_INDEX;i++)
		{
			byte cbCardCount=cbCardIndexTemp[i];

			//单牌统计
			if (cbCardCount == 1 || cbCardCount == 3) 	/*cbReplaceCount++;*/
			{
				return false;
			}

			if (cbCardCount == 4)
			{
				nGenCount++;
			}
		}

		//王牌不够
		if( m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
			m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0 )
			return false;

		return true;

	}

	//带幺
	bool CGameLogic::IsDaiYao( const tagAnalyseItem *pAnalyseItem )
	{
		//检查牌眼
		byte cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
		if( cbCardValue != 1 && cbCardValue != 9 ) return false;

		for( byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			{
				byte j = 0;
				for(; j < 3; j++ )
				{
					cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
					if( cbCardValue == 1 || cbCardValue == 9 ) break;
				}
				if( j == 3 ) return false;
			}
			else
			{
				cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
				if( cbCardValue != 1 && cbCardValue != 9 ) return false;
			}
		}
		return true;
	}

	//将对
	bool CGameLogic::IsJiangDui( const tagAnalyseItem *pAnalyseItem )
	{
		//是否大对子
		if( !IsPengPeng(pAnalyseItem) ) return false;

		//检查牌眼
		byte cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
		if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;

		for( byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			{
				byte j = 0;
				for(; j < 3; j++ )
				{
					cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
					if( cbCardValue == 2 || cbCardValue == 5 || cbCardValue == 8 ) break;
				}
				if( j == 3 ) return false;
			}
			else
			{
				cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
				if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;
			}
		}
		return true;
	}
	bool CGameLogic::isJiangQiDui(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const byte cbWeaveCount,const byte cbCurrentCard)
	{
		int nGenCount = 0;
		if (!IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,nGenCount))
		{
			return false;
		}

		//单牌数目
		byte cbReplaceCount = 0;

		//临时数据
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//插入数据
		byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[cbCurrentIndex]++;

		//计算单牌
		for (byte i=0;i<MAX_INDEX;i++)
		{
			byte cbCardCount=cbCardIndexTemp[i];

			//王牌过滤
			if( i == m_cbMagicIndex ) continue;
			byte cbCardValue = GetCardValue(SwitchToCardData(i)) ;

			//单牌统计
			if( cbCardValue !=2 && cbCardValue != 5 && cbCardValue != 8)
				return false;
		}
		return true;
	}

	//是否花猪
	bool CGameLogic::IsHuaZhu( const byte cbCardIndex[], const tagWeaveItem WeaveItem[], byte cbWeaveCount )
	{
		byte cbColor[3] = { 0,0,0 };
		for( byte i = 0; i < MAX_INDEX; i++ )
		{
			if( cbCardIndex[i] > 0 )
			{
				byte cbCardColor = SwitchToCardData(i)&MASK_COLOR;
				cbColor[cbCardColor>>4]++;

				i = (i/9+1)*9-1;
			}
		}
		for( byte i = 0; i < cbWeaveCount; i++ )
		{
			byte cbCardColor = WeaveItem[i].cbCenterCard&MASK_COLOR;
			cbColor[cbCardColor>>4]++;
		}
		//缺一门就不是花猪
		for( byte i = 0; i < CountArray(cbColor); i++ )
			if( cbColor[i] == 0 ) return false;

		return true;
	}

	byte CGameLogic::GetCardColor(byte cbCardDat)
	{
		ASSERT(IsValidCard(cbCardDat));
		return ((cbCardDat&MASK_COLOR)>>4)+1;
	}

	byte CGameLogic::GetCardValue(byte cbCardDat)
	{
		ASSERT(IsValidCard(cbCardDat));
		return (cbCardDat&MASK_VALUE);
	}
	void CGameLogic::GetCardInfoList(byte* pHandCardData, int nHandCardCount,std::vector<Card_Info>& kCardInfoList )
	{
		for (int m=1;m<=CARD_COLOR_TIAO;m++)
		{	
			Card_Info kInfo;
			kInfo.nColor = m;
			kInfo.nLen = 0;
			for(int i=0;i<nHandCardCount;i++)
			{
				byte cbColor= GetCardColor(pHandCardData[i]);
				if (cbColor== kInfo.nColor )
				{
					kInfo.kCardList.push_back(pHandCardData[i]);
					kInfo.nLen++;
				}
			}
			kCardInfoList.push_back(kInfo);
		}
		std::sort(kCardInfoList.begin(),kCardInfoList.end());
	}
	//吃胡分析
	byte CGameLogic::AnalyseChiHuCard(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount, byte cbCurrentCard, CChiHuRight &ChiHuRight)
	{
		//变量定义
		byte cbChiHuKind=WIK_NULL;
		CAnalyseItemArray.clear();

		ChiHuRight.SetEmpty();

		//构造扑克
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//cbCurrentCard一定不为0			!!!!!!!!!
		ASSERT( cbCurrentCard != 0 );
		if( cbCurrentCard == 0 ) return WIK_NULL;

		
			//	特殊番型
			
		int nGenCount = 0;
		//七小对牌
		if( IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard,nGenCount) ) 
		{
			if (nGenCount > 0)		//判断龙七对					
			{
				ChiHuRight |= CHR_LONG_QI_DUI;
			}
			else
			{
				ChiHuRight |= CHR_QI_XIAO_DUI;
			}
			if(isJiangQiDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard))
				ChiHuRight = CHR_JIANG_QI_DUI;
		}

		if( !ChiHuRight.IsEmpty() )
			cbChiHuKind = WIK_CHI_HU;

		//插入扑克
		if (cbCurrentCard!=0)
			cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

		//必须缺一门
		if( IsHuaZhu(cbCardIndexTemp,WeaveItem,cbWeaveCount) ) return WIK_NULL;

		//分析扑克
		AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount);

		//胡牌分析
		if (CAnalyseItemArray.size()>0)
		{
			//
			cbChiHuKind = WIK_CHI_HU;

			//牌型分析
			for (int i=0;i<CAnalyseItemArray.size();i++)
			{
				//变量定义
				tagAnalyseItem * pAnalyseItem= &(CAnalyseItemArray[i]);

				/*
				//	判断番型
				*/
				//碰碰和
				if( IsPengPeng(pAnalyseItem) ) 
					ChiHuRight |= CHR_DA_DUI_ZI;
				//带幺
				if( IsDaiYao(pAnalyseItem) )
					ChiHuRight |= CHR_DAI_YAO;
				//将对
				if( IsJiangDui(pAnalyseItem) )
					ChiHuRight |= CHR_JIANG_DUI;
			}
		}

		//素番
		if( cbChiHuKind == WIK_CHI_HU && ChiHuRight.IsEmpty() )
			ChiHuRight |= CHR_PIN_HU;

		if( cbChiHuKind == WIK_CHI_HU )
		{
			//清一色牌
			if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
				ChiHuRight |= CHR_QING_YI_SE;
		}

		return cbChiHuKind;

	}
	//听牌分析
	byte CGameLogic::AnalyseTingCard( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount,std::vector<byte>& shuzu )
	{
		//复制数据
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

		byte cbCardCount = GetCardCount(cbCardIndexTemp);
		CChiHuRight chr;

		if( (cbCardCount-2)%3==0 )
		{
			for( byte i = 0; i < MAX_INDEX-ZI_PAI_COUNT; i++ )
			{
				byte m_data = 0;
				if( cbCardIndexTemp[i] == 0 ) continue;
				
				m_data = SwitchToCardData(i);

				cbCardIndexTemp[i]--;

				bool  pai_shuju = false;
				for( byte j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
				{
					byte cbCurrentCard = SwitchToCardData(j);
					if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
					{
						shuzu.push_back(m_data);
						//return WIK_LISTEN;	
						break;
					}
				}

				cbCardIndexTemp[i]++;
			}
		}
		else
		{
			for( byte j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
			{
				byte cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
					return WIK_LISTEN;
			}
		}

		return WIK_NULL;
	}

	byte CGameLogic::AnalyseTingCard_QUSHU( const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount,std::vector<byte>& shuzu ,byte data)
	{
		//复制数据
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

		byte cbCardCount = GetCardCount(cbCardIndexTemp);
		CChiHuRight chr;

		if( (cbCardCount-2)%3==0 )
		{
			for( byte i = 0; i < MAX_INDEX-ZI_PAI_COUNT; i++ )
			{
				byte m_data = 0;
				if( cbCardIndexTemp[i] == 0 ) continue;
				
				m_data = SwitchToCardData(i);
				if(data == m_data)
				{
					cbCardIndexTemp[i]--;

					bool  pai_shuju = false;
					for( byte j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
					{
						byte cbCurrentCard = SwitchToCardData(j);
						if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
						{
							shuzu.push_back(cbCurrentCard);
							
						}
					}

					cbCardIndexTemp[i]++;
				}
			}
		}
		else
		{
			for( byte j = 0; j < MAX_INDEX-ZI_PAI_COUNT; j++ )
			{
				byte cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
					return WIK_LISTEN;
			}
		}

		return WIK_NULL;
	}

	//分析扑克
	bool CGameLogic::AnalyseCard(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], byte cbWeaveCount)
	{
		//计算数目
		byte cbCardCount=GetCardCount(cbCardIndex);
		//效验数目
		//ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
		if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
			return false;

		//变量定义
		byte cbKindItemCount=0;
		tagKindItem KindItem[27*2+7+14];
		zeromemory(KindItem,sizeof(KindItem));

		//需求判断
		byte cbLessKindItem=(cbCardCount-2)/3;
		ASSERT((cbLessKindItem+cbWeaveCount)==4);

		//单吊判断
		if (cbLessKindItem==0)
		{
			//效验参数
			ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

			//牌眼判断
			for (byte i=0;i<MAX_INDEX;i++)
			{
				if (cbCardIndex[i]==2 || 
					( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
				{
					//变量定义
					tagAnalyseItem AnalyseItem;
					zeromemory(&AnalyseItem,sizeof(AnalyseItem));

					//设置结果
					for (byte j=0;j<cbWeaveCount;j++)
					{
						AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
						AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
						GetWeaveCard( WeaveItem[j].cbWeaveKind,WeaveItem[j].cbCenterCard,AnalyseItem.cbCardData[j] );
					}
					AnalyseItem.cbCardEye=SwitchToCardData(i);
					if( cbCardIndex[i] < 2 || i == m_cbMagicIndex )
						AnalyseItem.bMagicEye = true;
					else AnalyseItem.bMagicEye = false;

					//插入结果
					CAnalyseItemArray.push_back(AnalyseItem);

					return true;
				}
			}

			return false;
		}

		//拆分分析
		byte cbMagicCardIndex[MAX_INDEX];
		memcpy(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
		byte cbMagicCardCount = 0;
		if( m_cbMagicIndex != MAX_INDEX )
		{
			cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
			if( cbMagicCardIndex[m_cbMagicIndex] ) cbMagicCardIndex[m_cbMagicIndex] = 1;		//减小多余组合
		}
		if (cbCardCount>=3)
		{
			for (byte i=0;i<MAX_INDEX;i++)
			{
				//同牌判断
				if (cbMagicCardIndex[i]+cbMagicCardCount>=3)
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					KindItem[cbKindItemCount].cbCardIndex[0]=i;
					KindItem[cbKindItemCount].cbCardIndex[1]=i;
					KindItem[cbKindItemCount].cbCardIndex[2]=i;
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
					KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>0?i:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = cbMagicCardIndex[i]>1?i:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = cbMagicCardIndex[i]>2?i:m_cbMagicIndex;
					cbKindItemCount++;
					if(cbMagicCardIndex[i]+cbMagicCardCount>=6)
					{
						ASSERT( cbKindItemCount < CountArray(KindItem) );
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i;
						KindItem[cbKindItemCount].cbCardIndex[2]=i;
						KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
						KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
						KindItem[cbKindItemCount].cbValidIndex[0] = cbMagicCardIndex[i]>3?i:m_cbMagicIndex;
						KindItem[cbKindItemCount].cbValidIndex[1] = m_cbMagicIndex;
						KindItem[cbKindItemCount].cbValidIndex[2] = m_cbMagicIndex;
						cbKindItemCount++;
					}
				}

				//连牌判断
				if ((i<(MAX_INDEX-9))&&((i%9)<7))
				{
					//只要财神牌数加上3个顺序索引的牌数大于等于3,则进行组合
					if( cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3 )
					{
						int cbIndex[3] = { i==m_cbMagicIndex?0:cbMagicCardIndex[i],(i+1)==m_cbMagicIndex?0:cbMagicCardIndex[i+1],
							(i+2)==m_cbMagicIndex?0:cbMagicCardIndex[i+2] };
						int nMagicCountTemp = cbMagicCardCount;
						byte cbValidIndex[3];
						while( nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3 )
						{
							for( byte j = 0; j < CountArray(cbIndex); j++ )
							{
								if( cbIndex[j] > 0 ) 
								{
									cbIndex[j]--;
									cbValidIndex[j] = i+j;
								}
								else 
								{
									nMagicCountTemp--;
									cbValidIndex[j] = m_cbMagicIndex;
								}
							}
							if( nMagicCountTemp >= 0 )
							{
								ASSERT( cbKindItemCount < CountArray(KindItem) );
								KindItem[cbKindItemCount].cbCardIndex[0]=i;
								KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
								KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
								KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
								KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
								memcpy( KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex) );
								cbKindItemCount++;
							}
							else break;
						}
					}
				}
			}
		}

		//组合分析
		if (cbKindItemCount>=cbLessKindItem)
		{
			//变量定义
			byte cbCardIndexTemp[MAX_INDEX];
			memset(cbCardIndexTemp,0,sizeof(cbCardIndexTemp));

			//变量定义
			byte cbIndex[4]={0,1,2,3};
			tagKindItem * pKindItem[4];
			memset(&pKindItem,0,sizeof(pKindItem));

			//开始组合
			do
			{
				//设置变量
				memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
				for (byte i=0;i<cbLessKindItem;i++)
					pKindItem[i]=&KindItem[cbIndex[i]];

				//数量判断
				bool bEnoughCard=true;
				for (byte i=0;i<cbLessKindItem*3;i++)
				{
					//存在判断
					byte cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
					if (cbCardIndexTemp[cbCardIndex]==0)
					{
						bEnoughCard=false;
						break;
					}
					else 
						cbCardIndexTemp[cbCardIndex]--;
				}

				//胡牌判断
				if (bEnoughCard==true)
				{
					//牌眼判断
					byte cbCardEye=0;
					bool bMagicEye = false;
					for (byte i=0;i<MAX_INDEX;i++)
					{
						if (cbCardIndexTemp[i]==2)
						{
							cbCardEye=SwitchToCardData(i);
							if( i == m_cbMagicIndex ) bMagicEye = true;
							break;
						}
						else if( i!=m_cbMagicIndex && 
							m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2 )
						{
							cbCardEye = SwitchToCardData(i);
							bMagicEye = true;
						}
					}

					//组合类型
					if (cbCardEye!=0)
					{
						//变量定义
						tagAnalyseItem AnalyseItem;
						memset(&AnalyseItem,0,sizeof(AnalyseItem));

						//设置组合
						for (byte i=0;i<cbWeaveCount;i++)
						{
							AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
							AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
							GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,
								AnalyseItem.cbCardData[i] );
						}

						//设置牌型
						for (byte i=0;i<cbLessKindItem;i++) 
						{
							AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
							AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
							AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
							AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
							AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
						}

						//设置牌眼
						AnalyseItem.cbCardEye=cbCardEye;
						AnalyseItem.bMagicEye = bMagicEye;

						//插入结果
						CAnalyseItemArray.push_back(AnalyseItem);
					}
				}

				//设置索引
				if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
				{
					byte i=cbLessKindItem-1;
					for (;i>0;i--)
					{
						if ((cbIndex[i-1]+1)!=cbIndex[i])
						{
							byte cbNewIndex=cbIndex[i-1];
							for (byte j=(i-1);j<cbLessKindItem;j++) 
								cbIndex[j]=cbNewIndex+j-i+2;
							break;
						}
					}
					if (i==0)
						break;
				}
				else
					cbIndex[cbLessKindItem-1]++;
			} while (true);

		}

		return (CAnalyseItemArray.size()>0);
	}
}

//////////////////////////////////////////////////////////////////////////
