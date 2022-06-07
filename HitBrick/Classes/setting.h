#pragma once
#ifndef __setting_H__
#define __setting_H__

#include "cocos2d.h"

USING_NS_CC;



class setting :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuClickCallBack(Ref* sender);
	void menuClickCallBack2(Ref* sender);
	//void menuGoToHelloworld(Ref* pSender);

	void update(float delta);
	void addBackGround();
	Size visibleSize;
	Vec2 origin;

	Sprite* ball;
	Sprite* board;

	CREATE_FUNC(setting);

};

#endif // __setting_H__