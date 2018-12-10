#include <cocos2d.h>

#include "JniCallback.h"
#include "Game/Game/MissionWeiXin.h"
#include "Game/Game/PayMananger.h"
#include "Game/Game/UserInfo.h"
#include "Game/Game/GameManagerBase.h"
#include "Game/Game/MissionWeiXin.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <Jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

namespace JniFun
{
	extern "C" 
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_onWxShareResult(JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s", data);
			int result = atoi(data);
			pGameMan->onWeiXinResult(result);
			
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT jstring JNICALL Java_org_cocos2dx_cpp_Native_getAppID(JNIEnv* env, jclass method)
		{
			std::string& strAppID = MissionWeiXin::Instance().getAppID();
			jstring jsAppID = env->NewStringUTF(strAppID.c_str());
			cocos2d::log("JinCallBack getAppID:%s", strAppID.c_str());

			return jsAppID;
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_onLoginWithRoomNO(JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack1 :%s", data);
			int result = atoi(data);
			cocos2d::log("JinCallBack2 :%d", result);
			GameManagerBase::setJoinWithRoomNO(result);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_WxLoginGetAccessToken (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			MissionWeiXin::Instance().Req_Access_token(data);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_WxLoginGetFailToken (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			MissionWeiXin::Instance().Req_Fail_token(data);
			env->ReleaseStringUTFChars(param, data);
		}

		JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Native_WxPayResp (JNIEnv* env, jclass method, jstring param)
		{
			const char* data = env->GetStringUTFChars(param, 0);
			cocos2d::log("JinCallBack :%s",data);
			PayMananger::Instance().jni_WeiXinResoult(data);
			env->ReleaseStringUTFChars(param, data);
		}

#endif
	}

} 