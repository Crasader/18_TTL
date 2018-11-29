#pragma once
#include <assert.h>

class ScenceManagerBase
{
protected:
	ScenceManagerBase() {};
	~ScenceManagerBase() {};
public:
	virtual void GameBackScence() = 0;
	virtual void setInstance(ScenceManagerBase* pInstance) {
		assert(scene_man_instance == nullptr);
		scene_man_instance = pInstance;
	}
	static ScenceManagerBase* pScenceManager() {
		assert(scene_man_instance != nullptr);
		return scene_man_instance;
	};
protected:
	static ScenceManagerBase* scene_man_instance;
};
