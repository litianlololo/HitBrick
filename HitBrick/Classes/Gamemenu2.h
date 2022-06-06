
#pragma once
#ifndef __Gamemenu2_H__
#define __Gamemenu2_H__

#include "cocos2d.h"

USING_NS_CC;



class Gamemenu2 :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuClickCallBack(Ref* sender);
	//void menuGoToHelloworld(Ref* pSender);
	void addBackGround();
	void update(float delta);
	void Game();
	int Gamechoice = 0;
	Size visibleSize;
	Vec2 origin;


	CREATE_FUNC(Gamemenu2);

};

#endif // __Gamemenu2_H__