#pragma once
#ifndef __Gamemenu_H__
#define __Gamemenu_H__

#include "cocos2d.h"

USING_NS_CC;



class Gamemenu:public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	Gamemenu();

	virtual bool init();

	void menuClickCallBack(Ref* sender);
	//void menuGoToHelloworld(Ref* pSender);

	void update(float delta);
	void Game();
	void addBackGround();
	int Gamechoice;
	Size visibleSize;
	Vec2 origin;


	CREATE_FUNC(Gamemenu);

};

#endif // __Gamemenu_H__