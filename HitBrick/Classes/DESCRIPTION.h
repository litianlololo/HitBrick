#pragma once
#ifndef __DESCRIPTION_H__
#define ___DESCRIPTION_H__

#include "cocos2d.h"

USING_NS_CC;



class DESCRIPTION :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuClickCallBack(Ref* sender);
	void addBackGround();
	Size visibleSize;
	Vec2 origin;

	CREATE_FUNC(DESCRIPTION);

};

#endif // __DESCRIPTION_H__