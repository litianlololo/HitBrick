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

	Label* label;

	static PhysicsWorld* world;
	void setPhysicsWorld(PhysicsWorld* world);      //带物理引擎的场景
	
	void addBackGround();   // 添加背景
	void addBoard();   // 添加板
    void addball();    //添加球
	void addWall();    //添加边界
	void addKeyListener(); // 添加键盘监听器
	void addBricks();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //用于ball蓄力判断
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //

	void update(float dt);     //用于处理board移动 和小球蓄力
    

    void setJoint();
	Sprite* ball;
	Sprite* board;
	Sprite* Wall;                  //边界，ball碰到会反弹
	 
	int speed = 4;                 //board speed
	int tagboard = 0;
	int tagball = 1;

	int ifstart = 0;              //是否已经开始蓄力   0=未蓄力
	int startF;                   //赋予小球的动量

	//void updateShip(float dt);  // 更新船的平衡情况
	//void BrickGeneraetd();  // 掉落箱子

	
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
