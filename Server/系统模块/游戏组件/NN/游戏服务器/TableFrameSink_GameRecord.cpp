 #include "StdAfx.h"
#include "TableFrameSink.h"

void CTableFrameSink::startGameRecord()
{
	m_GameRecord = NNGameRecord();
	m_GameRecord.gameRuleIndex = m_GameRuleIdex;
	m_GameRecord.gameTypeIndex = m_GameTypeIdex;
	m_GameRecord.hostUser = m_HostUserID;

	for (WORD i = 0; i < NN_GAME_PLAYER; i++) {
		IServerUserItem* pUserItem = m_pITableFrame->GetTableUserItem(i);

		if (pUserItem == NULL || m_PlayerStatus[pUserItem->GetChairID()] != NNPlayerStatus_Playing) {
			continue;
		}

		NNGameRecordPlayer kReocrdPlayer;
		kReocrdPlayer.dwUserID = pUserItem->GetUserID();
		kReocrdPlayer.chairID = pUserItem->GetChairID();
		kReocrdPlayer.kHead = "";
		kReocrdPlayer.kNickName = pUserItem->GetNickName();
		m_GameRecord.kPlayers.push_back(kReocrdPlayer);
	}
}

void CTableFrameSink::addGameOperator(CMD_S_BankerInfo& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_Start;
	result.bankerChairID = oper.bankerChairID;
	result.bankerRatio = static_cast<BYTE>(oper.bankerRatio);
	if (m_GameTypeIdex == NNGameType_AllCompare) {
		if (hasRule(NNGameRule_Score_0)) {
			result.userBetsRatio = 1;
		} else if (hasRule(NNGameRule_Score_1)){
			result.userBetsRatio = 2;
		} else{
			result.userBetsRatio = 4;
		}
	}
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_UserCall& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_UserCall;
	result.operatorChairID = oper.chairID;
	result.userBetsRatio = static_cast<BYTE>(oper.playerBets.wBet);
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_SnatchBanker& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_SnatchBanker;
	result.cardCount = oper.cardCount;
	memcpy(result.playerCards, oper.cards, sizeof(oper.cards));
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_UserSnatchBanker& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_UserSnatchBanker;
	result.operatorChairID = oper.userChairID;
	result.bankerRatio = oper.ratio;
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_SendCardAll& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_SendCardAll;
	result.cardCount = MAX_HAND_CARD;
	memcpy(result.playerCards, oper.playerCards, sizeof(result.playerCards));
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_SendCardAdd& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_SendCardAdd;
	memcpy(result.playerCardsAdd, oper.playerCards, sizeof(oper.playerCards));
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_UserShowCard& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_UserSplitCard;
	result.operatorChairID = oper.chairID;
	result.cardType = oper.result;
	memcpy(result.playerCards, oper.playerCards, sizeof(oper.playerCards));
	m_GameRecord.kAction.push_back(result);
}

void CTableFrameSink::addGameOperator(CMD_S_Calculate& oper)
{
	NNGameRecordOperateResult result;
	result.actionType = NNGameRecordOperateResult::TYPE_CompareCard;
	memcpy(result.playerScores, oper.playerScores, sizeof(oper.playerScores));
	m_GameRecord.kAction.push_back(result);
}