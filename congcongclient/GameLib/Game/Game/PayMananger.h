#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Platform/PFKernel/CGPShopInfoMission.h"

struct WeiXinPayInfo
{
	std::string appid;
	std::string attach;
	std::string body;
	std::string mch_id;
	std::string nonce_str;
	std::string notify_url;
	std::string out_trade_no;
	std::string spbill_create_ip;
	std::string total_fee;
	std::string trade_type;

	std::string key;
};

class PayMananger
	:public cocos2d::Ref
	,public CGShopInfoMissionSink
	,public FvSingleton<PayMananger> 
{
public:
	PayMananger();
	~PayMananger();
public:
	void clearItem();
	bool isInShopping();
	void test(int iShopID);
	void startWeiXinPay(WeiXinPayInfo& kWeiXinPayInfo,int iItemID);
	void CB_StartWeiXinPay(std::string kStr);
	void jni_WeiXinResoult(std::string kErrorCode);
public:
	std::string m_kWeiXinPrepay_id;
	WeiXinPayInfo m_kWeiXinPayInfo;
public:
	int m_iItemID;
	CGPShopInfoMission	m_kShopInfoMission;
};