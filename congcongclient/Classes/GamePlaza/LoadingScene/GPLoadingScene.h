#ifndef __GPLOADING_SCENE_H__
#define __GPLOADING_SCENE_H__

#include "cocos2d.h"

class GPLoadingScene : public cocos2d::CCLayer
{
public:
    virtual bool init();  
	virtual bool init_2();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(GPLoadingScene);

	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>&touches, cocos2d::Event *unused_event);
	virtual void update(float delta);
	virtual void onExit();
	void exitRoom();
};

#endif // __GPLOADING_SCENE_H__
