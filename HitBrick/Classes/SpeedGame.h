#pragma once
#ifndef __HITBRICK2_H__
#define __HITBRICK2_H__

#include "cocos2d.h"
#include "BoardBall.h"
USING_NS_CC;

class HitBrick2 : public cocos2d::Layer {
public:

	cocos2d::TMXTiledMap* map;
	HitBrick2();

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

	//���ש��
	void addBricks();  //�ӵ�ͼ�����ש��
	void add2xbricks(); //���2��ש��
	void addslowbricks(); //��Ӽ���ש��
	void addcrazybricks(); //��ӷ��ש��
	void addstaticbricks(); //��ӹ̶�ש��
	void addrevivebricks(); //��ӹ̶�ש��

	void addKeyListener(); // ��Ӽ��̼�����
	void addcontactListener(); //�����ײ������

	void addscore();     //��ӻ���
	void addhscore();    //�����߷�
	void addtime();      //�����Ϸʱ��
	void addbutton();

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //����ball�����ж�
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //
	bool onConcactBegin(PhysicsContact& contact);
	void update(float dt);     //���ڴ���board�ƶ� ��С������

	void Gameover();
	void settlement();  //���а����
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
	Sprite* revivepng;          //����ͼ��
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
	int tagrevivebrick = 7;

	int ballspeedup;        //С���������
	int Gamechoice;           //��Ϸ�ؿ�
	int score;                //����
	int ifstart;              //�Ƿ��Ѿ���ʼ����   0=δ���� 1=��������  -1=�Ѿ�����
	int startF;                   //����С��Ķ���
	int bricksnum;            //ʣ��ש����
	int speed;                 //board speed
	int Gametime;                   //��Ϸ��ʼʱ��
	int perscore;                 //����ÿ��ש�ӵ�score
	int revivetimes = 0;      //�������

	std::vector<Vec2> Brickpath;
	std::string strscore;
	std::string strtime;

	Point point;
	Size visibleSize;
	Vec2 origin;

	Ball BL;
	Board BD;
	CREATE_FUNC(HitBrick2);

private:
	PhysicsWorld* HitBrick_world;                       //���������������  HitBrick_world

};

class ranks
{
public:
	int score;
	int times;
};

#endif // __HITBRICK2_H__
