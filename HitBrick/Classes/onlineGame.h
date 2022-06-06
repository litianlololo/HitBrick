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
	std::map<int,int> BrickPath;

	Label* label;

	static PhysicsWorld* world;
	void setPhysicsWorld(PhysicsWorld* world);      //带物理引擎的场景

	void addBackGround();   // 添加背景
	void addBoard();   // 添加板
	void addball();    //添加球
	void addWall();    //添加边界
	void addmap();     //添加地图

	//添加砖块
	void addBricks();  //从地图中添加砖块
	void add2xbricks(); //添加2倍砖块

	void addKeyListener(); // 添加键盘监听器
	void addcontactListener(); //添加碰撞监听器

	void addscore();     //添加积分
	void addhscore();    //添加最高分
	void addtime();      //添加游戏时间
	void addbutton();

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);      //用于ball蓄力判断
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);     //
	bool onConcactBegin(PhysicsContact& contact);
	void update(float dt);     //用于处理board移动 和小球蓄力

	void Gameover();
	void Gamewin();
	void Winsend(float dt);
	void unSchedule(float dt);

	void pauseClickCallBack(Ref* pSender);

	void backGamemenu(float dt);
	void pausemenuClickCallBack(Ref* sender);

	void setJoint();    //添加ball和board之间的关节joint，未蓄力前可一起移动

	void timer(float dt);

	PhysicsJointFixed* joint;

	cocos2d::TMXTiledMap* map;
	Sprite* ball;
	Sprite* board;
	Sprite* Wall;                  //边界，ball碰到会反弹
	Sprite* pausebg;               //暂停背景
	Menu* pausemenu;            //暂停菜单
	Label* scorelabel;
	Label* timelabel;


	//Tag
	int tagboard = 0;
	int tagball = 1;
	int tagbrickstart1 =100;
	int tagbrickstart2 = 200;
	int tagbrickstop1 = 100;
	int tagbrickstop2 = 200;
	int tagslowbrick = 4;
	int tagcrazybrick = 5;
	int tagstaticbrick = 6;

	int ballspeedup = 250;        //小球加速增益
	int Gamechoice = 0;           //游戏关卡
	int score = 0;                //积分
	int ifstart = 0;              //是否已经开始蓄力   0=未蓄力 1=正在蓄力  -1=已经发射
	int startF=180;                   //赋予小球的动量
	int bricksnum = 0;            //剩余砖块数
	int speed = 6;                 //board speed
	int Gametime = 0;                   //游戏开始时间
	int perscore = 1;                 //击碎每块砖加的score

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
	void addboardball2();
	void DataT(float dt);
	void BrickT();
	std::string DataCMP(int value);    //数据压缩打包成string
	std::string BrickCMP(int value);    //数字压缩打包成string
	cocos2d::TMXTiledMap* map2;
	Sprite* ball2;
	Sprite* board2;
	Sprite* Wall2;                  //边界，ball碰到会反弹
	Label* scorelabel2;
	Label* timelabel2;
	int score2 = 0;
	int tagboard2 = 10;
	int tagball2 = 11;
	bool ifhit = 0;
	//onlineGame
	int state = 0;    //游戏进行状态
	bool ifout = 1;
	std::string dataSend = "";
	char dataRecv[128];
	int sendcont = 0;

private:
	PhysicsWorld* HitBrick_world;                       //带物理引擎的世界  HitBrick_world

};

#endif // __onlineGame_H__