#pragma once
#ifndef __HITBRICK_H__
#define __HITBRICK_H__

#include "cocos2d.h"

USING_NS_CC;

class HitBrick : public cocos2d::Layer {
public:

	cocos2d::TMXTiledMap* map;
	
	virtual bool init();
	static Scene* scene();
	static cocos2d::Scene* createScene();

	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;

	Label* label;

	static PhysicsWorld* world;
	void setPhysicsWorld(PhysicsWorld* world);      //����������ĳ���
	
	void addBackGround();   // ��ӱ���
	void addBoard();   // ��Ӱ�
    void addball();    //�����
	void addWall();    //��ӱ߽�
	void addmap();     //��ӵ�ͼ

	void addBricks();  //�ӵ�ͼ�����ש��
	void add2xbricks(); //���2��ש��
	void addslowbricks(); //��Ӽ���ש��
	void addcrazybricks(); //��ӷ��ש��
	void addstaticbricks(); //��ӹ̶�ש��

	void addKeyListener(); // ��Ӽ��̼�����
	void addcontactListener(); //�����ײ������

	void addscore();     //��ӻ���
	void addhscore();    //�����߷�

	void addbutton();
	void addkuang();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //����ball�����ж�
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //
	bool onConcactBegin(PhysicsContact& contact);
	void update(float dt);     //���ڴ���board�ƶ� ��С������

	void Gameover();
	void replayCallback(Ref* pSender);
	void pauseClickCallBack(Ref* pSender);
	void exitCallback(Ref* pSender);

	void backGamemenu(float dt);
	void HitBrick::pausemenuClickCallBack(Ref* sender);

	void setJoint();    //���ball��board֮��Ĺؽ�joint��δ����ǰ��һ���ƶ�
	PhysicsJointFixed* joint;
    
	Sprite* ball;
	Sprite* kuang;
	Sprite* board;
	Sprite* Wall;                  //�߽磬ball�����ᷴ��
	Sprite* pausebg;               //��ͣ����
	Menu* pausemenu;            //��ͣ�˵�
	Label* scorelabel;

	//Tag
	int speed = 6;                 //board speed
	int tagboard = 0;
	int tagball = 1;
	int tagbrick = 2;
	int tag2xbrick = 3;
	int tagslowbrick = 4;
	int tagcrazybrick = 5;
	int tagstaticbrick = 6;
	int tagkuang = 7;
	int Gamechoice = 0;           //��Ϸ�ؿ�
	int score=0;
	int ifstart = 0;              //�Ƿ��Ѿ���ʼ����   0=δ���� 1=��������  -1=�Ѿ�����
	int startF;                   //����С��Ķ���
	int bricksnum = 0;            //ʣ��ש����
	int perscore = 1;                 //����ÿ��ש�ӵ�score
	int ballspeedup = 250;        //С���������

	std::string strscore;

	std::vector<Vec2> Brickpath;
	//std::vector<Sprite*> Bricks;
	//void updateShip(float dt);  // ���´���ƽ�����
	//void BrickGeneraetd();  // ��������

	
	//bool onConcactBegin(PhysicsContact& contact);

	//void preloadMusic();                   // Ԥ��������
	//void GameOver();                      // ��Ϸ����
	//void loadAnimation(std::string filepath); // ���ض���

	// ������˳���ť��Ӧ����
	//void replayCallback(Ref* pSender);
	

	bool HitBrick::TouchBegan(cocos2d::Touch* pTouch, Event* pEvent);
	Point HitBrick::tilePosFromLocation(Point location, TMXTiledMap* tileMap);
	Point point;
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
