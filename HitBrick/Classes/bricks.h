#pragma once
#ifndef __bricks_H__
#define __bricks_H__

#include "cocos2d.h"

USING_NS_CC;



class bricks :public cocos2d::Layer {
public:
	virtual bool init();
	Size visibleSize;
	Vec2 origin;


	CREATE_FUNC(bricks);

};

#endif // _bricks_H__