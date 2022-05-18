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
	void setPhysicsWorld(PhysicsWorld* world);      //����������ĳ���
	
	void addBackGround();   // ��ӱ���
	void addBoard();   // ��Ӱ�
	void addListener(); // ��Ӽ��̼�����
	void addball();    //�����

	void update(float dt);


    void setJoint();

	Sprite* ball;
	Sprite* board;

	int tagboard = 0;
	int tagball = 1;
	//void updateShip(float dt);  // ���´���ƽ�����
	//void BrickGeneraetd();  // ��������

	//void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	//void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	//bool onConcactBegin(PhysicsContact& contact);

	//void preloadMusic();                   // Ԥ��������
	//void GameOver();                      // ��Ϸ����
	//void loadAnimation(std::string filepath); // ���ض���

	// ������˳���ť��Ӧ����
	//void replayCallback(Ref* pSender);
	//void exitCallback(Ref* pSender);
	Size visibleSize;
	Vec2 origin;
	// implement the "static create()" method manually

	CREATE_FUNC(HitBrick);

private:
	PhysicsWorld* HitBrick_world;                       //���������������  HitBrick_world

	//Sprite* player;
	//Sprite* ship;
	//bool onBall;
	//Sprite* ball;
	// �̶��ؽ�
	//PhysicsJointPin* joint1;
	//int spHolded;
	//int spFactor;
	//�¼��˱�ǩ���ֱ�����˵ײ��Ĵ�����ש�飬��ҵİ���
	//enum Tag { SHIP, BALL, BOX, BOARD };
};

#endif // __HITBRICK_H__
