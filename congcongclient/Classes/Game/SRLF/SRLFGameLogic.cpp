#include "SRLFGameLogic.h"

//////////////////////////////////////////////////////////////////////////

FV_SINGLETON_STORAGE(SRLFLOGIC::CGameLogic);

namespace SRLFLOGIC
{
	//��̬����
	bool		CChiHuRight::m_bInit = false;
	dword		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

	//���캯��
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

	//��ֵ������
	CChiHuRight & CChiHuRight::operator = ( dword dwRight )
	{
		dword dwOtherRight = 0;
		//��֤Ȩλ
		if( !IsValidRight( dwRight ) )
		{
			//��֤ȡ��Ȩλ
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

	//�����
	CChiHuRight & CChiHuRight::operator &= ( dword dwRight )
	{
		bool bNavigate = false;
		//��֤Ȩλ
		if( !IsValidRight( dwRight ) )
		{
			//��֤ȡ��Ȩλ
			ASSERT( IsValidRight( ~dwRight ) );
			if( !IsValidRight( ~dwRight ) ) return *this;
			//����Ȩλ
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

	//�����
	CChiHuRight & CChiHuRight::operator |= ( dword dwRight )
	{
		//��֤Ȩλ
		if( !IsValidRight( dwRight ) ) return *this;

		for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
				m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
		}

		return *this;
	}

	//��
	CChiHuRight CChiHuRight::operator & ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//��
	CChiHuRight CChiHuRight::operator & ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return (chr &= dwRight);
	}

	//��
	CChiHuRight CChiHuRight::operator | ( dword dwRight )
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//��
	CChiHuRight CChiHuRight::operator | ( dword dwRight ) const
	{
		CChiHuRight chr = *this;
		return chr |= dwRight;
	}

	//�Ƿ�ȨλΪ��
	bool CChiHuRight::IsEmpty()
	{
		for( byte i = 0; i < CountArray(m_dwRight); i++ )
			if( m_dwRight[i] ) return false;
		return true;
	}

	//����ȨλΪ��
	void CChiHuRight::SetEmpty()
	{
		zeromemory( m_dwRight,sizeof(m_dwRight) );
		return;
	}

	//��ȡȨλ��ֵ
	byte CChiHuRight::GetRightData( dword dwRight[], byte cbMaxCount )
	{
		ASSERT( cbMaxCount >= CountArray(m_dwRight) );
		if( cbMaxCount < CountArray(m_dwRight) ) return 0;

		memcpy( dwRight,m_dwRight,sizeof(dword)*CountArray(m_dwRight) );
		return CountArray(m_dwRight);
	}

	//����Ȩλ��ֵ
	bool CChiHuRight::SetRightData( const dword dwRight[], byte cbRightCount )
	{
		ASSERT( cbRightCount <= CountArray(m_dwRight) );
		if( cbRightCount > CountArray(m_dwRight) ) return false;

		zeromemory( m_dwRight,sizeof(m_dwRight) );
		memcpy( m_dwRight,dwRight,sizeof(dword)*cbRightCount );

		return true;
	}

	//����λ�Ƿ���ȷ
	bool CChiHuRight::IsValidRight( dword dwRight )
	{
		dword dwRightHead = dwRight & 0xF0000000;
		for( byte i = 0; i < CountArray(m_dwRightMask); i++ )
			if( m_dwRightMask[i] == dwRightHead ) return true;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	//��̬����

	//�˿�����
	const byte CGameLogic::m_cbCardDataArray[MAX_REPERTORY_SRLF]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
		//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	};

	//////////////////////////////////////////////////////////////////////////

	//���캯��
	CGameLogic::CGameLogic()
	{
		m_cbMagicIndex = MAX_INDEX;
	}

	//��������
	CGameLogic::~CGameLogic()
	{
	}

	//�����˿�
	void CGameLogic::RandCardData(byte cbCardData[], byte cbMaxCount)
	{
		//����׼��
		byte cbCardDataTemp[CountArray(m_cbCardDataArray)];
		memcpy(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

		//�����˿�
		byte cbRandCount=0,cbPosition=0;
		do
		{
			cbPosition=rand()%(cbMaxCount-cbRandCount);
			cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
			cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
		} while (cbRandCount<cbMaxCount);

		return;
	}

	//ɾ���˿�
	bool CGameLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], byte cbRemoveCard)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

		//ɾ���˿�
		byte cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
		if (cbCardIndex[cbRemoveIndex]>0)
		{
			cbCardIndex[cbRemoveIndex]--;
			return true;
		}

		//ʧ��Ч��
		ASSERT(FALSE);

		return false;
	}

	//ɾ���˿�
	bool CGameLogic::RemoveCard(byte cbCardIndex[MAX_INDEX], const byte cbRemoveCard[], byte cbRemoveCount)
	{
		//ɾ���˿�
		for (byte i=0;i<cbRemoveCount;i++)
		{
			//Ч���˿�
			ASSERT(IsValidCard(cbRemoveCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

			//ɾ���˿�
			byte cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
			if (cbCardIndex[cbRemoveIndex]==0)
			{
				//�������
				ASSERT(FALSE);

				//��ԭɾ��
				for (byte j=0;j<i;j++) 
				{
					ASSERT(IsValidCard(cbRemoveCard[j]));
					cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
				}

				return false;
			}
			else 
			{
				//ɾ���˿�
				--cbCardIndex[cbRemoveIndex];
			}
		}

		return true;
	}

	byte CGameLogic::RemoveValueCardAll(byte cbCardData[], byte cbCardCount,byte cbRemoveCard)
	{
		byte cbCardIndex[MAX_INDEX];			//�����˿�
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
	//ɾ���˿�
	bool CGameLogic::RemoveValueCard(byte cbCardData[], byte cbCardCount, const byte cbRemoveCard[], byte cbRemoveCount)
	{
		//��������
		ASSERT(cbCardCount<=14);
		ASSERT(cbRemoveCount<=cbCardCount);

		//�������
		byte cbDeleteCount=0,cbTempCardData[14];
		if (cbCardCount>CountArray(cbTempCardData))
			return false;
		memcpy(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

		//�����˿�
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

		//�ɹ��ж�
		if (cbDeleteCount!=cbRemoveCount) 
		{
			ASSERT(FALSE);
			return false;
		}

		//�����˿�
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

		//�����˿�
		for (byte i=0;i<cbAddCount;i++)
		{
			//Ч���˿�
			ASSERT(IsValidCard(cbAddCard[i]));
			ASSERT(cbCardIndex[SwitchToCardIndex(cbAddCard[i])]<4);

			//ɾ���˿�
			byte cbAddIndex=SwitchToCardIndex(cbAddCard[i]);
			if (cbCardIndex[cbAddIndex]<4)
			{
				//ɾ���˿�
				++cbCardIndex[cbAddIndex];
			}
			else 
			{
				//�������
				ASSERT(FALSE);
				//��ԭ����
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

	//��Ч�ж�
	bool CGameLogic::IsValidCard(byte cbCardData)
	{
		byte cbValue=(cbCardData&MASK_VALUE);
		byte cbColor=(cbCardData&MASK_COLOR)>>4;
		return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
	}

	//�˿���Ŀ
	byte CGameLogic::GetCardCount(const byte cbCardIndex[MAX_INDEX])
	{
		//��Ŀͳ��
		byte cbCardCount=0;
		for (byte i=0;i<MAX_INDEX;i++) 
			cbCardCount+=cbCardIndex[i];

		return cbCardCount;
	}

	//��ȡ���
	byte CGameLogic::GetWeaveCard(byte cbWeaveKind, byte cbCenterCard, byte cbCardBuffer[4])
	{
		//����˿�
		switch (cbWeaveKind)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard+1;
				cbCardBuffer[2]=cbCenterCard+2;

				return 3;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard-2;

				return 3;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard-1;
				cbCardBuffer[2]=cbCenterCard+1;

				return 3;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//���ñ���
				cbCardBuffer[0]=cbCenterCard;
				cbCardBuffer[1]=cbCenterCard;
				cbCardBuffer[2]=cbCenterCard;

				return 3;
			}
		case WIK_GANG:		//���Ʋ���
			{
				//���ñ���
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

	//�����ȼ�
	byte CGameLogic::GetUserActionRank(byte cbUserAction)
	{
		//���Ƶȼ�
		if (cbUserAction&WIK_CHI_HU) { return 4; }

		//���Ƶȼ�
		if (cbUserAction&WIK_GANG) { return 3; }

		//���Ƶȼ�
		if (cbUserAction&WIK_PENG) { return 2; }

		//���Ƶȼ�
		if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

		return 0;
	}

	//���Ƶȼ�
	word CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
	{
		word wFanShu = 0;
		return wFanShu;
	}

	//�����ж�
	byte CGameLogic::EstimateEatCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCurrentCard));

		//�����ж�
		if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//��������
		byte cbExcursion[3]={0,1,2};
		byte cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

		//�����ж�
		byte cbEatKind=0,cbFirstIndex=0;
		byte cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
		for (byte i=0;i<CountArray(cbItemKind);i++)
		{
			byte cbValueIndex=cbCurrentIndex%9;
			if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
			{
				//�����ж�
				cbFirstIndex=cbCurrentIndex-cbExcursion[i];

				//���Ʋ��ܰ���������
				if( m_cbMagicIndex != MAX_INDEX &&
					m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

				if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
					continue;
				if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
					continue;

				//��������
				cbEatKind|=cbItemKind[i];
			}
		}

		return cbEatKind;
	}

	//�����ж�
	byte CGameLogic::EstimatePengCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCurrentCard));

		//�����ж�
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//�����ж�
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
	}

	//�����ж�
	byte CGameLogic::EstimateGangCard(const byte cbCardIndex[MAX_INDEX], byte cbCurrentCard)
	{
		//����Ч��
		ASSERT(IsValidCard(cbCurrentCard));

		//�����ж�
		if ( IsMagicCard(cbCurrentCard) ) 
			return WIK_NULL;

		//�����ж�
		return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
	}

	//���Ʒ���
	byte CGameLogic::AnalyseGangCard(const  byte cbCardIndex[MAX_INDEX], const CMD_WeaveItem WeaveItem[], byte cbWeaveCount, tagGangCardResult & GangCardResult )
	{
		//���ñ���
		byte cbActionMask=WIK_NULL;
		zeromemory(&GangCardResult,sizeof(GangCardResult));

		//���ϸ���
		for (byte i=0;i<MAX_INDEX;i++)
		{
			if( i == m_cbMagicIndex ) continue;
			if (cbCardIndex[i]==4)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[0]=SwitchToCardData(i);
				GangCardResult.cbCardData[1]=SwitchToCardData(i);
				GangCardResult.cbCardData[2]=SwitchToCardData(i);
				GangCardResult.cbCardData[3]=SwitchToCardData(i);
				GangCardResult.cbCardCount++;
			}
		}

		//��ϸ���
		for (byte i=0;i<MAX_WEAVE;i++)
		{
			if (WeaveItem[i].cbWeaveKind==WIK_PENG)
			{
				if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
				{
					cbActionMask|=WIK_GANG;
					GangCardResult.cbCardData[0]=WeaveItem[i].cbCenterCard;
					GangCardResult.cbCardCount++;
				}
			}
		}
		return cbActionMask;
	}


	//�˿�ת��
	byte CGameLogic::SwitchToCardData(byte cbCardIndex)//��ֵת��
	{
		ASSERT(cbCardIndex<34);
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	}

	//�˿�ת��
	byte CGameLogic::SwitchToCardIndex(byte cbCardData) //������
	{
		ASSERT(IsValidCard(cbCardData));
		return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
	}

	//�˿�ת��
	byte CGameLogic::SwitchToCardData(const byte cbCardIndex[MAX_INDEX], byte cbCardData[MAX_COUNT])
	{
		//ת���˿�
		byte cbPosition=0;
		//����
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

	//�˿�ת��
	byte CGameLogic::SwitchToCardIndex(const byte cbCardData[], byte cbCardCount, byte cbCardIndex[MAX_INDEX])
	{
		//���ñ���
		zeromemory(cbCardIndex,sizeof(byte)*MAX_INDEX);

		//ת���˿�
		for (byte i=0;i<cbCardCount;i++)
		{
			//ASSERT(IsValidCard(cbCardData[i]));
			if(IsValidCard(cbCardData[i]))
			cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
		}

		return cbCardCount;
	}

	//����
	bool CGameLogic::IsMagicCard( byte cbCardData )
	{
		if( m_cbMagicIndex != MAX_INDEX )
			return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
		return false;
	}

	//����,������ֵ����
	bool CGameLogic::SortCardList( byte cbCardData[MAX_COUNT], byte cbCardCount )
	{
		//��Ŀ����
		if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

		//�������
		bool bSorted=true;
		byte cbSwitchData=0,cbLast=cbCardCount-1;
		do
		{
			bSorted=true;
			for (byte i=0;i<cbLast;i++)
			{
				if (cbCardData[i]>cbCardData[i+1])
				{
					//���ñ�־
					bSorted=false;

					//�˿�����
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
		//��Ŀ����
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
	// ������������
	*/

	//�����
	bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
	{
		for( byte i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
		{
			if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
				return false;
		}
		return true;
	}

	//��һɫ��
	bool CGameLogic::IsQingYiSe(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const byte cbItemCount,const byte cbCurrentCard)
	{
		//�����ж�
		byte cbCardColor=0xFF;

		for (byte i=0;i<MAX_INDEX;i++)
		{
			if(i==m_cbMagicIndex) continue;
			if (cbCardIndex[i]!=0)
			{
				//��ɫ�ж�
				if (cbCardColor!=0xFF)
					return false;

				//���û�ɫ
				cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

				//��������
				i=(i/9+1)*9-1;
			}
		}

		//�������ֻ������
		if( cbCardColor == 0xFF )
		{
			ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0 );
			//������
			ASSERT( cbItemCount > 0 );
			cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
		}

		if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

		//����ж�
		for (byte i=0;i<cbItemCount;i++)
		{
			byte cbCenterCard=WeaveItem[i].cbCenterCard;
			if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
		}

		return true;
	}

	//��С����
	bool CGameLogic::IsQiXiaoDui(const byte cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const byte cbWeaveCount,const byte cbCurrentCard)
	{
		//����ж�
		if (cbWeaveCount!=0) return false;

		//������Ŀ
		byte cbReplaceCount = 0;

		//��ʱ����
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

		//��������
		byte cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
		cbCardIndexTemp[cbCurrentIndex]++;

		//���㵥��
		for (byte i=0;i<MAX_INDEX;i++)
		{
			byte cbCardCount=cbCardIndexTemp[i];

			//���ƹ���
			if( i == m_cbMagicIndex ) continue;

			//����ͳ��
			if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount++;
		}

		//���Ʋ���
		if( m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
			m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0 )
			return false;

		return true;

	}

	//����
	bool CGameLogic::IsDaiYao( const tagAnalyseItem *pAnalyseItem )
	{
		//�������
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

	//����
	bool CGameLogic::IsJiangDui( const tagAnalyseItem *pAnalyseItem )
	{
		//�Ƿ�����
		if( !IsPengPeng(pAnalyseItem) ) return false;

		//�������
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

	//�Ƿ���
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
		//ȱһ�žͲ��ǻ���
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

	//�Ƿ�����
	bool CGameLogic::IsTingCard( const byte cbCardIndex[MAX_INDEX])
	{
		bool  isCanFu = IsQiDui(cbCardIndex);
		if(isCanFu == true)
			return true;
		isCanFu = CheckHu(cbCardIndex);
		if(isCanFu == true)
			return true;
		return false;
	}

	bool CGameLogic::CheckHu(const byte cbCardIndexTemp[MAX_INDEX])
	{
		byte cbCardIndexWangemp[MAX_INDEX];// �� 
		byte cbCardIndexTiaoemp[MAX_INDEX];//  ��
		byte cbCardIndexTongemp[MAX_INDEX];//  ͬ 
		memset(cbCardIndexWangemp,0,sizeof(cbCardIndexWangemp));
		memset(cbCardIndexTiaoemp,0,sizeof(cbCardIndexTiaoemp));
		memset(cbCardIndexTongemp,0,sizeof(cbCardIndexTongemp));
		for (int i = 0; i < 9; i++)cbCardIndexWangemp[i] = cbCardIndexTemp[i];
		for (int i = 9; i < 18; i++)cbCardIndexTiaoemp[i] = cbCardIndexTemp[i];
		for (int i = 18; i < 27; i++)cbCardIndexTongemp[i] = cbCardIndexTemp[i];
		//������ ��
		if (getAnyCardsCount(cbCardIndexWangemp) % 3 == 2){
			for (int i = 0; i < 9; i++){
				byte tempCardIndexWangemp[MAX_INDEX];
				byte tempCardIndexTiaoemp[MAX_INDEX];//  ��
				byte tempCardIndexTongemp[MAX_INDEX];//  ͬ 
				memset(tempCardIndexWangemp,0,sizeof(tempCardIndexWangemp));
				memset(tempCardIndexTiaoemp,0,sizeof(tempCardIndexTiaoemp));
				memset(tempCardIndexTongemp,0,sizeof(tempCardIndexTongemp));
				
				memcpy(tempCardIndexWangemp, cbCardIndexWangemp, sizeof(cbCardIndexWangemp));
				memcpy(tempCardIndexTiaoemp, cbCardIndexTiaoemp, sizeof(cbCardIndexTiaoemp));
				memcpy(tempCardIndexTongemp, cbCardIndexTongemp, sizeof(cbCardIndexTongemp));
				
				if (isHaveAnyCard(tempCardIndexWangemp, SwitchToCardData(i), 2))
				{
					removeAnyCard(tempCardIndexWangemp, SwitchToCardData(i), 2);
					if (Check3nWang(tempCardIndexWangemp) && Check3nTiao(tempCardIndexTiaoemp) && Check3nTong(tempCardIndexTongemp)){
						return true;
					}
				}
			}
		}
		//������ ��
		if (getAnyCardsCount(cbCardIndexTiaoemp) % 3 == 2){
			for (int i = 9; i < 18; i++){
				byte tempCardIndexWangemp[MAX_INDEX];
				byte tempCardIndexTiaoemp[MAX_INDEX];//  ��
				byte tempCardIndexTongemp[MAX_INDEX];//  ͬ 
				memset(tempCardIndexWangemp,0,sizeof(tempCardIndexWangemp));
				memset(tempCardIndexTiaoemp,0,sizeof(tempCardIndexTiaoemp));
				memset(tempCardIndexTongemp,0,sizeof(tempCardIndexTongemp));
				
				memcpy(tempCardIndexWangemp, cbCardIndexWangemp, sizeof(cbCardIndexWangemp));
				memcpy(tempCardIndexTiaoemp, cbCardIndexTiaoemp, sizeof(cbCardIndexTiaoemp));
				memcpy(tempCardIndexTongemp, cbCardIndexTongemp, sizeof(cbCardIndexTongemp));
				if (isHaveAnyCard(tempCardIndexTiaoemp, SwitchToCardData(i), 2))
				{	
					removeAnyCard(tempCardIndexTiaoemp, SwitchToCardData(i), 2);
					if (Check3nWang(tempCardIndexWangemp) && Check3nTiao(tempCardIndexTiaoemp) && Check3nTong(tempCardIndexTongemp)){
						return true;
					}
				}
			}
		}
		//������ Ͳ
		if (getAnyCardsCount(cbCardIndexTongemp) % 3 == 2){
			for (int i = 18; i < 27; i++){
				byte tempCardIndexWangemp[MAX_INDEX];
				byte tempCardIndexTiaoemp[MAX_INDEX];//  ��
				byte tempCardIndexTongemp[MAX_INDEX];//  ͬ 
				memset(tempCardIndexWangemp,0,sizeof(tempCardIndexWangemp));
				memset(tempCardIndexTiaoemp,0,sizeof(tempCardIndexTiaoemp));
				memset(tempCardIndexTongemp,0,sizeof(tempCardIndexTongemp));

				memcpy(tempCardIndexWangemp, cbCardIndexWangemp, sizeof(cbCardIndexWangemp));
				memcpy(tempCardIndexTiaoemp, cbCardIndexTiaoemp, sizeof(cbCardIndexTiaoemp));
				memcpy(tempCardIndexTongemp, cbCardIndexTongemp, sizeof(cbCardIndexTongemp));
				if (isHaveAnyCard(tempCardIndexTongemp, SwitchToCardData(i), 2)){
					removeAnyCard(tempCardIndexTongemp, SwitchToCardData(i), 2);
					if (Check3nWang(tempCardIndexWangemp) && Check3nTiao(tempCardIndexTiaoemp) && Check3nTong(tempCardIndexTongemp)){
						return true;
					}
				}
			}
		}
		return false;

	}

	byte CGameLogic::getAnyCardsCount( byte cbCardIndex[MAX_INDEX])
	{
		byte count =0;
		for(byte i =0;i<MAX_INDEX;i++)
			count+= cbCardIndex[i];
		return count;
	}

	//ȥ��������
	void CGameLogic::removeAnyCard( byte cbCardIndex[MAX_INDEX],byte cbCardData,byte cardCount)
	{
		if (cbCardIndex[SwitchToCardIndex(cbCardData)]>=cardCount)
			cbCardIndex[SwitchToCardIndex(cbCardData)]-= cardCount;
	}
	
	bool CGameLogic::isHaveAnyCard( byte cbCardIndex[MAX_INDEX],byte cbCardData,byte cardCount )
	{
		byte cardIndex = SwitchToCardIndex(cbCardData);
		return cbCardIndex[cardIndex]>= cardCount;
	}

	//ȥ��������
	bool CGameLogic::isHaveAnyCards( byte cbCardIndex[MAX_INDEX])
	{
		byte count =0;
		for(byte i =0;i<MAX_INDEX;i++)
			count+= cbCardIndex[i];
		return count >0;
	}

	//ȥ����Ͱ������
	bool CGameLogic::Check3nWang(byte cbCardIndex[MAX_INDEX])
	{
		if (getAnyCardsCount(cbCardIndex) == 0)
			return true;
		byte indexCard = 0;
		for (byte i = 0; i < 9; i++)
		{
			byte cardData = SwitchToCardData(i);
			if (isHaveAnyCard(cbCardIndex, cardData, 1)){
				indexCard = i;
				break;
			}
		}
		byte cardData = SwitchToCardData(indexCard);
		if (isHaveAnyCard(cbCardIndex, cardData, 3)){
			removeAnyCard(cbCardIndex, cardData, 3);
		}
		else{
			if ((indexCard + 2) < MAX_INDEX)
			{
				byte cardData1 = SwitchToCardData(indexCard);
				byte cardData2 = SwitchToCardData(indexCard + 1);
				byte cardData3 = SwitchToCardData(indexCard + 2);
				if (isHaveAnyCard(cbCardIndex, cardData1, 1)
					&& isHaveAnyCard(cbCardIndex, cardData2, 1)
					&& isHaveAnyCard(cbCardIndex, cardData3, 1))
				{
					removeAnyCard(cbCardIndex, cardData1, 1);
					removeAnyCard(cbCardIndex, cardData2, 1);
					removeAnyCard(cbCardIndex, cardData3, 1);
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		if (isHaveAnyCards(cbCardIndex) == false)
			return true;
		if (getAnyCardsCount(cbCardIndex) >= 3)
			return Check3nWang(cbCardIndex);
		return false;
	}

	//ȥ����
	bool CGameLogic::Check3nTiao(byte cbCardIndex[MAX_INDEX])
	{
		if (getAnyCardsCount(cbCardIndex) == 0)
			return true;
		byte indexCard = 0;
		for (byte i = 9; i < 18; i++)
		{
			byte cardData = SwitchToCardData(i);
			if (isHaveAnyCard(cbCardIndex, cardData, 1)){
				indexCard = i;
				break;
			}
		}
		byte cardData = SwitchToCardData(indexCard);
		if (isHaveAnyCard(cbCardIndex, cardData, 3)){
			removeAnyCard(cbCardIndex, cardData, 3);
		}
		else{
			if ((indexCard + 2) < MAX_INDEX)
			{
				byte cardData1 = SwitchToCardData(indexCard);
				byte cardData2 = SwitchToCardData(indexCard + 1);
				byte cardData3 = SwitchToCardData(indexCard + 2);
				if (isHaveAnyCard(cbCardIndex, cardData1, 1)
					&& isHaveAnyCard(cbCardIndex, cardData2, 1)
					&& isHaveAnyCard(cbCardIndex, cardData3, 1))
				{
					removeAnyCard(cbCardIndex, cardData1, 1);
					removeAnyCard(cbCardIndex, cardData2, 1);
					removeAnyCard(cbCardIndex, cardData3, 1);
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		if (isHaveAnyCards(cbCardIndex) == false)
			return true;
		if (getAnyCardsCount(cbCardIndex) >= 3)
			return Check3nTiao(cbCardIndex);
		return false;
	}
	//ȥ��Ͱ
	bool CGameLogic::Check3nTong(byte cbCardIndex[MAX_INDEX])
	{
		if (getAnyCardsCount(cbCardIndex) == 0)
			return true;
		byte indexCard = 0;
		for (byte i = 18; i < 27; i++)
		{
			byte cardData = SwitchToCardData(i);
			if (isHaveAnyCard(cbCardIndex, cardData, 1)){
				indexCard = i;
				break;
			}
		}
		byte cardData = SwitchToCardData(indexCard);
		if (isHaveAnyCard(cbCardIndex, cardData, 3)){
			removeAnyCard(cbCardIndex, cardData, 3);
		}
		else{
			if ((indexCard + 2) < MAX_INDEX)
			{
				byte cardData1 = SwitchToCardData(indexCard);
				byte cardData2 = SwitchToCardData(indexCard + 1);
				byte cardData3 = SwitchToCardData(indexCard + 2);
				if (isHaveAnyCard(cbCardIndex, cardData1, 1)
					&& isHaveAnyCard(cbCardIndex, cardData2, 1)
					&& isHaveAnyCard(cbCardIndex, cardData3, 1))
				{
					removeAnyCard(cbCardIndex, cardData1, 1);
					removeAnyCard(cbCardIndex, cardData2, 1);
					removeAnyCard(cbCardIndex, cardData3, 1);
				}
				else{
					return false;
				}
			}
			else{
				return false;
			}
		}
		if (isHaveAnyCards(cbCardIndex) == false)
			return true;
		if (getAnyCardsCount(cbCardIndex) >= 3)
			return Check3nTong(cbCardIndex);
		return false;
	}

	//�߶ԣ�����ʱ�����е�����Ϊ7�����ӡ�
	bool CGameLogic::IsQiDui(const byte cbCardIndex[MAX_INDEX]){
		byte cbCardIndexTemp[MAX_INDEX];
		memcpy(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
		byte duiCount = 0;
		for (int i=0;i<MAX_INDEX;i++){
			duiCount += (cbCardIndexTemp[i] == 2 )? 1:0;
			duiCount += (cbCardIndexTemp[i] == 4 )? 2:0;
		}
		if(((duiCount == 7) && (getAnyCardsCount(cbCardIndexTemp) == 14)))
			return true;
		if(((duiCount == 4) && (getAnyCardsCount(cbCardIndexTemp) == 8)))
			return true;
		return false;
	}

}

//////////////////////////////////////////////////////////////////////////