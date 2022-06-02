#include "cocos2d.h"
#include "Gamemodel.h"
#include "Matching.h"

USING_NS_CC;


Scene* Matching::createScene()
{
    auto scene = Scene::create();
    auto layer = Gamemodel::create();
    scene->addChild(layer);
    return scene;
}

bool Matching::init()
{

    return true;
}