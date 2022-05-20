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
	void setPhysicsWorld(PhysicsWorld* world);      //����������ĳ���
	
	void addBackGround();   // ��ӱ���
	void addBoard();   // ��Ӱ�
    void addball();    //�����
	void addWall();    //��ӱ߽�
	void addKeyListener(); // ��Ӽ��̼�����
	void addBricks();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //����ball�����ж�
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //

	void update(float dt);     //���ڴ���board�ƶ� ��С������
    

    void setJoint();
	Sprite* ball;
	Sprite* board;
	Sprite* Wall;                  //�߽磬ball�����ᷴ��
	 
	int speed = 4;                 //board speed
	int tagboard = 0;
	int tagball = 1;

	int ifstart = 0;              //�Ƿ��Ѿ���ʼ����   0=δ����
	int startF;                   //����С��Ķ���

	//void updateShip(float dt);  // ���´���ƽ�����
	//void BrickGeneraetd();  // ��������

	
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
