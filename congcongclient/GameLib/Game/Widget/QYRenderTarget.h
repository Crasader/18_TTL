#pragma once

#include <cocos2d.h>

USING_NS_CC;

namespace widget
{

class QYRenderTarget : public cocos2d::RenderTexture
{
public:
	static QYRenderTarget * create(int w, int h);
public:
	void QYInit();
public:
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
};

}
