#ifndef __UTILITY_MATH_H__
#define __UTILITY_MATH_H__

#include <cocos2d.h>
#include "utilityString.h"

namespace utility
{

template<typename T>
inline T tmin(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
inline T tmax(const T& a, const T& b)
{
	return a < b ? b : a;
}

template<typename T>
inline bool haveInVector(const std::vector<T>& pVector, T pValue)
{
	typename std::vector<T>::const_iterator itor = pVector.begin();
	for (; itor != pVector.end(); itor++)
	{
		if (*itor == pValue)
		{
			return true;
		}
	}
	return false;
}

template<class T>
inline bool removeVectorByValue(std::vector<T>& a, const T& b)
{
	typename std::vector<T>::iterator itor = a.begin();
	for (; itor != a.end(); itor++)
	{
		if ((*itor) == b)
		{
			a.erase(itor);
			return true;
		}
	}
	return false;
}

template<class T>
inline bool haveInVectorByValue(std::vector<T>& a, const T& b)
{
	typename std::vector<T>::iterator itor = a.begin();
	for (; itor != a.end(); itor++)
	{
		if ((*itor) == b)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
inline T Abs (const T& iValue)
{
	return iValue > 0 ? iValue : -iValue;
}

inline float fAbs(const float& fValue)
{
	if (fValue < 0.f)
	{
		return -fValue;
	}
	return fValue;
}

template<typename T>
inline T Min (const T& iValue1, const T& iValue2)
{
	return  (((iValue1) < (iValue2)) ? (iValue1) : (iValue2));
}

inline float fMin(const float& fValue1, const float& fValue2)
{
	if (fValue1 < fValue2)
	{
		return fValue1;
	}
	return fValue2;
}

template<typename T>
inline T Max (const T& iValue1, const T& iValue2)
{
	return  (((iValue1) > (iValue2)) ? (iValue1) : (iValue2));
}

inline float fMax(const float& fValue1, const float& fValue2)
{
	if (fValue1 < fValue2)
	{
		return fValue2;
	}
	return fValue1;
}

inline float toDest(const float& fSart, const float& fDest, const float& fAdd)
{
	float fReturn = fSart;
	if (fSart < fDest)
	{
		fReturn = fSart + fAbs(fAdd);
		fReturn = fMin(fDest, fReturn);
	}
	else
	{
		fReturn = fSart - fAbs(fAdd);
		fReturn = fMax(fDest, fReturn);
	}
	return fReturn;
}

inline float getMoveTime(cocos2d::Vec2 kSrc, cocos2d::Vec2 kDest, float fSpd)
{
	if (fSpd <= 0.00f)
	{
		return 0.f;
	}
	return kSrc.distance(kDest) / fSpd;
}

inline float getMoveTime(float fSrc, float fDest, float fSpd)
{
	if (fSpd <= 0.00f)
	{
		return 0.f;
	}
	return fAbs(fSrc - fDest) / fSpd;
}

inline cocos2d::Vec2 getDistancePos(cocos2d::Vec2 kSrc, cocos2d::Vec2 kDest, float fDistance)
{
	if (kSrc.x == kDest.x && kSrc.y == kDest.y)
	{
		return kSrc;
	}
	if (fDistance < 0.001f)
	{
		return kSrc;
	}
	cocos2d::Vec2 kDistance = (kDest - kSrc);
	float fTempDistacne = kDistance.getLength();
	if (fTempDistacne < 0.001f)
	{
		return kSrc;
	}
	cocos2d::Vec2 kReturn = kSrc;
	kReturn.x += kDistance.x / fTempDistacne * fDistance;
	kReturn.y += kDistance.y / fTempDistacne * fDistance;

	return kReturn;
}

inline bool MoveTo(float fSrc, float fDest, float& fkOut, float fDistance)
{
	float fDis = fAbs(fSrc - fDest);
	if (fDis < 0.001f)
	{
		fkOut = fDest;
		return true;
	}
	float fPercent = fDistance / fDis;
	if (fPercent >= 1.0f)
	{
		fkOut = fDest;
		return false;
	}
	fkOut = fSrc + (fDest - fSrc)*fPercent;
	return false;
}

inline bool MoveTo(cocos2d::Vec2 kSrc, cocos2d::Vec2 kDest, cocos2d::Vec2& kOut, float fDistance)
{
	float fDis = kSrc.getDistance(kDest);
	if (fDis < 0.0001f)
	{
		kOut = kDest;
		return true;
	}
	float fPercent = fDistance / fDis;
	if (fPercent >= 1.0f)
	{
		kOut = kDest;
		return true;
	}
	kOut.x = kSrc.x + (kDest.x - kSrc.x)*fPercent;
	kOut.y = kSrc.y + (kDest.y - kSrc.y)*fPercent;
	return false;
}

inline cocos2d::Vec2 getAnglePos(cocos2d::Vec2 kSrc, cocos2d::Vec2 kDest, float fAngle)
{
	if (kSrc.x == kDest.x && kSrc.y == kDest.y)
	{
		return kSrc;
	}
	cocos2d::Vec2 kDistance = (kDest - kSrc);
	float fDistance = kDistance.getLength();
	float fTempAngle = kDistance.getAngle() - fAngle;
	cocos2d::Vec2 kReturn = kSrc;
	kReturn.x += fDistance * cos(fTempAngle);
	kReturn.y += fDistance * sin(fTempAngle);
	return kReturn;
}

inline cocos2d::Vec2 getPosByDistanceAngle(float fDistance, float fRoata)
{
	cocos2d::Vec2 kReturn;
	kReturn.x = fDistance * cos((90 - fRoata) / 180.f*3.14f);
	kReturn.y = fDistance * sin((90 - fRoata) / 180.f*3.14f);
	return kReturn;
}

inline cocos2d::Vec2 getPosByAngle(cocos2d::Vec2 kDest, float fRoata)
{
	float fDistance = kDest.getLength();
	float fAngle = kDest.getAngle();
	cocos2d::Vec2 kReturn;
	kReturn.x = static_cast<float>(fDistance * cos((90 - fRoata) / 180.f*3.14f + fAngle - 3.14 / 2.0f));
	kReturn.y = static_cast<float>(fDistance * sin((90 - fRoata) / 180.f*3.14f + fAngle - 3.14 / 2.0f));
	return kReturn;
}

inline float getRotation(cocos2d::Vec2 kSrcPos, cocos2d::Vec2 kDestPoint)
{
	cocos2d::Vec2 kDestTance = kDestPoint - kSrcPos;
	float fAngle = kDestTance.getAngle();
	return (-fAngle * 180.0f / 3.14f);
}

}

#endif