#pragma once
#ifndef __Gamemodel_H__
#define __Gamemodel_H__
#include "Gamemenu.h"
#include "Gamemenu2.h"
#include "Mainmenu.h"
#include "demo.h"
#include "Matching.h"
#include "AudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;



class Gamemodel :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuClickCallBack(Ref* sender);
	//void menuGoToHelloworld(Ref* pSender);
	void addBackGround();
	void update(float delta);
	void Game1();
	void Game2();
	void Game3();


	int model = 0;
	Size visibleSize;
	Vec2 origin;


	CREATE_FUNC(Gamemodel);

};

#endif // __Gamemodel_H__#pragma once
