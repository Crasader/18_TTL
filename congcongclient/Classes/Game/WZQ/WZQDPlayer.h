
#pragma once

#include "Game/Game/GamePlayer.h"
#include "GAME.h"
#include <cocos2d.h>
#include "CMD_WZQ.h"
using namespace WZQ;

struct OutCardInfo
{
	OutCardInfo(cocos2d::Node* pCard,int nData)
		:pCardNode(pCard)
		,nCardDarta(nData)
	{

	}
	cocos2d::Node* pCardNode;
	int           nCardDarta;
};
//游戏逻辑
class WZQDPlayer:public GamePlayer
{
public:
	const static int CARD_DWON_POSY = 70;
	const static int CARD_UP_POSY = 160;
	const static int HUAN_CARD_NUM = 3;//换牌数量
public:
	WZQDPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~WZQDPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void setGameRecord(bool bRecord);

	cocos2d::Node* getPlayerNode();
	void startGame();
	void startGameNoTime();//不要时间动画
	void EndGame();
	void setZhuang();
	void showAddGold(int iGold);
	void showEffect(std::string kKey);
	void showStatusImagic(std::string kKey);
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
	virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
public:
	//void setHandCard(byte* pCardData,int iCout);
	void setWeaveItem(WZQ::CMD_WeaveItem* pWeave,int iCout);
	//void setOperateResoult(CMD_S_OperateResult* pNetInfo);
	void addNewInCard(int iCard);
	//void sendOutCard(int iCard);
	void showMJPoint();
	//void setActOutCard(int iCard);
	cocos2d::Node* addHandOutCard(int iCard);
	void showHandCard();
	void showHandCard(WZQ::CMD_WeaveItem* pWeave,int iWeaveCout,byte* pHandCard,int iHandCout);
	//void setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront);
	//cocos2d::Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
	byte getTouchCardVlaue(cocos2d::Node* pNode);
	//void showHuPai(bool bGaizhu);
	void showJieSuanCard(cocos2d::Node* pCardNode,	WZQ::CMD_WeaveItem* pWeave,int iWeaveCout,byte* pHandCard,int iHandCout);
	//void setChiHuCard(byte cbCard);
	//void clearChiHuCard();
	byte getChiHuCard();
	void showChiHuCard();
	//void removeHandCard(byte cbCard);
public:
	//void removeHandOutCard( byte cbCardData );
	void sortHandCard();
	//void showHuanPai(int nGameType = GAME_TYPE_CHENGDU);
	void standUpCard(byte cbCard,bool nOnly = true);
	//void seatDownCard();
	cocos2d::Node* getCardNodeByCard(byte cbCard);
	void getCardNodeByCard(std::vector<byte> CardDataList,std::vector<cocos2d::Node*>& kCardList);
	//bool getHuanCards(byte nHuanCard[HUAN_CARD_NUM]);
	int getDefuaultHuanValue(int nIndex);//获取第几个默认换牌的值
	int getMastKindColor(); //最多张牌花色
	void showCard();
	void showCard(byte* cbCardData,byte cbCardCount);
	void showjiesuanCard(cocos2d::Node* pCardNode);//结算手牌显示
	void setValid(bool valid);
public:
	template<typename T>
	bool isHave(std::vector<T*> kCardList,T* pItem)
	{
		for (int i=0;i<(int)kCardList.size();i++)
		{
			if(kCardList[i] == pItem)
				return true;
		}
		return false;
	}

private:
	int						m_iIdex;
	cocos2d::Node*			m_pSeatNode;

	byte					m_kHandCardData[MAX_COUNT];//手牌数据
	byte					m_kHandCardCout;
	byte					m_kWeaveCount;					//组合数目
	std::vector<OutCardInfo>	m_kOutCardList;
	bool						m_bGameRecord;
	byte                    m_cbChiHuCard;//所胡的牌
	cocos2d::Node*          m_pHandOutCard;
	std::vector<byte>		m_kHuCardList;//胡牌表
	bool					m_Valid;
};

