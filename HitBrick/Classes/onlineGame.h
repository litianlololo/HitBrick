#pragma once
#ifndef __onlineGame_H__
#define __onlineGame_H__

#include "cocos2d.h"
#include<iostream>
#include<string>
#include <vector>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
typedef int	socklen_t;

USING_NS_CC;

class onlineGame : public cocos2d::Layer {
public:

	

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
	void pauseClickCallBack(Ref* pSender);

	void backGamemenu(float dt);
	void pausemenuClickCallBack(Ref* sender);

	void setJoint();    //���ball��board֮��Ĺؽ�joint��δ����ǰ��һ���ƶ�

	void timer(float dt);

	PhysicsJointFixed* joint;

	cocos2d::TMXTiledMap* map;
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
	int score = 0;                //����
	int ifstart = 0;              //�Ƿ��Ѿ���ʼ����   0=δ���� 1=��������  -1=�Ѿ�����
	int startF=200;                   //����С��Ķ���
	int bricksnum = 0;            //ʣ��ש����
	int speed = 6;                 //board speed
	int Gametime = 0;                   //��Ϸ��ʼʱ��
	int perscore = 1;                 //����ÿ��ש�ӵ�score

	std::vector<Vec2> Brickpath;
	std::string strscore;
	std::string strtime;
	std::string strscore2;
	std::string strtime2;

	Point point;
	Size visibleSize;
	Vec2 origin;

	CREATE_FUNC(onlineGame);
	/// <summary>
	/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// </summary>
	void addball2();
	void addboard2();
	void DataSend();
	std::string DataCMP(int value);    //����ѹ�������string
	cocos2d::TMXTiledMap* map2;
	Sprite* ball2;
	Sprite* board2;
	Sprite* Wall2;                  //�߽磬ball�����ᷴ��
	Label* scorelabel2;
	Label* timelabel2;
	int score2 = 0;
	int tagboard2 = 10;
	int tagball2 = 11;

	//onlineGame
	int state = 0;    //��Ϸ����״̬

private:
	PhysicsWorld* HitBrick_world;                       //���������������  HitBrick_world

};

#endif // __onlineGame_H__