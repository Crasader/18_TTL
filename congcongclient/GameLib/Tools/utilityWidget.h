#ifndef _UTILITY_WIDGET_H_
#define _UTILITY_WIDGET_H_

#include <cocos2d.h>
#include "utilityString.h"
#include "Game/Script/StructType.h"

using namespace cocos2d;
using namespace script;

namespace utility
{
//////////////////////////////////////////////////////////////////////////

#define FullPath(fileName) cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName)

	inline std::vector<std::string> getFilePath()
	{
		std::string kPathTxt = "FilePath";
		ssize_t iSize = 0;
		char* pChar = (char*)cocos2d::FileUtils::getInstance()->getFileData(FullPath(kPathTxt), "rb", &iSize);
		if (!pChar)
		{
			return std::vector<std::string>();
		}
		std::string kStr = pChar;
		free(pChar);
		return split(kStr, "|");
	}

	inline void setSpriteScaleBySize(cocos2d::Node* pSprite, cocos2d::Size kSize)
	{
		if (!pSprite)
		{
			return;
		}
		float fScaleX = 0;
		float fScaleY = 0;
		fScaleX = kSize.width / (float)pSprite->getContentSize().width;
		fScaleY = kSize.height / (float)pSprite->getContentSize().height;
		pSprite->setScaleX(fScaleX);
		pSprite->setScaleY(fScaleY);
	}

	inline cocos2d::Vec2 getOffectParentPos(cocos2d::Node* pNode, cocos2d::Vec2 kOffect)
	{
		cocos2d::Mat4 tmp = pNode->getNodeToParentTransform();
		tmp.m[12] = 0;
		tmp.m[13] = 0;
		cocos2d::Vec3 vec3(kOffect.x, kOffect.y, 0);
		cocos2d::Vec3 ret;
		tmp.transformPoint(vec3, &ret);
		return cocos2d::Vec2(ret.x, ret.y);
	}

	inline void schedule(cocos2d::SEL_SCHEDULE selector, cocos2d::Ref* pTarget)
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(selector, pTarget, 0.0f, false);
	}

	inline void unschedule(cocos2d::SEL_SCHEDULE selector, cocos2d::Ref* pTarget)
	{
		cocos2d::Director::getInstance()->getScheduler()->unschedule(selector, pTarget);
	}

	inline void scheduleOnce(cocos2d::SEL_SCHEDULE selector, cocos2d::Ref* pTarget, float interval)
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(selector, pTarget, interval, 0, 0.0f, false);
	}

	inline std::string toString(cocos2d::Vec2 p)
	{
		return toString(p.x, " ", p.y);
	}

	inline std::string toString(cocos2d::Size p)
	{
		return toString(p.width, " ", p.height);
	}

	inline void CCAssertWithDes(bool bAssert, std::string kDes1, std::string kDes2)
	{
		if (bAssert)
		{
			return;
		}
		std::string kDes = kDes1 + kDes2;
		CCAssert(bAssert, kDes.c_str());
	}

	inline cocos2d::Vec2 parsePoint(std::string kValue)
	{
		cocos2d::Vec2 kPoint;
		std::vector<std::string> kStr = split(kValue, " ");
		if (kStr.size() == 2)
		{
			kPoint.x = parseFloat(kStr[0]);
			kPoint.y = parseFloat(kStr[1]);
		}
		else
		{
			utility::CCAssertWithDes(false, kValue, " parseRect Error");
		}
		return kPoint;
	}

	inline std::vector<cocos2d::Vec2> parsePoints(std::string kValue)
	{
		auto vctStrPos = utility::split(kValue, ":");
		std::vector<cocos2d::Vec2> pos;
		for (size_t idx = 0; idx < vctStrPos.size(); idx++) {
			pos.push_back(utility::parsePoint(vctStrPos[idx]));
		}
		return pos;
	}

	inline cocos2d::Rect parseRect(std::string kValue)
	{
		cocos2d::Rect kRect;
		std::vector<std::string> kStr = split(kValue, " ");
		if (kStr.size() == 4)
		{
			kRect.origin.x = parseFloat(kStr[0]);
			kRect.origin.y = parseFloat(kStr[1]);
			kRect.size.width = parseFloat(kStr[2]);
			kRect.size.height = parseFloat(kStr[3]);
		}
		else
		{
			utility::CCAssertWithDes(false, kValue," parseRect Error");
		}
		return kRect;
	}

	inline RandFloat parseRandFloat(std::string kStr)
	{
		RandFloat kValue;
		std::vector<std::string> kVec = utility::split(kStr, ":");
		if (kVec.size() && kVec[0] == "Key")
		{
			kValue.kUserKey = kVec[1];
		}
		else if (kVec.size() == 1)
		{
			float fTemp = utility::parseFloat(kVec[0]);
			kValue.fMin = fTemp;
			kValue.fMax = fTemp;
		}
		else if (kVec.size() == 2)
		{
			kValue.fMin = utility::parseFloat(kVec[0]);
			kValue.fMax = utility::parseFloat(kVec[1]);
		}
		else
		{
			utility::CCAssertWithDes(false, kStr, " parseRect Error");
		}
		return kValue;
	}
	
	inline RandPos parseRandPos(std::string kStr)
	{
		RandPos kValue;
		std::vector<std::string> kVec = utility::split(kStr, ":");
		if (kVec.size() && kVec[0] == "Key")
		{
			kValue.kUserKey = kVec[1];
		}
		else if (kVec.size() == 1)
		{
			cocos2d::Vec2 kPos = utility::parsePoint(kVec[0]);
			kValue.kRandX.fMin = kPos.x;
			kValue.kRandX.fMax = kPos.x;
			kValue.kRandY.fMin = kPos.y;
			kValue.kRandY.fMax = kPos.y;
		}
		else if (kVec.size() == 2)
		{
			cocos2d::Vec2 kPos1 = utility::parsePoint(kVec[0]);
			cocos2d::Vec2 kPos2 = utility::parsePoint(kVec[1]);
			kValue.kRandX.fMin = kPos1.x;
			kValue.kRandX.fMax = kPos2.x;
			kValue.kRandY.fMin = kPos1.y;
			kValue.kRandY.fMax = kPos2.y;
		}
		else
		{
			utility::CCAssertWithDes(false, kStr, " parseRect Error");
		}
		return kValue;
	}
	
	inline cocos2d::Size parseSize(std::string kValue)
	{
		cocos2d::Size kSize;
		std::vector<std::string> kStr = split(kValue, " ");
		if (kStr.size() == 2)
		{
			kSize.width = parseFloat(kStr[0]);
			kSize.height = parseFloat(kStr[1]);
		}
		else
		{
			utility::CCAssertWithDes(false, kValue, " parseRect Error");
		}
		return kSize;
	}

	inline float getRandFloat(const RandFloat& kRand)
	{
		int iRand = (int)((kRand.fMax - kRand.fMin) * 10);
		if (iRand == 0)
		{
			return kRand.fMin;
		}
		if (iRand < 0)
		{
			return kRand.fMax + (float)(rand() % (-iRand) / 10.0f);
		}
		return kRand.fMin + (float)(rand() % (iRand) / 10.0f);
	}

	inline cocos2d::Color3B parseColour(std::string kValue)
	{
		cocos2d::Color3B kColour = cocos2d::Color3B(255, 255, 255);
		cocos2d::Size kSize;
		std::vector<std::string> kStr = split(kValue, " ");
		if (kStr.size() == 3)
		{
			kColour.r = static_cast<GLubyte>(parseFloat(kStr[0]));
			kColour.g = static_cast<GLubyte>(parseFloat(kStr[1]));
			kColour.b = static_cast<GLubyte>(parseFloat(kStr[2]));
		}
		else
		{
			utility::CCAssertWithDes(false, kValue, " parseColour Error");
		}
		return kColour;
	}
	
	inline cocos2d::Color4B parseColour4(std::string kValue)
	{
		cocos2d::Color4B kColour = cocos2d::Color4B(255, 255, 255, 255);
		cocos2d::Size kSize;
		std::vector<std::string> kStr = split(kValue, " ");
		if (kStr.size() == 4)
		{
			kColour.r = static_cast<GLubyte>(parseFloat(kStr[0]));
			kColour.g = static_cast<GLubyte>(parseFloat(kStr[1]));
			kColour.b = static_cast<GLubyte>(parseFloat(kStr[2]));
			kColour.a = static_cast<GLubyte>(parseFloat(kStr[3]));
		}
		else
		{
			utility::CCAssertWithDes(false, kValue, " parseColour Error");
		}
		return kColour;
	}

	inline UserString parseUserString(std::string kStr)
	{
		UserString kValue;
		std::vector<std::string> kVec = utility::split(kStr, ":");
		if (kVec.size() && kVec[0] == "Key")
		{
			kValue.kUserKey = kVec[1];
		}
		else if (kVec.size() == 1)
		{
			kValue.kUserValue = kVec[0];
		}
		else
		{
			CCASSERT(false, "");
		}
		return kValue;
	}

	inline cocos2d::Vec2 valueForPoint(const char *key, std::map<std::string, std::string>& dict)
	{
		return parsePoint(valueForKey(key, dict));
	}

	inline cocos2d::Size valueForSize(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseSize(valueForKey(key, dict));
	}

	inline cocos2d::Rect valueForRect(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseRect(valueForKey(key, dict));
	}

	inline RandFloat valueForRandFloat(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseRandFloat(valueForKey(key, dict));
	}

	inline RandPos valueForRandPos(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseRandPos(valueForKey(key, dict));
	}

	inline void valueForFloat4(float* fValue, const char *key, std::map<std::string, std::string>& dict)
	{
		std::string kStr = valueForKey(key, dict);
		std::vector<std::string> kVec = utility::split(kStr, ":");
		float fTempValue = 0;
		int iIdex = 0;
		for (iIdex = 0; iIdex < 4 && iIdex < (int)kVec.size(); iIdex++)
		{
			fValue[iIdex] = static_cast<float>(parseInt(kVec[iIdex]));
			fTempValue = fValue[iIdex];
		}
		for (; iIdex < 4; iIdex++)
		{
			fValue[iIdex] = fTempValue;
		}
	}

	inline void valueForInt4(int* iValue, const char *key, std::map<std::string, std::string>& dict)
	{
		std::string kStr = valueForKey(key, dict);
		std::vector<std::string> kVec = utility::split(kStr, ":");
		CCAssert(kVec.size() != 0, "");
		int iTempValue = 0;
		int iIdex = 0;
		for (iIdex = 0; iIdex < 4 && iIdex < (int)kVec.size(); iIdex++)
		{
			iValue[iIdex] = parseInt(kVec[iIdex]);
			iTempValue = iValue[iIdex];
		}
		for (; iIdex < 4; iIdex++)
		{
			iValue[iIdex] = iTempValue;
		}
	}

	inline UserString valueForUserString(const char *key, std::map<std::string, std::string>& dict)
	{
		return parseUserString(valueForKey(key, dict));
	}

	inline bool isAllVisible(cocos2d::Node* pNode)
	{
		while (pNode)
		{
			if (!pNode->isVisible())
			{
				return false;
			}
			pNode = pNode->getParent();
		}
		return true;
	}

	inline bool IsSpriteInTouch(cocos2d::Vec2 kWorldPos, cocos2d::Sprite *pSrite)
	{
		kWorldPos = pSrite->convertToNodeSpace(kWorldPos);
		cocos2d::Size elementSize = pSrite->getContentSize();
		cocos2d::Rect elementRect;
		cocos2d::Vec2 kAnchor = pSrite->getAnchorPoint();
		elementRect = cocos2d::Rect(
			0, 0,
			elementSize.width,
			elementSize.height
		);
		return elementRect.containsPoint(kWorldPos);
	}
}

#endif
