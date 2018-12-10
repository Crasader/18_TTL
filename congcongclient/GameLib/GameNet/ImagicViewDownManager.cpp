#include "ImagicViewDownManager.h"
#include "Game/Widget/WidgetFun.h"
#include "Tools/utilityString.h"

FV_SINGLETON_STORAGE(ImagicViewDownManager);

ImagicViewDownManager::ImagicViewDownManager()
	:m_iIdexCout(0)
{
	cocos2d::Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(ImagicViewDownManager::upTime),this,0.0f,false);
}
ImagicViewDownManager::~ImagicViewDownManager(void)
{
	cocos2d::Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(ImagicViewDownManager::upTime),this);
}
std::string DownImageViewGetFileName(std::string kName)
{
	std::string::size_type iStart = 0;
	while (true)
	{
		std::string::size_type iTempPos = kName.find("/",iStart);
		if (iTempPos == std::string::npos)
		{
			break;
		}
		iStart = iTempPos;
		iStart++;
	}
	size_t iEnd = kName.size();
	return kName.substr(iStart,iEnd-iStart);
}
void ImagicViewDownManager::GetImagic(gamelib::network::HttpClientEx *sender, gamelib::network::HttpResponseEx *response)
{
	response->retain();
	m_pGetList.push_back(response);
}
void ImagicViewDownManager::OnImagic()
{
	if (m_pGetList.size() == 0)
	{
		return;
	}
	gamelib::network::HttpResponseEx *response = m_pGetList[0];
	//m_pGetList.erase(m_pGetList.begin());
	std::string kUrl = response->getHttpRequest()->getUrl();
	std::string kImagicName = cocos2d::FileUtils::getInstance()->getWritablePath()+"test.png";

	ImagicViewDownInfoList::iterator itor1 = m_pDownList.begin();
	while(itor1 != m_pDownList.end())
	{
		ImagicViewDownInfo& kInfo = *itor1;
		if (kInfo.kUrl.find(kUrl.c_str()) != std::string::npos)
		{
			kImagicName = kInfo.kImagicName;
			break;
		}
		itor1++;
	}
	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() == 0)
	{
		return;
	}
	cocos2d::CCImage* img = new cocos2d::CCImage;
	img->initWithImageData((unsigned char*)buffer->data(),buffer->size());
	bool bSucess = false;
	if (img->getData())
	{
		img->saveToFile(kImagicName,false);
		bSucess = true;
	}
	delete img;

	ImagicViewDownInfoList::iterator itor2 = m_pDownList.begin();
	while(itor2 != m_pDownList.end())
	{
		ImagicViewDownInfo& kInfo = *itor2;
        if (kInfo.kUrl.find(kUrl.c_str()) != std::string::npos)
		{
			if (kInfo.pSprite)
			{
				if (bSucess)
				{
					kInfo.pSprite->loadTexture(kImagicName,cocos2d::ui::TextureResType::LOCAL);
				}
				kInfo.pSprite->release();
			}
			itor2 = m_pDownList.erase(itor2);
		}
		else
		{
			itor2++;
		}
	}
	response->release();
	m_pGetList.erase(m_pGetList.begin());
}
cocos2d::Texture2D* ImagicViewDownManager::getDownTexture(std::string kFile)
{
	cocos2d::Texture2D* pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(kFile.c_str());
	if (pTexture)
	{
		return pTexture;
	}
	pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(utility::toString("head/",kFile.c_str()));
	if (pTexture)
	{
		return pTexture;
	}
	
	std::string kImagicName = DownImageViewGetFileName(kFile);
	kImagicName = cocos2d::FileUtils::getInstance()->getWritablePath()+kImagicName;
	std::string StrSavePath = kImagicName;
	pTexture = cocos2d::CCTextureCache::sharedTextureCache()->addImage(StrSavePath.c_str());
	if (pTexture)
	{
		return pTexture;
	}
	return NULL;
}
void ImagicViewDownManager::addDown(cocos2d::ui::ImageView* pSprite,std::string kUrl,int iUserID,bool bSameSize )
{
	int iIdex = 0;
	for (size_t i = 0;i<kUrl.size();i++)
	{
		iIdex += kUrl[i];
	}
	std::string strSrc = "/0";
	std::string strDes = "/132";
	std::string::size_type pos = 0;  
	std::string::size_type srcLen = strSrc.size();  
	std::string::size_type desLen = strDes.size();  
	pos=kUrl.find(strSrc, pos);   
	while (pos != std::string::npos)
	{  
		if (kUrl.find(strSrc, (pos+desLen)) == std::string::npos)
		{
			kUrl.replace(pos, srcLen, strDes);
			break;
		}
		pos = kUrl.find(strSrc, (pos+desLen));
	}  
	std::string kFileName = utility::toString(iUserID,"Idex",iIdex);
	addDown(pSprite,kUrl,kFileName,bSameSize);
}
void ImagicViewDownManager::addDown(cocos2d::ui::ImageView* pSprite,std::string kUrl,std::string kFileName,bool bSameSize )
{
	if (kUrl == "")
	{
		return;
	}
	if (!pSprite)
	{
		CCASSERT(false,"");
		return;
	}
	kFileName = cocos2d::FileUtils::getInstance()->getWritablePath()+kFileName+".png";
	std::string StrSavePath = kFileName;
	if (cocos2d::CCTextureCache::sharedTextureCache()->addImage(StrSavePath.c_str()))
	{
		if (pSprite)
		{
			pSprite->loadTexture(StrSavePath,cocos2d::ui::TextureResType::LOCAL);
		}
		return;
	}
	bool bHaveGet = false;
	for (int i = 0;i<(int)m_pDownList.size();i++)
	{
		ImagicViewDownInfo& kInfo = m_pDownList[i];
		if (kInfo.kImagicName == kFileName && kInfo.pSprite == pSprite)
		{
			return;
		}
		if (kInfo.kImagicName == kFileName)
		{
			bHaveGet = true;
		}
		
	}
	ImagicViewDownInfo kInfo;
	kInfo.pSprite = pSprite;
	kInfo.kImagicName = kFileName;
	kInfo.kUrl = kUrl;
	kInfo.bSameSize = bSameSize;
	if (pSprite)
	{
		pSprite->retain();
	}
	if (!bHaveGet)
	{
		gamelib::network::HttpRequestEx* pRequest = MCWebReq::instance().getFileEx(kUrl,
			 CC_CALLBACK_2(ImagicViewDownManager::GetImagic, this));
		kInfo.pRequest = pRequest;
	}
	m_pDownList.push_back(kInfo);
}
void ImagicViewDownManager::upTime(float fTime)
{
	OnImagic();
	ImagicViewDownInfoList::iterator itor = m_pDownList.begin();
	while(itor != m_pDownList.end())
	{
		ImagicViewDownInfo& kInfo = *itor;
		kInfo.fActTime += fTime;
		if (kInfo.fActTime > 20.0f)
		{
			if (kInfo.pSprite)
			{
				kInfo.pSprite->release();
			}
			itor = m_pDownList.erase(itor);
		}
		else
		{
			itor++;
		}
	}
}