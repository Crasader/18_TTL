#pragma once

#include <cocos2d.h>

#include "Game/Game/GamePlayer.h"
#include "common.h"
#include "CMD_XZDD.h"

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
//��Ϸ�߼�
class XZDDPlayer:public GamePlayer
{
public:
	const static int CARD_DWON_POSY = 70;
	const static int CARD_UP_POSY = 120;
	const static int HUAN_CARD_NUM = 3;//��������
public:
	XZDDPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~XZDDPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void setGameRecord(bool bRecord);

	cocos2d::Node* getPlayerNode();
	void startGame();
	void startGameNoTime();//��Ҫʱ�䶯��
	void EndGame();
	void setZhuang(bool bZhuang);
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
	void setHandCard(byte* pCardData,int iCout);
	void setWeaveItem(XZDD::CMD_WeaveItem* pWeave, int iCout);
	void setOperateResoult(XZDD::CMD_S_OperateResult* pNetInfo);
	void addNewInCard(int iCard);
	void sendOutCard(int iCard);
	void showMJPoint();
	void setActOutCard(int iCard);
	void setshanchuCard(int iCard);//ɾ������-�طŹ���

	cocos2d::Node* addHandOutCard(int iCard);
	void showHandCard();
	void showHandCard(XZDD::CMD_WeaveItem* pWeave, int iWeaveCout, byte* pHandCard, int iHandCout);
	void setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront);
	cocos2d::Node* getTouchCardNode(cocos2d::Vec2 kTouchPos);
	byte getTouchCardVlaue(cocos2d::Node* pNode);
	void showHuPai(bool bGaizhu);
	void showJieSuanCard(cocos2d::Node* pCardNode, XZDD::CMD_WeaveItem* pWeave, int iWeaveCout, byte* pHandCard, int iHandCout);
	void setChiHuCard(byte cbCard);
	void clearChiHuCard();
	byte getChiHuCard();
	void showChiHuCard();
	void removeHandCard(byte cbCard);
public:
	void removeHandOutCard( byte cbCardData );
	void sortHandCard();
	byte getCanGangCard();//��������;
	void showHuanPai(int nGameType = GAME_TYPE_CHENGDU);
	void showHuanPaiInit(int nGameType = GAME_TYPE_CHENGDU);
	void standUpCard(byte cbCard,bool nOnly = true);
	void seatDownCard();
	cocos2d::Node* getCardNodeByCard(byte cbCard);
	void getCardNodeByCard(std::vector<byte> CardDataList,std::vector<cocos2d::Node*>& kCardList);
	bool getHuanCards(byte nHuanCard[HUAN_CARD_NUM]);
	int getDefuaultHuanValue(int nIndex);//��ȡ�ڼ���Ĭ�ϻ��Ƶ�ֵ
	int getMastKindColor(); //������ƻ�ɫ
	void showCard();
	void showCard(byte* cbCardData,byte cbCardCount);
	void setHuanPai(byte* nHuan);
	void saveRemoveHuanPai(byte* nHuan);
	void saveHuanPai(byte* nHuan);
	void setHuanPai_ChengDu();

	void setQueColor(int nColor);
	byte getQueColor();
	void showXuanQue(bool bShow);
	void showQueIcon_NoAni();
	void showQueIconAni();
	void showQueIconAni_Reset();

	void blackAllHandCard(bool bBlack);
	void blackHandCardByQueColor();
	void blackHandCardByColor(int nColor);

	void runAniDianPao();
	void runAniZiMo();
	void runAniPeng();
	void runAniMingGang();
	void runAniAnGang();

	void SET_paiSHUjU(std::vector<byte>& shuju,byte data);

	void SetKeYiHuPai(std::vector<byte>  hupaiX_S);

	void showjiesuanCard(cocos2d::Node* pCardNode);//����������ʾ

	XZDD::CMD_WeaveItem* getWeaveItemArray();
	byte getWeaveCount();

	std::vector<byte>  setHuanPaiSiChuan(std::vector<XZDD::Card_Info> kCardInfoList);//�Ĵ��齫���Ʊ�
	std::vector<byte>  setHuanPaiChengDu(std::vector<XZDD::Card_Info> kCardInfoList);//�ɶ��齫���Ʊ�

public:
	byte getGangCard(byte currentCard);
	void getGangCardInfo(byte* gangCards,byte& count,byte currentCard);
	void showGangCards(byte* gangCards,byte gangCount);
	byte isChiGangCard( byte currentCard);
public:
	void ClickCard(cocos2d::Node* pCard);
	int getUpCardList(std::vector<byte>& kCardDataList);

public:
	void setHandCard_EX(std::vector<byte> kCardList);
	void setMaskHandCard(std::vector<XZDD::MasterHandCardInfo> kMasterHandCardInfo);
	void setMingPai(bool bValue);
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
	void SetTingPaidepai();
	void setHSZInitCards(bool bInit){ m_bHSZInitCards = bInit;};
	bool IsHSHSZInitCards(){return m_bHSZInitCards;};

private:
	int						m_iIdex;
	cocos2d::Node*			m_pSeatNode;

	byte					m_kHandCardData[MAX_COUNT];//��������
	byte					m_kHandCardCout;

	XZDD::CMD_WeaveItem			m_kWeaveItemArray[MAX_WEAVE];		//����˿�
	byte					m_kWeaveCount;					//�����Ŀ

	byte				         m_nQueColor;
	byte				         m_pHuanCards[3];
	std::vector<cocos2d::Node*> m_pHuanCardNodeList;
	byte						 m_nMastKindColor;

	byte						m_pSaveHuanPai[3];
	std::vector<OutCardInfo>	m_kOutCardList;
	bool						m_bPlayHSAni;

	bool						m_bGameRecord;

	byte                  m_cbChiHuCard;//��������
	cocos2d::Node*         m_pHandOutCard;

	std::vector<byte>		m_kHuCardList;//���Ʊ�

	bool					m_bMingPai;//����

	std::vector<byte>  shuzu ; //������½ǵ���
	bool					m_bBanker;//����
	bool					m_bHSZInitCards;
};

