#pragma once
#ifndef  _MATCHING_H_
#define  _MATCHING_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include<string>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")

typedef int	socklen_t;



USING_NS_CC;

class Matching : public Layer
{
public:
    Size visibleSize;
    Vec2 origin;
    static Scene* createScene();
    

    virtual bool init();
    void update(float dt);
    void menuClickCallBack(Ref* sender);
    void Matched();
    void Start();
    void addBackGround();
    cocos2d::Label* matchLabel;


    sockaddr_in serveraddr;

    Menu* menu;
    MenuItemFont* match;
    MenuItemFont* back;
    cocos2d::ui::TextField* getIP;
    char dataRecv[128];
    std::string str="";
    CREATE_FUNC(Matching);
};
#endif //_MATCHING_H_