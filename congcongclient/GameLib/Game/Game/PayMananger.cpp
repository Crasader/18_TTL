#include "PayMananger.h"
#include "Game/GameLib.h"
#include "Game/Widget/SliderMenu.h"
#include "external/tinyxml2/tinyxml2.h"
#include "GameLib/GameNet/MCWebReq.h"
#include "Tools/core/MD5.h"
#include "GameLib/JniCross/JniFun.h"

FV_SINGLETON_STORAGE(PayMananger);

static tinyxml2::XMLElement* getPayXMLNodeForKey(const char* pKey, tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc,std::string xmlBuffer)
{
	tinyxml2::XMLElement* curNode = nullptr;

	// check the key value
	if (! pKey)
	{
		return nullptr;
	}

	do 
	{
		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
		*doc = xmlDoc;

		if (xmlBuffer.empty())
		{
			CCLOG("can not read xml file");
			break;
		}
		xmlDoc->Parse(xmlBuffer.c_str(), xmlBuffer.size());

		// get root node
		*rootNode = xmlDoc->RootElement();
		if (nullptr == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (nullptr != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}
	} while (0);

	return curNode;
}

static std::string setPayValueForKey(std::string pKey, std::string pValue,std::string xmlBuffer)
{
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	// check the params
	if (pKey=="")
	{
		return xmlBuffer;
	}
	// find the node
	node = getPayXMLNodeForKey(pKey.c_str(), &rootNode, &doc,xmlBuffer);
	// if node exist, change the content
	if (node)
	{
		if (node->FirstChild())
		{
			node->FirstChild()->SetValue(pValue.c_str());
		}
		else
		{
			tinyxml2::XMLText* content = doc->NewText(pValue.c_str());
			node->LinkEndChild(content);
		}
	}
	else
	{
		if (rootNode)
		{
			tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey.c_str());//new tinyxml2::XMLElement(pKey);
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(pValue.c_str());//new tinyxml2::XMLText(pValue);
			tmpNode->LinkEndChild(content);
		}    
	}

	// save file and free doc
	if (doc)
	{
		tinyxml2::XMLPrinter stream;
		doc->Print( &stream );
		return stream.CStr();
	}
	return xmlBuffer;
}

std::string getPayForKey(const char* pKey, std::string xmlBuffer)
{
	const char* value = nullptr;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getPayXMLNodeForKey(pKey, &rootNode, &doc,xmlBuffer);
	// find the node
	if (node && node->FirstChild())
	{
		return (const char*)(node->FirstChild()->Value());
	}
	return "";
}
PayMananger::PayMananger()
	:m_iItemID(-1)
	,m_kShopInfoMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	m_kShopInfoMission.setMissionSink(this);
}
PayMananger::~PayMananger()
{

}
void PayMananger::clearItem()
{
	m_iItemID = -1;
}
bool PayMananger::isInShopping()
{
	return m_iItemID != -1;
}
void PayMananger::test(int iShopID)
{
	std::string kTimeStr = utility::toString(time(NULL));
	WeiXinPayInfo kWeiXinPayInfo;
	kWeiXinPayInfo.appid = "wx26ebc7a3307ea22a";
	kWeiXinPayInfo.attach = "attach";//��������
	kWeiXinPayInfo.body = "APPzhifu";//��Ʒ����
	kWeiXinPayInfo.mch_id = "1374879202";//�̻���
	kWeiXinPayInfo.nonce_str = kTimeStr;//����ַ���
	kWeiXinPayInfo.notify_url = "182.110.11.40";
	kWeiXinPayInfo.out_trade_no = kTimeStr;//�̻�������
	kWeiXinPayInfo.spbill_create_ip = "182.110.11.40";
	kWeiXinPayInfo.total_fee = "1";//�ܽ��
	kWeiXinPayInfo.trade_type = "APP";
	kWeiXinPayInfo.key = "bosengame12345679123456789123456";//΢���̻���Կ����
	startWeiXinPay(kWeiXinPayInfo,1);
}
void PayMananger::startWeiXinPay(WeiXinPayInfo& kWeiXinPayInfo,int iItemID)
{
	if (isInShopping())
	{
		return;
	}
	m_iItemID = iItemID;
	m_kWeiXinPayInfo = kWeiXinPayInfo;

	if (kWeiXinPayInfo.attach == "")
	{
		kWeiXinPayInfo.attach = "attach";
	}
	if (kWeiXinPayInfo.body == "")
	{
		kWeiXinPayInfo.body = "body";
	}

	std::string kStrXML="<xml></xml>"; 
	kStrXML = setPayValueForKey("appid",kWeiXinPayInfo.appid,kStrXML);
	kStrXML = setPayValueForKey("attach",kWeiXinPayInfo.attach,kStrXML);
	kStrXML = setPayValueForKey("body",kWeiXinPayInfo.body,kStrXML);
	kStrXML = setPayValueForKey("mch_id",kWeiXinPayInfo.mch_id,kStrXML);
	kStrXML = setPayValueForKey("nonce_str",kWeiXinPayInfo.nonce_str,kStrXML);
	kStrXML = setPayValueForKey("notify_url",kWeiXinPayInfo.notify_url,kStrXML);
	kStrXML = setPayValueForKey("out_trade_no",kWeiXinPayInfo.out_trade_no,kStrXML);
	kStrXML = setPayValueForKey("spbill_create_ip",kWeiXinPayInfo.spbill_create_ip,kStrXML);
	kStrXML = setPayValueForKey("total_fee",kWeiXinPayInfo.total_fee,kStrXML);
	kStrXML = setPayValueForKey("trade_type",kWeiXinPayInfo.trade_type,kStrXML);

	std::string kStr1;
	MCWebReq::pushValue(kStr1,"appid",kWeiXinPayInfo.appid);
	MCWebReq::pushValue(kStr1,"attach",kWeiXinPayInfo.attach);
	MCWebReq::pushValue(kStr1,"body",kWeiXinPayInfo.body);
	MCWebReq::pushValue(kStr1,"mch_id",kWeiXinPayInfo.mch_id);
	MCWebReq::pushValue(kStr1,"nonce_str",kWeiXinPayInfo.nonce_str);
	MCWebReq::pushValue(kStr1,"notify_url",kWeiXinPayInfo.notify_url);
	MCWebReq::pushValue(kStr1,"out_trade_no",kWeiXinPayInfo.out_trade_no);
	MCWebReq::pushValue(kStr1,"spbill_create_ip",kWeiXinPayInfo.spbill_create_ip);
	MCWebReq::pushValue(kStr1,"total_fee",kWeiXinPayInfo.total_fee);
	MCWebReq::pushValue(kStr1,"trade_type",kWeiXinPayInfo.trade_type);
	MCWebReq::pushValue(kStr1,"key",kWeiXinPayInfo.key);

	std::string kStrMD5 = md5(kStr1);
	kStrXML = setPayValueForKey("sign",kStrMD5,kStrXML);

	MCWebReq::instance().sendRequestStr("https://api.mch.weixin.qq.com/pay/unifiedorder",CC_CALLBACK_1(PayMananger::CB_StartWeiXinPay,this),kStrXML);
}
void PayMananger::CB_StartWeiXinPay(std::string kStr)
{
	std::string kErrorMsg = getPayForKey("return_msg",kStr);
	if (kErrorMsg == "OK")
	{
		NoticeMsg::Instance().ShowTopMsg(kErrorMsg);

		std::string appId = m_kWeiXinPayInfo.appid;
		std::string nonceStr = m_kWeiXinPayInfo.nonce_str;
		std::string packageValue = "Sign=WXPay";
		std::string partnerId = m_kWeiXinPayInfo.mch_id;
		m_kWeiXinPrepay_id = getPayForKey("prepay_id",kStr);
		std::string timeStamp = m_kWeiXinPayInfo.nonce_str;

		std::string kStr1;
		MCWebReq::pushValue(kStr1,"appid",appId);
		MCWebReq::pushValue(kStr1,"noncestr",nonceStr);
		MCWebReq::pushValue(kStr1,"package",packageValue);
		MCWebReq::pushValue(kStr1,"partnerid",partnerId);
		MCWebReq::pushValue(kStr1,"prepayid",m_kWeiXinPrepay_id);
		MCWebReq::pushValue(kStr1,"timestamp",timeStamp);
		MCWebReq::pushValue(kStr1,"key",m_kWeiXinPayInfo.key);
		std::string kStrMD5 = md5(kStr1);

		std::string kSendStr = "";
		kSendStr += appId;
		kSendStr += "|";
		kSendStr += partnerId;
		kSendStr += "|";
		kSendStr += m_kWeiXinPrepay_id;
		kSendStr += "|";
		kSendStr += nonceStr;
		kSendStr += "|";
		kSendStr += timeStamp;
		kSendStr += "|";
		kSendStr += packageValue;
		kSendStr += "|";
		kSendStr += kStrMD5;

		JniFun::startWeiXinPay(kSendStr);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		jni_WeiXinResoult("0");
#endif
	}
	else
	{
		NoticeMsg::Instance().ShowTopMsg(utility::toString("WeiXinOder Error = ",kErrorMsg));
		clearItem();
	}
}
void PayMananger::jni_WeiXinResoult(std::string kErrorCode)
{
	NoticeMsg::Instance().ShowTopMsg(kErrorCode);
	if (kErrorCode == "0")
	{
		CMD_GP_UpShopOder kUpShoperID;
		kUpShoperID.appid = m_kWeiXinPayInfo.appid;
		kUpShoperID.dwItemID = m_iItemID;
		kUpShoperID.dwUserID = UserInfo::Instance().getUserID();
		kUpShoperID.mch_id = m_kWeiXinPayInfo.mch_id;
		kUpShoperID.prepay_id = m_kWeiXinPrepay_id;
		kUpShoperID.total_fee = utility::parseInt(m_kWeiXinPayInfo.total_fee);
		m_kShopInfoMission.UpShopOder(kUpShoperID);
	}
	clearItem();
}
