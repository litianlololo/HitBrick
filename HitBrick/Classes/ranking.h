#pragma once
#ifndef __ranking_H__
#define __ranking_H__

#include "cocos2d.h"

USING_NS_CC;



class ranking :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuClickCallBack(Ref* sender);

	void update(float delta);

	int model = 0;
	Size visibleSize;
	Vec2 origin;

	CREATE_FUNC(ranking);

};

#endif // __ranking_H__#pragma once