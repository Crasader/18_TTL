#include "AppDelegate.h"

#include "common.h"
#include "constant.h"
#include "Tools/utilityString.h"
#include "Game/Game/GameManagerBase.h"
#include "Game/Script/SoundFun.h"

#include "Plaza/LoadingScene/GPLoadingScene.h"
#include "Plaza/GameManager/GPGameLink.h"

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
    /*初始化用户数据*/
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
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
	SoundFun::Instance().SetSoundBackground(0);

	SimpleAudioEngine::getInstance()->setEffectsVolume(0);
	SoundFun::Instance().SetSoundEffect(0);
	
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    AudioEngine::resumeAll();
	float effect_volume = UserDefault::getInstance()->getFloatForKey("effect_volume", Constant::DEFAULT_SOUND);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effect_volume);
	SoundFun::Instance().SetSoundEffect(effect_volume);

	float sound_volume = UserDefault::getInstance()->getFloatForKey("sound_volume", Constant::DEFAULT_SOUND);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(sound_volume);
	SoundFun::Instance().SetSoundBackground(sound_volume);

	dword dwRoomNO = GameManagerBase::getJoinWithRoomNO();
	if (GameManagerBase::getConecetedServer() && dwRoomNO != 0) {
		CCLOG("applicationWillEnterForeground dwRoomNo=%d", dwRoomNO);
		GPGameLink::Instance().JoinRoom(utility::toString(dwRoomNO));
	}
}
