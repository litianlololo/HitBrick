#include "cocos2d.h"
#include "Gamemodel.h"
#include "Gamemenu.h"
#include "Gamemenu2.h"
#include "Mainmenu.h"
#include "SpeedGame.h"
#include "store.h"
#include "ranking.h"

USING_NS_CC;


Scene* ranking::createScene()
{
    auto scene = Scene::create();
    auto layer = Gamemodel::create();
    scene->addChild(layer);
    return scene;
}

bool ranking::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();                              //»ñÈ¡ÆÁÄ»×ø±ê
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
    auto computer = Sprite::create("mainmenu/computer.png");
    computer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2));
    addChild(computer,4);
    */
    MenuItemFont::setFontSize(20);
    auto ur1 = FileUtils::getInstance()->getStringFromFile("ranking/start.txt");
    auto start = MenuItemFont::create(ur1, CC_CALLBACK_1(ranking::menuClickCallBack, this));
    auto back = MenuItemFont::create("BACK", CC_CALLBACK_1(ranking::menuClickCallBack, this));
    //MainmenuTag
    start->setTag(1);
    back->setTag(0);
    auto menu = Menu::create(start, back, NULL);
    menu->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2);
    addChild(menu);
    menu->alignItemsVertically();

    scheduleUpdate();
    return true;
}


void ranking::menuClickCallBack(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    if (NULL != node)
    {
        auto Gamemenuscene =Gamemenu2::createScene();
        switch (tag)
        {
        case 0:
            Director::getInstance()->replaceScene(Gamemenuscene);
            break;
        case 1:
            auto SpeedGame = HitBrick2::createScene();
            Director::getInstance()->replaceScene(SpeedGame);
            break;
        default:
            break;
        }
    }
}


void ranking::update(float delta) {
    
}





