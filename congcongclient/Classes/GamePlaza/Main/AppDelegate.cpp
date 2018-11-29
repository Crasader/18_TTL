#include "AppDelegate.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"

#include "CommonHeader/LocalConstant.h"
#include "GamePlaza/LoadingScene/GPLoadingScene.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "Game/Game/GameManagerBase.h"

USING_NS_CC;
using namespace experimental;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    /*��ʼ���û�����*/
    /*std::string Str = CCFileUtils::getInstance()->getWritablePath();*/
    /*HNGameManager GYZJ;
    GYZJ.init_data();*/

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("GameBase", Rect(0, 0, 1280, 720));
#else
        glview = GLViewImpl::create("GameBase");
#endif
        director->setOpenGLView(glview);
    }
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);
    register_all_packages();
    Scene *pScene = GPLoadingScene::scene();
    srand(time(0));
    director->runWithScene(pScene);
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    AudioEngine::pauseAll();
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	SoundFun::Instance().SetSoundBackground(0);

	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	SoundFun::Instance().SetSoundEffect(0);
	
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    AudioEngine::resumeAll();
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", LocalContant::DEFAULT_EFFECT);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effect_volume);
	SoundFun::Instance().SetSoundEffect(effect_volume);

	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", LocalContant::DEFAULT_SOUND);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(sound_volume);
	SoundFun::Instance().SetSoundBackground(sound_volume);

	dword dwRoomNO = GameManagerBase::getJoinWithRoomNO();
	if (dwRoomNO != 0) {
		CCLOG("applicationWillEnterForeground dwRoomNo=%d", dwRoomNO);
		GPGameLink::Instance().JoinRoom(utility::toString(dwRoomNO));
	}
}
