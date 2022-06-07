#pragma once
#ifndef __ranking_H__
#define __ranking_H__

#include "cocos2d.h"

USING_NS_CC;



class ranking :public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	ranking();
	void menuClickCallBack(Ref* sender);
	void bg();
	void getscore();
	void gettime();
	void update(float delta);
	void addBackGround();
	int gamechoice;
	int model;
	Size visibleSize;
	Vec2 origin;

	CREATE_FUNC(ranking);

};

#endif // __ranking_H__#pragma once
