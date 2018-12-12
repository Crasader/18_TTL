#include "NoticeMsgBox.h"

#include "Game/Widget/WidgetFun.h"
#include "Game/Widget/WidgetManager.h"
#include "Game/Widget/WidgetSkinXMLparse.h"
#include "Game/Widget/WidgetScenceXMLParse.h"
#include "JniCross/JniFun.h"

 FV_SINGLETON_STORAGE(NoticeMsgBox);
 
NoticeMsgBox::NoticeMsgBox()
	:m_szDownHttp("www.baidu.com")
{
	init();
	WidgetScenceXMLparse xml("Script/NoticeMsgBox.xml",this);
	WidgetManager::addButtonCB("UpDataButton_Ok", this, button_selector(NoticeMsgBox::Button_Ok));
}
NoticeMsgBox::~NoticeMsgBox()
{

}
bool NoticeMsgBox::init()
{
	if (!Node::init())
	{
		return false;
	};
	return true;
}

void NoticeMsgBox::setDownLoadURL(std::string strURL )
{
	m_szDownHttp = strURL;
}

void NoticeMsgBox::Button_Ok(cocos2d::Ref*, WidgetUserInfo*)
{
	hide();
	JniFun::showWebView(m_szDownHttp.c_str());
	cocos2d::CCDirector::getInstance()->end();
}

void NoticeMsgBox::ShowNoticeMsgBox(std::string kText )
{
	WidgetFun::setText(this,"UpDataNoticeMsg1",kText);
}

void NoticeMsgBox::show()
{
	setVisible(true);
}
void NoticeMsgBox::hide()
{
	setVisible(false);
}



