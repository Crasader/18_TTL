#pragma once
#include "Game/FV/FvSingleton.h"
#include "cocos2d.h"
#include "MCWebReq.h"
#include "cocos2d/cocos/ui/UIImageView.h"

struct ImagicViewDownInfo
{
	ImagicViewDownInfo()
	{
		fActTime = 0;
		pRequest = NULL;
		pSprite  = NULL;
		bSameSize = false;
	}
	cocos2d::ui::ImageView* pSprite;
	cocos2d::network::HttpRequestEx* pRequest;
	std::string kUrl;
	std::string kImagicName;
	float fActTime;
	bool bSameSize;
};

typedef std::vector<ImagicViewDownInfo> ImagicViewDownInfoList;

class ImagicViewDownManager
	:public FvSingleton<ImagicViewDownManager> 
	,public cocos2d::Ref
{
public:
	ImagicViewDownManager();
	~ImagicViewDownManager();
public:
	void OnImagic();
	void GetImagic(cocos2d::network::HttpClientEx *sender, cocos2d::network::HttpResponseEx *response);
public:
	cocos2d::Texture2D* getDownTexture(std::string kFile);
	void addDown(cocos2d::ui::ImageView* pSprite,std::string kUrl,int iUserID,bool bSameSize = true);
	void addDown(cocos2d::ui::ImageView* pSprite,std::string kUrl,std::string kFileName,bool bSameSize = true);
	void upTime(float fTime);
public:
	int m_iIdexCout;
	ImagicViewDownInfoList m_pDownList;
	std::vector<cocos2d::network::HttpResponseEx*> m_pGetList;
};

