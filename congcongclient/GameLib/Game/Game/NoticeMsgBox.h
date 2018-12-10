#pragma once
#include <cocos2d.h>
#include "Game/FV/FvSingleton.h"

struct WidgetUserInfo;

class NoticeMsgBox
	:public FvSingleton<NoticeMsgBox> 
	,public cocos2d::Node
{
public:
	NoticeMsgBox();
	~NoticeMsgBox();
	
	 bool init();
	 void show();
	 void hide();
public:
	void ShowNoticeMsgBox(std::string kText );
	void setDownLoadURL(std::string strURL );
	void Button_Ok(cocos2d::Ref*, WidgetUserInfo*);
private:
    std::string	m_szDownHttp;
};