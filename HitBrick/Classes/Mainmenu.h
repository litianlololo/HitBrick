#pragma once
#ifndef __Mainmenu_H__
#define __Mainmenu_H__

#include "cocos2d.h"

USING_NS_CC;
class Mainmenu :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuClickCallBack(Ref* sender);
    void menuGoToHelloworld(Ref* pSender);
	void addBackGround();
	Size visibleSize;
	Vec2 origin;


	CREATE_FUNC(Mainmenu);

};

#endif // __Mainmenu_H__