#include "WidgetScenceXmlParse.h"

#include "WidgetFun.h"
#include "WidgetManager.h"
#include "WidgetSkinXMLparse.h"
#include "Game/Script/SoundFun.h"
#include "Tools/utilityString.h"

WidgetScenceXMLparse::WidgetScenceXMLparse(std::string xmlFileName,cocos2d::Node* pNode)
{
	_pAddNodes.push_back(pNode);
	_pRootNode = pNode;
	_xmlName = xmlFileName;
	WidgetUserInfo* pUserInfo = new WidgetUserInfo;
	pUserInfo->bRoot = true;
	pNode->setUserObject(pUserInfo);
 
	SAXParser parser;
	parser.setDelegator(this);
	std::string kFullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(xmlFileName);
	parser.parse(kFullPath.c_str());
}

void WidgetScenceXMLparse::startElement(void *ctx, const char *name, const char **atts)
{
	std::string elementName = (char*)name;
	std::map<std::string, std::string> attributeDict;
	utility::createAttributeDict(attributeDict, atts);

	if(elementName == "LoadSkin")
	{
		std::string kSkin = utility::valueForKey("Skin", attributeDict);
		WidgetSkinXMLparse kXml(kSkin);
	}
	if(elementName == "ReloadSound")
	{
		std::string kName = utility::valueForKey("SoundName", attributeDict);
		SoundFun::Instance().playEffect(kName,true);
	}
	if (elementName == "Widget")
	{
		CCAssert(_pAddNodes.size(),"m_pAddNodes");
		std::string kName = utility::valueForKey("Name",attributeDict);
		std::string kSkinTempName = utility::valueForKey("SkinTempName",attributeDict);
		WidgetInfo* pWidgetInfo = WidgetManager::Instance().getWidgetInfo(kSkinTempName,false);
		if (!pWidgetInfo)
		{
			pWidgetInfo = WidgetManager::Instance().getWidgetInfo("Error");
		}
		cocos2d::Node* pNode = WidgetManager::Instance().createWidget(pWidgetInfo,_pAddNodes.back());
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		pUserInfo->bIsSkin = false;
		pUserInfo->kName = kName;
		pUserInfo->kSkinTempName = kSkinTempName;
		_pAddNodes.push_back(pNode);
		_pWidgetInfos.push_back(pWidgetInfo);
	}
	if (elementName == "ScenceXml")
	{
		CCAssert(_pAddNodes.size(),"m_pAddNodes");
		std::string kScenceXml = utility::valueForKey("File",attributeDict);
		WidgetScenceXMLparse kXml(kScenceXml,_pAddNodes.back());
	}
	if (elementName == "Property")
	{
		CCAssert(_pAddNodes.size() > 1 && _pWidgetInfos.size(),"m_pAddNodes");
		cocos2d::Node* pNode = _pAddNodes.back();
		WidgetInfo* pWidgetInfo = _pWidgetInfos.back();
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		WidgetManager::Instance().changePropertyByType(pNode,pWidgetInfo->kType,kKey,kValue);
	}
	if (elementName == "UserInfo")
	{
		CCAssert(_pAddNodes.size() > 1 && _pWidgetInfos.size(),"m_pAddNodes");
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		cocos2d::Node* pNode = _pAddNodes.back();
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		pUserInfo->kUserInfo.insert(std::make_pair(kKey,kValue));
	}
	if (elementName == "Action")
	{
		CCAssert(_pAddNodes.size() > 1 && _pWidgetInfos.size(),"m_pAddNodes");
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		cocos2d::Node* pNode = _pAddNodes.back();
		WidgetUserInfo* pUserInfo = dynamic_cast<WidgetUserInfo*>(pNode->getUserObject());
		pUserInfo->kAction.push_back(stringKeyValue(kKey,kValue));
	}
	if (elementName == "ChildProperty")
	{
		cocos2d::Node* pNode = _pAddNodes.back();
		std::string kName = utility::valueForKey("Name",attributeDict);
		std::string kKey = utility::valueForKey("Key",attributeDict);
		std::string kValue = utility::valueForKey("Value",attributeDict);
		WidgetFun::setWidgetProperty(pNode,kName,kKey,kValue);
	}
	if (elementName == "MoveWidget")
	{
		std::string kName = utility::valueForKey("Name",attributeDict);
		cocos2d::Node* pNode = WidgetFun::getChildWidget(_pRootNode,kName);
		_pAddNodes.push_back(pNode);
	}
}

void WidgetScenceXMLparse::endElement(void *ctx, const char *name)
{
	std::string elementName = (char*)name;
	if (elementName == "Widget" && _pAddNodes.size() && _pWidgetInfos.size())
	{
		_pAddNodes.pop_back();
		_pWidgetInfos.pop_back();
	}
	if (elementName == "MoveWidget" && _pAddNodes.size() && _pWidgetInfos.size())
	{
		_pAddNodes.pop_back();
		_pWidgetInfos.pop_back();
	}
}

void WidgetScenceXMLparse::textHandler(void *ctx, const char *s, int len)
{

}
