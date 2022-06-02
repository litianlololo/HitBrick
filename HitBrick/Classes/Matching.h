#pragma once
#ifndef  _MATCHING_H_
#define  _MATCHING_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
typedef int	socklen_t;
typedef int	SOCKET;

USING_NS_CC;

class Matching : public Layer
{
public:
    
    static Scene* createScene();
    
    Sprite* background_1;
    Sprite* background_2;

    virtual bool init();
    void update(float dt);
    void backtomodel(cocos2d::Ref* pSender);

    cocos2d::Label* matchLabel;

    CREATE_FUNC(Matching);
};
#endif //_MATCHING_H_