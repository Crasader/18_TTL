#ifndef __TURNCARDLAYER_H__
#define __TURNCARDLAYER_H__

#include <cocos2d.h>
#include "CardTurnAction.h"

using namespace cardturn;
using namespace cocos2d;

class TurnCardNode : public cocos2d::Node
{
public:
	virtual bool init();
	virtual void update(float fTime) override;
	void setTurnCardData( 
		std::string backMaterialPath,
		std::string frontMaterialPath,
		std::string cylinderMaterialPath,
		std::string frontMaterialPathEnd,
		Vec2 basePos, 
		float scaleSize,
		cardTurnCompleteFunc* completeFuc);
	void setTurnCardPrc( 
		std::string backMaterialPath,
		std::string frontMaterialPath,
		std::string cylinderMaterialPath);
public:
	CREATE_FUNC(TurnCardNode);
};

#endif // __HELLOWORLD_SCENE_H__
