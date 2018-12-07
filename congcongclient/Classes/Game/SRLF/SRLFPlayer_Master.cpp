#include "SRLFPlayer.h"
#include "SRLFGameLogic.h"
#include "SRLFSoundFun.h"

void SRLFPlayer::setHandCard_EX(std::vector<BYTE> kCardList)
{
	int nCardCount = kCardList.size();
	ASSERT(nCardCount <= MAX_COUNT);
	for (int i=0;i<nCardCount;i++)
	{
		m_kHandCardData[i] = kCardList[i];
	}
	m_kHandCardCout = nCardCount;
}
void SRLFPlayer::setMaskHandCard(std::vector<SRLF::MasterHandCardInfo> kMasterHandCardInfo)
{
	for (int i=0;i<(int)kMasterHandCardInfo.size();i++)
	{
		if (GetChairID() == kMasterHandCardInfo[i].nChairId)
		{
			setHandCard_EX(kMasterHandCardInfo[i].kMasterHandCard);
		}
	}
}
void SRLFPlayer::setMingPai(bool bValue)
{
	m_bMingPai = bValue;
}
//��Ϸ����
void SRLFPlayer::setPlayCount(WORD wCount)
{
	m_wPlayCount = wCount;
}
WORD SRLFPlayer::getPlayCount()
{
	return m_wPlayCount;
}