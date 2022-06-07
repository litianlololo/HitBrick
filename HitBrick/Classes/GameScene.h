#pragma once
#ifndef __GameScene_H__
#define __GameScene_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer {
public:

	cocos2d::TMXTiledMap* map;

	bool init();
	//virtual bool myinit();
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

	//���ש��
	void addBricks();  //�ӵ�ͼ�����ש��
	void add2xbricks(); //���2��ש��
	void addslowbricks(); //��Ӽ���ש��
	void addcrazybricks(); //��ӷ��ש��
	void addstaticbricks(); //��ӹ̶�ש��

	//virtual void addKeyListener(); // ��Ӽ��̼�����
	//virtual void addcontactListener(); //�����ײ������

	void addscore();     //��ӻ���
	void addhscore();    //�����߷�
	void addtime();      //�����Ϸʱ��
	void addbutton();

	//virtual void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //����ball�����ж�
	//virtual void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //
	//virtual bool onConcactBegin(PhysicsContact& contact);
	//virtual void update(float dt);     //���ڴ���board�ƶ� ��С������

	//virtual void Gameover();
	void pauseClickCallBack(Ref* pSender);

	void backGamemenu(float dt);
	void pausemenuClickCallBack(Ref* sender);

	void setJoint();    //���ball��board֮��Ĺؽ�joint��δ����ǰ��һ���ƶ�

	void timer(float dt);

	PhysicsJointFixed* joint;

	Sprite* ball;
	Sprite* board;
	Sprite* Wall;                  //�߽磬ball�����ᷴ��
	Sprite* pausebg;               //��ͣ����
	Menu* pausemenu;            //��ͣ�˵�
	Label* scorelabel;
	Label* timelabel;

	//Tag
	int tagboard = 0;
	int tagball = 1;
	int tagbrick = 2;
	int tag2xbrick = 3;
	int tagslowbrick = 4;
	int tagcrazybrick = 5;
	int tagstaticbrick = 6;

	int ballspeedup = 250;        //С���������
	int Gamechoice = 0;           //��Ϸ�ؿ�
	int Gamemodel = 0;
	int score = 0;                //����
	int ifstart = 0;              //�Ƿ��Ѿ���ʼ����   0=δ���� 1=��������  -1=�Ѿ�����
	int startF;                   //����С��Ķ���
	int bricksnum = 0;            //ʣ��ש����
	int speed = 6;                 //board speed
	int Gametime = 0;                   //��Ϸ��ʼʱ��
	int perscore = 1;                 //����ÿ��ש�ӵ�score

	std::vector<Vec2> Brickpath;
	std::string strscore;
	std::string strtime;

	Point point;
	Size visibleSize;
	Vec2 origin;
	
	PhysicsWorld* HitBrick_world;                       //���������������  HitBrick_world
	
	CREATE_FUNC(GameScene);

private:
	

};

class SpeedGame : public GameScene
{
public:
	bool myinit();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //����ball�����ж�
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //
	bool onConcactBegin(PhysicsContact& contact);
	void update(float dt);     //���ڴ���board�ƶ� ��С������
	void Gameover();
	void addKeyListener(); // ��Ӽ��̼�����
	void addcontactListener(); //�����ײ������

	void settlement();  //���а����
};

class LeisureGame : public GameScene
{
public:
	bool myinit();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //����ball�����ж�
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //
	bool onConcactBegin(PhysicsContact& contact);
	void update(float dt);     //���ڴ���board�ƶ� ��С������
	void Gameover();
	void addKeyListener(); // ��Ӽ��̼�����
	void addcontactListener(); //�����ײ������

	void addkuang();
	Sprite* kuang;
	int tagkuang = 7;
};

class ranks
{
public:
	int score;
	int times;
};

#endif // __HITBRICK2_H__
