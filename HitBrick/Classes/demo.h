#pragma once
#ifndef __HITBRICK_H__
#define __HITBRICK_H__

#include "cocos2d.h"

USING_NS_CC;

class HitBrick : public cocos2d::Layer {
public:

	cocos2d::TMXTiledMap* map = cocos2d::TMXTiledMap::create("map.tmx");
	
	virtual bool init();

	static cocos2d::Scene* createScene();

	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;

	

	static PhysicsWorld* world;
	void setPhysicsWorld(PhysicsWorld* world);      //带物理引擎的场景
	
	void addBackGround();   // 添加背景
	void addBoard();   // 添加板
	void addListener(); // 添加键盘监听器
	void addball();    //添加球

	void update(float dt);


    void setJoint();

	Sprite* ball;
	Sprite* board;

	int tagboard = 0;
	int tagball = 1;
	//void updateShip(float dt);  // 更新船的平衡情况
	//void BrickGeneraetd();  // 掉落箱子

	//void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	//void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	//bool onConcactBegin(PhysicsContact& contact);

	//void preloadMusic();                   // 预加载音乐
	//void GameOver();                      // 游戏结束
	//void loadAnimation(std::string filepath); // 加载动画

	// 重玩或退出按钮响应函数
	//void replayCallback(Ref* pSender);
	//void exitCallback(Ref* pSender);
	Size visibleSize;
	Vec2 origin;
	// implement the "static create()" method manually

	CREATE_FUNC(HitBrick);

private:
	PhysicsWorld* HitBrick_world;                       //带物理引擎的世界  HitBrick_world

	//Sprite* player;
	//Sprite* ship;
	//bool onBall;
	//Sprite* ball;
	// 固定关节
	//PhysicsJointPin* joint1;
	//int spHolded;
	//int spFactor;
	//新加了标签，分别代表了底部的船，球，砖块，玩家的板子
	//enum Tag { SHIP, BALL, BOX, BOARD };
};

#endif // __HITBRICK_H__
